#ifndef WASI_V8_ISOLATE_EXTENSIONS_H_
#define WASI_V8_ISOLATE_EXTENSIONS_H_

// Extension to v8::Isolate class for WASI build
// This should be included right after v8-isolate.h

namespace v8 {

// Extend the already-declared Isolate class with missing enums
class Isolate {
 public:
  // MessageErrorLevel enum that v8-inspector.h expects
  enum MessageErrorLevel {
    kMessageLog = (1 << 0),
    kMessageDebug = (1 << 1),
    kMessageInfo = (1 << 2),
    kMessageError = (1 << 3),
    kMessageWarning = (1 << 4),
    kMessageAll = kMessageLog | kMessageDebug | kMessageInfo | kMessageError | kMessageWarning
  };
  
  // Static method Node.js expects
  static Isolate* GetCurrent() { return nullptr; }
  
  // Missing typedefs
  typedef void (*AbortOnUncaughtExceptionCallback)(Isolate* isolate);
  
  // Methods Node.js expects
  void SetOOMErrorHandler(OOMErrorCallback callback) {}
  void SetFatalErrorHandler(FatalErrorCallback callback) {}
  void SetPromiseRejectCallback(PromiseRejectCallback callback) {}
  void SetAllowWasmCodeGenerationCallback(AllowWasmCodeGenerationCallback callback) {}
  void SetModifyCodeGenerationFromStringsCallback(ModifyCodeGenerationFromStringsCallback2 callback) {}
  void SetMicrotasksPolicy(MicrotasksPolicy policy) {}
  void SetCppHeap(CppHeap* cpp_heap) {}
};

} // namespace v8

#endif // WASI_V8_ISOLATE_EXTENSIONS_H_