#ifndef SRC_WASI_ISOLATE_EXTENSIONS_H_
#define SRC_WASI_ISOLATE_EXTENSIONS_H_

#ifdef __wasi__

// Extension methods for V8 Isolate in WASI builds
namespace v8 {

// Inline stub methods for missing Isolate functionality
inline bool IsolateInContext(Isolate* isolate) {
  // WASI always has a context
  return true;
}

inline CppHeap* IsolateGetCppHeap(Isolate* isolate) {
  // WASI doesn't use CppHeap
  return nullptr;
}

} // namespace v8

// Macros to redirect method calls
#define InContext() v8::IsolateInContext(this)
#define GetCppHeap() v8::IsolateGetCppHeap(this)

#endif // __wasi__

#endif // SRC_WASI_ISOLATE_EXTENSIONS_H_
