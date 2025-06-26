#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef WASI_BITSET_FIX_H
#define WASI_BITSET_FIX_H

#ifdef __wasi__

// Fix bitset namespace conflicts
#include <algorithm>

// Ensure std algorithms are visible in global scope
using std::count;
using std::find;

#endif // __wasi__
#endif // WASI_BITSET_FIX_H
