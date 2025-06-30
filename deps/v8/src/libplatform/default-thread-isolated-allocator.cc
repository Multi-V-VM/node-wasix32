// Copyright 2023 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/libplatform/default-thread-isolated-allocator.h"
#include <stdlib.h>

#ifdef __wasi__
#define UNREACHABLE() __builtin_unreachable()
#else
#include "src/base/logging.h"
#endif

#if V8_HAS_PKU_JIT_WRITE_PROTECT

#if !V8_OS_LINUX
#error pkey support in this file is only implemented on Linux
#endif

#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/utsname.h>
#include <unistd.h>
#endif

#if V8_HAS_PKU_JIT_WRITE_PROTECT

extern int pkey_alloc(unsigned int flags, unsigned int access_rights) V8_WEAK;
extern int pkey_free(int pkey) V8_WEAK;

namespace {

bool KernelHasPkruFix() {
  // PKU was broken on Linux kernels before 5.13 (see
  // https://lore.kernel.org/all/20210623121456.399107624@linutronix.de/).
  // A fix is also included in the 5.4.182 and 5.10.103 versions ("x86/fpu:
  // Correct pkru/xstate inconsistency" by Brian Geffon <bgeffon@google.com>).
  // Thus check the kernel version we are running on, and bail out if does not
  // contain the fix.
  struct utsname uname_buffer;
  CHECK_EQ(0, uname(&uname_buffer));
  int kernel, major, minor;
  // Conservatively return if the release does not match the format we expect.
  if (sscanf(uname_buffer.release, "%d.%d.%d", &kernel, &major, &minor) != 3) {
    return false;
  }

  return kernel > 5 || (kernel == 5 && major >= 13) ||   // anything >= 5.13
         (kernel == 5 && major == 4 && minor >= 182) ||  // 5.4 >= 5.4.182
         (kernel == 5 && major == 10 && minor >= 103);   // 5.10 >= 5.10.103
}

int PkeyAlloc() {
#ifdef PKEY_DISABLE_WRITE
  if (!pkey_alloc) return -1;

  static bool kernel_has_pkru_fix = KernelHasPkruFix();
  if (!kernel_has_pkru_fix) return -1;

  return pkey_alloc(0, PKEY_DISABLE_WRITE);
#else  // PKEY_DISABLE_WRITE
  return -1;
#endif
}

int PkeyFree(int pkey) {
  DCHECK(pkey_free);
  return pkey_free(pkey);
}

}  // namespace

#endif  // V8_HAS_PKU_JIT_WRITE_PROTECT

namespace v8::platform {

// Constructor and destructor are defined in the header as = default

// Destructor is defined in the header as = default

// TODO(sroettger): this should return thread isolated (e.g. pkey-tagged) memory
//                  for testing.
void* DefaultThreadIsolatedAllocator::Allocate(size_t size) {
  return malloc(size);
}

void DefaultThreadIsolatedAllocator::Free(void* object) { ::free(object); }

// GetType() is defined inline in the header

// Pkey() removed - not in interface

// Valid() removed - not in interface

}  // namespace v8::platform
