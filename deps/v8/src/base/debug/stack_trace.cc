// Copyright 2016 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/base/debug/stack_trace.h"

#include <string.h>

#include <algorithm>
#include <sstream>

#include "src/base/macros.h"

::v8::base::debug::StackTrace::StackTrace(const void* const* trace, size_t count) {
  count = std::min(count, arraysize(trace_));
  if (count) memcpy(trace_, trace, count * sizeof(trace_[0]));
  count_ = count;
}

::v8::base::debug::StackTrace::~StackTrace() = default;

const void* const* ::v8::base::debug::StackTrace::Addresses(size_t* count) const {
  *count = count_;
  if (count_) return trace_;
  return nullptr;
}

std::string ::v8::base::debug::StackTrace::ToString() const {
  std::stringstream stream;
  OutputToStream(&stream);
  return stream.str();
}
