/* adler32_simd.c
 *
 * Copyright 2017 The Chromium Authors
 * Use of this source code is governed by a BSD-style license that can be
 * found in the Chromium source repository LICENSE file.
 */

#include "zlib.h"
#include "zutil.h"
#include "adler32_simd.h"

/* SIMD implementations for Adler-32 checksums. */

#ifdef ADLER32_SIMD_SSSE3
#  if defined(__GNUC__) || defined(__clang__)
#    define TARGET_CPU_WITH_SSSE3 __attribute__((target("ssse3")))
#  else
#    define TARGET_CPU_WITH_SSSE3
#  endif
#endif

#ifdef ADLER32_SIMD_NEON
#  if defined(__GNUC__) || defined(__clang__)
#    define TARGET_CPU_WITH_NEON __attribute__((target("fpu=neon")))
#  else
#    define TARGET_CPU_WITH_NEON
#  endif
#endif

#ifdef ADLER32_SIMD_RVV
#  if defined(__GNUC__) || defined(__clang__)
#    define TARGET_CPU_WITH_RVV __attribute__((target("arch=+v")))
#  else
#    define TARGET_CPU_WITH_RVV
#  endif
#endif

/* Disable SIMD for WASI */
#ifdef __wasi__
#undef ADLER32_SIMD_SSSE3
#undef ADLER32_SIMD_NEON
#undef ADLER32_SIMD_RVV
#endif

#if defined(ADLER32_SIMD_SSSE3) && !defined(__wasi__)

#include <tmmintrin.h>

uint32_t ZLIB_INTERNAL adler32_simd_(  /* SSSE3 */
    uint32_t adler,
    const unsigned char *buf,
    z_size_t len)
{
    /* SIMD implementation would go here for non-WASI builds */
    return adler32_z(adler, buf, len);
}

#else

/* Fallback implementation for WASI and other platforms */
uint32_t ZLIB_INTERNAL adler32_simd_(
    uint32_t adler,
    const unsigned char *buf,
    z_size_t len)
{
    /* WASI doesn't support SIMD, so return the original adler value
     * The main adler32 function will handle the computation */
    return adler32_z(adler, buf, len);
}

#endif