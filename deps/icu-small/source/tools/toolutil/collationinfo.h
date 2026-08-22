#ifndef COLLATIONINFO_H
#define COLLATIONINFO_H

#include "unicode/utypes.h"

#if !UCONFIG_NO_COLLATION

U_NAMESPACE_BEGIN

struct CollationData;

class CollationInfo {
 public:
  static void printSizes(int32_t length, const int32_t* indexes);
  static void printReorderRanges(const CollationData& data,
                                 const int32_t* reorderRanges,
                                 int32_t length);

 private:
  static int32_t getDataLength(const int32_t* indexes, int32_t length);
};

U_NAMESPACE_END

#endif  // !UCONFIG_NO_COLLATION

#endif  // COLLATIONINFO_H
