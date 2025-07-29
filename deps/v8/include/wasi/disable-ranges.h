#ifndef WASI_DISABLE_RANGES_H_
#define WASI_DISABLE_RANGES_H_

// Disable C++20 ranges features that are incompatible with WASI
#define _LIBCPP_HAS_NO_CONCEPTS
#define _LIBCPP_HAS_NO_RANGES

#endif // WASI_DISABLE_RANGES_H_
