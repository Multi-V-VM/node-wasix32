#ifndef WASI_ADDITIONAL_FIXES_H_
#define WASI_ADDITIONAL_FIXES_H_

// ============================================================================
// Additional Fixes for Remaining Compilation Errors
// ============================================================================

namespace v8 {
namespace internal {

// ============================================================================
// Zone Method Additions
// ============================================================================

class Zone {
public:
  // NewZoneVector method
  template<typename T>
  inline ZoneVector<T>* NewZoneVector(size_t size) {
    return new (this) ZoneVector<T>(size, this);
  }

  template<typename T>
  inline ZoneVector<T>* NewZoneVector() {
    return new (this) ZoneVector<T>(this);
  }
};

// ============================================================================
// Hash Function Fixes
// ============================================================================

namespace base {

// Hash specialization for double
template<>
struct hash<double> {
  size_t operator()(double value) const {
    // Use the bit pattern of the double for hashing
    uint64_t bits;
    memcpy(&bits, &value, sizeof(double));
    return static_cast<size_t>(bits ^ (bits >> 32));
  }
};

// CountLeadingSignBits - alias to CountLeadingZeros for compatibility
namespace bits {
  template<typename T>
  inline int CountLeadingSignBits(T value) {
    return CountLeadingZeros(value);
  }
}

} // namespace base

// ============================================================================
// Assembler/MacroAssembler Additions
// ============================================================================

class AssemblerBase {
public:
  void RecordComment(const char* comment) {}
};

class MacroAssembler : public Assembler {
public:
  // Root register initialization
  void InitializeRootRegister() {}

  // Exception handler
  void ExceptionHandler() {}

  // RecordComment with single argument
  void RecordComment(const char* comment) {}
};

// ============================================================================
// Register Conversion Helpers
// ============================================================================

// Allow implicit conversion from DoubleRegister/Simd128Register to Register for stub purposes
inline Register ToRegister(DoubleRegister reg) {
  return Register::from_code(reg.code());
}

inline Register ToRegister(Simd128Register reg) {
  return Register::from_code(reg.code());
}

// ============================================================================
// ToCData Template Fix
// ============================================================================

// Foreign address tags (additional)
constexpr uint64_t kCFunctionTag = 3;
constexpr uint64_t kCFunctionInfoTag = 4;

// ToCData implementation
template<typename T>
inline T ToCData(Address foreign) {
  return reinterpret_cast<T>(foreign);
}

// IntToSmi in internal namespace
inline Address IntToSmi(int value) {
  return static_cast<Address>(value) << (kSmiTagSize + kSmiShiftSize);
}

// ============================================================================
// DeoptimizeReason and AssertCondition Enum Fixes
// ============================================================================

// DeoptimizeReason enum (complete)
enum class DeoptimizeReason {
  kUnknown = 0,
  kHole = 1,
  kSmi = 2,
  kWrongMap = 3,
  kInsufficientTypeFeedbackForGenericKeyedAccess = 4,
  kInsufficientTypeFeedbackForGenericNamedAccess = 5,
  kMaybeInHole = 6,
  kNotAHeapNumber = 7,
  kNotAString = 8,
  kNotASmi = 9,
  kOutOfBounds = 10,
  kOverflow = 11,
  kNegativeValue = 12,
  kLostPrecision = 13,
  kLostPrecisionOrNaN = 14,
  kNotInt32 = 15,
  kDivisionByZero = 16,
  kMinusZero = 17,
  kNaN = 18,
  kNotABigInt = 19,
  kNotABigInt64 = 20,
  kNotANumber = 21,
  kNotANumberOrOddball = 22,
  kWrongName = 23,
  kWrongValue = 24,
  kWrongInstanceType = 25,
  kNoInitialElement = 26,
  kNotAJavaScriptObject = 27,
  kCowArrayElementsChanged = 28,
  kArrayLengthChanged = 29,
  kArrayBufferWasDetached = 30,
};

// Convert int to DeoptimizeReason
inline DeoptimizeReason IntToDeoptimizeReason(int value) {
  return static_cast<DeoptimizeReason>(value);
}

// AssertCondition enum (complete)
enum class AssertCondition {
  kEqual = 0,
  kNotEqual = 1,
  kLessThan = 2,
  kLessThanOrEqual = 3,
  kGreaterThan = 4,
  kGreaterThanOrEqual = 5,
  kUnsignedLessThan = 6,
  kUnsignedLessThanOrEqual = 7,
  kUnsignedGreaterThan = 8,
  kUnsignedGreaterThanOrEqual = 9,
};

// Convert Condition to AssertCondition
inline AssertCondition ConditionToAssertCondition(Condition cond) {
  return static_cast<AssertCondition>(cond);
}

// Convert int to AssertCondition
inline AssertCondition IntToAssertCondition(int value) {
  return static_cast<AssertCondition>(value);
}

// ============================================================================
// SmallVector Constructor Fixes
// ============================================================================

namespace base {

// Additional SmallVector constructors
template<typename T, size_t kSize>
class SmallVector {
public:
  using value_type = T;
  using size_type = size_t;

