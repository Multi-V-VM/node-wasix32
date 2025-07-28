#ifndef WASI_V8_INTERNALS_CONSTANTS_H_
#define WASI_V8_INTERNALS_CONSTANTS_H_

// Define missing Internals constants for WASI build
namespace v8 {
namespace internal {

// Only define ValueHelper if not already defined
#ifndef V8_VALUE_HELPER_DEFINED
#define V8_VALUE_HELPER_DEFINED
// ValueHelper class for internal value conversions
class ValueHelper {
 public:
  using InternalRepresentationType = uintptr_t;
  
  static constexpr InternalRepresentationType kEmpty = 0;
  
  static InternalRepresentationType ValueAsAddress(const void* value) {
    return reinterpret_cast<InternalRepresentationType>(value);
  }
  
  template<typename T>
  static InternalRepresentationType ValueAsAddress(const T* value) {
    return reinterpret_cast<InternalRepresentationType>(value);
  }
};
#endif // V8_VALUE_HELPER_DEFINED

// Only define Internals if not already defined
#ifndef V8_INTERNALS_DEFINED
#define V8_INTERNALS_DEFINED
class Internals {
 public:
  // Instance type constants that are referenced in instance-type.h
  static constexpr int kJSObjectType = 0x421;  
  static constexpr int kFirstJSApiObjectType = 0x422;
  static constexpr int kLastJSApiObjectType = 0x423;
  static constexpr int kJSSpecialApiObjectType = 0x424;
  static constexpr int kFirstNonstringType = 0x80;
  static constexpr int kOddballType = 0x81;
  static constexpr int kForeignType = 0x82;
  
  // Other constants that might be needed
  static constexpr int kUndefinedOddballKind = 5;
  static constexpr int kNullOddballKind = 3;
  static constexpr int kTrueOddballKind = 1;
  static constexpr int kFalseOddballKind = 0;
  static constexpr int kEmptyStringRootIndex = 0;
  
  // Method needed for v8-metrics.h
  static void IncrementLongTasksStatsCounter(v8::Isolate* isolate) {}
  
  // Methods for v8-initialization.h
  static bool PointerCompressionIsEnabled() { return false; }
  static bool SandboxIsEnabled() { return false; }
};
#endif // V8_INTERNALS_DEFINED

// Define these constants at the global scope as well for compatibility
// These will be defined if Internals class above was defined
#ifdef V8_INTERNALS_DEFINED
constexpr int JS_OBJECT_TYPE = Internals::kJSObjectType;
constexpr int FIRST_JS_API_OBJECT_TYPE = Internals::kFirstJSApiObjectType;
constexpr int LAST_JS_API_OBJECT_TYPE = Internals::kLastJSApiObjectType;
constexpr int JS_SPECIAL_API_OBJECT_TYPE = Internals::kJSSpecialApiObjectType;
constexpr int FIRST_NONSTRING_TYPE = Internals::kFirstNonstringType;
constexpr int ODDBALL_TYPE = Internals::kOddballType;
constexpr int FOREIGN_TYPE = Internals::kForeignType;
#endif

// Forward declarations for register types
class DoubleRegister;

// Define DoubleRegList as RegListBase<DoubleRegister>
template<typename RegisterType>
class RegListBase;

using DoubleRegList = RegListBase<DoubleRegister>;

} // namespace internal
} // namespace v8

#endif // WASI_V8_INTERNALS_CONSTANTS_H_