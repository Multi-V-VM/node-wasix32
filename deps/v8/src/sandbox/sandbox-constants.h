
#ifdef V8_TARGET_ARCH_WASM32
// Adjust sandbox constants for WASI 32-bit
#undef kMaxJSDispatchEntries
#define kMaxJSDispatchEntries kMaxCapacity
#undef kTrustedPointerTableEntrySize
#define kTrustedPointerTableEntrySize sizeof(TrustedPointerTableEntry)
#endif
