// Copyright 2024 Node.js contributors
// WASI implementation of Node.js fs module

#include "node.h"
#include "node_buffer.h"
#include "node_internals.h"
#include "env-inl.h"
#include "string_bytes.h"
#include "util-inl.h"

#include <wasi/api.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

namespace node {
namespace wasi_fs {

using v8::Array;
using v8::Context;
using v8::EscapableHandleScope;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::HandleScope;
using v8::Integer;
using v8::Isolate;
using v8::Local;
using v8::MaybeLocal;
using v8::Number;
using v8::Object;
using v8::ObjectTemplate;
using v8::String;
using v8::Value;

// WASI file descriptor wrapper
class WASIFileDescriptor {
 public:
  explicit WASIFileDescriptor(int fd) : fd_(fd) {}
  
  int fd() const { return fd_; }
  
  __wasi_errno_t Read(void* buf, size_t len, size_t* nread) {
    __wasi_iovec_t iov = { .buf = static_cast<uint8_t*>(buf), .buf_len = len };
    return __wasi_fd_read(fd_, &iov, 1, nread);
  }
  
  __wasi_errno_t Write(const void* buf, size_t len, size_t* nwritten) {
    __wasi_ciovec_t iov = { 
      .buf = static_cast<const uint8_t*>(buf), 
      .buf_len = len 
    };
    return __wasi_fd_write(fd_, &iov, 1, nwritten);
  }
  
  __wasi_errno_t Close() {
    return __wasi_fd_close(fd_);
  }
  
  __wasi_errno_t Seek(__wasi_filedelta_t offset, __wasi_whence_t whence, __wasi_filesize_t* newoffset) {
    return __wasi_fd_seek(fd_, offset, whence, newoffset);
  }
  
  __wasi_errno_t Stat(__wasi_filestat_t* stat) {
    return __wasi_fd_filestat_get(fd_, stat);
  }

