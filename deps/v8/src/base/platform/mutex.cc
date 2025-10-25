// Copyright 2013 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/base/platform/mutex.h"

#include "src/base/platform/platform.h"

namespace v8::base {

// Define the lightweight WASI platform data type when targeting WASI.
#ifdef __wasi__
class Mutex::PlatformData {
 public:
  PlatformData() {}
};
#endif

RecursiveMutex::~RecursiveMutex() { DCHECK_EQ(0, level_); }

void RecursiveMutex::Lock() {
  int own_id = ::v8::base::OS::GetCurrentThreadId();
  if (thread_id_ == own_id) {
    level_++;
    return;
  }
  mutex_.Lock();
  DCHECK_EQ(0, level_);
  thread_id_ = own_id;
  level_ = 1;
}

void RecursiveMutex::Unlock() {
#ifdef DEBUG
  int own_id = ::v8::base::OS::GetCurrentThreadId();
  CHECK_EQ(thread_id_, own_id);
#endif
  if ((--level_) == 0) {
    thread_id_ = 0;
    mutex_.Unlock();
  }
}

bool RecursiveMutex::TryLock() {
  int own_id = ::v8::base::OS::GetCurrentThreadId();
  if (thread_id_ == own_id) {
    level_++;
    return true;
  }
  if (mutex_.TryLock()) {
    DCHECK_EQ(0, level_);
    thread_id_ = own_id;
    level_ = 1;
    return true;
  }
  return false;
}

Mutex::Mutex() {
#ifdef DEBUG
  level_ = 0;
#endif
#ifdef __wasi__
  // Allocate lightweight WASI platform data
  data_ = new PlatformData();
#endif
}

Mutex::~Mutex() {
  DCHECK_EQ(0, level_);
#ifdef __wasi__
  delete data_;
#endif
}

void Mutex::Lock() ABSL_NO_THREAD_SAFETY_ANALYSIS {
#ifdef __wasi__
  // No-op lock for WASI stub; maintain debug state semantics.
  AssertUnheldAndMark();
#else
  native_handle_.Lock();
  AssertUnheldAndMark();
#endif
}

void Mutex::Unlock() ABSL_NO_THREAD_SAFETY_ANALYSIS {
#ifdef __wasi__
  // No-op unlock for WASI stub; maintain debug state semantics.
  AssertHeldAndUnmark();
#else
  AssertHeldAndUnmark();
  native_handle_.Unlock();
#endif
}

bool Mutex::TryLock() ABSL_NO_THREAD_SAFETY_ANALYSIS {
#ifdef __wasi__
  // Always succeeds in WASI stub; maintain debug state semantics.
  AssertUnheldAndMark();
  return true;
#else
  if (!native_handle_.TryLock()) return false;
  AssertUnheldAndMark();
  return true;
#endif
}

}  // namespace v8::base
