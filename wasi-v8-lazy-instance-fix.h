#ifndef WASI_V8_LAZY_INSTANCE_FIX_H_
#define WASI_V8_LAZY_INSTANCE_FIX_H_

#ifdef __wasi__

// Close any open v8 namespace before we declare our own
}  // close any v8 namespace if open

// This header MUST be included AFTER lazy-instance.h has been processed
// It creates nested namespace aliases for traits defined in lazy-instance.h

// Forward lazy-instance traits to nested namespaces
// Level 1: v8::v8::base -> ::v8::base
namespace v8 {
namespace v8 {
namespace base {

template <typename T> using DefaultConstructTrait = ::v8::base::DefaultConstructTrait<T>;
using ThreadSafeInitOnceTrait = ::v8::base::ThreadSafeInitOnceTrait;
template <typename T> using LazyStaticInstance = ::v8::base::LazyStaticInstance<T, ::v8::base::DefaultConstructTrait<T>, ::v8::base::ThreadSafeInitOnceTrait>;

}  // namespace base
}  // namespace v8
}  // namespace v8

// Level 2: v8::v8::v8::base -> ::v8::base (for triple-nested cases)
namespace v8 {
namespace v8 {
namespace v8 {
namespace base {

template <typename T> using DefaultConstructTrait = ::v8::base::DefaultConstructTrait<T>;
using ThreadSafeInitOnceTrait = ::v8::base::ThreadSafeInitOnceTrait;
template <typename T> using LazyStaticInstance = ::v8::base::LazyStaticInstance<T, ::v8::base::DefaultConstructTrait<T>, ::v8::base::ThreadSafeInitOnceTrait>;

}  // namespace base
}  // namespace v8
}  // namespace v8
}  // namespace v8

// Re-open the v8 namespace if it was closed at the beginning
namespace v8 {

#endif  // __wasi__

#endif  // WASI_V8_LAZY_INSTANCE_FIX_H_
