#ifndef WASI_V8_NAMESPACE_FIX_H_
#define WASI_V8_NAMESPACE_FIX_H_

// Fix namespace issues for V8 internal types

// Forward declare LockGuard template
namespace v8 {
namespace base {
template <typename Mutex> class LockGuard;
class Mutex;
using MutexGuard = LockGuard<Mutex>;

template <class T, int shift, int size, class U = uint32_t>
class BitField;

// BitField64 is just BitField with uint64_t
template <class T, int shift, int size>
using BitField64 = BitField<T, shift, size, uint64_t>;
}
}

namespace v8 {
namespace internal {
namespace base {

// Add missing namespace aliases
using Mutex = ::v8::base::Mutex;
using MutexGuard = ::v8::base::MutexGuard;

template <typename T, int shift, int size>
using BitField64 = ::v8::base::BitField64<T, shift, size>;

template <class T, int shift, int size, class U = uint32_t>
using BitField = ::v8::base::BitField<T, shift, size, U>;

// AddressRegion is defined in v8::internal::base namespace in wasi-v8-base-fixes.h
class AddressRegion;

} // namespace base
} // namespace internal
} // namespace v8

// Fix for AtomicWord type conflict
#ifdef AtomicWord
#undef AtomicWord
#endif

namespace v8 {
namespace internal {
namespace base {
using AtomicWord = intptr_t;
} // namespace base
} // namespace internal
} // namespace v8

#endif // WASI_V8_NAMESPACE_FIX_H_