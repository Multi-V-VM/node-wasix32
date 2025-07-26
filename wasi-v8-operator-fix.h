#ifndef WASI_V8_OPERATOR_FIX_H_
#define WASI_V8_OPERATOR_FIX_H_

// Fix for V8 Operator Properties flag operations

namespace v8 {
namespace internal {
namespace compiler {

class Operator {
 public:
  enum Property : unsigned char {
    kNoProperties = 0,
    kCommutative = 1 << 0,
    kAssociative = 1 << 1,
    kIdempotent = 1 << 2,
    kNoRead = 1 << 3,
    kNoWrite = 1 << 4,
    kNoThrow = 1 << 5,
    kEliminatable = kNoWrite | kNoThrow,
    kPure = kNoRead | kEliminatable
  };
  
  // Define Properties as a proper flags type
  class Properties {
   public:
    using flag_type = Property;
    using mask_type = unsigned char;
    
    Properties() : value_(0) {}
    Properties(Property p) : value_(static_cast<unsigned char>(p)) {}
    Properties(unsigned char v) : value_(v) {}
    
    // Bitwise operators
    Properties operator&(Property p) const {
      return Properties(value_ & static_cast<unsigned char>(p));
    }
    
    Properties operator|(Property p) const {
      return Properties(value_ | static_cast<unsigned char>(p));
    }
    
    bool operator==(Property p) const {
      return value_ == static_cast<unsigned char>(p);
    }
    
    bool operator==(const Properties& other) const {
      return value_ == other.value_;
    }
    
    operator unsigned char() const { return value_; }
    
   private:
    unsigned char value_;
  };
};

} // namespace compiler
} // namespace internal
} // namespace v8

#endif // WASI_V8_OPERATOR_FIX_H_