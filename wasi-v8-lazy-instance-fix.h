#ifndef WASI_V8_LAZY_INSTANCE_FIX_H_
#define WASI_V8_LAZY_INSTANCE_FIX_H_

#ifdef __wasi__

// Previously this header created forwarding aliases for lazy-instance traits
// inside nested `namespace v8` scopes. With the cleaned include ordering for the
// WASI configuration the V8 base headers are now included at global scope, so
// the extra aliases are unnecessary. The file remains as a no-op stub so
// existing include lists stay intact.

#endif  // __wasi__

#endif  // WASI_V8_LAZY_INSTANCE_FIX_H_
