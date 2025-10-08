#ifndef WASI_ABSEIL_NAMESPACE_FIX_H_
#define WASI_ABSEIL_NAMESPACE_FIX_H_

#ifdef __wasi__

// NOTE: We do NOT create v8::std namespace forwarding here because it causes
// system headers (iomanip, chrono, mutex, etc.) to fail when they use std::
// and accidentally resolve to v8::std instead of ::std.
//
// Instead, all code in Abseil and V8 that uses std:: must use global
// qualification (::std::) to bypass nested namespace lookup.
//
// The original intent was to help abseil code find std when included in v8
// namespace blocks, but the side effects are too severe.

#endif  // __wasi__

#endif  // WASI_ABSEIL_NAMESPACE_FIX_H_
