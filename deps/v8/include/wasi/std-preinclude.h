#ifndef V8_WASI_STD_PREINCLUDE_H_
#define V8_WASI_STD_PREINCLUDE_H_

#if defined(__wasi__)

// Preload the standard library while we are still at global scope. Some V8
// headers are pulled in from within namespace v8 blocks, and including the
// standard library for the first time in that context pushes libc++ symbols
// into v8::std.  By force-including this header from the compiler command line
// we guarantee libc++ has already been instantiated in ::std (and friends)
// before any V8 header is parsed.

// Pull in the key WASI compatibility shims up front so they are processed while
// we are still in the global namespace. Subsequent includes inside namespace v8
// become no-ops thanks to include guards, preventing accidental creation of
// v8::std shadows.
#include "wasi/concepts-fix.h"
// Also bring in base/once.h very early so that ::v8::Once is defined at global
// scope even if later headers are included from within `namespace v8` blocks.
#include "src/base/once.h"

#include <algorithm>
#include <array>
#include <atomic>
#include <bit>
#include <chrono>
#include <deque>
#include <functional>
#include <iomanip>
#include <ios>
#include <iosfwd>
#include <iostream>
#include <istream>
#include <iterator>
#include <cstring>
#include <string.h>
#include <cwchar>
#include <cmath>
#include <math.h>
#include <limits>
#include <locale>
#include <map>
#include <memory>
#include <mutex>
#include <list>
#include <forward_list>
#include <new>
#include <optional>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
// Pull in filesystem early to avoid it being first included from within
// a nested namespace (e.g. inside namespace v8), which would create v8::std
// shadows in libc++ headers on WASI.
#include <filesystem>

#endif  // defined(__wasi__)

#endif  // V8_WASI_STD_PREINCLUDE_H_
