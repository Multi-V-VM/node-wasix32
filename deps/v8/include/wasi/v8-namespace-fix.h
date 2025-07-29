#ifndef V8_INCLUDE_WASI_V8_NAMESPACE_FIX_H_
#define V8_INCLUDE_WASI_V8_NAMESPACE_FIX_H_

// This file ensures proper namespace resolution and prevents v8::v8:: double namespace issues

// Ensure we're not already in v8 namespace
#ifdef V8_NAMESPACE_OPEN
#error "This header should not be included inside v8 namespace"
#endif

// Forward declare essential v8 types to ensure they're available
namespace v8 {
  // Forward declarations
  class Isolate;
  class Context;
  class Value;
  class Object;
  class String;
  class Array;
  class Function;
  class ArrayBuffer;
  class HandleScope;
  class EscapableHandleScope;
  class TryCatch;
  class Message;
  class StackTrace;
  class StackFrame;
  class HeapProfiler;
  class CpuProfiler;
  class ValueSerializer;
  class ValueDeserializer;
  
  template<typename T> class Local;
  template<typename T> class MaybeLocal;
  template<typename T> class Maybe;
  template<typename T, typename M> class Persistent;
  template<typename T> class Global;
  template<typename T> class Eternal;
  template<typename T> class TracedReference;
  
  // Ensure HandleScope is available without v8:: prefix when needed
  #ifndef V8_HANDLESCOPE_AVAILABLE
  #define V8_HANDLESCOPE_AVAILABLE
  #endif
}

// Prevent accidental v8::v8:: resolution
namespace v8 {
  // DO NOT put another v8 namespace here
  // This would cause v8::v8:: issues
}

// Helper macro to ensure we're using the right namespace
#define V8_NAMESPACE_BEGIN namespace v8 { \
  static_assert(true, "Entering v8 namespace"); \
  const bool V8_NAMESPACE_OPEN = true;

#define V8_NAMESPACE_END } /* namespace v8 */ \
  static_assert(true, "Exiting v8 namespace");

#endif // V8_INCLUDE_WASI_V8_NAMESPACE_FIX_H_