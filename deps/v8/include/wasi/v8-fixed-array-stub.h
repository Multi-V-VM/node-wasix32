#ifndef V8_INCLUDE_WASI_V8_FIXED_ARRAY_STUB_H_
#define V8_INCLUDE_WASI_V8_FIXED_ARRAY_STUB_H_

#ifdef __wasi__

// This file should be included AFTER v8.h or v8-data.h is included
// to ensure Data and Local classes are properly defined

namespace v8 {

// WASI implementation of FixedArray
#ifndef V8_FIXED_ARRAY_DEFINED
#define V8_FIXED_ARRAY_DEFINED
class FixedArray : public Data {
 public:
  int Length() const { return 0; }
  // Return Local<Data> for compatibility with V8 API
  Local<Data> Get(Local<Context> context, int index) const {
    (void)context; (void)index;
    return Local<Data>();
  }
  V8_INLINE static FixedArray* Cast(Data* data) {
    return reinterpret_cast<FixedArray*>(data);
  }
  // WASI: Add IsValue method for compatibility
  bool IsValue() const { return false; }
};
#endif // V8_FIXED_ARRAY_DEFINED

} // namespace v8

#endif // __wasi__

#endif // V8_INCLUDE_WASI_V8_FIXED_ARRAY_STUB_H_