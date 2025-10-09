#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef WASI_BITSET_FIX_H
#define WASI_BITSET_FIX_H

#ifdef __wasi__

// Close any open v8 namespace before including standard library headers
}  // close any v8 namespace if open

// Fix bitset namespace conflicts
#include <algorithm>

// Ensure std algorithms are visible in global scope
using std::count;
using std::find;

// Re-open the v8 namespace if it was closed at the beginning
namespace v8 {

#endif // __wasi__
#endif // WASI_BITSET_FIX_H
