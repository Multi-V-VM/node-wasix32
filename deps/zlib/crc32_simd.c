/* crc32_simd.c
 *
 * Copyright 2017 The Chromium Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the Chromium source repository LICENSE file.
 *
 * ARM-specific CRC32 using ARMv8 crypto instructions.
 */

#include "crc32_simd.h"

#if defined(CRC32_ARMV8_CRC32)

#if defined(__clang__) || defined(__GNUC__)
#define ZLIB_INTERNAL __attribute__((visibility("hidden")))
#else
#define ZLIB_INTERNAL
#endif

#include <arm_acle.h>

uint32_t ZLIB_INTERNAL armv8_crc32_little(const unsigned char* buf,
                                          z_size_t len,
                                          uint32_t crc) {
    uint32_t c = ~crc;
    const uint32_t *buf4 = (const uint32_t *)buf;

    while (len >= sizeof(uint64_t)) {
        c = __crc32d(c, *(const uint64_t *)buf4);
        buf4 += 2;
        len -= sizeof(uint64_t);
    }

    if (len >= sizeof(uint32_t)) {
        c = __crc32w(c, *buf4++);
        len -= sizeof(uint32_t);
    }

    const unsigned char *buf1 = (const unsigned char *)buf4;
    if (len >= sizeof(uint16_t)) {
        c = __crc32h(c, *(const uint16_t *)buf1);
        buf1 += sizeof(uint16_t);
        len -= sizeof(uint16_t);
    }

    if (len >= sizeof(uint8_t)) {
        c = __crc32b(c, *buf1);
    }

    return ~c;
}

Pos* ZLIB_INTERNAL insert_string_arm(deflate_state* const s,
                                     const Pos str,
                                     uInt count) {
    Pos* ret;
    unsigned int hash;
    Pos prev_head;
    unsigned int h0, h1, byteswap, val;
    
    for (unsigned int i = 0; i < count; i++) {
        INSERT_STRING(s, str + i, hash);
        prev_head = s->head[hash];
        s->head[hash] = str + i;
        ret = &s->prev[(str + i) & s->w_mask];
        *ret = prev_head;
    }
    return ret;
}

#endif  /* CRC32_ARMV8_CRC32 */