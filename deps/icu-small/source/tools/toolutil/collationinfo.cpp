// Minimal stub for WASI build to avoid section size limits
#include "unicode/utypes.h"

#if !UCONFIG_NO_COLLATION

// Forward declare to avoid including heavy headers
struct CollationData;

class CollationInfo {
public:
    static void printSizes(int32_t, const int32_t*) {}
    static void printReorderRanges(const CollationData&, const int32_t*, int32_t) {}
private:
    static int32_t getDataLength(const int32_t*, int32_t) { return 0; }
};

#endif