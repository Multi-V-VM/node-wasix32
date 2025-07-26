#ifndef SRC_WASI_V8_EXTENSIONS_H_
#define SRC_WASI_V8_EXTENSIONS_H_

#ifdef __wasi__

// Forward declarations
namespace v8 {
class Isolate;
class CppHeap;
}

namespace node {
namespace wasi_compat {

// Extension functions for missing V8 Isolate methods
// IsolateInContext is already defined in wasi-isolate-extensions.h

inline v8::CppHeap* IsolateGetCppHeap(v8::Isolate* isolate) {
  // WASI doesn't use CppHeap
  return nullptr;
}

// Helper for allocation handle when CppHeap is null
struct DummyAllocationHandle {
  // Minimal implementation
};

inline DummyAllocationHandle& GetAllocationHandle() {
  static DummyAllocationHandle handle;
  return handle;
}

} // namespace wasi_compat
} // namespace node

// Fix for BaseObjectList range-based for loop
// Add global begin/end functions for ADL (Argument Dependent Lookup)
template<typename T>
auto begin(T& list) -> decltype(list.begin()) {
  return list.begin();
}

template<typename T>
auto begin(const T& list) -> decltype(list.begin()) {
  return list.begin();
}

template<typename T>
auto end(T& list) -> decltype(list.end()) {
  return list.end();
}

template<typename T>
auto end(const T& list) -> decltype(list.end()) {
  return list.end();
}

// WASI compatibility for cppgc types
namespace cppgc {
class AllocationHandle {
 public:
  // Minimal stub implementation
};
}

#endif // __wasi__

#endif // SRC_WASI_V8_EXTENSIONS_H_

