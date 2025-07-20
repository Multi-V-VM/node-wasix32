#ifdef __wasi__
namespace v8 {
namespace base {
namespace bits {

// Minimal stub implementations for WASI
int CountLeadingZeros(uint32_t value) {
  return __builtin_clz(value);
}

int CountLeadingZeros(uint64_t value) {
  return __builtin_clzll(value);
}

int CountTrailingZeros(uint32_t value) {
  return __builtin_ctz(value);
}

int CountTrailingZeros(uint64_t value) {
  return __builtin_ctzll(value);
}

int CountPopulation(uint32_t value) {
  return __builtin_popcount(value);
}

int CountPopulation(uint64_t value) {
  return __builtin_popcountll(value);
}

}  // namespace bits
}  // namespace base
}  // namespace v8
#endif