 private:
  int fd_;
};

// WASI implementation of fs.open
static void Open(const FunctionCallbackInfo<Value>& args) {
  Environment* env = Environment::GetCurrent(args);
  Isolate* isolate = env->isolate();
  
  CHECK_GE(args.Length(), 2);
  CHECK(args[0]->IsString());
  CHECK(args[1]->IsInt32());
  
  String::Utf8Value path(isolate, args[0]);
  int flags = args[1].As<Integer>()->Value();
  
  // Convert Node.js flags to WASI flags
  __wasi_oflags_t oflags = 0;
  __wasi_rights_t rights_base = 0;
  __wasi_rights_t rights_inheriting = 0;
  
  if (flags & O_CREAT) oflags |= __WASI_OFLAGS_CREAT;
  if (flags & O_EXCL) oflags |= __WASI_OFLAGS_EXCL;
  if (flags & O_TRUNC) oflags |= __WASI_OFLAGS_TRUNC;
  
  if (flags & O_RDONLY || flags & O_RDWR) {
    rights_base |= __WASI_RIGHTS_FD_READ;
    rights_base |= __WASI_RIGHTS_FD_SEEK;
  }
  if (flags & O_WRONLY || flags & O_RDWR) {
    rights_base |= __WASI_RIGHTS_FD_WRITE;
    rights_base |= __WASI_RIGHTS_FD_SEEK;
  }
  
  __wasi_fd_t fd;
  __wasi_errno_t err = __wasi_path_open(
    AT_FDCWD,  // dirfd (current working directory)
    0,         // dirflags
    *path,
    strlen(*path),
    oflags,
    rights_base,
    rights_inheriting,
    0,  // fdflags
    &fd
  );
  
  if (err != __WASI_ERRNO_SUCCESS) {
    isolate->ThrowException(
      Exception::Error(String::NewFromUtf8(isolate, "Failed to open file").ToLocalChecked())
    );
    return;
  }
  
  args.GetReturnValue().Set(Integer::New(isolate, fd));
}

// WASI implementation of fs.read
static void Read(const FunctionCallbackInfo<Value>& args) {
  Environment* env = Environment::GetCurrent(args);
  Isolate* isolate = env->isolate();
  
  CHECK_GE(args.Length(), 2);
  CHECK(args[0]->IsInt32());
  CHECK(node::Buffer::HasInstance(args[1]));
  
  int fd = args[0].As<Integer>()->Value();
  Local<Object> buffer_obj = args[1].As<Object>();
  char* buffer = node::Buffer::Data(buffer_obj);
  size_t length = node::Buffer::Length(buffer_obj);
  
  size_t offset = 0;
  if (args.Length() > 2 && args[2]->IsNumber()) {
    offset = args[2].As<Number>()->Value();
  }
  
  size_t len = length;
  if (args.Length() > 3 && args[3]->IsNumber()) {
    len = args[3].As<Number>()->Value();
  }
  
  WASIFileDescriptor wfd(fd);
  size_t nread;
  __wasi_errno_t err = wfd.Read(buffer + offset, len, &nread);
  
  if (err != __WASI_ERRNO_SUCCESS) {
    isolate->ThrowException(
      Exception::Error(String::NewFromUtf8(isolate, "Read failed").ToLocalChecked())
    );
    return;
  }
  
  args.GetReturnValue().Set(Integer::New(isolate, nread));
}

// WASI implementation of fs.write
static void Write(const FunctionCallbackInfo<Value>& args) {
  Environment* env = Environment::GetCurrent(args);
  Isolate* isolate = env->isolate();
  
  CHECK_GE(args.Length(), 2);
  CHECK(args[0]->IsInt32());
  
  int fd = args[0].As<Integer>()->Value();
  
  const char* data = nullptr;
  size_t length = 0;
  
  if (node::Buffer::HasInstance(args[1])) {
    Local<Object> buffer_obj = args[1].As<Object>();
    data = node::Buffer::Data(buffer_obj);
    length = node::Buffer::Length(buffer_obj);
  } else if (args[1]->IsString()) {
    String::Utf8Value str(isolate, args[1]);
    data = *str;
    length = str.length();
  }
  
  WASIFileDescriptor wfd(fd);
  size_t nwritten;
  __wasi_errno_t err = wfd.Write(data, length, &nwritten);
  
  if (err != __WASI_ERRNO_SUCCESS) {
    isolate->ThrowException(
      Exception::Error(String::NewFromUtf8(isolate, "Write failed").ToLocalChecked())
    );
    return;
  }
  
  args.GetReturnValue().Set(Integer::New(isolate, nwritten));
}

// WASI implementation of fs.close
static void Close(const FunctionCallbackInfo<Value>& args) {
  Environment* env = Environment::GetCurrent(args);
  Isolate* isolate = env->isolate();
  
  CHECK_GE(args.Length(), 1);
  CHECK(args[0]->IsInt32());
  
  int fd = args[0].As<Integer>()->Value();
  
  WASIFileDescriptor wfd(fd);
  __wasi_errno_t err = wfd.Close();
  
  if (err != __WASI_ERRNO_SUCCESS) {
    isolate->ThrowException(
      Exception::Error(String::NewFromUtf8(isolate, "Close failed").ToLocalChecked())
    );
    return;
  }
  
  args.GetReturnValue().Set(Integer::New(isolate, 0));
}

// WASI implementation of fs.stat
static void Stat(const FunctionCallbackInfo<Value>& args) {
  Environment* env = Environment::GetCurrent(args);
  Isolate* isolate = env->isolate();
  
  CHECK_GE(args.Length(), 1);
  CHECK(args[0]->IsString());
  
  String::Utf8Value path(isolate, args[0]);
  
  __wasi_filestat_t stat;
  __wasi_errno_t err = __wasi_path_filestat_get(
    AT_FDCWD,
    0,  // flags
    *path,
    strlen(*path),
    &stat
  );
  
  if (err != __WASI_ERRNO_SUCCESS) {
    isolate->ThrowException(
      Exception::Error(String::NewFromUtf8(isolate, "Stat failed").ToLocalChecked())
    );
    return;
  }
  
  Local<Object> result = Object::New(isolate);
  result->Set(env->context(),
              String::NewFromUtf8(isolate, "size").ToLocalChecked(),
              Number::New(isolate, stat.size)).Check();
  result->Set(env->context(),
              String::NewFromUtf8(isolate, "mtime").ToLocalChecked(),
              Number::New(isolate, stat.mtim / 1000000)).Check();  // Convert nanoseconds to milliseconds
  result->Set(env->context(),
              String::NewFromUtf8(isolate, "ctime").ToLocalChecked(),
              Number::New(isolate, stat.ctim / 1000000)).Check();
  result->Set(env->context(),
              String::NewFromUtf8(isolate, "atime").ToLocalChecked(),
              Number::New(isolate, stat.atim / 1000000)).Check();
  
  args.GetReturnValue().Set(result);
}

// Initialize WASI fs module
void Initialize(Local<Object> target,
                Local<Value> unused,
                Local<Context> context,
                void* priv) {
  Environment* env = Environment::GetCurrent(context);
  Isolate* isolate = env->isolate();
  
  // Register WASI fs functions
  env->SetMethod(target, "open", Open);
  env->SetMethod(target, "read", Read);
  env->SetMethod(target, "write", Write);
  env->SetMethod(target, "close", Close);
  env->SetMethod(target, "stat", Stat);
  
  // Add fs constants
  Local<Object> constants = Object::New(isolate);
  
  // File open flags
  constants->Set(context,
                 String::NewFromUtf8(isolate, "O_RDONLY").ToLocalChecked(),
                 Integer::New(isolate, O_RDONLY)).Check();
  constants->Set(context,
                 String::NewFromUtf8(isolate, "O_WRONLY").ToLocalChecked(),
                 Integer::New(isolate, O_WRONLY)).Check();
  constants->Set(context,
                 String::NewFromUtf8(isolate, "O_RDWR").ToLocalChecked(),
                 Integer::New(isolate, O_RDWR)).Check();
  constants->Set(context,
                 String::NewFromUtf8(isolate, "O_CREAT").ToLocalChecked(),
                 Integer::New(isolate, O_CREAT)).Check();
  constants->Set(context,
                 String::NewFromUtf8(isolate, "O_EXCL").ToLocalChecked(),
                 Integer::New(isolate, O_EXCL)).Check();
  constants->Set(context,
                 String::NewFromUtf8(isolate, "O_TRUNC").ToLocalChecked(),
                 Integer::New(isolate, O_TRUNC)).Check();
  
  target->Set(context,
              String::NewFromUtf8(isolate, "constants").ToLocalChecked(),
              constants).Check();
}

}  // namespace wasi_fs
}  // namespace node

NODE_MODULE_CONTEXT_AWARE_INTERNAL(wasi_fs, node::wasi_fs::Initialize)