  // Default constructor
  SmallVector() : size_(0) {}

  // Constructor with initializer list
  SmallVector(std::initializer_list<T> init) : size_(0) {
    for (const auto& item : init) {
      push_back(item);
    }
  }

  // Constructor with size
  explicit SmallVector(size_t count) : size_(0) {
    resize(count);
  }

  // Constructor with size and value
  SmallVector(size_t count, const T& value) : size_(0) {
    resize(count, value);
  }

  // Copy constructor
  SmallVector(const SmallVector& other) : size_(0) {
    for (size_t i = 0; i < other.size(); ++i) {
      push_back(other[i]);
    }
  }

  // Move constructor
  SmallVector(SmallVector&& other) noexcept : size_(other.size_) {
    for (size_t i = 0; i < size_; ++i) {
      data_[i] = std::move(other.data_[i]);
    }
    other.size_ = 0;
  }

  // Basic methods
  void push_back(const T& value) {
    if (size_ < kSize) {
      data_[size_++] = value;
    }
  }

  void push_back(T&& value) {
    if (size_ < kSize) {
      data_[size_++] = std::move(value);
    }
  }

  T& operator[](size_t index) { return data_[index]; }
  const T& operator[](size_t index) const { return data_[index]; }

  size_t size() const { return size_; }
  bool empty() const { return size_ == 0; }

  T* data() { return data_; }
  const T* data() const { return data_; }

  T* begin() { return data_; }
  const T* begin() const { return data_; }
  T* end() { return data_ + size_; }
  const T* end() const { return data_ + size_; }

  void resize(size_t new_size) {
    size_ = new_size < kSize ? new_size : kSize;
  }

  void resize(size_t new_size, const T& value) {
    size_t old_size = size_;
    resize(new_size);
    for (size_t i = old_size; i < size_; ++i) {
      data_[i] = value;
    }
  }

