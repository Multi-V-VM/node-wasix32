#ifndef SRC_SPAWN_SYNC_INL_H_
#define SRC_SPAWN_SYNC_INL_H_

#include "spawn_sync.h"

namespace node {

inline unsigned int SyncProcessOutputBuffer::available() const {
  return kBufferSize - used_;
}

inline unsigned int SyncProcessOutputBuffer::used() const {
  return used_;
}

inline size_t SyncProcessOutputBuffer::Copy(char* dest) const {
  memcpy(dest, data_, used_);
  return used_;
}

inline SyncProcessOutputBuffer* SyncProcessOutputBuffer::next() const {
  return next_;
}

inline void SyncProcessOutputBuffer::set_next(SyncProcessOutputBuffer* next) {
  next_ = next;
}

} // namespace node

#endif // SRC_SPAWN_SYNC_INL_H_