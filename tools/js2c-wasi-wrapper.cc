// WASI-compatible wrapper for js2c tool
#ifdef __wasi__
#define concept bool
#define requires(...) true ||
#include "wasi/wasi-compat.h"
#endif

// Include the original js2c
#include "js2c.cc"
