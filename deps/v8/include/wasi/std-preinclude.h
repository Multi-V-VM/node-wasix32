#ifndef V8_WASI_STD_PREINCLUDE_H_
#define V8_WASI_STD_PREINCLUDE_H_

#if defined(__wasi__)
// Intentionally empty. Do not include any std or shim headers here because
// this file is sometimes included from within `namespace v8` contexts via
// other headers. Including standard headers here would risk creating
// nested v8::std symbols. Translation units should include the standard
// library at global scope as needed.
#endif  // defined(__wasi__)

#endif  // V8_WASI_STD_PREINCLUDE_H_
