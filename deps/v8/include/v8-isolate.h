#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef V8_V8_ISOLATE_H_
#define V8_V8_ISOLATE_H_

#ifdef __wasi__
#include "wasi/v8-wasi-compat.h"
#include "v8-local-handle.h"

// V8_EXPORT macro for WASI
#ifndef V8_EXPORT
#define V8_EXPORT
#endif
#endif

namespace v8 {

#ifdef __wasi__
// Forward declarations
class Context;
#endif

#ifdef __wasi__
// Minimal Isolate stub for WASI
class V8_EXPORT Isolate {
 public:
  // CreateParams stub
  struct CreateParams {
    CreateParams() = default;
  };
  
  static Isolate* GetCurrent() { return nullptr; }
  static Isolate* TryGetCurrent() { return nullptr; }
  
  void Enter() {}
  void Exit() {}
  void Dispose() {}
  
  // Additional methods as needed
  void* GetData(uint32_t slot) { return nullptr; }
  void SetData(uint32_t slot, void* data) {}
  Local<Context> GetCurrentContext() { return Local<Context>(); }
  
  // Scope class
  class Scope {
   public:
    explicit Scope(Isolate* isolate) {}
    ~Scope() {}
  };
  
  // MessageErrorLevel enum
  enum MessageErrorLevel {
    kMessageLog,
    kMessageDebug,
    kMessageInfo,
    kMessageError,
    kMessageWarning
  };
  
  // Priority enum
  enum class Priority {
    kBestEffort = 0,
    kUserVisible = 1,
    kUserBlocking = 2,
  };
  
  // AbortOnUncaughtExceptionCallback typedef
  using AbortOnUncaughtExceptionCallback = bool (*)(Isolate*);
  
  // Exception handling
  Local<Value> ThrowException(Local<Value> exception) {
    return exception;
  }
};
#else
// Forward declaration for Isolate
class Isolate;
#endif

// StackTracePrinter function type
using StackTracePrinter = void (*)();

}  // namespace v8

#endif  // V8_V8_ISOLATE_H_
