#ifndef NODE_WASI_FIXES_H_
#define NODE_WASI_FIXES_H_

#ifdef __wasi__

// Compatibility macros for WASI build
#define V8_ISOLATE_GET_CURRENT() v8::Isolate_GetCurrent()
#define V8_ISOLATE_GET_CURRENT_CONTEXT(isolate) v8::Isolate_GetCurrentContext(isolate)

// Redefine Isolate static methods
namespace v8 {
class Isolate {
 public:
  static Isolate* GetCurrent() { return Isolate_GetCurrent(); }
  Local<Context> GetCurrentContext() { return Isolate_GetCurrentContext(this); }
  
  // Include MessageErrorLevel enum
  using MessageErrorLevel = v8::Isolate::MessageErrorLevel;
  
  // Include callback typedef
  using AbortOnUncaughtExceptionCallback = v8::Isolate::AbortOnUncaughtExceptionCallback;
};
} // namespace v8

#else

// Non-WASI builds use normal methods
#define V8_ISOLATE_GET_CURRENT() v8::Isolate::GetCurrent()
#define V8_ISOLATE_GET_CURRENT_CONTEXT(isolate) (isolate)->GetCurrentContext()

#endif // __wasi__

#endif // NODE_WASI_FIXES_H_