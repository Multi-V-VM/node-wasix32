// Minimal stub for WASI build to avoid section size limits.
#include "collationinfo.h"

#if !UCONFIG_NO_COLLATION

U_NAMESPACE_BEGIN

void CollationInfo::printSizes(int32_t, const int32_t*) {}

void CollationInfo::printReorderRanges(const CollationData&, const int32_t*,
                                       int32_t) {}

int32_t CollationInfo::getDataLength(const int32_t*, int32_t) { return 0; }

U_NAMESPACE_END

#endif  // !UCONFIG_NO_COLLATION
