#include "stubdata.h"

#ifndef __wasi__

extern "C" U_EXPORT const ICU_Data_Header U_ICUDATA_ENTRY_POINT alignas(16) = {
    32,
    0xda,
    0x27,
    {sizeof(UDataInfo),
     0,
#if U_IS_BIG_ENDIAN
     1,
#else
     0,
#endif
     U_CHARSET_FAMILY,
     sizeof(char16_t),
     0,
     {0x54, 0x6f, 0x43, 0x50},
     {1, 0, 0, 0},
     {0, 0, 0, 0}},
    {'s', 't', 'u', 'b', 'd', 'a', 't', 'a'},
    0,
    0,
    {0, 0}};

#endif  // !__wasi__
