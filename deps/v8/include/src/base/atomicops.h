// Only include V8 internal atomicops when compiling V8 source code itself
#ifdef V8_EXPORT_PRIVATE
#include "../../../src/base/atomicops.h"
#endif

