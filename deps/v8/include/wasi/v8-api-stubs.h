#ifndef V8_INCLUDE_WASI_V8_API_STUBS_H_
#define V8_INCLUDE_WASI_V8_API_STUBS_H_

// This file stubs out missing V8 API types for WASI compilation
// These are defined OUTSIDE of any class to be used as needed

#include <cstddef>
#include <cstdint>

namespace v8 {

// Memory pressure level enum
enum class MemoryPressureLevel {
  kNone,
  kModerate,
  kCritical
};

// MessageErrorLevel enum for inspector
enum MessageErrorLevel {
  kMessageLog = 0,
  kMessageDebug = 1,
  kMessageInfo = 2,
  kMessageWarning = 3,
  kMessageError = 4
};

// StackState - forward declare and alias
// The actual definition is in cppgc/heap.h
#ifndef V8_STACKSTATE_DEFINED
#define V8_STACKSTATE_DEFINED
enum class StackState {
  kMayContainHeapPointers,
  kNoHeapPointers
};
#endif

}  // namespace v8

#endif  // V8_INCLUDE_WASI_V8_API_STUBS_H_
