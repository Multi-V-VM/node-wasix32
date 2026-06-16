// Copyright 2019 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/parsing/literal-buffer.h"

#include "src/base/strings.h"
#include "src/execution/isolate.h"
#include "src/execution/local-isolate.h"
#include "src/heap/factory.h"
#include "src/utils/memcopy.h"

#include <stdio.h>

namespace v8 {
namespace internal {

template <typename IsolateT>
DirectHandle<String> LiteralBuffer::Internalize(IsolateT* isolate) const {
  if (is_one_byte()) {
    return isolate->factory()->InternalizeString(one_byte_literal());
  }
  return isolate->factory()->InternalizeString(two_byte_literal());
}

template DirectHandle<String> LiteralBuffer::Internalize(
    Isolate* isolate) const;
template DirectHandle<String> LiteralBuffer::Internalize(
    LocalIsolate* isolate) const;

int LiteralBuffer::NewCapacity(int min_capacity) {
  return min_capacity < (kMaxGrowth / (kGrowthFactor - 1))
             ? min_capacity * kGrowthFactor
             : min_capacity + kMaxGrowth;
}

void LiteralBuffer::ExpandBuffer() {
  int min_capacity =
      std::max({kInitialCapacity, static_cast<int>(backing_store_.size())});
  int new_capacity = NewCapacity(min_capacity);
#if V8_TARGET_ARCH_WASM32
  static int trace_count = 0;
  if (trace_count < 64 || position_ > min_capacity) {
    fprintf(stderr,
            "LiteralBuffer::ExpandBuffer this=%p pos=%d size=%zu "
            "min=%d new=%d begin=%p one_byte=%d mem_pages=%lu\n",
            static_cast<void*>(this), position_, backing_store_.size(),
            min_capacity, new_capacity,
            static_cast<void*>(backing_store_.begin()), is_one_byte_,
            static_cast<unsigned long>(__builtin_wasm_memory_size(0)));
    fflush(stderr);
  }
  ++trace_count;
#endif
  ZoneVector<uint8_t> new_store = ZoneVector<uint8_t>::New(new_capacity);
#if V8_TARGET_ARCH_WASM32
  if (trace_count <= 64 || position_ > min_capacity) {
    fprintf(stderr,
            "LiteralBuffer::ExpandBuffer allocated this=%p new_begin=%p "
            "new_size=%zu pos=%d old_begin=%p mem_pages=%lu\n",
            static_cast<void*>(this), static_cast<void*>(new_store.begin()),
            new_store.size(), position_,
            static_cast<void*>(backing_store_.begin()),
            static_cast<unsigned long>(__builtin_wasm_memory_size(0)));
    fflush(stderr);
  }
#endif
  if (position_ > 0) {
#if V8_TARGET_ARCH_WASM32
    if (position_ > new_capacity) {
      fprintf(stderr,
              "LiteralBuffer::ExpandBuffer invalid copy pos=%d new=%d "
              "old_size=%zu old_begin=%p new_begin=%p\n",
              position_, new_capacity, backing_store_.size(),
              static_cast<void*>(backing_store_.begin()),
              static_cast<void*>(new_store.begin()));
      fflush(stderr);
    }
#endif
    MemCopy(new_store.begin(), backing_store_.begin(), position_);
  }
  backing_store_.Dispose();
  backing_store_ = std::move(new_store);
}

void LiteralBuffer::ConvertToTwoByte() {
  DCHECK(is_one_byte());
  ZoneVector<uint8_t> new_store;
  int new_content_size = position_ * base::kUC16Size;
  if (new_content_size >= static_cast<int>(backing_store_.size())) {
    // Ensure room for all currently read code units as UC16 as well
    // as the code unit about to be stored.
    new_store = ZoneVector<uint8_t>::New(NewCapacity(new_content_size));
  } else {
    new_store = ZoneVector<uint8_t>(backing_store_.begin(),
                                    backing_store_.size());
  }
  uint8_t* src = backing_store_.begin();
  uint16_t* dst = reinterpret_cast<uint16_t*>(new_store.begin());
  for (int i = position_ - 1; i >= 0; i--) {
    dst[i] = src[i];
  }
  if (new_store.begin() != backing_store_.begin()) {
    backing_store_.Dispose();
    backing_store_ = std::move(new_store);
  }
  position_ = new_content_size;
  is_one_byte_ = false;
}

void LiteralBuffer::AddTwoByteChar(base::uc32 code_unit) {
  DCHECK(!is_one_byte());
  if (position_ >= static_cast<int>(backing_store_.size())) ExpandBuffer();
  if (code_unit <=
      static_cast<base::uc32>(unibrow::Utf16::kMaxNonSurrogateCharCode)) {
    *reinterpret_cast<uint16_t*>(&backing_store_[position_]) = code_unit;
    position_ += base::kUC16Size;
  } else {
    *reinterpret_cast<uint16_t*>(&backing_store_[position_]) =
        unibrow::Utf16::LeadSurrogate(code_unit);
    position_ += base::kUC16Size;
    if (position_ >= static_cast<int>(backing_store_.size())) ExpandBuffer();
    *reinterpret_cast<uint16_t*>(&backing_store_[position_]) =
        unibrow::Utf16::TrailSurrogate(code_unit);
    position_ += base::kUC16Size;
  }
}

}  // namespace internal
}  // namespace v8
