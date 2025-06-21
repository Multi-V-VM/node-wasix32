#ifndef INCLUDE_V8_INTERNAL_H_
#define INCLUDE_V8_INTERNAL_H_

#ifdef __wasi__
#include <cstddef>
#include <cstdint>

#include "wasi/concepts-fix.h"

// V8 macro definitions for WASI
#define V8_EXPORT
#define V8_EXPORT_PRIVATE
#define V8_INLINE inline
#define V8_NOINLINE
#define V8_TRIVIAL_ABI
#define V8_LIKELY(x) (x)
#define V8_UNLIKELY(x) (x)
#define V8_WARN_UNUSED_RESULT
#define V8_NODISCARD

namespace v8 {

// Forward declarations
class Object;
class Value;
class Isolate;

namespace internal {

// Forward declaration for Isolate
class Isolate {};

// Fundamental internal types
class Object {};
class Value {};

// Handle management
class IndirectHandleBase {
 public:
  IndirectHandleBase() = default;
  explicit IndirectHandleBase(void* location) {}
  ~IndirectHandleBase() = default;
};

// Helper classes for handles
class HandleHelper {
 public:
  template <typename T, typename S>
  static bool EqualHandles(const T& a, const S& b) {
    return false;
  }
};

class ValueHelper {
 public:
  static bool IsEmpty(const void* handle) { return true; }
  static internal::Address ValueAsAddress(const void* value) { return 0; }
};

// Additional constants
static constexpr int kApiSystemPointerSize = sizeof(void*);

// Internal constants and types
class Internals {
 public:
  static constexpr int kNodeStateIsWeakValue = 1;
  static constexpr int kNodeClassIdOffset = 4;

  static int GetNodeState(void* slot) { return 0; }
};

// Stack frame information
class StackFrame {
 public:
  enum Type { kJavaScript = 0, kWasm = 1, kConstruct = 2 };
};

// JavaScript stack trace
class StackTrace {
 public:
  enum StackTraceOptions {
    kLineNumber = 1,
    kColumnNumber = 2,
    kScriptName = 4,
    kFunctionName = 8,
    kScriptSource = 16,
    kScriptId = 32,
    kExposeFramesAcrossSecurityOrigins = 64,
    kOverview = kLineNumber | kColumnNumber | kScriptName | kFunctionName,
    kDetailed = kOverview | kScriptSource | kScriptId
  };
};

// Address type for WASI (32-bit)
using Address = uint32_t;

// Array buffer allocation info
struct ArrayBufferAllocationInfo {
  void* data = nullptr;
  size_t byte_length = 0;
};

// WASI-specific constants (32-bit safe)
static constexpr int kApiObjectOffset = 0;
static constexpr int kApiDataOffset = 4;
static constexpr int kStringResourceOffset = 8;

// Type traits for V8 objects
template <typename T>
struct ObjectTraits {
  static const bool is_api_object = false;
};

}  // namespace internal

namespace api_internal {

// API internal base class for handles
class IndirectHandleBase {
 public:
  IndirectHandleBase() = default;
  explicit IndirectHandleBase(void* location) : location_(location) {}
  ~IndirectHandleBase() = default;

  // Slot access for persistent handles
  void*& slot() { return location_; }
  void* const& slot() const { return location_; }

 protected:
  void* location_ = nullptr;
};

}  // namespace api_internal

// Template declarations that need to be visible
template <class T, class M = void>
class Persistent;

template <class T>
class MaybeLocal {
 public:
  MaybeLocal() = default;
  template <class S>
  MaybeLocal(Local<S> that) {}

  bool IsEmpty() const { return true; }
  Local<T> ToLocalChecked() const { return Local<T>(); }
  bool ToLocal(Local<T>* out) const { return false; }
};

// Local template is defined in v8-local-handle.h, we don't redefine it here

// Only declare forward declarations for WASI, actual types come from V8 headers

}  // namespace v8

#else

// Non-WASI: Original V8 internal definitions would go here
// For WASI build only, we skip the original complex definitions

#endif  // __wasi__

#endif  // INCLUDE_V8_INTERNAL_H_
