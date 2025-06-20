// Copyright 2013 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8CONFIG_H_
#define V8CONFIG_H_

// For WASI/Emscripten, include custom WASI config file first
#if defined(__wasi__) || defined(__EMSCRIPTEN__)
#include "wasi/v8config-wasi.h"
#endif

// Basic V8 macros
#define V8_INLINE inline
#define V8_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#define V8_EXPORT
#define V8_NOINLINE __attribute__((noinline))
#define V8_BASE_EXPORT
#define V8_EXPORT_PRIVATE
#define V8_NODISCARD [[nodiscard]]

// Branch prediction macros
#define V8_LIKELY(condition) __builtin_expect(!!(condition), 1)
#define V8_UNLIKELY(condition) __builtin_expect(!!(condition), 0)

// GLIBC version checking macro for WASI
#define V8_GLIBC_PREREQ(major, minor) 0

// OS and architecture detection for WASI
#define V8_OS_WASI 1
#define V8_HOST_ARCH_32_BIT 1

// Platform-specific types for WASI
#ifdef __wasi__
typedef int PlatformSharedMemoryHandle;
#endif

// Additional macros for WASI compatibility
#define V8_ASSUME(condition) __builtin_assume(condition)
#define V8_TRIVIAL_ABI
#define V8_DEPRECATED_SOON(message)
#define V8_DEPRECATE_SOON(message)
#define V8_ENUM_DEPRECATE_SOON(message)
#define V8_CLANG_NO_SANITIZE(what)

// Add basic macro definitions to prevent redefinition conflicts
#ifndef EXPORT_CONTEXTUAL_VARIABLE
#define EXPORT_CONTEXTUAL_VARIABLE(TYPE, NAME) \
  using NAME = ContextualVariable<TYPE, TYPE>
#endif

// Forward declare missing V8 types for WASI compatibility
namespace v8 {
class ConvertableToTraceFormat;
class VirtualAddressSpace;

// Basic type aliases for WASI compatibility
using Address = uintptr_t;
enum class PagePermissions : int {
  kNoAccess = 0,
  kRead = 1,
  kReadWrite = 2,
  kReadExecute = 3,
  kReadWriteExecute = 4
};

// Forward declarations for job system
class JobTask;
class JobDelegate;
class JobHandle;

// Page allocator class for memory management
class PageAllocator {
public:
  virtual ~PageAllocator() = default;
  virtual void* AllocatePages(void* address, size_t length, size_t alignment,
                             PagePermissions permissions) = 0;
  virtual bool FreePages(void* address, size_t length) = 0;
  virtual bool ReleasePages(void* address, size_t length, size_t new_length) = 0;
  virtual bool SetPermissions(void* address, size_t length,
                             PagePermissions permissions) = 0;
  virtual bool RecommitPages(void* address, size_t length,
                            PagePermissions permissions) = 0;
  virtual bool DiscardSystemPages(void* address, size_t size) = 0;
  virtual bool DecommitPages(void* address, size_t size) = 0;
};

// Platform interface for V8
class Platform {
public:
  virtual ~Platform() = default;
  virtual PageAllocator* GetPageAllocator() = 0;
  virtual void OnCriticalMemoryPressure() {}
  virtual int NumberOfWorkerThreads() = 0;
};

// Tracing related forward declarations
class TracingController {
public:
  class TraceStateObserver;
};
}

#endif  // V8CONFIG_H_
