#ifndef V8_BASE_ENUM_SET_FIX_H_
#define V8_BASE_ENUM_SET_FIX_H_

namespace v8 {
namespace base {

template<typename E, typename T = uint64_t>
class EnumSet {
 public:
  EnumSet() : bits_(0) {}
  explicit EnumSet(T bits) : bits_(bits) {}
  
  bool contains(E element) const {
    return (bits_ & (1ULL << static_cast<int>(element))) != 0;
  }
  
  void Add(E element) {
    bits_ |= (1ULL << static_cast<int>(element));
  }
  
  void Remove(E element) {
    bits_ &= ~(1ULL << static_cast<int>(element));
  }
  
  bool empty() const { return bits_ == 0; }
  
  static EnumSet All() { return EnumSet(~T(0)); }
  static EnumSet None() { return EnumSet(0); }
  
 private:
  T bits_;
};

} // namespace base
} // namespace v8

#endif
