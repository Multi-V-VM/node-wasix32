#ifdef __wasi__
#include "wasi/concepts-fix.h"
#endif
#ifndef V8_INCLUDE_WASI_V8_WASI_COMPAT_H_
#define V8_INCLUDE_WASI_V8_WASI_COMPAT_H_

#ifdef __wasi__

// 防止重复定义的保护宏
#ifndef V8_WASI_CONSTANTS_DEFINED
#define V8_WASI_CONSTANTS_DEFINED

#include <cstddef>
#include <cstdint>
#include <limits>

// WASI 平台基础常量
constexpr int kCppHeapPointerTagShift = 1;
constexpr int kJSAPIObjectWithEmbedderSlotsHeaderSize = 16;
constexpr int kEmbedderDataSlotSize = 8;

// Smi 相关常量（32位 WASI 系统）
constexpr int kSmiTagSize = 1;
constexpr int kSmiShiftSize = 0;
constexpr int kSmiValueSize = 31;
constexpr int kSmiTag = 0;

// 指针和标记大小常量
constexpr int kTaggedSize = 4;  // 32位指针
constexpr int kSystemPointerSize = 4;
constexpr int kSystemPointerSizeLog2 = 2;
constexpr int kTaggedSizeLog2 = 2;

// API 常量
constexpr int kApiTaggedSize = 8;  // API 层使用不同的大小
constexpr int kApiSystemPointerSize = 4;

// 堆对象常量
constexpr int kHeapObjectTag = 1;
constexpr int kObjectAlignmentBits = 2;
constexpr intptr_t kObjectAlignment = 4;
constexpr intptr_t kObjectAlignmentMask = 3;

// 外部分配限制
constexpr size_t kExternalAllocationSoftLimit = 64 * 1024 * 1024;

// V8 内部类型定义
namespace v8 {
namespace internal {

using Address = uint32_t;
using ExternalPointer_t = uint32_t;
using CppHeapPointer_t = uint32_t;
using IndirectPointerHandle = uint32_t;

// Smi 工具函数
constexpr bool SmiValuesAre31Bits() { return kSmiValueSize == 31; }
constexpr bool SmiValuesAre32Bits() { return kSmiValueSize == 32; }

// 内部常量类（仅声明，避免重复定义）
class Internals {
 public:
  static constexpr int kNodeStateIsWeakValue = 1;
  static constexpr int kNodeClassIdOffset = 4;
  static constexpr int kEmptyStringRootIndex = 0;
  static constexpr int kUndefinedValueRootIndex = 1;
  static constexpr int kNullValueRootIndex = 2;
  static constexpr int kTrueValueRootIndex = 3;
  static constexpr int kFalseValueRootIndex = 4;
  static constexpr int kStringResourceOffset = 8;
  static constexpr int kStringRepresentationAndEncodingMask = 0x0f;
  static constexpr int kStringEncodingMask = 0x08;
  static constexpr int kExternalOneByteRepresentationTag = 0x04;
  static constexpr int kExternalTwoByteRepresentationTag = 0x02;
  static constexpr int kDontThrow = 0;
  static constexpr int kThrowOnError = 1;

  // SMI 转换
  static constexpr Address IntegralToSmi(int value) {
    return (static_cast<Address>(value) << (kSmiTagSize + kSmiShiftSize)) |
           kSmiTag;
  }

  // 虚拟函数声明
  static int GetNodeState(void* slot) { return 0; }
  static void CheckInitialized(void* isolate) {}
  static Address* GetRootSlot(void* isolate, int index) {
    static Address dummy = 0;
    return &dummy;
  }
  static bool IsExternalTwoByteString(int instance_type) { return false; }
  static int GetInstanceType(Address obj) { return 0; }
  static void* GetIsolateForSandbox(Address obj) { return nullptr; }

  template <int tag>
  static Address ReadExternalPointerField(void* isolate, Address obj,
                                          int offset) {
    return 0;
  }
};

}  // namespace internal
}  // namespace v8

#endif  // V8_WASI_CONSTANTS_DEFINED

#endif  // __wasi__

#endif  // V8_INCLUDE_WASI_V8_WASI_COMPAT_H_