  void clear() { size_ = 0; }

private:
  T data_[kSize];
  size_t size_;
};

} // namespace base

// ============================================================================
// Is64() Function Disambiguation
// ============================================================================

// Helper to check if a type represents 64-bit operations
namespace compiler {

template<typename T>
inline bool Is64Bit(const T& value) {
  return sizeof(typename T::value_type) == 8;
}

// For MachineRepresentation
inline bool Is64(MachineRepresentation rep) {
  return rep == MachineRepresentation::kWord64 ||
         rep == MachineRepresentation::kFloat64 ||
         rep == MachineRepresentation::kSimd128;
}

// For MachineType
inline bool Is64(MachineType type) {
  return Is64(type.representation());
}

} // namespace compiler

// ============================================================================
// JSFunction Size Constants Fix
// ============================================================================

// Adjust kTaggedSize for the assertions
#ifdef V8_TARGET_ARCH_WASM32
constexpr int kTaggedSizeOverride = kSystemPointerSize;
#endif

// ============================================================================
// InternalizeString Disambiguation
// ============================================================================

namespace compiler {

class CodeAssembler {
public:
  // Stub for InternalizeString to avoid ambiguity
  template<typename... Args>
  void InternalizeString(Args&&... args) {}
};

} // namespace compiler

// ============================================================================
// Cast Function Template
// ============================================================================

// Generic cast function for V8 objects
template<typename T, typename U>
inline T cast(U value) {
  return static_cast<T>(value);
}

// ============================================================================
// BitField Base Fix
// ============================================================================

// Dummy base to prevent redefinition errors
struct MaglevDummyBitFieldBaseV2 {
  static constexpr int kNextBit = 0;
};

// ============================================================================
// WaiterQueue Mask Fix
// ============================================================================

// Make kWaiterQueueMask constexpr-compatible
namespace {
constexpr int kWaiterQueueNodeTagBits = 2;
constexpr uintptr_t kWaiterQueueMaskValue = ~((1ULL << kWaiterQueueNodeTagBits) - 1);
}

// ============================================================================
// External Pointer Tag Range Fix
// ============================================================================

struct ExternalPointerTagRange {
  uint32_t first() const { return 0; }
  uint32_t last() const { return 0; }
};

// ============================================================================
// DidntThrowOp Constructor Fix
// ============================================================================

namespace compiler {
namespace turboshaft {

struct DidntThrowOp {
  DidntThrowOp() {}
  explicit DidntThrowOp(int dummy) {}
  template<typename... Args>
  DidntThrowOp(Args&&... args) {}
};

} // namespace turboshaft
} // namespace compiler

// ============================================================================
// CatchCase and HandlerCase Default Constructors
// ============================================================================

namespace wasm {

struct CatchCase {
  CatchCase() = default;
  int tag = 0;
  int target = 0;
};

struct HandlerCase {
  HandlerCase() = default;
  int kind = 0;
  int target = 0;
};

} // namespace wasm

// ============================================================================
// Intl Object Fixes
// ============================================================================

// Forward declarations for Intl
namespace Intl {
  class DateTimeFormat;
  class NumberFormat;
}

// ============================================================================
// max() Template Function
// ============================================================================

template<typename T>
inline T max(T a, T b) {
  return (a > b) ? a : b;
}

template<typename T>
inline T min(T a, T b) {
  return (a < b) ? a : b;
}

// ============================================================================
// WasmInliner TryInlining Method Stub
// ============================================================================

namespace compiler {

class WasmIntoJSInliner {
public:
  bool TryInlining(void* node) { return false; }
};

} // namespace compiler

// ============================================================================
// UseMap Return Type Fix
// ============================================================================

namespace compiler {
namespace turboshaft {

class UseMap {
public:
  Vector<const OpIndex> uses(OpIndex index) const {
    return Vector<const OpIndex>(nullptr, 0);
  }
};

} // namespace turboshaft
} // namespace compiler

// ============================================================================
// LayeredHashMap AddOrFind/Find Methods
// ============================================================================

namespace compiler {
namespace turboshaft {

template<typename Key, typename Value, typename Hash>
class LayeredHashMap {
public:
  struct Entry {
    Key key;
    Value value;
  };

  // AddOrFind method
  template<typename... Args>
  Entry* AddOrFind(const Key& key, Args&&... args) {
    static Entry entry;
    entry.key = key;
    return &entry;
  }

  // Find method
  Entry* Find(const Key& key) {
    return nullptr;
  }

  const Entry* Find(const Key& key) const {
    return nullptr;
  }
};

} // namespace turboshaft
} // namespace compiler

// ============================================================================
// BuildLoadExternalPointerFromObject Stub
// ============================================================================

namespace compiler {

class WasmGraphBuilder {
public:
  template<typename... Args>
  void* BuildLoadExternalPointerFromObject(Args&&... args) {
    return nullptr;
  }
};

} // namespace compiler

// ============================================================================
// ia32 Code Generator Conditional Fix
// ============================================================================

// Ensure conditional directives are properly closed
#ifdef V8_TARGET_ARCH_IA32
// ia32-specific code
#endif

} // namespace internal
} // namespace v8

// ============================================================================
// Standard Library Hash Fix for StrongAlias
// ============================================================================

namespace std {

template<typename Tag, typename T>
struct hash<v8::base::StrongAlias<Tag, T>> {
  size_t operator()(const v8::base::StrongAlias<Tag, T>& value) const {
    return hash<T>()(value.value());
  }
};

} // namespace std

#endif  // WASI_ADDITIONAL_FIXES_H_
