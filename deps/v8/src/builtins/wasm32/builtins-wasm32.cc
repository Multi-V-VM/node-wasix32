// Copyright 2024 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// WASM32 builtins stub - WASM32 is a virtual architecture that doesn't
// generate native code, so builtins are minimal stubs.

#if V8_TARGET_ARCH_WASM32

#include "src/ast/ast.h"
#include "src/base/logging.h"
#include "src/builtins/builtins.h"
#include "src/builtins/builtins-promise.h"
#include "src/builtins/wasm32/builtins-wasm32-abi.h"
#include "src/codegen/compiler.h"
#include "src/codegen/handler-table.h"
#include "src/codegen/macro-assembler.h"
#include "src/codegen/wasm32/register-wasm32.h"
#include "src/execution/execution.h"
#include "src/execution/frame-constants.h"
#include "src/execution/isolate-inl.h"
#include "src/execution/microtask-queue.h"
#include "src/execution/messages.h"
#include "src/heap/combined-heap.h"
#include "src/heap/heap.h"
#include "src/ic/ic.h"
#include "src/interpreter/bytecode-flags-and-tokens.h"
#include "src/interpreter/bytecode-register.h"
#include "src/interpreter/bytecodes.h"
#include "src/interpreter/interpreter-intrinsics.h"
#include "src/interpreter/interpreter.h"
#include "src/json/json-parser.h"
#include "src/json/json-stringifier.h"
#include "src/objects/bytecode-array-inl.h"
#include "src/objects/js-array-buffer-inl.h"
#include "src/objects/code-inl.h"
#include "src/objects/cell-inl.h"
#include "src/objects/feedback-cell-inl.h"
#include "src/objects/feedback-vector-inl.h"
#include "src/objects/fixed-array-inl.h"
#include "src/objects/function-kind.h"
#include "src/objects/js-array-inl.h"
#include "src/objects/backing-store.h"
#include "src/objects/js-collection-inl.h"
#include "src/objects/js-function-inl.h"
#include "src/objects/js-generator-inl.h"
#include "src/objects/js-objects-inl.h"
#include "src/objects/js-proxy-inl.h"
#include "src/objects/js-promise-inl.h"
#include "src/objects/js-regexp-inl.h"
#ifdef V8_INTL_SUPPORT
#include "src/objects/intl-objects.h"
#endif
#include "src/objects/keys.h"
#include "src/objects/lookup-inl.h"
#include "src/objects/literal-objects-inl.h"
#include "src/objects/name-inl.h"
#include "src/objects/objects-inl.h"
#include "src/objects/property-descriptor-object.h"
#include "src/objects/property-descriptor.h"
#include "src/objects/promise-inl.h"
#include "src/objects/scope-info-inl.h"
#include "src/objects/script-inl.h"
#include "src/objects/shared-function-info-inl.h"
#include "src/objects/smi.h"
#include "src/objects/source-text-module.h"
#include "src/objects/tagged-index.h"
#include "src/numbers/conversions.h"
#include "src/regexp/regexp.h"
#include "src/regexp/regexp-utils.h"
#include "src/roots/roots-inl.h"
#include "src/runtime/runtime.h"
#include "src/strings/string-builder.h"
#include "src/strings/string-builder-inl.h"
#include "src/strings/uri.h"
#include "src/utils/utils.h"
#include "include/v8-locker.h"

#include <cstring>
#include <cstdio>
#include <cmath>
#include <vector>

namespace v8 {
namespace internal {

extern "C" int v8_wasm32_silent_fprintf(FILE*, const char*, ...) { return 0; }
extern "C" int v8_wasm32_silent_fflush(FILE*) { return 0; }
void v8_wasm32_silent_printf(const char*, ...) {}
void v8_wasm32_silent_printf(FILE*, const char*, ...) {}
#define PrintF v8_wasm32_silent_printf

Address Runtime_CreateArrayLiteral(int args_length, Address* args_object,
                                   Isolate* isolate);
Address Runtime_CreateObjectLiteral(int args_length, Address* args_object,
                                    Isolate* isolate);
Address Runtime_CreateRegExpLiteral(int args_length, Address* args_object,
                                    Isolate* isolate);
Address Runtime_CloneObjectIC_Slow(int args_length, Address* args_object,
                                   Isolate* isolate);
Address Runtime_DefineKeyedOwnPropertyInLiteral(int args_length,
                                                Address* args_object,
                                                Isolate* isolate);
Address Runtime_DeleteProperty(int args_length, Address* args_object,
                               Isolate* isolate);
Address Runtime_SetKeyedProperty(int args_length, Address* args_object,
                                 Isolate* isolate);
Address Runtime_TypedArrayCopyElements(int args_length, Address* args_object,
                                       Isolate* isolate);
Address Runtime_TypedArraySet(int args_length, Address* args_object,
                              Isolate* isolate);
Address Runtime_Add(int args_length, Address* args_object, Isolate* isolate);
Address Runtime_ForInEnumerate(int args_length, Address* args_object,
                               Isolate* isolate);
Address Runtime_ForInHasProperty(int args_length, Address* args_object,
                                 Isolate* isolate);
Address Runtime_ThrowAccessedUninitializedVariable(int args_length,
                                                   Address* args_object,
                                                   Isolate* isolate);
Address Runtime_ThrowNotSuperConstructor(int args_length, Address* args_object,
                                         Isolate* isolate);
Address Runtime_ThrowSuperAlreadyCalledError(int args_length,
                                             Address* args_object,
                                             Isolate* isolate);
Address Runtime_ThrowSuperNotCalled(int args_length, Address* args_object,
                                    Isolate* isolate);
extern "C" Address WasmJSEntry(Address root, Address new_target,
                               Address target, Address receiver, intptr_t argc,
                               Address** argv);

// WASM32 doesn't generate native builtins - these are stub implementations
// that satisfy the linker. Actual functionality is provided through
// the interpreter or runtime calls.

void Builtins::Generate_Adaptor(MacroAssembler* masm,
                                int formal_parameter_count, Address address) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_CEntry(MacroAssembler* masm, int result_size,
                               ArgvMode argv_mode, bool builtin_exit_frame,
                               bool switch_to_central_stack) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_JSConstructStubGeneric(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_JSBuiltinsConstructStub(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_ResumeGeneratorTrampoline(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_ConstructedNonConstructable(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_JSEntry(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_JSConstructEntry(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_JSRunMicrotasksEntry(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_JSEntryTrampoline(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_JSConstructEntryTrampoline(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_RunMicrotasksTrampoline(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_DeoptimizationEntry_Eager(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_DeoptimizationEntry_Lazy(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_BaselineOutOfLinePrologue(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_BaselineOutOfLinePrologueDeopt(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_InterpreterOnStackReplacement_ToBaseline(
    MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_InterpreterEntryTrampoline(
    MacroAssembler* masm, InterpreterEntryTrampolineMode mode) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_InterpreterPushArgsThenCallImpl(
    MacroAssembler* masm, ConvertReceiverMode receiver_mode,
    InterpreterPushArgsMode mode) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_InterpreterPushArgsThenConstructImpl(
    MacroAssembler* masm, InterpreterPushArgsMode mode) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_ConstructForwardAllArgsImpl(
    MacroAssembler* masm, ForwardWhichFrame which_frame) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_InterpreterPushArgsThenFastConstructFunction(
    MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_InterpreterEnterAtNextBytecode(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_InterpreterEnterAtBytecode(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_ContinueToCodeStubBuiltin(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_ContinueToCodeStubBuiltinWithResult(
    MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_ContinueToJavaScriptBuiltin(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_ContinueToJavaScriptBuiltinWithResult(
    MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_NotifyDeoptimized(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_InterpreterOnStackReplacement(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_BaselineOnStackReplacement(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_FunctionPrototypeApply(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_FunctionPrototypeCall(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_ReflectApply(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_ReflectConstruct(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_CallOrConstructVarargs(MacroAssembler* masm,
                                               Builtin target_builtin) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_CallOrConstructForwardVarargs(MacroAssembler* masm,
                                                      CallOrConstructMode mode,
                                                      Builtin target_builtin) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_CallFunction(MacroAssembler* masm,
                                     ConvertReceiverMode mode) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_CallBoundFunctionImpl(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_Call(MacroAssembler* masm, ConvertReceiverMode mode) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_ConstructFunction(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_ConstructBoundFunction(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_Construct(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

#if V8_ENABLE_WEBASSEMBLY
void Builtins::Generate_WasmLiftoffFrameSetup(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_WasmCompileLazy(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_WasmDebugBreak(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_WasmToJsWrapperAsm(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_WasmTrapHandlerLandingPad(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_WasmSuspend(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_WasmResume(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_WasmReject(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_WasmOnStackReplace(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_JSToWasmWrapperAsm(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_WasmReturnPromiseOnSuspendAsm(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_JSToWasmStressSwitchStacksAsm(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_WasmHandleStackOverflow(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}
#endif  // V8_ENABLE_WEBASSEMBLY

void Builtins::Generate_DoubleToI(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_CallApiCallbackImpl(MacroAssembler* masm,
                                            CallApiCallbackMode mode) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_CallApiGetter(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_DirectCEntry(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

void Builtins::Generate_RestartFrameTrampoline(MacroAssembler* masm) {
  // Stub - no native code generation for WASM32
}

// Probe builtin: a trap-free no-op used to validate the dispatch spine.
extern "C" void WasmProbeBuiltin() {
  Address offset =
      g_wasm_regs[WasmRegisterCodeToSlot(
          kInterpreterBytecodeOffsetRegister.code())];
  static int trace_count = 0;
  if (trace_count++ < 32) {
    Address target =
        g_wasm_regs[WasmRegisterCodeToSlot(
            kJavaScriptCallTargetRegister.code())];
    std::fprintf(stderr, "WASM32_PROBE offset=%u target=0x%x\n",
                 static_cast<unsigned>(offset),
                 static_cast<unsigned>(target));
    std::fflush(stderr);
  }
}

extern "C" Address WasmTraceMemoryAccess(Address address, int kind) {
  if (!g_wasm_trace_memory) return address;
  size_t mem_bytes = static_cast<size_t>(__builtin_wasm_memory_size(0)) * 65536u;
  Address value = 0;
  bool readable = (address <= mem_bytes - sizeof(Address)) &&
                  (address % sizeof(Address) == 0);
  if (readable) value = *reinterpret_cast<Address*>(address);
  PrintF("WasmMem: kind=%d addr=0x%x mem=0x%zx readable=%d value=0x%x "
         "r3=0x%x fp=0x%x\n",
         kind, static_cast<unsigned>(address), mem_bytes, readable,
         static_cast<unsigned>(value),
         static_cast<unsigned>(g_wasm_regs[WasmRegisterCodeToSlot(
             Register::r3().code())]),
         static_cast<unsigned>(g_wasm_current_frame_pointer));
  return address;
}

namespace {

MaybeDirectHandle<Object> GetObjectPropertyPreservingWasmInterpreterState(
    Isolate* isolate, DirectHandle<JSAny> lookup_start_object,
    DirectHandle<Object> key);
Tagged<SharedFunctionInfo> Wasm32JSFunctionShared(
    Tagged<JSFunction> function);
Address Wasm32JSFunctionPrototypeAddress(Isolate* isolate,
                                         DirectHandle<JSFunction> function);

constexpr bool kTraceWasmInterpreterSteps = false;
constexpr bool kTraceWasmFallbackDetails = false;
constexpr bool kTraceWasmJSEntry = false;
constexpr bool kTraceWasmCallBytecode = false;
int g_trace_after_collection_fallback_steps = 0;
bool g_dumped_set_keyed_primitive_receiver = false;
bool g_dumped_get_iterator_method_failure = false;
int g_context_slot46_trace_count = 0;
int g_context_slot43_depth2_trace_count = 0;
int g_set_keyed_property_trace_count = 0;
int g_module_property_store_trace_count = 0;
int g_module_undefined_store_trace_count = 0;
int g_delete_property_trace_count = 0;

constexpr char kNodePercentSpecifierPattern[] = "%[dfijoOs]";
constexpr char kWasm32RegExpPatternMarker[] = "__wasm32RegExpPattern";
constexpr char kWasm32RegExpFlagsMarker[] = "__wasm32RegExpFlags";

constexpr int SlotFor(Register reg) {
  return WasmRegisterCodeToSlot(reg.code());
}

void ClearEntrypointStackWindow() {
  for (int i = kWasmStackSlotBase; i < kWasmRegFileSize; ++i) {
    g_wasm_regs[i] = 0;
  }
}

void ClearEntrypointRegisterFile() {
  for (int i = 0; i < kWasmRegFileSize; ++i) {
    g_wasm_regs[i] = 0;
  }
}

int InterpreterFrameSlotForOffset(int offset) {
  DCHECK_EQ(offset % kSystemPointerSize, 0);
  int slot = kWasmInterpreterFrameFpSlot + offset / kSystemPointerSize;
  if (slot < 0 || slot >= kWasmInterpreterFrameSlots) {
    FATAL("wasm32 interpreter frame offset %d maps outside frame", offset);
  }
  return slot;
}

void MirrorWasmGCRegSlotForWrite(int slot, Address value);
void MirrorWasmGCFrameSlotForWrite(int slot, Address value);

int GeneratedFrameSlotForOffset(int offset) {
  DCHECK_EQ(offset % kSystemPointerSize, 0);
  int index = offset / kSystemPointerSize;
  int slot = index < 0 ? kWasmFixedFrameSlotBase + (-index - 1)
                       : kWasmStackSlotBase + index;
  if (slot < 0 || slot >= kWasmRegFileSize) {
    FATAL("wasm32 generated frame offset %d maps outside g_wasm_regs",
          offset);
  }
  return slot;
}

void StoreInterpreterFrameOffset(int offset, Address value) {
  int slot = InterpreterFrameSlotForOffset(offset);
  CHECK_GE(slot, 0);
  CHECK_LT(slot, kWasmInterpreterFrameSlots);
  g_wasm_interpreter_frame[slot] = value;
  MirrorWasmGCFrameSlotForWrite(slot, value);
}

void StoreGeneratedFrameOffset(int offset, Address value) {
  int slot = GeneratedFrameSlotForOffset(offset);
  CHECK_GE(slot, 0);
  CHECK_LT(slot, kWasmRegFileSize);
  g_wasm_regs[slot] = value;
  MirrorWasmGCRegSlotForWrite(slot, value);
}

Address ReadGeneratedJSArgument(int index) {
  DCHECK_GE(index, 0);
  int offset = CommonFrameConstants::kFixedFrameSizeAboveFp +
               (index + 1) * kSystemPointerSize;
  return g_wasm_regs[GeneratedFrameSlotForOffset(offset)];
}

void StoreInterpreterRegister(interpreter::Register reg, Address value) {
  StoreInterpreterFrameOffset(reg.ToOperand() * kSystemPointerSize, value);
}

void ClearInterpreterFrame() {
  for (int i = 0; i < kWasmInterpreterFrameSlots; ++i) {
    g_wasm_interpreter_frame[i] = 0;
  }
  g_wasm_current_frame_pointer = reinterpret_cast<Address>(
      &g_wasm_interpreter_frame[kWasmInterpreterFrameFpSlot]);
}

size_t WasmMemoryBytes() {
  return static_cast<size_t>(__builtin_wasm_memory_size(0)) * 65536u;
}

bool IsReadableWasmAddress(Address address, size_t byte_count) {
  size_t mem_bytes = WasmMemoryBytes();
  if (byte_count == 0 || mem_bytes < byte_count) return false;
  return address <= mem_bytes - byte_count;
}

bool IsReadableTaggedHeapObject(Address value, size_t min_bytes);
bool HasReadableHeapObjectMap(Address value);

void RepairWasm32NodeEnvironmentRealm(Tagged<Context> context) {
  // Node stores Environment* and Realm* in native-context embedder data slots
  // 32 and 38. The wasm32 C++ ABI currently leaves Environment::principal_realm_
  // unset for some API callbacks; repair that slot from the already-valid
  // Realm embedder slot before invoking Node native callbacks.
  if (!IsNativeContext(context)) return;

  constexpr int kNodeEnvironmentEmbedderIndex = 32;
  constexpr int kNodeRealmEmbedderIndex = 38;
  constexpr Address kNodeEnvironmentPrincipalRealmOffset = 1472;
  constexpr Address kNodeRealmContextPersistentOffset = 60;
  constexpr Address kNodeRealmContextVtableOffset = 36;

  v8::Local<v8::Context> v8_context =
      v8::Local<v8::Context>::FromAddress(context.ptr());
  if (v8_context.IsEmpty()) return;
  Address env = reinterpret_cast<Address>(
      v8_context->GetAlignedPointerFromEmbedderData(
          kNodeEnvironmentEmbedderIndex));
  Address realm = reinterpret_cast<Address>(
      v8_context->GetAlignedPointerFromEmbedderData(kNodeRealmEmbedderIndex));
  if (env == kNullAddress || realm == kNullAddress) return;
  Address slot = env + kNodeEnvironmentPrincipalRealmOffset;
  if (!IsReadableWasmAddress(slot, sizeof(Address))) return;
  Address* principal_realm = reinterpret_cast<Address*>(slot);
  Address* context_persistent = nullptr;
  Address context_persistent_slot = realm + kNodeRealmContextPersistentOffset;
  Address context_cell = kNullAddress;
  Address context_value = kNullAddress;
  bool context_cell_readable = false;
  bool context_value_is_context = false;
  if (IsReadableWasmAddress(context_persistent_slot, sizeof(Address))) {
    context_persistent = reinterpret_cast<Address*>(context_persistent_slot);
    context_cell = *context_persistent;
    context_cell_readable =
        IsReadableWasmAddress(context_cell, sizeof(Address));
    if (context_cell_readable) {
      context_value = *reinterpret_cast<Address*>(context_cell);
      context_value_is_context =
          IsReadableTaggedHeapObject(context_value,
                                     HeapObject::kMapOffset +
                                         sizeof(Address)) &&
          HasReadableHeapObjectMap(context_value) &&
          IsContext(Tagged<Object>(context_value));
    }
  }
  static int repair_trace_count = 0;
  if (kTraceWasmJSEntry && repair_trace_count < 64) {
    Address realm_vtable = 0;
    Address realm_context_slot = 0;
    Address env_isolate = 0;
    if (IsReadableWasmAddress(realm, sizeof(Address))) {
      realm_vtable = *reinterpret_cast<Address*>(realm);
    }
    if (IsReadableWasmAddress(env + 84, sizeof(Address))) {
      env_isolate = *reinterpret_cast<Address*>(env + 84);
    }
    if (IsReadableWasmAddress(realm_vtable + kNodeRealmContextVtableOffset,
                              sizeof(Address))) {
      realm_context_slot = *reinterpret_cast<Address*>(
          realm_vtable + kNodeRealmContextVtableOffset);
    }
    PrintF("WasmJSEntry: repair Node Environment principal_realm "
           "env=0x%x slot=0x%x old=0x%x realm=0x%x realm_vtbl=0x%x "
           "context_slot=0x%x env_isolate=0x%x "
           "context_persistent=0x%x context_cell=0x%x context_value=0x%x "
           "context_ok=%d\n",
           static_cast<unsigned>(env),
           static_cast<unsigned>(slot),
           static_cast<unsigned>(*principal_realm),
           static_cast<unsigned>(realm),
           static_cast<unsigned>(realm_vtable),
           static_cast<unsigned>(realm_context_slot),
           static_cast<unsigned>(env_isolate),
           static_cast<unsigned>(context_persistent_slot),
           static_cast<unsigned>(context_cell),
           static_cast<unsigned>(context_value),
           context_value_is_context ? 1 : 0);
    if (repair_trace_count == 0 && realm_vtable >= 16) {
      PrintF("WasmJSEntry: realm_vtbl entries");
      for (int offset = -16; offset <= 112; offset += 4) {
        Address entry_addr = realm_vtable + offset;
        Address entry = 0;
        if (IsReadableWasmAddress(entry_addr, sizeof(Address))) {
          entry = *reinterpret_cast<Address*>(entry_addr);
        }
        PrintF(" %+d:0x%x", offset, static_cast<unsigned>(entry));
      }
      PrintF("\n");
      Address prior_vtable = realm_vtable - 0x250;
      if (realm_vtable >= 0x250 && IsReadableWasmAddress(prior_vtable, 128)) {
        PrintF("WasmJSEntry: prior realm_vtbl entries base=0x%x",
               static_cast<unsigned>(prior_vtable));
        for (int offset = -16; offset <= 112; offset += 4) {
          Address entry_addr = prior_vtable + offset;
          Address entry = 0;
          if (IsReadableWasmAddress(entry_addr, sizeof(Address))) {
            entry = *reinterpret_cast<Address*>(entry_addr);
          }
          PrintF(" %+d:0x%x", offset, static_cast<unsigned>(entry));
        }
        PrintF("\n");
      }
    }
    ++repair_trace_count;
  }
  if (context_persistent != nullptr &&
      (!context_cell_readable || !context_value_is_context ||
       context_value != context.ptr())) {
    static Address context_cell_storage = kNullAddress;
    context_cell_storage = context.ptr();
    *context_persistent = reinterpret_cast<Address>(&context_cell_storage);
  }
  if (*principal_realm != realm) {
    *principal_realm = realm;
  }
}

bool IsReadableTaggedHeapObject(Address value, size_t min_bytes) {
  if (!HAS_STRONG_HEAP_OBJECT_TAG(value) || value < kHeapObjectTag) {
    return false;
  }
  Address object_address = value - kHeapObjectTag;
  if ((object_address % sizeof(Address)) != 0) return false;
  return IsReadableWasmAddress(object_address, min_bytes);
}

bool IsPlausibleTaggedValue(Address value) {
  if (HAS_SMI_TAG(value)) return true;
  return IsReadableTaggedHeapObject(value, HeapObject::kMapOffset +
                                               sizeof(Address));
}

bool TryReadHeapObjectMap(Address value, Address* out_map) {
  if (!IsReadableTaggedHeapObject(value, HeapObject::kMapOffset +
                                             sizeof(Address))) {
    return false;
  }
  Address object_address = value - kHeapObjectTag;
  Address map_value =
      *reinterpret_cast<Address*>(object_address + HeapObject::kMapOffset);
  if (!IsReadableTaggedHeapObject(map_value, 64)) return false;
  *out_map = map_value;
  return true;
}

bool HasReadableHeapObjectMap(Address value) {
  Address map_value = kNullAddress;
  return TryReadHeapObjectMap(value, &map_value) &&
         IsMap(Tagged<Object>(map_value));
}

bool IsSafeTaggedHandleValue(Address value) {
  if (!IsPlausibleTaggedValue(value)) return false;
  return HAS_SMI_TAG(value) || HasReadableHeapObjectMap(value);
}

bool IsKnownReadOnlyRootValue(Isolate* isolate, Address value) {
  ReadOnlyRoots roots(isolate);
  return value == roots.undefined_value().ptr() ||
         value == roots.null_value().ptr() ||
         value == roots.true_value().ptr() ||
         value == roots.false_value().ptr() ||
         value == roots.the_hole_value().ptr() ||
         value == roots.exception().ptr() ||
         value == roots.empty_string().ptr();
}

bool IsKnownJSAnyReadOnlyRootValue(Isolate* isolate, Address value) {
  ReadOnlyRoots roots(isolate);
  return value == roots.undefined_value().ptr() ||
         value == roots.null_value().ptr() ||
         value == roots.true_value().ptr() ||
         value == roots.false_value().ptr() ||
         value == roots.empty_string().ptr();
}

bool IsSafeTaggedRootValue(Isolate* isolate, Address value) {
  if (IsKnownReadOnlyRootValue(isolate, value)) return true;
  if (HAS_SMI_TAG(value)) return true;
  if (!IsReadableTaggedHeapObject(value,
                                  HeapObject::kMapOffset + sizeof(Address))) {
    return false;
  }

  Tagged<HeapObject> object =
      Cast<HeapObject>(Tagged<Object>(value));
  Heap* heap = isolate->heap();
  if (!ReadOnlyHeap::Contains(object) && !heap->Contains(object)) return false;

  Address map_value =
      *reinterpret_cast<Address*>(value - kHeapObjectTag +
                                  HeapObject::kMapOffset);
  if (!IsReadableTaggedHeapObject(map_value, 64)) return false;
  Tagged<HeapObject> map_object =
      Cast<HeapObject>(Tagged<Object>(map_value));
  if (!ReadOnlyHeap::Contains(map_object) && !heap->Contains(map_object)) {
    return false;
  }
  return IsMap(map_object);
}

Isolate* g_wasm32_last_isolate = nullptr;
Address g_wasm32_last_root = kNullAddress;

Isolate* GetWasm32IsolateFromRoot(Address* root_in_out) {
  Address root = *root_in_out;
  if ((root == kNullAddress || !IsReadableWasmAddress(root, sizeof(Address))) &&
      g_wasm32_last_root != kNullAddress) {
    root = g_wasm32_last_root;
    *root_in_out = root;
  }
  if (root == kNullAddress) return g_wasm32_last_isolate;
  Isolate* isolate = Isolate::FromRootAddress(root);
  if (isolate != nullptr) {
    g_wasm32_last_isolate = isolate;
    g_wasm32_last_root = root;
  }
  return isolate;
}

constexpr int kMaxWasmGCStateDepth = 1024;

struct WasmGCStateStorage {
  Address regs[kWasmRegFileSize];
  Address frame[kWasmInterpreterFrameSlots];
  Address original_regs[kWasmRegFileSize];
  Address original_frame[kWasmInterpreterFrameSlots];
  bool active_regs[kWasmRegFileSize];
  bool active_frame[kWasmInterpreterFrameSlots];
  Heap* registered_heap;
  StrongRootsEntry* regs_entry;
  StrongRootsEntry* frame_entry;
};

WasmGCStateStorage g_wasm_gc_state[kMaxWasmGCStateDepth];
int g_wasm_gc_state_depth = 0;

void PrepareWasmGCRootMirrorValue(Address value, bool* active,
                                  Address* mirror_value) {
  Isolate* isolate = g_wasm32_last_isolate;
  if (isolate == nullptr) {
    *active = false;
    *mirror_value = kNullAddress;
    return;
  }

  *active = IsSafeTaggedRootValue(isolate, value);
  *mirror_value = *active ? value : ReadOnlyRoots(isolate).undefined_value().ptr();
}

void MirrorWasmGCRegSlotForWrite(int slot, Address value) {
  if (slot < 0 || slot >= kWasmRegFileSize || g_wasm_gc_state_depth == 0) {
    return;
  }

  bool active = false;
  Address mirror_value = kNullAddress;
  PrepareWasmGCRootMirrorValue(value, &active, &mirror_value);
  for (int depth = 0; depth < g_wasm_gc_state_depth; ++depth) {
    WasmGCStateStorage* storage = &g_wasm_gc_state[depth];
    storage->original_regs[slot] = value;
    storage->active_regs[slot] = active;
    storage->regs[slot] = mirror_value;
  }
}

void MirrorWasmGCFrameSlotForWrite(int slot, Address value) {
  if (slot < 0 || slot >= kWasmInterpreterFrameSlots ||
      g_wasm_gc_state_depth == 0) {
    return;
  }

  bool active = false;
  Address mirror_value = kNullAddress;
  PrepareWasmGCRootMirrorValue(value, &active, &mirror_value);
  for (int depth = 0; depth < g_wasm_gc_state_depth; ++depth) {
    WasmGCStateStorage* storage = &g_wasm_gc_state[depth];
    storage->original_frame[slot] = value;
    storage->active_frame[slot] = active;
    storage->frame[slot] = mirror_value;
  }
}

class WasmGCStateScope {
 public:
  explicit WasmGCStateScope(Isolate* isolate)
      : isolate_(isolate), storage_(nullptr), depth_(-1), restored_(false) {
    if (g_wasm_gc_state_depth >= kMaxWasmGCStateDepth) {
      FATAL("wasm32 GC root state depth exceeded depth=%d limit=%d",
            g_wasm_gc_state_depth, kMaxWasmGCStateDepth);
    }
    depth_ = g_wasm_gc_state_depth++;
    storage_ = &g_wasm_gc_state[depth_];
    Heap* heap = isolate_->heap();
    if (storage_->registered_heap != heap) {
      storage_->registered_heap = heap;
      storage_->regs_entry = nullptr;
      storage_->frame_entry = nullptr;
    }

    Address undefined = ReadOnlyRoots(isolate).undefined_value().ptr();
    for (int i = 0; i < kWasmRegFileSize; ++i) {
      Address value = g_wasm_regs[i];
      storage_->original_regs[i] = value;
      storage_->active_regs[i] = IsSafeTaggedRootValue(isolate, value);
      storage_->regs[i] = storage_->active_regs[i] ? value : undefined;
    }
    for (int i = 0; i < kWasmInterpreterFrameSlots; ++i) {
      Address value = g_wasm_interpreter_frame[i];
      storage_->original_frame[i] = value;
      storage_->active_frame[i] = IsSafeTaggedRootValue(isolate, value);
      storage_->frame[i] = storage_->active_frame[i] ? value : undefined;
    }

    if (storage_->regs_entry == nullptr) {
      storage_->regs_entry = heap->RegisterStrongRoots(
          "wasm32-regs", FullObjectSlot(storage_->regs),
          FullObjectSlot(storage_->regs + kWasmRegFileSize));
    }
    if (storage_->frame_entry == nullptr) {
      storage_->frame_entry = heap->RegisterStrongRoots(
          "wasm32-interpreter-frame", FullObjectSlot(storage_->frame),
          FullObjectSlot(storage_->frame + kWasmInterpreterFrameSlots));
    }
  }

  ~WasmGCStateScope() { Restore(); }

  void Restore() {
    if (restored_) return;
    for (int i = 0; i < kWasmRegFileSize; ++i) {
      if (storage_->active_regs[i] &&
          g_wasm_regs[i] == storage_->original_regs[i]) {
        g_wasm_regs[i] = storage_->regs[i];
      }
    }
    for (int i = 0; i < kWasmInterpreterFrameSlots; ++i) {
      if (storage_->active_frame[i] &&
          g_wasm_interpreter_frame[i] == storage_->original_frame[i]) {
        g_wasm_interpreter_frame[i] = storage_->frame[i];
      }
    }
    Address undefined = ReadOnlyRoots(isolate_).undefined_value().ptr();
    for (Address& value : storage_->regs) value = undefined;
    for (Address& value : storage_->frame) value = undefined;
    if (g_wasm_gc_state_depth != depth_ + 1) {
      FATAL("wasm32 GC root state restore out of order");
    }
    g_wasm_gc_state_depth = depth_;
    restored_ = true;
    storage_ = nullptr;
  }

 private:
  Isolate* isolate_;
  WasmGCStateStorage* storage_;
  int depth_;
  bool restored_;
};

constexpr int kMaxWasmTemporaryRootDepth = 1024;
constexpr int kMaxWasmTemporaryRootSlots = 65;

struct WasmTemporaryRootStorage {
  Address values[kMaxWasmTemporaryRootSlots];
  Heap* registered_heap;
  StrongRootsEntry* entry;
};

WasmTemporaryRootStorage
    g_wasm_temporary_roots[kMaxWasmTemporaryRootDepth];
int g_wasm_temporary_root_depth = 0;

class WasmTemporaryRootScope {
 public:
  WasmTemporaryRootScope(Isolate* isolate, const Address* values, int count)
      : isolate_(isolate), storage_(nullptr), depth_(-1) {
    CHECK_GE(count, 0);
    CHECK_LE(count, kMaxWasmTemporaryRootSlots);
    if (g_wasm_temporary_root_depth >= kMaxWasmTemporaryRootDepth) {
      FATAL("wasm32 temporary root depth exceeded depth=%d limit=%d",
            g_wasm_temporary_root_depth, kMaxWasmTemporaryRootDepth);
    }

    depth_ = g_wasm_temporary_root_depth++;
    storage_ = &g_wasm_temporary_roots[depth_];
    Heap* heap = isolate_->heap();
    if (storage_->registered_heap != heap) {
      storage_->registered_heap = heap;
      storage_->entry = nullptr;
    }

    Address undefined = ReadOnlyRoots(isolate_).undefined_value().ptr();
    for (int i = 0; i < kMaxWasmTemporaryRootSlots; ++i) {
      storage_->values[i] = i < count ? values[i] : undefined;
    }
    if (storage_->entry == nullptr) {
      storage_->entry = heap->RegisterStrongRoots(
          "wasm32-temporary-roots", FullObjectSlot(storage_->values),
          FullObjectSlot(storage_->values + kMaxWasmTemporaryRootSlots));
    }
  }

  ~WasmTemporaryRootScope() {
    Address undefined = ReadOnlyRoots(isolate_).undefined_value().ptr();
    for (Address& value : storage_->values) value = undefined;
    if (g_wasm_temporary_root_depth != depth_ + 1) {
      FATAL("wasm32 temporary roots released out of order");
    }
    g_wasm_temporary_root_depth = depth_;
  }

  Address* data() { return storage_->values; }

 private:
  Isolate* isolate_;
  WasmTemporaryRootStorage* storage_;
  int depth_;
};

void DumpRuntimeArg(const char* label, int index, Address value) {
  PrintF(" %s[%d]=0x%x", label, index, static_cast<unsigned>(value));
  if (!IsPlausibleTaggedValue(value)) {
    PrintF("(invalid-tagged)");
    return;
  }
  Tagged<Object> object(value);
  if (IsSmi(object)) {
    PrintF("(smi=%d)", Smi::ToInt(object));
  } else if (IsHeapObject(object)) {
    Address map_value = kNullAddress;
    if (!TryReadHeapObjectMap(value, &map_value)) {
      PrintF("(invalid-map)");
      return;
    }
    Tagged<Map> map = Cast<Map>(Tagged<Object>(map_value));
    PrintF("(type=%d)", static_cast<int>(map->instance_type()));
    if (IsString(object)) {
      Tagged<String> string = Cast<String>(object);
      PrintF("(string-len=%d)", string->length());
    }
  }
}

void DumpCurrentRuntimeCallStateForTrace(const char* label) {
  Address code_start =
      g_wasm_regs[SlotFor(kJavaScriptCallCodeStartRegister)];
  PrintF(" %s code_start=0x%x", label, static_cast<unsigned>(code_start));
  if (code_start != kNullAddress) {
    for (Builtin builtin = Builtins::kFirst; builtin <= Builtins::kLast;
         ++builtin) {
      void* fn = WasmBuiltinFuncref(builtin);
      if (reinterpret_cast<Address>(fn) == code_start) {
        PrintF(" builtin=%d(%s)", static_cast<int>(builtin),
               Builtins::name(builtin));
        break;
      }
    }
  }
  DumpRuntimeArg(" target", 0,
                 g_wasm_regs[SlotFor(kJavaScriptCallTargetRegister)]);
  DumpRuntimeArg(" new_target", 0,
                 g_wasm_regs[SlotFor(kJavaScriptCallNewTargetRegister)]);
  DumpRuntimeArg(" argc", 0,
                 g_wasm_regs[SlotFor(kJavaScriptCallArgCountRegister)]);
  DumpRuntimeArg(" context", 0, g_wasm_regs[SlotFor(kContextRegister)]);
  DumpRuntimeArg(" ret0", 0, g_wasm_regs[SlotFor(kReturnRegister0)]);
  DumpRuntimeArg(" acc", 0,
                 g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
  PrintF("\n");
}

void PrintStringPreviewForTrace(const char* label, Tagged<Object> value,
                                uint32_t offset = 0,
                                uint32_t max_length = 160) {
  if (!IsString(value)) return;
  Tagged<String> string = Cast<String>(value);
  uint32_t length = string->length();
  if (offset > length) offset = length;
  uint32_t preview_length = length - offset;
  if (preview_length > max_length) preview_length = max_length;
  size_t c_length = 0;
  std::unique_ptr<char[]> preview =
      string->ToCString(offset, preview_length, &c_length);
  PrintF(" %s(offset=%u,len=%u/%u)=\"%s\"", label, offset,
         preview_length, length, preview.get());
}

void DumpNamedDataPropertyForTrace(Isolate* isolate, Address receiver_address,
                                   const char* property_name) {
  PrintF(" %s=", property_name);
  if (!IsSafeTaggedHandleValue(receiver_address) ||
      !IsJSReceiver(Tagged<Object>(receiver_address))) {
    PrintF("<invalid-receiver>");
    return;
  }

  HandleScope scope(isolate);
  DirectHandle<JSReceiver> receiver(
      Cast<JSReceiver>(Tagged<Object>(receiver_address)), isolate);
  DirectHandle<Name> name =
      isolate->factory()->InternalizeUtf8String(property_name);
  Handle<Object> value = JSReceiver::GetDataProperty(isolate, receiver, name);
  DumpRuntimeArg("value", 0, (*value).ptr());
  PrintStringPreviewForTrace("string", *value, 0, 96);
}

void DumpCurrentInterpreterBytecodeForTrace(const char* label) {
  Address bytecode_array_address =
      g_wasm_regs[SlotFor(kInterpreterBytecodeArrayRegister)];
  Address bytecode_offset =
      g_wasm_regs[SlotFor(kInterpreterBytecodeOffsetRegister)];
  Address accumulator = g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
  PrintF(" %s bytecode_array=0x%x offset=0x%x acc=0x%x",
         label, static_cast<unsigned>(bytecode_array_address),
         static_cast<unsigned>(bytecode_offset),
         static_cast<unsigned>(accumulator));

  Tagged<Object> object(bytecode_array_address);
  if (!IsBytecodeArray(object)) {
    PrintF(" (not-bytecode-array)\n");
    return;
  }

  Tagged<BytecodeArray> bytecode = Cast<BytecodeArray>(object);
  int bytecode_index =
      static_cast<int>(bytecode_offset -
                       (BytecodeArray::kHeaderSize - kHeapObjectTag));
  if (bytecode_index < 0 || bytecode_index >= bytecode->length()) {
    PrintF(" index=%d length=%d (out-of-range)\n", bytecode_index,
           bytecode->length());
    return;
  }

  uint8_t opcode = bytecode->get(bytecode_index);
  if (opcode > interpreter::Bytecodes::ToByte(interpreter::Bytecode::kLast)) {
    PrintF(" index=%d length=%d opcode=0x%x(invalid)\n", bytecode_index,
           bytecode->length(), static_cast<unsigned>(opcode));
    return;
  }
  interpreter::Bytecode bytecode_enum = interpreter::Bytecodes::FromByte(opcode);
  PrintF(" index=%d length=%d opcode=0x%x(%s)\n", bytecode_index,
         bytecode->length(), static_cast<unsigned>(opcode),
         interpreter::Bytecodes::ToString(bytecode_enum));
}

void DumpNameForTrace(Tagged<Object> name_object) {
  if (IsString(name_object)) {
    PrintF("<string len=%d ptr=0x%x>", Cast<String>(name_object)->length(),
           static_cast<unsigned>(name_object.ptr()));
  } else if (IsSymbol(name_object)) {
    PrintF("<symbol ptr=0x%x>", static_cast<unsigned>(name_object.ptr()));
  } else {
    PrintF("<name ptr=0x%x>", static_cast<unsigned>(name_object.ptr()));
  }
}

void DumpFunctionSourceForTrace(Address function_address) {
  DumpRuntimeArg(" function", 0, function_address);
  if (!IsSafeTaggedHandleValue(function_address) ||
      !IsJSFunction(Tagged<Object>(function_address))) {
    return;
  }

  Tagged<SharedFunctionInfo> shared =
      Wasm32JSFunctionShared(Cast<JSFunction>(Tagged<Object>(function_address)));
  PrintF(" sfi=0x%x name=", static_cast<unsigned>(shared.ptr()));
  DumpNameForTrace(shared->Name());
  PrintF(" start=%d end=%d literal_id=%d", shared->StartPosition(),
         shared->EndPosition(), shared->function_literal_id());
  Tagged<Object> script_object = shared->script();
  if (IsScript(script_object)) {
    Tagged<Script> script = Cast<Script>(script_object);
    PrintStringPreviewForTrace(" script_name", script->name());
    if (IsString(script->source())) {
      Tagged<String> source = Cast<String>(script->source());
      int source_start = shared->StartPosition() - 80;
      if (source_start < 0) source_start = 0;
      PrintStringPreviewForTrace(" source", source,
                                 static_cast<uint32_t>(source_start), 420);
    }
  }
}

bool StringContainsAsciiForTrace(Tagged<Object> value, const char* needle) {
  if (!IsString(value)) return false;
  size_t c_length = 0;
  std::unique_ptr<char[]> text = Cast<String>(value)->ToCString(
      0, Cast<String>(value)->length(), &c_length);
  return std::strstr(text.get(), needle) != nullptr;
}

bool SharedDebugNameEqualsAsciiForTrace(Tagged<SharedFunctionInfo> shared,
                                        const char* needle) {
  std::unique_ptr<char[]> debug_name = shared->DebugNameCStr();
  return std::strcmp(debug_name.get(), needle) == 0;
}

bool FunctionScriptNameContainsAsciiForTrace(
    Tagged<SharedFunctionInfo> shared, const char* needle) {
  Tagged<Object> script_object = shared->script();
  if (!IsScript(script_object)) return false;
  return StringContainsAsciiForTrace(Cast<Script>(script_object)->name(),
                                     needle);
}

bool FunctionSourceRangeContainsAsciiForTrace(
    Tagged<SharedFunctionInfo> shared, const char* needle) {
  Tagged<Object> script_object = shared->script();
  if (!IsScript(script_object)) return false;
  Tagged<Object> source_object = Cast<Script>(script_object)->source();
  if (!IsString(source_object)) return false;
  Tagged<String> source = Cast<String>(source_object);
  int start = shared->StartPosition();
  int end = shared->EndPosition();
  if (start < 0) start = 0;
  if (end < start) return false;
  if (start > source->length()) start = source->length();
  if (end > source->length()) end = source->length();
  size_t c_length = 0;
  std::unique_ptr<char[]> text = source->ToCString(
      static_cast<uint32_t>(start), static_cast<uint32_t>(end - start),
      &c_length);
  return std::strstr(text.get(), needle) != nullptr;
}

bool FunctionMatchesWasmEvalTraceNeedle(Tagged<SharedFunctionInfo> shared) {
  Tagged<Object> script_object = shared->script();
  if (!IsScript(script_object)) return false;
  Tagged<Script> script = Cast<Script>(script_object);
  if (StringContainsAsciiForTrace(script->name(),
                                  "node:internal/modules/cjs/loader") &&
      (SharedDebugNameEqualsAsciiForTrace(shared, "wrapSafe") ||
       SharedDebugNameEqualsAsciiForTrace(shared, "_compile") ||
       FunctionSourceRangeContainsAsciiForTrace(shared, "compiledWrapper") ||
       FunctionSourceRangeContainsAsciiForTrace(shared,
                                                "ReflectApply(compiledWrapper"))) {
    return true;
  }
  if (StringContainsAsciiForTrace(script->name(), "[eval]") ||
      StringContainsAsciiForTrace(script->name(), "eval")) {
    return true;
  }
  Tagged<Object> source_object = script->source();
  if (!IsString(source_object)) return false;
  return FunctionSourceRangeContainsAsciiForTrace(
             shared, "return (main) => main()") ||
         FunctionSourceRangeContainsAsciiForTrace(shared,
                                                  "process.stdout.write") ||
         FunctionSourceRangeContainsAsciiForTrace(shared, "console.log(42)") ||
         StringContainsAsciiForTrace(source_object, "[eval]");
}

bool FunctionMatchesPerContextPrimordialsTraceNeedle(
    Tagged<SharedFunctionInfo> shared) {
  Tagged<Object> script_object = shared->script();
  if (!IsScript(script_object)) return false;
  Tagged<Script> script = Cast<Script>(script_object);
  return StringContainsAsciiForTrace(script->name(),
                                    "internal/per_context/primordials");
}

bool FunctionMatchesPerContextPrimordialsGetNewKey(
    Tagged<SharedFunctionInfo> shared) {
  if (!FunctionMatchesPerContextPrimordialsTraceNeedle(shared)) return false;
  if (SharedDebugNameEqualsAsciiForTrace(shared, "getNewKey")) return true;
  if (shared->function_literal_id() == 2 && shared->StartPosition() == 2055 &&
      shared->EndPosition() == 2222) {
    return true;
  }
  return false;
}

bool FunctionMatchesPerContextPrimordialsCopyPrototype(
    Tagged<SharedFunctionInfo> shared) {
  if (!FunctionMatchesPerContextPrimordialsTraceNeedle(shared)) return false;
  if (SharedDebugNameEqualsAsciiForTrace(shared, "copyPrototype")) return true;
  return shared->function_literal_id() == 6 && shared->StartPosition() == 4080 &&
         shared->EndPosition() == 4749;
}

bool CurrentInterpreterFunctionMatchesPerContextPrimordialsCopyPrototype();

uint16_t UpperAsciiForGetNewKey(uint16_t ch) {
  return (ch >= 'a' && ch <= 'z') ? static_cast<uint16_t>(ch - 'a' + 'A')
                                  : ch;
}

bool ConcatTwoStringsForGetNewKey(Isolate* isolate, DirectHandle<String> left,
                                  DirectHandle<String> right,
                                  DirectHandle<String>* out) {
  MaybeDirectHandle<String> maybe =
      isolate->factory()->NewConsString(left, right);
  return maybe.ToHandle(out);
}

bool TryRunPerContextPrimordialsGetNewKey(Isolate* isolate,
                                          Tagged<SharedFunctionInfo> shared,
                                          int argc, Address* out_result) {
  if (!FunctionMatchesPerContextPrimordialsGetNewKey(shared)) return false;
  if (argc <= kJSArgcReceiverSlots) return false;

  Tagged<Object> key(g_wasm_regs[kWasmOutgoingArgSlotBase + 1]);
  DirectHandle<String> suffix;
  DirectHandle<String> first;
  if (IsString(key)) {
    DirectHandle<String> input = direct_handle(Cast<String>(key), isolate);
    input = String::Flatten(isolate, input);
    if (input->length() == 0) return false;
    first = isolate->factory()->LookupSingleCharacterStringFromCode(
        UpperAsciiForGetNewKey(input->Get(0)));
    suffix = input->length() <= 1
                 ? isolate->factory()->empty_string()
                 : isolate->factory()->NewProperSubString(
                       input, 1, input->length());
    DirectHandle<String> result;
    if (!ConcatTwoStringsForGetNewKey(isolate, first, suffix, &result)) {
      *out_result = ReadOnlyRoots(isolate).exception().ptr();
      return true;
    }
    *out_result = (*result).ptr();
    return true;
  }

  if (!IsSymbol(key)) return false;
  Tagged<Object> description_object = Cast<Symbol>(key)->description();
  if (!IsString(description_object)) return false;
  DirectHandle<String> description =
      direct_handle(Cast<String>(description_object), isolate);
  description = String::Flatten(isolate, description);
  if (description->length() <= 7) return false;

  DirectHandle<String> symbol_prefix =
      isolate->factory()->NewStringFromAsciiChecked("Symbol");
  first = isolate->factory()->LookupSingleCharacterStringFromCode(
      UpperAsciiForGetNewKey(description->Get(7)));
  suffix = description->length() <= 8
               ? isolate->factory()->empty_string()
               : isolate->factory()->NewProperSubString(
                     description, 8, description->length());
  DirectHandle<String> prefix_with_first;
  if (!ConcatTwoStringsForGetNewKey(isolate, symbol_prefix, first,
                                    &prefix_with_first)) {
    *out_result = ReadOnlyRoots(isolate).exception().ptr();
    return true;
  }
  DirectHandle<String> result;
  if (!ConcatTwoStringsForGetNewKey(isolate, prefix_with_first, suffix,
                                    &result)) {
    *out_result = ReadOnlyRoots(isolate).exception().ptr();
    return true;
  }
  *out_result = (*result).ptr();
  return true;
}

bool TryFindNativeContextMapByInstanceType(Isolate* isolate,
                                           InstanceType instance_type,
                                           DirectHandle<Map>* out_map) {
  Tagged<Context> current_context = isolate->context();
  DirectHandle<NativeContext> native_context =
      IsNativeContext(current_context)
          ? direct_handle(Cast<NativeContext>(current_context), isolate)
          : direct_handle(current_context->native_context(), isolate);
  for (int index = 0; index < Context::NATIVE_CONTEXT_SLOTS; ++index) {
    Tagged<Object> value = TaggedField<Object>::load(
        *native_context, Context::OffsetOfElementAt(index));
    if (!IsSafeTaggedHandleValue(value.ptr())) continue;
    if (!IsMap(value)) continue;
    Tagged<Map> map = Cast<Map>(value);
    if (map->instance_type() != instance_type) continue;
    *out_map = direct_handle(map, isolate);
    return true;
  }
  return false;
}

bool TryReadWasm32CopyPrototypeIteratorResultLayout(Isolate* isolate,
                                                    Tagged<Object> receiver,
                                                    Handle<Name> name,
                                                    Address* out_result) {
  if (!CurrentInterpreterFunctionMatchesPerContextPrimordialsCopyPrototype()) {
    return false;
  }
  if (!IsJSObject(receiver)) return false;
  Tagged<Map> map = Cast<JSObject>(receiver)->map();
  if (map->instance_type() != JS_OBJECT_TYPE ||
      map->instance_size() < JSIteratorResult::kSize) {
    return false;
  }

  Tagged<JSIteratorResult> iterator_result = Cast<JSIteratorResult>(receiver);
  if (Name::Equals(isolate, name, isolate->factory()->done_string())) {
    *out_result = iterator_result->done().ptr();
    return true;
  }
  if (Name::Equals(isolate, name, isolate->factory()->value_string())) {
    *out_result = iterator_result->value().ptr();
    return true;
  }
  return false;
}

bool IsReflectOwnKeysTraceName(Isolate* isolate, Tagged<Object> name_object) {
  if (!IsName(name_object)) return false;
  HandleScope scope(isolate);
  Handle<Name> name = handle(Cast<Name>(name_object), isolate);
  return Name::Equals(isolate, name,
                      isolate->factory()->InternalizeUtf8String("ownKeys")) ||
         Name::Equals(isolate, name,
                      isolate->factory()->InternalizeUtf8String(
                          "ReflectOwnKeys"));
}

bool IsBuiltinModuleStateTraceName(Isolate* isolate,
                                   Tagged<Object> name_object) {
  if (!IsName(name_object)) return false;
  HandleScope scope(isolate);
  Handle<Name> name = handle(Cast<Name>(name_object), isolate);
  return Name::Equals(isolate, name,
                      isolate->factory()->InternalizeUtf8String("loaded")) ||
         Name::Equals(isolate, name,
                      isolate->factory()->InternalizeUtf8String("loading")) ||
         Name::Equals(isolate, name,
                      isolate->factory()->InternalizeUtf8String("exports"));
}

Address SafeTaggedOrUndefined(Isolate* isolate, Address value);

extern "C" void Wasm32CopyDataPropertiesBuiltin() {
  Address root = g_wasm_regs[kWasmRegRoot];
  if (root == kNullAddress) root = g_wasm_regs[SlotFor(kRootRegister)];
  Isolate* isolate = GetWasm32IsolateFromRoot(&root);
  if (isolate == nullptr) return;
  ReadOnlyRoots roots(isolate);

  Address target_address =
      SafeTaggedOrUndefined(isolate, g_wasm_regs[SlotFor(Register::r0())]);
  Address source_address =
      SafeTaggedOrUndefined(isolate, g_wasm_regs[SlotFor(Register::r1())]);

  static int copy_data_properties_trace_count = 0;
  if (kTraceWasmFallbackDetails && copy_data_properties_trace_count < 12) {
    ++copy_data_properties_trace_count;
    PrintF("Wasm32CopyDataPropertiesBuiltin: call count=%d ",
           copy_data_properties_trace_count);
    DumpRuntimeArg("target", 0, target_address);
    DumpRuntimeArg("source", 0, source_address);
    PrintF("\n");
  }

  Tagged<Context> saved_context = isolate->context();
  Address context_address = g_wasm_regs[SlotFor(kContextRegister)];
  bool switched_context = false;
  if (IsSafeTaggedHandleValue(context_address) &&
      IsContext(Tagged<Object>(context_address))) {
    isolate->set_context(Cast<Context>(Tagged<Object>(context_address)));
    switched_context = true;
  }

  Address result = roots.undefined_value().ptr();
  if (!IsSafeTaggedHandleValue(target_address) ||
      !IsJSReceiver(Tagged<Object>(target_address))) {
    PrintF("Wasm32CopyDataPropertiesBuiltin: target invalid ");
    DumpRuntimeArg("target", 0, target_address);
    PrintF("\n");
    result = roots.exception().ptr();
  } else if (!IsUndefined(Tagged<Object>(source_address), isolate) &&
             !IsNull(Tagged<Object>(source_address), isolate)) {
    HandleScope scope(isolate);
    WasmGCStateScope gc_state(isolate);
    DirectHandle<JSReceiver> target =
        direct_handle(Cast<JSReceiver>(Tagged<Object>(target_address)),
                      isolate);
    DirectHandle<Object> source =
        direct_handle(Tagged<Object>(source_address), isolate);
    Maybe<bool> copied = JSReceiver::SetOrCopyDataProperties(
        isolate, target, source,
        PropertiesEnumerationMode::kPropertyAdditionOrder, {}, false);
    if (copied.IsNothing()) result = roots.exception().ptr();
  }

  if (switched_context) isolate->set_context(saved_context);
  g_wasm_regs[SlotFor(kReturnRegister0)] = result;
}

extern "C" void Wasm32LoadICFunctionPrototypeBuiltin() {
  Address root = g_wasm_regs[kWasmRegRoot];
  if (root == kNullAddress) root = g_wasm_regs[SlotFor(kRootRegister)];
  Isolate* isolate = GetWasm32IsolateFromRoot(&root);
  if (isolate == nullptr) return;
  ReadOnlyRoots roots(isolate);

  Address receiver_address =
      SafeTaggedOrUndefined(isolate, g_wasm_regs[SlotFor(Register::r1())]);
  Address result = roots.exception().ptr();
  Tagged<Context> saved_context = isolate->context();
  Address context_address = g_wasm_regs[SlotFor(kContextRegister)];
  bool switched_context = false;
  if (IsSafeTaggedHandleValue(context_address) &&
      IsContext(Tagged<Object>(context_address))) {
    isolate->set_context(Cast<Context>(Tagged<Object>(context_address)));
    switched_context = true;
  }

  {
    HandleScope scope(isolate);
    WasmGCStateScope gc_state(isolate);
    if (IsSafeTaggedHandleValue(receiver_address) &&
        IsJSFunction(Tagged<Object>(receiver_address))) {
      DirectHandle<JSFunction> function =
          direct_handle(Cast<JSFunction>(Tagged<Object>(receiver_address)),
                        isolate);
      if (function->has_prototype_slot()) {
        result = Wasm32JSFunctionPrototypeAddress(isolate, function);
      } else {
        result = roots.undefined_value().ptr();
      }
    } else if (IsSafeTaggedHandleValue(receiver_address) &&
               IsJSReceiver(Tagged<Object>(receiver_address))) {
      DirectHandle<JSAny> receiver =
          direct_handle(Cast<JSAny>(Tagged<Object>(receiver_address)), isolate);
      DirectHandle<Name> name = isolate->factory()->prototype_string();
      DirectHandle<Object> value;
      if (GetObjectPropertyPreservingWasmInterpreterState(isolate, receiver,
                                                          name)
              .ToHandle(&value)) {
        result = (*value).ptr();
      }
    }
  }

  if (switched_context) isolate->set_context(saved_context);
  g_wasm_regs[SlotFor(kReturnRegister0)] = result;
}

Address RuntimeVisibleArg(Address* argv, int argc, int index) {
  DCHECK_GE(index, 0);
  DCHECK_LT(index, argc);
  return argv[argc - 1 - index];
}

Tagged<SharedFunctionInfo> Wasm32JSFunctionShared(
    Tagged<JSFunction> function) {
  Tagged<Object> direct =
      TaggedField<Object>::load(function, JSFunction::kSharedFunctionInfoOffset);
  if (IsSharedFunctionInfo(direct)) return Cast<SharedFunctionInfo>(direct);
  Tagged<Object> shifted = TaggedField<Object>::load(
      function, JSFunction::kSharedFunctionInfoOffset - kTaggedSize);
  if (IsSharedFunctionInfo(shifted)) return Cast<SharedFunctionInfo>(shifted);
  return Cast<SharedFunctionInfo>(direct);
}

Tagged<Context> Wasm32JSFunctionContext(Tagged<JSFunction> function) {
  Tagged<Object> direct =
      TaggedField<Object>::load(function, JSFunction::kContextOffset);
  if (IsContext(direct)) return Cast<Context>(direct);
  Tagged<Object> shifted =
      TaggedField<Object>::load(function, JSFunction::kContextOffset -
                                              kTaggedSize);
  if (IsContext(shifted)) return Cast<Context>(shifted);
  return Cast<Context>(direct);
}

Tagged<Object> Wasm32JSFunctionFeedbackCellObject(
    Tagged<JSFunction> function) {
  Tagged<Object> direct =
      TaggedField<Object>::load(function, JSFunction::kFeedbackCellOffset);
  if (IsFeedbackCell(direct)) return direct;
  Tagged<Object> shifted = TaggedField<Object>::load(
      function, JSFunction::kFeedbackCellOffset - kTaggedSize);
  if (IsFeedbackCell(shifted)) return shifted;
  return direct;
}

int Wasm32JSFunctionFeedbackCellOffset(Tagged<JSFunction> function) {
  Tagged<Object> direct =
      TaggedField<Object>::load(function, JSFunction::kFeedbackCellOffset);
  if (IsFeedbackCell(direct)) return JSFunction::kFeedbackCellOffset;
  Tagged<Object> shifted = TaggedField<Object>::load(
      function, JSFunction::kFeedbackCellOffset - kTaggedSize);
  if (IsFeedbackCell(shifted)) return JSFunction::kFeedbackCellOffset -
                                      kTaggedSize;
  return JSFunction::kFeedbackCellOffset;
}

Tagged<FeedbackCell> Wasm32JSFunctionRawFeedbackCell(
    Tagged<JSFunction> function) {
  return Cast<FeedbackCell>(Wasm32JSFunctionFeedbackCellObject(function));
}

void Wasm32StoreJSFunctionRawFeedbackCell(Tagged<JSFunction> function,
                                          Tagged<FeedbackCell> value) {
  int offset = Wasm32JSFunctionFeedbackCellOffset(function);
  TaggedField<Object>::Release_Store(function, offset, value);
  WriteBarrier::ForValue(function, function->RawMaybeWeakField(offset), value,
                         UPDATE_WRITE_BARRIER);
}

bool Wasm32IsPrototypeOrInitialMapValue(Tagged<Object> value) {
  return IsMap(value) || IsJSReceiver(value) || IsTheHole(value);
}

int Wasm32JSFunctionPrototypeOrInitialMapOffset(Tagged<JSFunction> function) {
  USE(function);
  return JSFunction::kPrototypeOrInitialMapOffset;
}

Tagged<Object> Wasm32JSFunctionPrototypeOrInitialMapObject(
    Tagged<JSFunction> function) {
  return TaggedField<Object>::load(
      function, Wasm32JSFunctionPrototypeOrInitialMapOffset(function));
}

bool Wasm32JSFunctionHasInitialMap(Tagged<JSFunction> function) {
  return function->has_prototype_slot() && function->has_initial_map();
}

Tagged<Map> Wasm32JSFunctionInitialMap(Tagged<JSFunction> function) {
  return function->initial_map();
}

bool TryResolveWasm32NativeContext(Tagged<Context> context,
                                   Tagged<Context>* out_context);

Handle<JSObject> NewWasm32PlainJSObject(Isolate* isolate) {
  Tagged<Context> native_context_value = isolate->context();
  if (TryResolveWasm32NativeContext(isolate->context(), &native_context_value) &&
      IsNativeContext(native_context_value)) {
    DirectHandle<NativeContext> native_context(
        Cast<NativeContext>(native_context_value), isolate);
    DirectHandle<Map> map =
        isolate->factory()->ObjectLiteralMapFromCache(native_context, 0);
    return isolate->factory()->NewFastOrSlowJSObjectFromMap(map, 0);
  }

  DirectHandle<JSFunction> object_function = isolate->object_function();
  if (Wasm32JSFunctionHasInitialMap(*object_function)) {
    DirectHandle<Map> map(Wasm32JSFunctionInitialMap(*object_function),
                          isolate);
    if (map->instance_type() == JS_OBJECT_TYPE) {
      return isolate->factory()->NewFastOrSlowJSObjectFromMap(map, 0);
    }
  }
  return isolate->factory()->NewJSObject(object_function);
}

void Wasm32StoreJSFunctionPrototypeOrInitialMap(Tagged<JSFunction> function,
                                                Tagged<Object> value) {
  int offset = Wasm32JSFunctionPrototypeOrInitialMapOffset(function);
  TaggedField<Object>::Release_Store(function, offset, value);
  WriteBarrier::ForValue(function, function->RawMaybeWeakField(offset), value,
                         UPDATE_WRITE_BARRIER);
}

bool Wasm32IsMapPrototypeValue(Tagged<Object> value, ReadOnlyRoots roots) {
  return IsNull(value, roots) || IsJSReceiver(value);
}

Tagged<Object> Wasm32MapPrototypeObject(Isolate* isolate, Tagged<Map> map) {
  ReadOnlyRoots roots(isolate);
  Tagged<Object> shifted =
      TaggedField<Object>::load(map, Map::kPrototypeOffset - kTaggedSize);
  if (Wasm32IsMapPrototypeValue(shifted, roots)) return shifted;

  Tagged<Object> direct =
      TaggedField<Object>::load(map, Map::kPrototypeOffset);
  if (Wasm32IsMapPrototypeValue(direct, roots)) return direct;

  return map->prototype();
}

bool Wasm32IsContextValue(Tagged<Object> value) {
  return IsSafeTaggedHandleValue(value.ptr()) && IsContext(value);
}

bool Wasm32IsNativeContextValue(Tagged<Object> value) {
  return IsSafeTaggedHandleValue(value.ptr()) && IsNativeContext(value);
}

bool TryWasm32NativeContextValue(Tagged<Object> value,
                                 Tagged<Context>* out_context) {
  if (!Wasm32IsNativeContextValue(value)) return false;
  *out_context = Cast<Context>(value);
  return true;
}

bool TryWasm32ContextValue(Tagged<Object> value, Tagged<Context>* out_context) {
  if (!Wasm32IsContextValue(value)) return false;
  *out_context = Cast<Context>(value);
  return true;
}

bool TryWasm32MapNativeContextSlot(Tagged<Map> map, int offset,
                                   Tagged<Context>* out_context) {
  if (offset < 0) return false;
  if (!IsReadableTaggedHeapObject(map.ptr(), offset + sizeof(Address))) {
    return false;
  }
  return TryWasm32NativeContextValue(TaggedField<Object>::load(map, offset),
                                    out_context);
}

bool TryWasm32ContextMapNativeContext(Tagged<Context> context,
                                      Tagged<Context>* out_context) {
  if (TryWasm32NativeContextValue(context, out_context)) return true;
  if (!Wasm32IsContextValue(context)) return false;

  Tagged<Map> map = context->map();
  if (TryWasm32MapNativeContextSlot(
          map, Map::kConstructorOrBackPointerOrNativeContextOffset - kTaggedSize,
          out_context)) {
    return true;
  }
  if (TryWasm32MapNativeContextSlot(
          map, Map::kConstructorOrBackPointerOrNativeContextOffset,
          out_context)) {
    return true;
  }

  Tagged<NativeContext> native_context = context->native_context();
  return TryWasm32NativeContextValue(native_context, out_context);
}

bool TryWasm32ContextPrevious(Tagged<Context> context,
                              Tagged<Context>* out_context) {
  Tagged<Object> previous = context->unchecked_previous();
  if (TryWasm32ContextValue(previous, out_context)) return true;

  int shifted_offset =
      Context::OffsetOfElementAt(Context::PREVIOUS_INDEX) - kTaggedSize;
  if (shifted_offset < 0 ||
      !IsReadableTaggedHeapObject(context.ptr(),
                                  shifted_offset + sizeof(Address))) {
    return false;
  }
  return TryWasm32ContextValue(TaggedField<Object>::load(context, shifted_offset),
                               out_context);
}

bool TryResolveWasm32NativeContext(Tagged<Context> context,
                                   Tagged<Context>* out_context) {
  Tagged<Context> current = context;
  for (int depth = 0; depth < 16; ++depth) {
    if (TryWasm32ContextMapNativeContext(current, out_context)) return true;
    Tagged<Context> previous = current;
    if (!TryWasm32ContextPrevious(current, &previous)) return false;
    if (previous.ptr() == current.ptr()) return false;
    current = previous;
  }
  return false;
}

Address Wasm32JSFunctionPrototypeAddress(Isolate* isolate,
                                         DirectHandle<JSFunction> function) {
  ReadOnlyRoots roots(isolate);
  if (!function->has_prototype_slot()) return roots.undefined_value().ptr();

  if (!function->has_prototype()) {
    DirectHandle<JSObject> prototype =
        isolate->factory()->NewFunctionPrototype(function);
    JSFunction::SetPrototype(isolate, function, prototype);
  }
  return function->prototype().ptr();
}

Tagged<Object> Wasm32JSFunctionFeedbackVectorOrUndefined(
    Isolate* isolate, Tagged<JSFunction> function) {
  Tagged<Object> cell = Wasm32JSFunctionFeedbackCellObject(function);
  if (!IsFeedbackCell(cell)) return ReadOnlyRoots(isolate).undefined_value();
  Tagged<Object> value = Cast<FeedbackCell>(cell)->value();
  if (!IsFeedbackVector(value)) return ReadOnlyRoots(isolate).undefined_value();
  return value;
}

Tagged<ClosureFeedbackCellArray> Wasm32EnsureClosureFeedbackCellArray(
    Isolate* isolate, DirectHandle<JSFunction> function_handle) {
  Tagged<JSFunction> function = *function_handle;
  Tagged<FeedbackCell> raw_feedback_cell =
      Wasm32JSFunctionRawFeedbackCell(function);
  Tagged<Object> feedback_value = raw_feedback_cell->value();
  if (IsClosureFeedbackCellArray(feedback_value)) {
    return Cast<ClosureFeedbackCellArray>(feedback_value);
  }
  if (IsFeedbackVector(feedback_value)) {
    return Cast<FeedbackVector>(feedback_value)->closure_feedback_cell_array();
  }

  DirectHandle<SharedFunctionInfo> shared(
      Wasm32JSFunctionShared(function), isolate);
  DirectHandle<ClosureFeedbackCellArray> feedback_cell_array =
      ClosureFeedbackCellArray::New(isolate, shared);
  if (raw_feedback_cell == *isolate->factory()->many_closures_cell()) {
    DirectHandle<FeedbackCell> feedback_cell =
        isolate->factory()->NewOneClosureCell(feedback_cell_array);
#ifdef V8_ENABLE_LEAPTIERING
    feedback_cell->set_dispatch_handle(function->dispatch_handle());
#endif
    Wasm32StoreJSFunctionRawFeedbackCell(function, *feedback_cell);
  } else {
    raw_feedback_cell->set_value(*feedback_cell_array, kReleaseStore);
  }
  return *feedback_cell_array;
}

bool IsFeedbackVectorAddress(Address value) {
  return IsReadableTaggedHeapObject(value, HeapObject::kMapOffset +
                                               sizeof(Address)) &&
         HasReadableHeapObjectMap(value) &&
         IsFeedbackVector(Tagged<Object>(value));
}

bool IsNameAddress(Address value) {
  return IsReadableTaggedHeapObject(value, HeapObject::kMapOffset +
                                               sizeof(Address)) &&
         HasReadableHeapObjectMap(value) && IsName(Tagged<Object>(value));
}

bool IsJSAnyForWasmPropertyLookup(Isolate* isolate, Address value) {
  if (IsKnownJSAnyReadOnlyRootValue(isolate, value)) return true;
  if (!IsPlausibleTaggedValue(value)) return false;
  Tagged<Object> object(value);
  ReadOnlyRoots roots(isolate);
  if (IsSmi(object)) return true;
  if (!IsHeapObject(object)) return false;
  Tagged<HeapObject> heap_object = Cast<HeapObject>(object);
  if (!ReadOnlyHeap::Contains(heap_object) &&
      !isolate->heap()->Contains(heap_object)) {
    return false;
  }
  return IsJSReceiver(object) || IsString(object) || IsSymbol(object) ||
         IsBigInt(object) || IsHeapNumber(object) || IsOddball(object);
}

bool IsSafeJSAnyForWasmPropertyLookup(Isolate* isolate, Address value) {
  if (IsKnownJSAnyReadOnlyRootValue(isolate, value)) return true;
  if (!IsSafeTaggedRootValue(isolate, value)) return false;
  Tagged<Object> object(value);
  ReadOnlyRoots roots(isolate);
  return IsSmi(object) || IsJSReceiver(object) || IsString(object) ||
         IsSymbol(object) || IsBigInt(object) || IsHeapNumber(object) ||
         IsOddball(object);
}

Address CurrentInterpreterContext() {
  return g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
      interpreter::Register::current_context().ToOperand() * kSystemPointerSize)];
}

uint32_t ReadBytecodeUnsignedOperand(Tagged<BytecodeArray> bytecode,
                                     int bytecode_index,
                                     interpreter::Bytecode bytecode_enum,
                                     int operand_index,
                                     interpreter::OperandScale operand_scale) {
  int offset = interpreter::Bytecodes::GetOperandOffset(bytecode_enum,
                                                        operand_index,
                                                        operand_scale);
  interpreter::OperandSize size =
      interpreter::Bytecodes::GetOperandSize(bytecode_enum, operand_index,
                                             operand_scale);
  int index = bytecode_index + offset;
  switch (size) {
    case interpreter::OperandSize::kByte:
      return bytecode->get(index);
    case interpreter::OperandSize::kShort:
      return bytecode->get(index) | (bytecode->get(index + 1) << 8);
    case interpreter::OperandSize::kQuad:
      return bytecode->get(index) | (bytecode->get(index + 1) << 8) |
             (bytecode->get(index + 2) << 16) |
             (bytecode->get(index + 3) << 24);
    case interpreter::OperandSize::kNone:
      return 0;
  }
}

int32_t ReadBytecodeSignedOperand(Tagged<BytecodeArray> bytecode,
                                  int bytecode_index,
                                  interpreter::Bytecode bytecode_enum,
                                  int operand_index,
                                  interpreter::OperandScale operand_scale) {
  int offset = interpreter::Bytecodes::GetOperandOffset(bytecode_enum,
                                                        operand_index,
                                                        operand_scale);
  interpreter::OperandSize size =
      interpreter::Bytecodes::GetOperandSize(bytecode_enum, operand_index,
                                             operand_scale);
  int index = bytecode_index + offset;
  switch (size) {
    case interpreter::OperandSize::kByte:
      return static_cast<int8_t>(bytecode->get(index));
    case interpreter::OperandSize::kShort:
      return static_cast<int16_t>(bytecode->get(index) |
                                  (bytecode->get(index + 1) << 8));
    case interpreter::OperandSize::kQuad:
      return static_cast<int32_t>(bytecode->get(index) |
                                  (bytecode->get(index + 1) << 8) |
                                  (bytecode->get(index + 2) << 16) |
                                  (bytecode->get(index + 3) << 24));
    case interpreter::OperandSize::kNone:
      return 0;
  }
}

void DumpBytecodeWindowForTrace(Tagged<BytecodeArray> bytecode,
                                int center_index, int before, int after,
                                const char* label) {
  int window_start = center_index - before;
  if (window_start < 0) window_start = 0;
  int window_end = center_index + after;
  if (window_end > bytecode->length()) window_end = bytecode->length();
  interpreter::OperandScale dump_scale = interpreter::OperandScale::kSingle;
  PrintF("WasmInterpreterEntryTrampoline: %s bytecode_window center=%d "
         "range=[%d,%d) length=%d\n",
         label, center_index, window_start, window_end, bytecode->length());
  for (int index = 0; index < bytecode->length();) {
    uint8_t dump_opcode = bytecode->get(index);
    interpreter::Bytecode dump_bytecode =
        interpreter::Bytecodes::FromByte(dump_opcode);
    int dump_size = interpreter::Bytecodes::Size(dump_bytecode, dump_scale);
    if (index >= window_start && index <= window_end) {
      PrintF("  %s bc index=%d op=0x%x(%s) scale=%d size=%d",
             label, index, static_cast<unsigned>(dump_opcode),
             interpreter::Bytecodes::ToString(dump_bytecode),
             static_cast<int>(dump_scale), dump_size);
      int operand_count =
          interpreter::Bytecodes::NumberOfOperands(dump_bytecode);
      for (int operand_index = 0; operand_index < operand_count;
           ++operand_index) {
        uint32_t operand_u = ReadBytecodeUnsignedOperand(
            bytecode, index, dump_bytecode, operand_index, dump_scale);
        int32_t operand_s = ReadBytecodeSignedOperand(
            bytecode, index, dump_bytecode, operand_index, dump_scale);
        PrintF(" #%d=u0x%x/s%d", operand_index, operand_u, operand_s);
      }
      PrintF(" raw:");
      for (int raw_index = 0; raw_index < dump_size; ++raw_index) {
        PrintF(" %02x", bytecode->get(index + raw_index));
      }
      PrintF("\n");
    }
    if (interpreter::Bytecodes::IsPrefixScalingBytecode(dump_bytecode)) {
      dump_scale =
          interpreter::Bytecodes::PrefixBytecodeToOperandScale(dump_bytecode);
      index += interpreter::Bytecodes::Size(
          dump_bytecode, interpreter::OperandScale::kSingle);
      continue;
    }
    dump_scale = interpreter::OperandScale::kSingle;
    index += dump_size;
  }
}

Address ReadInterpreterRegister(interpreter::Register reg) {
  int slot = InterpreterFrameSlotForOffset(
      reg.ToOperand() * kSystemPointerSize);
  CHECK_GE(slot, 0);
  CHECK_LT(slot, kWasmInterpreterFrameSlots);
  return g_wasm_interpreter_frame[slot];
}

void DumpInterpreterRegisterValue(const char* label, int index,
                                  interpreter::Register reg) {
  PrintF(" %s[%d]{operand=%d,index=%d}", label, index, reg.ToOperand(),
         reg.index());
  DumpRuntimeArg("value", 0, ReadInterpreterRegister(reg));
}

bool CurrentInterpreterFunctionMatchesTraceNeedle() {
  Address function_address =
      g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
          StandardFrameConstants::kFunctionOffset)];
  if (!IsSafeTaggedHandleValue(function_address) ||
      !IsJSFunction(Tagged<Object>(function_address))) {
    return false;
  }
  return FunctionMatchesWasmEvalTraceNeedle(
      Cast<JSFunction>(Tagged<Object>(function_address))->shared());
}

bool CurrentInterpreterFunctionMatchesPerContextPrimordialsCopyPrototype() {
  Address function_address =
      g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
          StandardFrameConstants::kFunctionOffset)];
  if (!IsSafeTaggedHandleValue(function_address) ||
      !IsJSFunction(Tagged<Object>(function_address))) {
    return false;
  }
  return FunctionMatchesPerContextPrimordialsCopyPrototype(
      Wasm32JSFunctionShared(Cast<JSFunction>(Tagged<Object>(function_address))));
}

bool TryRunCreateFunctionContextBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kCreateFunctionContext &&
      bytecode_enum != interpreter::Bytecode::kCreateEvalContext) {
    return false;
  }

  uint32_t scope_info_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
  Tagged<TrustedFixedArray> constant_pool = bytecode->constant_pool();
  if (scope_info_index >= static_cast<uint32_t>(constant_pool->length())) {
    PrintF("WasmInterpreterEntryTrampoline: bad scope_info index=%u length=%d\n",
           scope_info_index, constant_pool->length());
    return false;
  }
  Tagged<Object> scope_info_object = constant_pool->get(scope_info_index);
  if (!IsScopeInfo(scope_info_object)) {
    PrintF("WasmInterpreterEntryTrampoline: constant is not ScopeInfo index=%u ",
           scope_info_index);
    DumpRuntimeArg("value", 0, scope_info_object.ptr());
    PrintF("\n");
    return false;
  }

  Address context_address = CurrentInterpreterContext();
  if (!IsContext(Tagged<Object>(context_address))) {
    PrintF("WasmInterpreterEntryTrampoline: current context is invalid ");
    DumpRuntimeArg("context", 0, context_address);
    PrintF("\n");
    return false;
  }

  HandleScope scope(isolate);
  DirectHandle<Context> outer =
      direct_handle(Cast<Context>(Tagged<Object>(context_address)), isolate);
  DirectHandle<ScopeInfo> scope_info =
      direct_handle(Cast<ScopeInfo>(scope_info_object), isolate);
  DirectHandle<Context> result =
      isolate->factory()->NewFunctionContext(outer, scope_info);
  *out_result = (*result).ptr();
  return true;
}

bool TryRunCreateBlockContextBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kCreateBlockContext) {
    return false;
  }

  uint32_t scope_info_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
  Tagged<TrustedFixedArray> constant_pool = bytecode->constant_pool();
  if (scope_info_index >= static_cast<uint32_t>(constant_pool->length())) {
    PrintF("WasmInterpreterEntryTrampoline: bad block scope index=%u "
           "length=%d\n",
           scope_info_index, constant_pool->length());
    return false;
  }
  Tagged<Object> scope_info_object = constant_pool->get(scope_info_index);
  if (!IsScopeInfo(scope_info_object)) {
    PrintF("WasmInterpreterEntryTrampoline: block constant is not ScopeInfo "
           "index=%u ",
           scope_info_index);
    DumpRuntimeArg("value", 0, scope_info_object.ptr());
    PrintF("\n");
    return false;
  }

  Address context_address = CurrentInterpreterContext();
  if (!IsContext(Tagged<Object>(context_address))) {
    PrintF("WasmInterpreterEntryTrampoline: block current context invalid ");
    DumpRuntimeArg("context", 0, context_address);
    PrintF("\n");
    return false;
  }

  HandleScope scope(isolate);
  DirectHandle<Context> outer =
      direct_handle(Cast<Context>(Tagged<Object>(context_address)), isolate);
  DirectHandle<ScopeInfo> scope_info =
      direct_handle(Cast<ScopeInfo>(scope_info_object), isolate);
  DirectHandle<Context> result =
      isolate->factory()->NewBlockContext(outer, scope_info);
  *out_result = (*result).ptr();
  return true;
}

bool TryRunCreateClosureBytecode(Isolate* isolate,
                                 Tagged<BytecodeArray> bytecode,
                                 int bytecode_index,
                                 interpreter::Bytecode bytecode_enum,
                                 interpreter::OperandScale operand_scale,
                                 Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kCreateClosure) return false;

  uint32_t shared_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
  uint32_t feedback_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                  operand_scale);
  uint32_t flags =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 2,
                                  operand_scale);

  Tagged<TrustedFixedArray> constant_pool = bytecode->constant_pool();
  if (shared_index >= static_cast<uint32_t>(constant_pool->length())) {
    PrintF("WasmInterpreterEntryTrampoline: bad shared index=%u length=%d\n",
           shared_index, constant_pool->length());
    return false;
  }
  Tagged<Object> shared_object = constant_pool->get(shared_index);
  if (!IsSharedFunctionInfo(shared_object)) {
    PrintF("WasmInterpreterEntryTrampoline: constant is not SFI index=%u ",
           shared_index);
    DumpRuntimeArg("value", 0, shared_object.ptr());
    PrintF("\n");
    return false;
  }

  Address function_address =
      g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
          StandardFrameConstants::kFunctionOffset)];
  if (!IsJSFunction(Tagged<Object>(function_address))) {
    PrintF("WasmInterpreterEntryTrampoline: function slot is invalid ");
    DumpRuntimeArg("function", 0, function_address);
    PrintF("\n");
    return false;
  }
  Address context_address = CurrentInterpreterContext();
  if (!IsContext(Tagged<Object>(context_address))) {
    PrintF("WasmInterpreterEntryTrampoline: current context is invalid ");
    DumpRuntimeArg("context", 0, context_address);
    PrintF("\n");
    return false;
  }

  HandleScope scope(isolate);
  DirectHandle<JSFunction> closure =
      direct_handle(Cast<JSFunction>(Tagged<Object>(function_address)), isolate);
  Tagged<ClosureFeedbackCellArray> feedback_cell_array =
      Wasm32EnsureClosureFeedbackCellArray(isolate, closure);
  DirectHandle<SharedFunctionInfo> shared =
      direct_handle(Cast<SharedFunctionInfo>(shared_object), isolate);
  MaybeDirectHandle<FeedbackCell> maybe_feedback_cell;
  if (feedback_index <
      static_cast<uint32_t>(feedback_cell_array->length())) {
    maybe_feedback_cell =
        direct_handle(feedback_cell_array->get(feedback_index), isolate);
  } else if (kTraceWasmFallbackDetails) {
    PrintF("WasmInterpreterEntryTrampoline: CreateClosure missing feedback "
           "index=%u length=%d\n",
           feedback_index, feedback_cell_array->length());
  }
  DirectHandle<Context> context =
      direct_handle(Cast<Context>(Tagged<Object>(context_address)), isolate);
  AllocationType allocation =
      interpreter::CreateClosureFlags::PretenuredBit::decode(flags)
          ? AllocationType::kOld
          : AllocationType::kYoung;
  Factory::JSFunctionBuilder builder{isolate, shared, context};
  DirectHandle<FeedbackCell> feedback_cell;
  if (maybe_feedback_cell.ToHandle(&feedback_cell)) {
    builder.set_feedback_cell(feedback_cell);
  }
  Handle<JSFunction> result = builder.set_allocation_type(allocation).Build();
  *out_result = (*result).ptr();
  return true;
}

bool TryRunLdaContextSlotBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  bool current_context = false;
  bool no_cell = false;
  int slot_operand_index = 0;
  int depth_operand_index = -1;
  Address context_address = kNullAddress;

  switch (bytecode_enum) {
    case interpreter::Bytecode::kLdaContextSlotNoCell:
      no_cell = true;
      slot_operand_index = 1;
      depth_operand_index = 2;
      break;
    case interpreter::Bytecode::kLdaContextSlot:
      slot_operand_index = 1;
      depth_operand_index = 2;
      break;
    case interpreter::Bytecode::kLdaImmutableContextSlot:
      no_cell = true;
      slot_operand_index = 1;
      depth_operand_index = 2;
      break;
    case interpreter::Bytecode::kLdaCurrentContextSlotNoCell:
      current_context = true;
      no_cell = true;
      break;
    case interpreter::Bytecode::kLdaCurrentContextSlot:
      current_context = true;
      break;
    case interpreter::Bytecode::kLdaImmutableCurrentContextSlot:
      current_context = true;
      no_cell = true;
      break;
    default:
      return false;
  }

  if (current_context) {
    context_address = CurrentInterpreterContext();
  } else {
    int32_t context_operand =
        ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
    context_address = ReadInterpreterRegister(
        interpreter::Register::FromOperand(context_operand));
  }
  ReadOnlyRoots roots(isolate);
  if (!IsSafeTaggedHandleValue(context_address) ||
      !IsContext(Tagged<Object>(context_address))) {
    *out_result = roots.exception().ptr();
    return true;
  }

  Tagged<Context> context = Cast<Context>(Tagged<Object>(context_address));
  Tagged<Context> base_context = context;
  uint32_t depth = 0;
  if (depth_operand_index >= 0) {
    depth = ReadBytecodeUnsignedOperand(bytecode, bytecode_index,
                                        bytecode_enum, depth_operand_index,
                                        operand_scale);
    for (uint32_t i = 0; i < depth; ++i) {
      Tagged<Object> previous = context->previous();
      if (!IsContext(previous)) {
        *out_result = roots.exception().ptr();
        return true;
      }
      context = Cast<Context>(previous);
    }
  }

  uint32_t slot_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum,
                                  slot_operand_index, operand_scale);
  if (slot_index >= static_cast<uint32_t>(context->length())) {
    *out_result = roots.exception().ptr();
    return true;
  }

  if (!no_cell) {
    HandleScope scope(isolate);
    DirectHandle<Context> context_handle = direct_handle(context, isolate);
    DirectHandle<Object> value =
        Context::Get(context_handle, static_cast<int>(slot_index), isolate);
    if (kTraceWasmFallbackDetails && slot_index == 46 &&
        g_context_slot46_trace_count < 24) {
      ++g_context_slot46_trace_count;
      PrintF("WasmInterpreterEntryTrampoline: LdaContextSlot slot46 "
             "bytecode_index=%d no_cell=%d current=%d depth_index=%d "
             "context=0x%x",
             bytecode_index, no_cell, current_context, depth_operand_index,
             static_cast<unsigned>(context.ptr()));
      DumpRuntimeArg(" value", 0, (*value).ptr());
      PrintF("\n");
    }
    *out_result = (*value).ptr();
    return true;
  }
  Tagged<Object> value = context->GetNoCell(slot_index);
  if (kTraceWasmFallbackDetails && slot_index == 43 && depth == 2 &&
      g_context_slot43_depth2_trace_count < 8) {
    ++g_context_slot43_depth2_trace_count;
    PrintF("WasmInterpreterEntryTrampoline: LdaContextSlot slot43-depth2 "
           "bytecode_index=%d current=%d base=0x%x base_len=%d final=0x%x "
           "final_len=%d",
           bytecode_index, current_context ? 1 : 0,
           static_cast<unsigned>(base_context.ptr()), base_context->length(),
           static_cast<unsigned>(context.ptr()), context->length());
    DumpRuntimeArg(" value", 0, value.ptr());
    Tagged<Context> chain = base_context;
    for (uint32_t i = 0; i < 3; ++i) {
      PrintF(" chain%u=0x%x len=%d", i, static_cast<unsigned>(chain.ptr()),
             chain->length());
      if (static_cast<int>(slot_index) < chain->length()) {
        DumpRuntimeArg(" slot43", static_cast<int>(i),
                       chain->GetNoCell(slot_index).ptr());
      }
      chain = chain->previous();
    }
    PrintF("\n");
  }
  if (kTraceWasmFallbackDetails && slot_index == 46 &&
      g_context_slot46_trace_count < 24) {
    ++g_context_slot46_trace_count;
    PrintF("WasmInterpreterEntryTrampoline: LdaContextSlot slot46 "
           "bytecode_index=%d no_cell=%d current=%d depth_index=%d "
           "context=0x%x",
           bytecode_index, no_cell, current_context, depth_operand_index,
           static_cast<unsigned>(context.ptr()));
    DumpRuntimeArg(" value", 0, value.ptr());
    PrintF("\n");
  }
  *out_result = value.ptr();
  return true;
}

bool TryRunStaContextSlotBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  bool current_context = false;
  bool no_cell = false;
  int slot_operand_index = 0;
  int depth_operand_index = -1;
  Address context_address = kNullAddress;

  switch (bytecode_enum) {
    case interpreter::Bytecode::kStaContextSlotNoCell:
      no_cell = true;
      slot_operand_index = 1;
      depth_operand_index = 2;
      break;
    case interpreter::Bytecode::kStaContextSlot:
      slot_operand_index = 1;
      depth_operand_index = 2;
      break;
    case interpreter::Bytecode::kStaCurrentContextSlotNoCell:
      current_context = true;
      no_cell = true;
      break;
    case interpreter::Bytecode::kStaCurrentContextSlot:
      current_context = true;
      break;
    default:
      return false;
  }

  if (current_context) {
    context_address = CurrentInterpreterContext();
  } else {
    int32_t context_operand =
        ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
    context_address = ReadInterpreterRegister(
        interpreter::Register::FromOperand(context_operand));
  }
  ReadOnlyRoots roots(isolate);
  if (!IsSafeTaggedHandleValue(context_address) ||
      !IsContext(Tagged<Object>(context_address))) {
    *out_result = roots.exception().ptr();
    return true;
  }

  Tagged<Context> context = Cast<Context>(Tagged<Object>(context_address));
  if (depth_operand_index >= 0) {
    uint32_t depth =
        ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum,
                                    depth_operand_index, operand_scale);
    for (uint32_t i = 0; i < depth; ++i) {
      context = context->previous();
    }
  }

  uint32_t slot_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum,
                                  slot_operand_index, operand_scale);
  if (slot_index >= static_cast<uint32_t>(context->length())) {
    *out_result = roots.exception().ptr();
    return true;
  }

  Tagged<Object> raw_value(g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
  Tagged<Object> value = Is<JSAny>(raw_value) ? raw_value : roots.undefined_value();
  if (no_cell) {
    context->SetNoCell(slot_index, value, UPDATE_WRITE_BARRIER);
  } else {
    HandleScope scope(isolate);
    DirectHandle<Context> context_handle = direct_handle(context, isolate);
    DirectHandle<Object> value_handle = direct_handle(value, isolate);
    Context::Set(context_handle, static_cast<int>(slot_index), value_handle,
                 isolate);
  }
  if (kTraceWasmFallbackDetails && slot_index == 46 &&
      g_context_slot46_trace_count < 24) {
    ++g_context_slot46_trace_count;
    PrintF("WasmInterpreterEntryTrampoline: StaContextSlot slot46 "
           "bytecode_index=%d no_cell=%d current=%d depth_index=%d "
           "context=0x%x",
           bytecode_index, no_cell, current_context, depth_operand_index,
           static_cast<unsigned>(context.ptr()));
    DumpRuntimeArg(" value", 0, value.ptr());
    PrintF("\n");
  }
  *out_result = value.ptr();
  return true;
}

interpreter::Register RegisterFromListOperand(int32_t first_operand, int index);
Address SafeTaggedOrUndefined(Isolate* isolate, Address value);
Address SafeRuntimeArgOrUndefined(Isolate* isolate, Address value);
Address SafeTaggedRootOrUndefined(Isolate* isolate, Address value);

bool TryRunAsyncFunctionEnterIntrinsic(Isolate* isolate, Address* argv,
                                       int argc, Address* out_result) {
  ReadOnlyRoots roots(isolate);
  if (argc != 2) {
    *out_result = roots.undefined_value().ptr();
    return true;
  }

  Address closure_address = SafeRuntimeArgOrUndefined(
      isolate, RuntimeVisibleArg(argv, argc, 0));
  Address receiver_address = SafeRuntimeArgOrUndefined(
      isolate, RuntimeVisibleArg(argv, argc, 1));
  if (!IsSafeTaggedHandleValue(closure_address) ||
      !IsJSFunction(Tagged<Object>(closure_address))) {
    *out_result = roots.undefined_value().ptr();
    return true;
  }
  if (!IsSafeTaggedHandleValue(receiver_address) ||
      !Is<JSAny>(Tagged<Object>(receiver_address))) {
    receiver_address = roots.undefined_value().ptr();
  }

  HandleScope scope(isolate);
  DirectHandle<JSFunction> closure(
      Cast<JSFunction>(Tagged<Object>(closure_address)), isolate);
  Tagged<SharedFunctionInfo> shared = Wasm32JSFunctionShared(*closure);
  if ((!IsAsyncFunction(shared->kind()) &&
       !IsModuleWithTopLevelAwait(shared->kind())) ||
      !shared->HasBytecodeArray()) {
    *out_result = roots.undefined_value().ptr();
    return true;
  }

  Tagged<BytecodeArray> bytecode = shared->GetBytecodeArray(isolate);
  int length = bytecode->parameter_count_without_receiver() +
               bytecode->register_count();
  DirectHandle<FixedArray> parameters_and_registers =
      isolate->factory()->NewFixedArray(length);
  Handle<JSPromise> promise = isolate->factory()->NewJSPromise();

  Tagged<Context> context = isolate->context();
  DirectHandle<NativeContext> native_context(context->native_context(),
                                             isolate);
  DirectHandle<Map> map(native_context->async_function_object_map(), isolate);
  Handle<JSObject> object = isolate->factory()->NewJSObjectFromMap(map);
  Handle<JSAsyncFunctionObject> async_function_object =
      Cast<JSAsyncFunctionObject>(object);

  Tagged<JSAsyncFunctionObject> raw_async_function = *async_function_object;
  raw_async_function->set_function(*closure);
  raw_async_function->set_context(context);
  raw_async_function->set_receiver(
      Cast<JSAny>(Tagged<Object>(receiver_address)));
  raw_async_function->set_input_or_debug_pos(Smi::zero());
  raw_async_function->set_resume_mode(JSGeneratorObject::ResumeMode::kNext);
  raw_async_function->set_continuation(JSGeneratorObject::kGeneratorExecuting);
  raw_async_function->set_parameters_and_registers(*parameters_and_registers);
  raw_async_function->set_promise(*promise);

  *out_result = raw_async_function.ptr();
  return true;
}

bool TryRunRuntimeCallBytecode(Isolate* isolate, Tagged<BytecodeArray> bytecode,
                               int bytecode_index,
                               interpreter::Bytecode bytecode_enum,
                               interpreter::OperandScale operand_scale,
                               Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kCallRuntime &&
      bytecode_enum != interpreter::Bytecode::kInvokeIntrinsic) {
    return false;
  }

  uint32_t raw_id =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
  bool invokes_generator_create = false;
  bool invokes_generator_resume_mode = false;
  Runtime::FunctionId function_id;
  if (bytecode_enum == interpreter::Bytecode::kInvokeIntrinsic) {
    auto intrinsic_id =
        static_cast<interpreter::IntrinsicsHelper::IntrinsicId>(raw_id);
    invokes_generator_create =
        intrinsic_id ==
        interpreter::IntrinsicsHelper::IntrinsicId::kCreateJSGeneratorObject;
    invokes_generator_resume_mode =
        intrinsic_id ==
        interpreter::IntrinsicsHelper::IntrinsicId::kGeneratorGetResumeMode;
    function_id = interpreter::IntrinsicsHelper::ToRuntimeId(intrinsic_id);
  } else {
    function_id = static_cast<Runtime::FunctionId>(raw_id);
  }
  bool is_generator_create =
      invokes_generator_create ||
      function_id == Runtime::kInlineCreateJSGeneratorObject ||
      function_id == Runtime::kCreateJSGeneratorObject;
  bool is_generator_resume_mode =
      invokes_generator_resume_mode ||
      function_id == Runtime::kInlineGeneratorGetResumeMode ||
      function_id == Runtime::kGeneratorGetResumeMode;
  const Runtime::Function* function = Runtime::FunctionForId(function_id);
  ReadOnlyRoots roots(isolate);
  if ((function == nullptr || function->result_size != 1) &&
      !is_generator_create && !is_generator_resume_mode) {
    *out_result = roots.undefined_value().ptr();
    return true;
  }
  if (function_id == Runtime::kThrowIteratorResultNotAnObject) {
    *out_result = roots.undefined_value().ptr();
    return true;
  }

  uint32_t reg_count =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 2,
                                  operand_scale);
  constexpr int kMaxRuntimeFallbackArgs = 64;
  if (reg_count > kMaxRuntimeFallbackArgs ||
      (function != nullptr && function->nargs >= 0 &&
       function->nargs != static_cast<int>(reg_count)) ||
      (is_generator_create && reg_count != 2) ||
      (is_generator_resume_mode && reg_count != 1)) {
    *out_result = roots.undefined_value().ptr();
    return true;
  }

  int32_t first_arg_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                operand_scale);
  Address argv[kMaxRuntimeFallbackArgs == 0 ? 1 : kMaxRuntimeFallbackArgs];
  for (uint32_t i = 0; i < reg_count; ++i) {
    argv[reg_count - 1 - i] = SafeRuntimeArgOrUndefined(
        isolate, ReadInterpreterRegister(
                     RegisterFromListOperand(first_arg_operand, i)));
  }

  WasmGCStateScope gc_state(isolate);
  SetCurrentIsolateScope current_isolate_scope(isolate);

  Tagged<Context> saved_context = isolate->context();
  Address context_address = CurrentInterpreterContext();
  bool switched_context = false;
  if (IsSafeTaggedHandleValue(context_address) &&
      IsContext(Tagged<Object>(context_address))) {
    isolate->set_context(Cast<Context>(Tagged<Object>(context_address)));
    switched_context = true;
  }

  using RuntimeEntry = Address (*)(int, Address*, Isolate*);
  WasmTemporaryRootScope argv_roots(isolate, argv,
                                    static_cast<int>(reg_count));
  Address* rooted_argv = argv_roots.data();
  Address* args_object =
      reg_count == 0 ? rooted_argv : &rooted_argv[reg_count - 1];
  if (function_id ==
          Runtime::kInlineCopyDataPropertiesWithExcludedPropertiesOnStack &&
      reg_count >= 1) {
    Address* excluded_property_base =
        reg_count == 1 ? rooted_argv : &rooted_argv[reg_count - 2];
    Address runtime_args[3] = {
        reinterpret_cast<Address>(excluded_property_base),
        Smi::FromInt(static_cast<int>(reg_count - 1)).ptr(),
        rooted_argv[reg_count - 1]};
    Address result = reinterpret_cast<RuntimeEntry>(function->entry)(
        3, &runtime_args[2], isolate);
    if (switched_context) isolate->set_context(saved_context);
    *out_result = result;
    return true;
  }
  if (function_id == Runtime::kInlineAsyncFunctionEnter) {
    Address result = roots.undefined_value().ptr();
    bool handled =
        TryRunAsyncFunctionEnterIntrinsic(isolate, rooted_argv,
                                          static_cast<int>(reg_count), &result);
    if (switched_context) isolate->set_context(saved_context);
    if (handled) {
      *out_result = result;
      return true;
    }  }
  if ((function_id == Runtime::kInlineAsyncFunctionResolve ||
       function_id == Runtime::kAsyncFunctionResolve ||
       function_id == Runtime::kInlineAsyncFunctionReject ||
       function_id == Runtime::kAsyncFunctionReject) &&
      reg_count == 2) {
    Address async_function_address = rooted_argv[1];
    Address value_address = rooted_argv[0];
    if (!IsSafeTaggedHandleValue(async_function_address) ||
        !IsJSAsyncFunctionObject(
            Tagged<Object>(async_function_address))) {
      if (switched_context) isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<JSAsyncFunctionObject> async_function(
        Cast<JSAsyncFunctionObject>(Tagged<Object>(async_function_address)),
        isolate);
    DirectHandle<JSPromise> promise(async_function->promise(), isolate);
    Address normalized_value =
        SafeTaggedOrUndefined(isolate, value_address);
    DirectHandle<Object> value(
        Tagged<Object>(normalized_value), isolate);

    bool is_resolve =
        function_id == Runtime::kInlineAsyncFunctionResolve ||
        function_id == Runtime::kAsyncFunctionResolve;
    if (is_resolve) {
      DirectHandle<Object> resolve_result;
      if (!JSPromise::Resolve(promise, value).ToHandle(&resolve_result)) {
        if (switched_context) isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
    } else {
#ifdef __wasi__
      PrintF("WASM32_ASYNC_REJECT");
      DumpRuntimeArg("reason", 0, (*value).ptr());
      PrintStringPreviewForTrace("reason_string", *value, 0, 240);
      if (IsJSReceiver(*value)) {
        DumpNamedDataPropertyForTrace(isolate, (*value).ptr(), "name");
        DumpNamedDataPropertyForTrace(isolate, (*value).ptr(), "message");
        DumpNamedDataPropertyForTrace(isolate, (*value).ptr(), "code");
      }
      PrintF("\n");
#endif
      JSPromise::Reject(promise, value, false);
    }

    *out_result = (*promise).ptr();
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if ((function_id == Runtime::kInlineAsyncFunctionAwait ||
       function_id == Runtime::kAsyncFunctionAwait) &&
      reg_count == 2) {
    Address async_function_address = rooted_argv[1];
    Address value_address =
        SafeTaggedOrUndefined(isolate, rooted_argv[0]);
    if (!IsSafeTaggedHandleValue(async_function_address) ||
        !IsJSAsyncFunctionObject(Tagged<Object>(async_function_address))) {
      if (switched_context) isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<JSAsyncFunctionObject> async_function(
        Cast<JSAsyncFunctionObject>(Tagged<Object>(async_function_address)),
        isolate);
    DirectHandle<JSPromise> awaited;
    if (IsJSPromise(Tagged<Object>(value_address))) {
      awaited = direct_handle(
          Cast<JSPromise>(Tagged<Object>(value_address)), isolate);
    } else {
      awaited = isolate->factory()->NewJSPromise();
      DirectHandle<Object> value(Tagged<Object>(value_address), isolate);
      DirectHandle<Object> resolve_result;
      if (!JSPromise::Resolve(awaited, value).ToHandle(&resolve_result)) {
        if (switched_context) isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
    }

    DirectHandle<NativeContext> native_context = isolate->native_context();
    DirectHandle<Context> closure_context =
        isolate->factory()->NewBuiltinContext(
            native_context, Context::MIN_CONTEXT_EXTENDED_SLOTS);
    closure_context->set_extension(*async_function);
    DirectHandle<SharedFunctionInfo> resolve_info =
        isolate->factory()
            ->async_function_await_resolve_closure_shared_fun();
    DirectHandle<SharedFunctionInfo> reject_info =
        isolate->factory()
            ->async_function_await_reject_closure_shared_fun();
    DirectHandle<JSFunction> on_resolve =
        Factory::JSFunctionBuilder{isolate, resolve_info, closure_context}
            .Build();
    DirectHandle<JSFunction> on_reject =
        Factory::JSFunctionBuilder{isolate, reject_info, closure_context}
            .Build();
    DirectHandle<Object> then_args[] = {on_resolve, on_reject};
    if (Execution::CallBuiltin(isolate, isolate->promise_then(),
                               awaited, base::VectorOf(then_args))
            .is_null()) {
      if (switched_context) isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    *out_result = async_function->promise().ptr();
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if (function_id == Runtime::kInlineCreateIterResultObject &&
      reg_count == 2) {
    DirectHandle<Object> value(
        Tagged<Object>(SafeTaggedOrUndefined(isolate, rooted_argv[1])),
        isolate);
    Tagged<Object> done_value(
        SafeTaggedOrUndefined(isolate, rooted_argv[0]));
    bool done = Object::BooleanValue(done_value, isolate);
    *out_result =
        (*isolate->factory()->NewJSIteratorResult(value, done)).ptr();
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if (is_generator_resume_mode && reg_count == 1) {
    Address generator_address = rooted_argv[0];
    if (IsSafeTaggedHandleValue(generator_address) &&
        (IsJSGeneratorObject(Tagged<Object>(generator_address)) ||
         IsJSAsyncFunctionObject(Tagged<Object>(generator_address)) ||
         IsJSAsyncGeneratorObject(Tagged<Object>(generator_address)))) {
      *out_result = Smi::FromInt(Cast<JSGeneratorObject>(
                                     Tagged<Object>(generator_address))
                                     ->resume_mode())
                        .ptr();
    } else {
      *out_result = roots.exception().ptr();
    }
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if (is_generator_create && reg_count == 2) {
    Address function_address = rooted_argv[1];
    Address receiver_address = rooted_argv[0];
    if (!IsSafeTaggedHandleValue(function_address) ||
        !IsJSFunction(Tagged<Object>(function_address)) ||
        !IsSafeTaggedHandleValue(receiver_address)) {
      if (switched_context) isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<JSFunction> closure(
        Cast<JSFunction>(Tagged<Object>(function_address)), isolate);
    Tagged<SharedFunctionInfo> shared = Wasm32JSFunctionShared(*closure);
    if (!shared->HasBytecodeArray()) {
      if (switched_context) isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    Tagged<BytecodeArray> generator_bytecode =
        shared->GetBytecodeArray(isolate);
    int storage_length =
        generator_bytecode->parameter_count_without_receiver() +
        generator_bytecode->register_count();
    DirectHandle<FixedArray> parameters_and_registers =
        isolate->factory()->NewFixedArray(storage_length);
    DirectHandle<JSGeneratorObject> generator =
        isolate->factory()->NewJSGeneratorObject(closure);
    Tagged<JSGeneratorObject> raw_generator = *generator;
    raw_generator->set_function(*closure);
    raw_generator->set_context(isolate->context());
    raw_generator->set_receiver(
        Cast<JSAny>(Tagged<Object>(receiver_address)));
    raw_generator->set_parameters_and_registers(*parameters_and_registers);
    raw_generator->set_resume_mode(JSGeneratorObject::ResumeMode::kNext);
    raw_generator->set_continuation(JSGeneratorObject::kGeneratorExecuting);
    if (IsJSAsyncGeneratorObject(raw_generator)) {
      Cast<JSAsyncGeneratorObject>(raw_generator)->set_is_awaiting(0);
    }

    if (switched_context) isolate->set_context(saved_context);
    *out_result = raw_generator.ptr();
    return true;
  }
  if (function_id == Runtime::kInlineCopyDataProperties && reg_count == 2) {
    static int inline_copy_data_properties_trace_count = 0;
    Address target_address = rooted_argv[reg_count - 1];
    Address source_address = rooted_argv[reg_count - 2];
    if (kTraceWasmFallbackDetails &&
        inline_copy_data_properties_trace_count < 12) {
      ++inline_copy_data_properties_trace_count;
      PrintF("WasmInterpreterEntryTrampoline: InlineCopyDataProperties "
             "count=%d ",
             inline_copy_data_properties_trace_count);
      DumpRuntimeArg("target", 0, target_address);
      DumpRuntimeArg("source", 0, source_address);
      PrintF("\n");
    }

    Address result = roots.undefined_value().ptr();
    if (!IsSafeTaggedHandleValue(target_address) ||
        !IsJSReceiver(Tagged<Object>(target_address))) {
      PrintF("WasmInterpreterEntryTrampoline: InlineCopyDataProperties "
             "target invalid bytecode_index=%d first_arg=%d ",
             bytecode_index, first_arg_operand);
      DumpRuntimeArg("target", 0, target_address);
      DumpRuntimeArg("source", 0, source_address);
      PrintF("\n");
      for (uint32_t i = 0; i < reg_count; ++i) {
        interpreter::Register reg =
            RegisterFromListOperand(first_arg_operand, i);
        Address raw_value = ReadInterpreterRegister(reg);
        PrintF("  InlineCopyDataProperties arg%u operand=%d reg_index=%d",
               i, reg.ToOperand(), reg.index());
        DumpRuntimeArg("raw", 0, raw_value);
        DumpRuntimeArg("safe", 0, argv[reg_count - 1 - i]);
        PrintF("\n");
      }
      DumpBytecodeWindowForTrace(bytecode, bytecode_index, 32, 32,
                                 "inline-copy-invalid");
      result = roots.exception().ptr();
    } else if (!IsUndefined(Tagged<Object>(source_address), isolate) &&
               !IsNull(Tagged<Object>(source_address), isolate)) {
      HandleScope scope(isolate);
      DirectHandle<JSReceiver> target =
          direct_handle(Cast<JSReceiver>(Tagged<Object>(target_address)),
                        isolate);
      DirectHandle<Object> source =
          direct_handle(Tagged<Object>(source_address), isolate);
      Maybe<bool> copied = JSReceiver::SetOrCopyDataProperties(
          isolate, target, source,
          PropertiesEnumerationMode::kPropertyAdditionOrder, {}, false);
      if (copied.IsNothing()) result = roots.exception().ptr();
    }

    if (switched_context) isolate->set_context(saved_context);
    *out_result = result;
    return true;
  }
  if (function_id == Runtime::kDefineClass && kTraceWasmFallbackDetails) {
    HandleScopeData* handle_scope_data = isolate->handle_scope_data();
    PrintF("WasmInterpreterEntryTrampoline: Runtime_DefineClass enter "
           "argc=%u isolate=%p current=%p next=%p limit=%p level=%d",
           reg_count, isolate, Isolate::TryGetCurrent(),
           handle_scope_data->next, handle_scope_data->limit,
           handle_scope_data->level);
    for (uint32_t i = 0; i < reg_count && i < 8; ++i) {
      DumpRuntimeArg(" arg", static_cast<int>(i), argv[reg_count - 1 - i]);
    }
    PrintF("\n");
  }
  Address result =
      reinterpret_cast<RuntimeEntry>(function->entry)(
          static_cast<int>(reg_count), args_object, isolate);
  if (function_id == Runtime::kDefineClass && kTraceWasmFallbackDetails) {
    PrintF("WasmInterpreterEntryTrampoline: Runtime_DefineClass argc=%u "
           "result=0x%x has_exception=%d",
           reg_count, static_cast<unsigned>(result), isolate->has_exception());
    for (uint32_t i = 0; i < reg_count && i < 8; ++i) {
      DumpRuntimeArg(" arg", static_cast<int>(i),
                     rooted_argv[reg_count - 1 - i]);
    }
    PrintF("\n");
  }
  if (switched_context) isolate->set_context(saved_context);
  *out_result = result;
  return true;
}

bool TryRunLdaGlobalBytecode(Isolate* isolate, Tagged<BytecodeArray> bytecode,
                             int bytecode_index,
                             interpreter::Bytecode bytecode_enum,
                             interpreter::OperandScale operand_scale,
                             Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kLdaGlobal &&
      bytecode_enum != interpreter::Bytecode::kLdaGlobalInsideTypeof) {
    return false;
  }

  uint32_t name_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
  uint32_t slot_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                  operand_scale);

  Tagged<TrustedFixedArray> constant_pool = bytecode->constant_pool();
  if (name_index >= static_cast<uint32_t>(constant_pool->length())) {
    PrintF("WasmInterpreterEntryTrampoline: bad global name index=%u "
           "length=%d\n",
           name_index, constant_pool->length());
    return false;
  }
  Tagged<Object> name_object = constant_pool->get(name_index);
  if (!IsName(name_object)) {
    PrintF("WasmInterpreterEntryTrampoline: global constant is not Name "
           "index=%u ",
           name_index);
    DumpRuntimeArg("value", 0, name_object.ptr());
    PrintF("\n");
    return false;
  }

  Address feedback_address =
      g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
          InterpreterFrameConstants::kFeedbackVectorFromFp)];
  HandleScope scope(isolate);
  Handle<FeedbackVector> vector;
  if (IsFeedbackVectorAddress(feedback_address)) {
    vector =
        handle(Cast<FeedbackVector>(Tagged<Object>(feedback_address)), isolate);
  }

  FeedbackSlot vector_slot = FeedbackVector::ToSlot(slot_index);
  FeedbackSlotKind kind =
      bytecode_enum == interpreter::Bytecode::kLdaGlobalInsideTypeof
          ? FeedbackSlotKind::kLoadGlobalInsideTypeof
          : FeedbackSlotKind::kLoadGlobalNotInsideTypeof;
  DirectHandle<JSGlobalObject> global = isolate->global_object();
  Handle<Name> name = handle(Cast<Name>(name_object), isolate);
  LoadGlobalIC ic(isolate, vector, vector_slot, kind);
  if (!vector.is_null()) ic.UpdateState(global, name);

  DirectHandle<Object> result;
  if (!ic.Load(name).ToHandle(&result)) {
    *out_result = ReadOnlyRoots(isolate).exception().ptr();
    return true;
  }
  *out_result = (*result).ptr();
  return true;
}

bool TryRunGetNamedPropertyBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  bool is_super =
      bytecode_enum == interpreter::Bytecode::kGetNamedPropertyFromSuper;
  if (bytecode_enum != interpreter::Bytecode::kGetNamedProperty && !is_super) {
    return false;
  }

  int32_t receiver_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  uint32_t name_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                  operand_scale);

  Tagged<TrustedFixedArray> constant_pool = bytecode->constant_pool();
  if (name_index >= static_cast<uint32_t>(constant_pool->length())) {
    PrintF("WasmInterpreterEntryTrampoline: bad property name index=%u "
           "length=%d\n",
           name_index, constant_pool->length());
    return false;
  }
  Tagged<Object> name_object = constant_pool->get(name_index);
  if (!IsName(name_object)) {
    PrintF("WasmInterpreterEntryTrampoline: property constant is not Name "
           "index=%u ",
           name_index);
    DumpRuntimeArg("value", 0, name_object.ptr());
    PrintF("\n");
    return false;
  }
  bool trace_native_hooks = false;
  bool trace_reflect_own_keys_name =
      kTraceWasmFallbackDetails &&
      IsReflectOwnKeysTraceName(isolate, name_object);
  bool trace_eval_named_property =
#ifdef __wasi__
      kTraceWasmFallbackDetails &&
      CurrentInterpreterFunctionMatchesTraceNeedle();
#else
      false;
#endif
  {
    HandleScope trace_scope(isolate);
    Handle<Name> trace_name = handle(Cast<Name>(name_object), isolate);
    trace_native_hooks = Name::Equals(
        isolate, trace_name, isolate->factory()->InternalizeUtf8String(
                                 "nativeHooks"));
  }

  Address receiver_address = ReadInterpreterRegister(
      interpreter::Register::FromOperand(receiver_operand));
  ReadOnlyRoots roots(isolate);
#ifdef __wasi__
  if (bytecode->length() <= 16) {
    bool plausible = IsPlausibleTaggedValue(receiver_address);
    bool heap_object =
        plausible && IsHeapObject(Tagged<Object>(receiver_address));
    bool readable_map = heap_object && HasReadableHeapObjectMap(receiver_address);
    bool in_read_only_heap = false;
    bool in_heap = false;
    if (heap_object) {
      Tagged<HeapObject> object =
          Cast<HeapObject>(Tagged<Object>(receiver_address));
      in_read_only_heap = ReadOnlyHeap::Contains(object);
      in_heap = isolate->heap()->Contains(object);
    }
    PrintF("WASM32_GETNAMED_PRECHECK pc=%d receiver=0x%x plausible=%d "
           "heap_object=%d readable_map=%d ro_heap=%d heap=%d\n",
           bytecode_index, static_cast<unsigned>(receiver_address),
           plausible ? 1 : 0, heap_object ? 1 : 0,
           readable_map ? 1 : 0, in_read_only_heap ? 1 : 0,
           in_heap ? 1 : 0);
  }
#endif
  if (trace_eval_named_property) {
    PrintF("WasmInterpreterEntryTrampoline: named load eval enter name=");
    DumpNameForTrace(name_object);
    DumpInterpreterRegisterValue(
        "receiver", 0, interpreter::Register::FromOperand(receiver_operand));
    PrintF("\n");
  }
  if (!IsPlausibleTaggedValue(receiver_address)) {
    if (kTraceWasmFallbackDetails &&
        (trace_native_hooks || trace_reflect_own_keys_name)) {
      PrintF("WasmInterpreterEntryTrampoline: named load invalid "
             "receiver_operand=%d name=",
             receiver_operand);
      DumpNameForTrace(name_object);
      PrintF(" ");
      DumpRuntimeArg("receiver", 0, receiver_address);
      PrintF("\n");
    }
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmInterpreterEntryTrampoline: named load receiver invalid ");
      DumpRuntimeArg("receiver", 0, receiver_address);
      PrintF(" ");
      DumpRuntimeArg("name", 0, name_object.ptr());
      PrintF("\n");
    }
    *out_result = roots.undefined_value().ptr();
    if (trace_eval_named_property) {
      PrintF("WasmInterpreterEntryTrampoline: named load eval invalid result ");
      DumpRuntimeArg("result", 0, *out_result);
      PrintF("\n");
    }
    return true;
  }
  Tagged<Object> receiver_object(receiver_address);
  if (IsTheHole(receiver_object, roots) ||
      IsUninitialized(receiver_object, roots) ||
      !IsJSAnyForWasmPropertyLookup(isolate, receiver_address)) {
    if (kTraceWasmFallbackDetails &&
        (trace_native_hooks || trace_reflect_own_keys_name)) {
      PrintF("WasmInterpreterEntryTrampoline: named load receiver not "
             "JSAny receiver_operand=%d name=",
             receiver_operand);
      DumpNameForTrace(name_object);
      PrintF(" ");
      DumpRuntimeArg("receiver", 0, receiver_address);
      PrintF("\n");
    }
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmInterpreterEntryTrampoline: named load receiver is not "
             "JSAny ");
      DumpRuntimeArg("receiver", 0, receiver_address);
      PrintF(" ");
      DumpRuntimeArg("name", 0, name_object.ptr());
      PrintF("\n");
    }
    *out_result = roots.undefined_value().ptr();
    if (trace_eval_named_property) {
      PrintF("WasmInterpreterEntryTrampoline: named load eval non-jsany result ");
      DumpRuntimeArg("result", 0, *out_result);
      PrintF("\n");
    }
    return true;
  }
  if (IsNullOrUndefined(receiver_object, isolate)) {
    *out_result = roots.undefined_value().ptr();
    return true;
  }

  HandleScope scope(isolate);
  Handle<JSAny> receiver = handle(Cast<JSAny>(receiver_object), isolate);
  Handle<Name> name = handle(Cast<Name>(name_object), isolate);
  if (is_super) {
    Address home_object_address =
        g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
    if (!IsPlausibleTaggedValue(home_object_address) ||
        !IsJSObject(Tagged<Object>(home_object_address))) {
      *out_result = roots.undefined_value().ptr();
      return true;
    }

    Address runtime_args[3] = {name_object.ptr(), home_object_address,
                               receiver_address};
    WasmGCStateScope gc_state(isolate);
    SetCurrentIsolateScope current_isolate_scope(isolate);

    Tagged<Context> saved_context = isolate->context();
    Address context_address = CurrentInterpreterContext();
    bool switched_context = false;
    if (IsSafeTaggedHandleValue(context_address) &&
        IsContext(Tagged<Object>(context_address))) {
      isolate->set_context(Cast<Context>(Tagged<Object>(context_address)));
      switched_context = true;
    }

    const Runtime::Function* load_from_super =
        Runtime::FunctionForId(Runtime::kLoadFromSuper);
    using RuntimeEntry = Address (*)(int, Address*, Isolate*);
    WasmTemporaryRootScope runtime_roots(isolate, runtime_args, 3);
    Address* rooted_args = runtime_roots.data();
    Address result = reinterpret_cast<RuntimeEntry>(load_from_super->entry)(
        3, &rooted_args[2], isolate);
    if (switched_context) isolate->set_context(saved_context);

    if (isolate->has_exception() || result == roots.exception().ptr()) {
      *out_result = roots.exception().ptr();
    } else {
      *out_result = result;
    }
    return true;
  }

  DirectHandle<Object> result;
  if (IsJSFunction(receiver_object) &&
      Name::Equals(isolate, name, isolate->factory()->prototype_string())) {
    DirectHandle<JSFunction> function =
        handle(Cast<JSFunction>(receiver_object), isolate);
    if (function->has_prototype_slot()) {
      *out_result = Wasm32JSFunctionPrototypeAddress(isolate, function);
      if (kTraceWasmFallbackDetails) {
        PrintF("WasmInterpreterEntryTrampoline: named load JSFunction "
               "prototype fallback ");
        DumpRuntimeArg("receiver", 0, receiver_address);
        PrintF(" ");
        DumpRuntimeArg("result", 0, *out_result);
        PrintF("\n");
      }
      return true;
    }
  }
  if (IsJSArray(receiver_object) &&
      Name::Equals(isolate, name, isolate->factory()->length_string())) {
    *out_result = Cast<JSArray>(receiver_object)->length().ptr();
    return true;
  }
  if (IsString(receiver_object) &&
      Name::Equals(isolate, name, isolate->factory()->length_string())) {
    *out_result = Smi::FromInt(Cast<String>(receiver_object)->length()).ptr();
    return true;
  }
  if (IsJSTypedArray(receiver_object)) {
    Tagged<JSTypedArray> typed_array = Cast<JSTypedArray>(receiver_object);
    size_t value;
    if (typed_array->IsDetachedOrOutOfBounds()) {
      value = 0;
    } else if (Name::Equals(isolate, name,
                            isolate->factory()->length_string())) {
      value = typed_array->GetLength();
    } else if (Name::Equals(isolate, name,
                            isolate->factory()->byte_length_string())) {
      value = typed_array->GetByteLength();
    } else if (Name::Equals(isolate, name,
                            isolate->factory()->byte_offset_string())) {
      value = typed_array->byte_offset();
    } else {
      value = std::numeric_limits<size_t>::max();
    }

    if (value != std::numeric_limits<size_t>::max()) {
      if (value <= static_cast<size_t>(Smi::kMaxValue)) {
        *out_result = Smi::FromInt(static_cast<int>(value)).ptr();
      } else {
        *out_result =
            (*isolate->factory()->NewNumberFromSize(value)).ptr();
      }
      return true;
    }
  }
  if (IsJSArrayBuffer(receiver_object) &&
      Name::Equals(isolate, name,
                   isolate->factory()->byte_length_string())) {
    Tagged<JSArrayBuffer> array_buffer =
        Cast<JSArrayBuffer>(receiver_object);
    size_t value =
        array_buffer->was_detached() ? 0 : array_buffer->GetByteLength();
    if (value <= static_cast<size_t>(Smi::kMaxValue)) {
      *out_result = Smi::FromInt(static_cast<int>(value)).ptr();
    } else {
      *out_result = (*isolate->factory()->NewNumberFromSize(value)).ptr();
    }
    return true;
  }
  if (TryReadWasm32CopyPrototypeIteratorResultLayout(isolate, receiver_object,
                                                     name, out_result)) {
    return true;
  }
  if (IsJSObject(receiver_object) &&
      Cast<JSObject>(receiver_object)->map() ==
          isolate->native_context()->iterator_result_map()) {
    Tagged<JSIteratorResult> iterator_result =
        Cast<JSIteratorResult>(receiver_object);
    if (Name::Equals(isolate, name, isolate->factory()->done_string())) {
      *out_result = iterator_result->done().ptr();
      return true;
    }
    if (Name::Equals(isolate, name, isolate->factory()->value_string())) {
      *out_result = iterator_result->value().ptr();
      return true;
    }
  }
  if (!GetObjectPropertyPreservingWasmInterpreterState(isolate, receiver, name)
           .ToHandle(&result)) {
    *out_result = isolate->has_exception()
                      ? ReadOnlyRoots(isolate).exception().ptr()
                      : Smi::zero().ptr();
    if (trace_eval_named_property) {
      PrintF("WasmInterpreterEntryTrampoline: named load eval exception result ");
      DumpRuntimeArg("result", 0, *out_result);
      PrintF("\n");
    }
    return true;
  }
  *out_result = (*result).ptr();
  if (trace_eval_named_property) {
    PrintF("WasmInterpreterEntryTrampoline: named load eval result ");
    DumpRuntimeArg("result", 0, *out_result);
    PrintF("\n");
  }
  if (kTraceWasmFallbackDetails &&
      (trace_native_hooks || trace_reflect_own_keys_name)) {
    PrintF("WasmInterpreterEntryTrampoline: named load trace "
           "receiver_operand=%d name=",
           receiver_operand);
    DumpNameForTrace(name_object);
    PrintF(" ");
    DumpRuntimeArg("receiver", 0, receiver_address);
    PrintF(" ");
    DumpRuntimeArg("result", 0, *out_result);
    PrintF("\n");
  }
  if (kTraceWasmFallbackDetails) {
    PrintF("WasmInterpreterEntryTrampoline: named load detail name=");
    DumpNameForTrace(name_object);
    PrintF(" receiver_operand=%d ", receiver_operand);
    DumpRuntimeArg("receiver", 0, receiver_address);
    PrintF(" ");
    DumpRuntimeArg("result", 0, *out_result);
    PrintF("\n");
  }
  return true;
}

bool TryRunGetKeyedPropertyBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kGetKeyedProperty) return false;

  int32_t receiver_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  uint32_t slot_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                  operand_scale);

  Address receiver_address = ReadInterpreterRegister(
      interpreter::Register::FromOperand(receiver_operand));
  Address key_address = g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
  ReadOnlyRoots roots(isolate);

  HandleScope scope(isolate);
  if (!IsPlausibleTaggedValue(receiver_address)) {
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmInterpreterEntryTrampoline: keyed load receiver invalid ");
      DumpRuntimeArg("receiver", 0, receiver_address);
      PrintF(" ");
      DumpRuntimeArg("key", 0, key_address);
      PrintF("\n");
    }
    *out_result = roots.undefined_value().ptr();
    return true;
  }
  Tagged<Object> receiver_object(receiver_address);
  if (IsTheHole(receiver_object, roots) ||
      IsUninitialized(receiver_object, roots) ||
      !IsJSAnyForWasmPropertyLookup(isolate, receiver_address)) {
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmInterpreterEntryTrampoline: keyed load receiver is not "
             "JSAny ");
      DumpRuntimeArg("receiver", 0, receiver_address);
      PrintF(" ");
      DumpRuntimeArg("key", 0, key_address);
      PrintF("\n");
    }
    *out_result = roots.undefined_value().ptr();
    return true;
  }
  if (IsNullOrUndefined(receiver_object, isolate)) {
    *out_result = roots.undefined_value().ptr();
    return true;
  }
  if (!IsSafeTaggedHandleValue(key_address)) {
    key_address = roots.undefined_value().ptr();
  }
  Tagged<Object> key_object(key_address);
  if (IsTheHole(key_object, roots) || IsUninitialized(key_object, roots)) {
    key_object = roots.undefined_value();
  }

  Handle<JSAny> receiver =
      handle(Cast<JSAny>(receiver_object), isolate);
  Handle<Object> key = handle(key_object, isolate);
  DirectHandle<Object> result;
  if (IsJSFunction(receiver_object) && IsName(key_object)) {
    DirectHandle<Name> name = handle(Cast<Name>(key_object), isolate);
    if (Name::Equals(isolate, name, isolate->factory()->prototype_string())) {
      DirectHandle<JSFunction> function =
          handle(Cast<JSFunction>(receiver_object), isolate);
      if (function->has_prototype_slot()) {
        *out_result = Wasm32JSFunctionPrototypeAddress(isolate, function);
        if (kTraceWasmFallbackDetails) {
          PrintF("WasmInterpreterEntryTrampoline: keyed load JSFunction "
                 "prototype fallback ");
          DumpRuntimeArg("receiver", 0, receiver_address);
          PrintF(" ");
          DumpRuntimeArg("result", 0, *out_result);
          PrintF("\n");
        }
        return true;
      }
    }
  }
  if (IsName(key_object)) {
    DirectHandle<Name> name = handle(Cast<Name>(key_object), isolate);
    if (IsString(receiver_object) &&
        Name::Equals(isolate, name, isolate->factory()->length_string())) {
      *out_result =
          Smi::FromInt(Cast<String>(receiver_object)->length()).ptr();
      return true;
    }
    if (IsJSTypedArray(receiver_object)) {
      Tagged<JSTypedArray> typed_array = Cast<JSTypedArray>(receiver_object);
      size_t value;
      if (typed_array->IsDetachedOrOutOfBounds()) {
        value = 0;
      } else if (Name::Equals(isolate, name,
                              isolate->factory()->length_string())) {
        value = typed_array->GetLength();
      } else if (Name::Equals(isolate, name,
                              isolate->factory()->byte_length_string())) {
        value = typed_array->GetByteLength();
      } else if (Name::Equals(isolate, name,
                              isolate->factory()->byte_offset_string())) {
        value = typed_array->byte_offset();
      } else {
        value = std::numeric_limits<size_t>::max();
      }
      if (value != std::numeric_limits<size_t>::max()) {
        if (value <= static_cast<size_t>(Smi::kMaxValue)) {
          *out_result = Smi::FromInt(static_cast<int>(value)).ptr();
        } else {
          *out_result =
              (*isolate->factory()->NewNumberFromSize(value)).ptr();
        }
        return true;
      }
    }
    if (IsJSArrayBuffer(receiver_object) &&
        Name::Equals(isolate, name,
                     isolate->factory()->byte_length_string())) {
      Tagged<JSArrayBuffer> array_buffer =
          Cast<JSArrayBuffer>(receiver_object);
      size_t value =
          array_buffer->was_detached() ? 0 : array_buffer->GetByteLength();
      if (value <= static_cast<size_t>(Smi::kMaxValue)) {
        *out_result = Smi::FromInt(static_cast<int>(value)).ptr();
      } else {
        *out_result = (*isolate->factory()->NewNumberFromSize(value)).ptr();
      }
      return true;
    }
  }
  if (!GetObjectPropertyPreservingWasmInterpreterState(isolate, receiver, key)
           .ToHandle(&result)) {
    *out_result = ReadOnlyRoots(isolate).exception().ptr();
    return true;
  }
  *out_result = (*result).ptr();
  return true;
}

bool TryRunToNameBytecode(Isolate* isolate,
                          interpreter::Bytecode bytecode_enum,
                          Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kToName) return false;

  Address input_address = g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
  ReadOnlyRoots roots(isolate);
  if (!IsSafeTaggedHandleValue(input_address)) {
    input_address = roots.undefined_value().ptr();
  }
  Tagged<Object> input_object(input_address);
  if (IsTheHole(input_object, roots) || IsUninitialized(input_object, roots)) {
    input_object = roots.undefined_value();
  }

  HandleScope scope(isolate);
  DirectHandle<Object> input = direct_handle(input_object, isolate);
  DirectHandle<Name> result;
  if (!Object::ToName(isolate, input).ToHandle(&result)) {
    *out_result = roots.exception().ptr();
    return true;
  }
  *out_result = (*result).ptr();
  return true;
}

bool TryRunStarBytecode(Tagged<BytecodeArray> bytecode, int bytecode_index,
                        interpreter::Bytecode bytecode_enum,
                        interpreter::OperandScale operand_scale,
                        Address* out_result) {
  if (!interpreter::Bytecodes::IsAnyStar(bytecode_enum)) return false;

  interpreter::Register target =
      interpreter::Bytecodes::IsShortStar(bytecode_enum)
          ? interpreter::Register::FromShortStar(bytecode_enum)
          : interpreter::Register::FromOperand(ReadBytecodeSignedOperand(
                bytecode, bytecode_index, bytecode_enum, 0, operand_scale));
  Address accumulator =
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
  StoreInterpreterRegister(target, accumulator);
  *out_result = accumulator;
  return true;
}

bool TryRunMovBytecode(Tagged<BytecodeArray> bytecode, int bytecode_index,
                       interpreter::Bytecode bytecode_enum,
                       interpreter::OperandScale operand_scale,
                       Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kMov) return false;

  int32_t source_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  int32_t target_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                operand_scale);
  Address value =
      ReadInterpreterRegister(interpreter::Register::FromOperand(source_operand));
  StoreInterpreterRegister(interpreter::Register::FromOperand(target_operand),
                           value);
  *out_result = g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
  return true;
}

void PublishCurrentInterpreterContext(Address context) {
  StoreInterpreterRegister(interpreter::Register::current_context(), context);
  StoreInterpreterFrameOffset(StandardFrameConstants::kContextOffset, context);
  g_wasm_regs[SlotFor(kContextRegister)] = context;
}

bool TryRunContextStackBytecode(Tagged<BytecodeArray> bytecode,
                                int bytecode_index,
                                interpreter::Bytecode bytecode_enum,
                                interpreter::OperandScale operand_scale,
                                Address* out_result) {
  if (bytecode_enum == interpreter::Bytecode::kPushContext) {
    int32_t target_operand =
        ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
    StoreInterpreterRegister(interpreter::Register::FromOperand(target_operand),
                             CurrentInterpreterContext());
    Address accumulator =
        g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
    PublishCurrentInterpreterContext(accumulator);
    *out_result = accumulator;
    return true;
  }
  if (bytecode_enum == interpreter::Bytecode::kPopContext) {
    int32_t source_operand =
        ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
    PublishCurrentInterpreterContext(ReadInterpreterRegister(
        interpreter::Register::FromOperand(source_operand)));
    *out_result = g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
    return true;
  }
  return false;
}

bool TryRunModuleVariableBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  const bool is_load =
      bytecode_enum == interpreter::Bytecode::kLdaModuleVariable;
  const bool is_store =
      bytecode_enum == interpreter::Bytecode::kStaModuleVariable;
  if (!is_load && !is_store) return false;

  ReadOnlyRoots roots(isolate);
  int32_t cell_index = ReadBytecodeSignedOperand(
      bytecode, bytecode_index, bytecode_enum, 0, operand_scale);
  uint32_t depth = ReadBytecodeUnsignedOperand(
      bytecode, bytecode_index, bytecode_enum, 1, operand_scale);
  Address context_address = CurrentInterpreterContext();
  if (!IsSafeTaggedHandleValue(context_address) ||
      !IsContext(Tagged<Object>(context_address))) {
    *out_result = roots.exception().ptr();
    return true;
  }

  Tagged<Context> module_context =
      Cast<Context>(Tagged<Object>(context_address));
  for (uint32_t i = 0; i < depth; ++i) {
    module_context = module_context->previous();
  }

  Tagged<HeapObject> extension = module_context->extension();
  if (!IsSourceTextModule(extension) || cell_index == 0) {
    *out_result = roots.exception().ptr();
    return true;
  }
  Tagged<SourceTextModule> module = Cast<SourceTextModule>(extension);
  Tagged<FixedArray> cells;
  int array_index;
  if (cell_index > 0) {
    cells = module->regular_exports();
    array_index = cell_index - 1;
  } else {
    if (is_store) {
      *out_result = roots.exception().ptr();
      return true;
    }
    cells = module->regular_imports();
    array_index = -cell_index - 1;
  }
  if (array_index < 0 || array_index >= cells->length() ||
      !IsCell(cells->get(array_index))) {
    *out_result = roots.exception().ptr();
    return true;
  }

  Tagged<Cell> cell = Cast<Cell>(cells->get(array_index));
  if (is_load) {
    *out_result = cell->value().ptr();
  } else {
    Address value = g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
    cell->set_value(Tagged<Object>(value));
    *out_result = value;
  }
  return true;
}

bool TryRunCatchContextBytecode(Isolate* isolate,
                                Tagged<BytecodeArray> bytecode,
                                int bytecode_index,
                                interpreter::Bytecode bytecode_enum,
                                interpreter::OperandScale operand_scale,
                                Address* out_result) {
  ReadOnlyRoots roots(isolate);
  if (bytecode_enum == interpreter::Bytecode::kSetPendingMessage) {
    Address message_address = SafeTaggedOrUndefined(
        isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
    Tagged<Object> message(message_address);
    if (!IsHeapObject(message)) {
      *out_result = roots.exception().ptr();
      return true;
    }
    Tagged<Object> previous = isolate->pending_message();
    isolate->set_pending_message(message);
    *out_result = previous.ptr();
    return true;
  }
  if (bytecode_enum != interpreter::Bytecode::kCreateCatchContext) {
    return false;
  }

  int32_t exception_operand = ReadBytecodeSignedOperand(
      bytecode, bytecode_index, bytecode_enum, 0, operand_scale);
  uint32_t scope_info_index = ReadBytecodeUnsignedOperand(
      bytecode, bytecode_index, bytecode_enum, 1, operand_scale);
  Tagged<TrustedFixedArray> constant_pool = bytecode->constant_pool();
  if (scope_info_index >= static_cast<uint32_t>(constant_pool->length())) {
    *out_result = roots.exception().ptr();
    return true;
  }
  Tagged<Object> scope_info_object = constant_pool->get(scope_info_index);
  Address context_address = CurrentInterpreterContext();
  Address exception_address = SafeTaggedOrUndefined(
      isolate, ReadInterpreterRegister(
                   interpreter::Register::FromOperand(exception_operand)));
  if (!IsScopeInfo(scope_info_object) ||
      !IsSafeTaggedHandleValue(context_address) ||
      !IsContext(Tagged<Object>(context_address))) {
    *out_result = roots.exception().ptr();
    return true;
  }

  HandleScope scope(isolate);
  DirectHandle<Context> previous(
      Cast<Context>(Tagged<Object>(context_address)), isolate);
  DirectHandle<ScopeInfo> scope_info(Cast<ScopeInfo>(scope_info_object),
                                     isolate);
  DirectHandle<Object> exception(Tagged<Object>(exception_address), isolate);
  *out_result = (*isolate->factory()
                      ->NewCatchContext(previous, scope_info, exception))
                    .ptr();
  return true;
}

bool TryRunLdarBytecode(Tagged<BytecodeArray> bytecode, int bytecode_index,
                        interpreter::Bytecode bytecode_enum,
                        interpreter::OperandScale operand_scale,
                        Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kLdar) return false;

  int32_t source_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  *out_result =
      ReadInterpreterRegister(interpreter::Register::FromOperand(source_operand));
  return true;
}

bool TryRunLdaConstantBytecode(Isolate* isolate, Tagged<BytecodeArray> bytecode,
                               int bytecode_index,
                               interpreter::Bytecode bytecode_enum,
                               interpreter::OperandScale operand_scale,
                               Address* out_result) {
  ReadOnlyRoots roots(isolate);
  switch (bytecode_enum) {
    case interpreter::Bytecode::kLdaZero:
      *out_result = Smi::zero().ptr();
      return true;
    case interpreter::Bytecode::kLdaSmi: {
      int32_t value =
          ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                    operand_scale);
      *out_result = Smi::FromInt(value).ptr();
      return true;
    }
    case interpreter::Bytecode::kLdaUndefined:
      *out_result = roots.undefined_value().ptr();
      return true;
    case interpreter::Bytecode::kLdaNull:
      *out_result = roots.null_value().ptr();
      return true;
    case interpreter::Bytecode::kLdaTheHole:
      *out_result = roots.the_hole_value().ptr();
      return true;
    case interpreter::Bytecode::kLdaTrue:
      *out_result = roots.true_value().ptr();
      return true;
    case interpreter::Bytecode::kLdaFalse:
      *out_result = roots.false_value().ptr();
      return true;
    case interpreter::Bytecode::kLdaConstant: {
      uint32_t constant_index =
          ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum,
                                      0, operand_scale);
      Tagged<TrustedFixedArray> constant_pool = bytecode->constant_pool();
      if (constant_index >= static_cast<uint32_t>(constant_pool->length())) {
        *out_result = roots.exception().ptr();
        return true;
      }
      *out_result = constant_pool->get(constant_index).ptr();
      return true;
    }
    default:
      return false;
  }
}

Address SafeTaggedOrUndefined(Isolate* isolate, Address value);

bool TryRunToStringBytecode(Isolate* isolate,
                            interpreter::Bytecode bytecode_enum,
                            Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kToString) return false;

  Address input_address = SafeTaggedOrUndefined(
      isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
  HandleScope scope(isolate);
  DirectHandle<Object> input =
      direct_handle(Tagged<Object>(input_address), isolate);
  DirectHandle<String> result;
  if (!Object::ToString(isolate, input).ToHandle(&result)) {
    *out_result = ReadOnlyRoots(isolate).exception().ptr();
    return true;
  }
  *out_result = (*result).ptr();
  return true;
}

bool TryRunToNumberOrNumericBytecode(Isolate* isolate,
                                     interpreter::Bytecode bytecode_enum,
                                     Address* out_result) {
  bool to_number = bytecode_enum == interpreter::Bytecode::kToNumber;
  bool to_numeric = bytecode_enum == interpreter::Bytecode::kToNumeric;
  if (!to_number && !to_numeric) return false;

  Address input_address = SafeTaggedOrUndefined(
      isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
  HandleScope scope(isolate);
  DirectHandle<Object> input =
      direct_handle(Tagged<Object>(input_address), isolate);
  if (to_number) {
    DirectHandle<Number> result;
    if (!Object::ToNumber(isolate, input).ToHandle(&result)) {
      *out_result = ReadOnlyRoots(isolate).exception().ptr();
      return true;
    }
    *out_result = (*result).ptr();
    return true;
  }

  DirectHandle<Object> result;
  if (!Object::ToNumeric(isolate, input).ToHandle(&result)) {
    *out_result = ReadOnlyRoots(isolate).exception().ptr();
    return true;
  }
  *out_result = (*result).ptr();
  return true;
}

bool TryRunThrowReferenceErrorIfHoleBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kThrowReferenceErrorIfHole) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  Address raw_accumulator =
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
  bool is_hole = (IsSafeTaggedHandleValue(raw_accumulator) ||
                  IsKnownReadOnlyRootValue(isolate, raw_accumulator)) &&
                 IsTheHole(Tagged<Object>(raw_accumulator), roots);
  if (!is_hole) {
    Address accumulator = SafeTaggedOrUndefined(isolate, raw_accumulator);
    *out_result = accumulator;
    return true;
  }

  uint32_t name_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
  Tagged<TrustedFixedArray> constant_pool = bytecode->constant_pool();
  if (name_index >= static_cast<uint32_t>(constant_pool->length())) {
    *out_result = roots.exception().ptr();
    return true;
  }
  Tagged<Object> name_object = constant_pool->get(name_index);
  Address args[1] = {name_object.ptr()};
  *out_result = Runtime_ThrowAccessedUninitializedVariable(1, args, isolate);
  return true;
}

bool TryRunThrowSuperBytecode(Isolate* isolate,
                              interpreter::Bytecode bytecode_enum,
                              Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kThrowSuperNotCalledIfHole &&
      bytecode_enum !=
          interpreter::Bytecode::kThrowSuperAlreadyCalledIfNotHole) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  Address accumulator =
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
  bool is_hole = (IsSafeTaggedHandleValue(accumulator) ||
                  IsKnownReadOnlyRootValue(isolate, accumulator)) &&
                 IsTheHole(Tagged<Object>(accumulator), roots);
  bool should_throw =
      bytecode_enum == interpreter::Bytecode::kThrowSuperNotCalledIfHole
          ? is_hole
          : !is_hole;
  if (!should_throw) {
    *out_result = accumulator;
    return true;
  }

  *out_result =
      bytecode_enum == interpreter::Bytecode::kThrowSuperNotCalledIfHole
          ? Runtime_ThrowSuperNotCalled(0, nullptr, isolate)
          : Runtime_ThrowSuperAlreadyCalledError(0, nullptr, isolate);
  return true;
}

bool TryRunThrowIfNotSuperConstructorBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kThrowIfNotSuperConstructor) {
    return false;
  }

  int32_t constructor_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  Address constructor_address = SafeTaggedOrUndefined(
      isolate, ReadInterpreterRegister(
                   interpreter::Register::FromOperand(constructor_operand)));
  if (IsConstructor(Tagged<Object>(constructor_address))) {
    *out_result = g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
    return true;
  }

  Address function_address =
      g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
          StandardFrameConstants::kFunctionOffset)];
  function_address = SafeTaggedOrUndefined(isolate, function_address);
  Address args[2] = {function_address, constructor_address};
  *out_result = Runtime_ThrowNotSuperConstructor(2, &args[1], isolate);
  return true;
}

bool TryRunThrowControlBytecode(Isolate* isolate, Tagged<BytecodeArray> bytecode,
                                int bytecode_index,
                                interpreter::Bytecode bytecode_enum,
                                interpreter::OperandScale operand_scale,
                                Address* out_result) {
  ReadOnlyRoots roots(isolate);
  if (bytecode_enum == interpreter::Bytecode::kThrow ||
      bytecode_enum == interpreter::Bytecode::kReThrow) {
    Address exception_address = SafeTaggedOrUndefined(
        isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
    Tagged<Object> exception(exception_address);
    if (bytecode_enum == interpreter::Bytecode::kThrow) {
      isolate->Throw(exception);
    } else {
      isolate->ReThrow(exception);
    }
    *out_result = roots.exception().ptr();
    return true;
  }

  if (bytecode_enum != interpreter::Bytecode::kAbort) return false;

  int reason = static_cast<int>(ReadBytecodeUnsignedOperand(
      bytecode, bytecode_index, bytecode_enum, 0, operand_scale));
  PrintF("WasmInterpreterEntryTrampoline: Abort bytecode reason=%d "
         "index=%d offset=0x%x acc=0x%x\n",
         reason, bytecode_index,
         static_cast<unsigned>(
             reinterpret_cast<Address>(bytecode->GetFirstBytecodeAddress()) +
             bytecode_index),
         static_cast<unsigned>(
             g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]));
  *out_result = roots.exception().ptr();
  return true;
}

bool TryRunReferenceTestBytecode(Isolate* isolate, Tagged<BytecodeArray> bytecode,
                                 int bytecode_index,
                                 interpreter::Bytecode bytecode_enum,
                                 interpreter::OperandScale operand_scale,
                                 Address* out_result) {
  ReadOnlyRoots roots(isolate);
  Address accumulator = SafeTaggedOrUndefined(
      isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
  bool result = false;

  switch (bytecode_enum) {
    case interpreter::Bytecode::kTestReferenceEqual: {
      int32_t operand =
          ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                    operand_scale);
      Address lhs = SafeTaggedOrUndefined(
          isolate, ReadInterpreterRegister(
                       interpreter::Register::FromOperand(operand)));
      result = lhs == accumulator;
      break;
    }
    case interpreter::Bytecode::kTestNull:
      result = accumulator == roots.null_value().ptr();
      break;
    case interpreter::Bytecode::kTestUndefined:
      result = accumulator == roots.undefined_value().ptr();
      break;
    case interpreter::Bytecode::kTestUndetectable: {
      Tagged<Object> value(accumulator);
      result = !IsSmi(value) && IsUndetectable(Cast<HeapObject>(value));
      break;
    }
    default:
      return false;
  }

  *out_result = result ? roots.true_value().ptr() : roots.false_value().ptr();
  return true;
}

bool TryRunCompareBytecode(Isolate* isolate, Tagged<BytecodeArray> bytecode,
                           int bytecode_index,
                           interpreter::Bytecode bytecode_enum,
                           interpreter::OperandScale operand_scale,
                           Address* out_result) {
  bool is_compare = bytecode_enum == interpreter::Bytecode::kTestEqual ||
                    bytecode_enum == interpreter::Bytecode::kTestEqualStrict ||
                    bytecode_enum == interpreter::Bytecode::kTestLessThan ||
                    bytecode_enum == interpreter::Bytecode::kTestGreaterThan ||
                    bytecode_enum ==
                        interpreter::Bytecode::kTestLessThanOrEqual ||
                    bytecode_enum ==
                        interpreter::Bytecode::kTestGreaterThanOrEqual;
  if (!is_compare) return false;

  int32_t lhs_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  Address lhs_address = SafeTaggedOrUndefined(
      isolate, ReadInterpreterRegister(
                   interpreter::Register::FromOperand(lhs_operand)));
  Address rhs_address = SafeTaggedOrUndefined(
      isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);

  Tagged<Context> saved_context = isolate->context();
  Address context_address = CurrentInterpreterContext();
  bool switched_context = false;
  if (IsSafeTaggedHandleValue(context_address) &&
      IsContext(Tagged<Object>(context_address))) {
    isolate->set_context(Cast<Context>(Tagged<Object>(context_address)));
    switched_context = true;
  }

  ReadOnlyRoots roots(isolate);
  bool result = false;
  if (bytecode_enum == interpreter::Bytecode::kTestEqualStrict) {
    result = Object::StrictEquals(Tagged<Object>(lhs_address),
                                  Tagged<Object>(rhs_address));
  } else if (IsNumber(Tagged<Object>(lhs_address)) &&
             IsNumber(Tagged<Object>(rhs_address))) {
    double lhs = Object::NumberValue(Tagged<Object>(lhs_address));
    double rhs = Object::NumberValue(Tagged<Object>(rhs_address));
    switch (bytecode_enum) {
      case interpreter::Bytecode::kTestEqual:
        result = lhs == rhs;
        break;
      case interpreter::Bytecode::kTestLessThan:
        result = lhs < rhs;
        break;
      case interpreter::Bytecode::kTestGreaterThan:
        result = lhs > rhs;
        break;
      case interpreter::Bytecode::kTestLessThanOrEqual:
        result = lhs <= rhs;
        break;
      case interpreter::Bytecode::kTestGreaterThanOrEqual:
        result = lhs >= rhs;
        break;
      default:
        UNREACHABLE();
    }
  } else {
    HandleScope scope(isolate);
    DirectHandle<Object> lhs =
        direct_handle(Tagged<Object>(lhs_address), isolate);
    DirectHandle<Object> rhs =
        direct_handle(Tagged<Object>(rhs_address), isolate);
    Maybe<bool> maybe_result = Nothing<bool>();
    switch (bytecode_enum) {
      case interpreter::Bytecode::kTestEqual:
        maybe_result = Object::Equals(isolate, lhs, rhs);
        break;
      case interpreter::Bytecode::kTestLessThan:
        maybe_result = Object::LessThan(isolate, lhs, rhs);
        break;
      case interpreter::Bytecode::kTestGreaterThan:
        maybe_result = Object::GreaterThan(isolate, lhs, rhs);
        break;
      case interpreter::Bytecode::kTestLessThanOrEqual:
        maybe_result = Object::LessThanOrEqual(isolate, lhs, rhs);
        break;
      case interpreter::Bytecode::kTestGreaterThanOrEqual:
        maybe_result = Object::GreaterThanOrEqual(isolate, lhs, rhs);
        break;
      default:
        UNREACHABLE();
    }
    if (maybe_result.IsNothing()) {
      if (switched_context) isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    result = maybe_result.FromJust();
  }

  if (switched_context) isolate->set_context(saved_context);
  *out_result = result ? roots.true_value().ptr() : roots.false_value().ptr();
  return true;
}

bool TryRunTestTypeOfBytecode(Isolate* isolate, Tagged<BytecodeArray> bytecode,
                              int bytecode_index,
                              interpreter::Bytecode bytecode_enum,
                              interpreter::OperandScale operand_scale,
                              Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kTestTypeOf) return false;

  uint32_t raw_flag =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
  auto flag = interpreter::TestTypeOfFlags::Decode(raw_flag);
  ReadOnlyRoots roots(isolate);
  Address value_address = SafeTaggedOrUndefined(
      isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
  Tagged<Object> value(value_address);

  bool result = false;
  switch (flag) {
    case interpreter::TestTypeOfFlags::LiteralFlag::kNumber:
      result = IsNumber(value);
      break;
    case interpreter::TestTypeOfFlags::LiteralFlag::kString:
      result = !IsSmi(value) && IsString(value);
      break;
    case interpreter::TestTypeOfFlags::LiteralFlag::kSymbol:
      result = !IsSmi(value) && IsSymbol(value);
      break;
    case interpreter::TestTypeOfFlags::LiteralFlag::kBoolean:
      result = IsTrue(value, roots) || IsFalse(value, roots);
      break;
    case interpreter::TestTypeOfFlags::LiteralFlag::kBigInt:
      result = !IsSmi(value) && IsBigInt(value);
      break;
    case interpreter::TestTypeOfFlags::LiteralFlag::kUndefined:
      result = !IsSmi(value) && !IsNull(value, roots) &&
               IsUndetectable(Cast<HeapObject>(value));
      break;
    case interpreter::TestTypeOfFlags::LiteralFlag::kFunction:
      result = !IsSmi(value) && IsCallable(value) &&
               !IsUndetectable(Cast<HeapObject>(value));
      break;
    case interpreter::TestTypeOfFlags::LiteralFlag::kObject:
      result = IsNull(value, roots) ||
               (!IsSmi(value) && IsJSReceiver(value) && !IsCallable(value) &&
                !IsUndetectable(Cast<HeapObject>(value)));
      break;
    case interpreter::TestTypeOfFlags::LiteralFlag::kOther:
      result = false;
      break;
  }

  *out_result = result ? roots.true_value().ptr() : roots.false_value().ptr();
  return true;
}

bool TryRunBooleanConversionBytecode(Isolate* isolate,
                                     interpreter::Bytecode bytecode_enum,
                                     Address* out_result) {
  bool is_to_boolean = bytecode_enum == interpreter::Bytecode::kToBoolean;
  bool is_to_boolean_not =
      bytecode_enum == interpreter::Bytecode::kToBooleanLogicalNot;
  bool is_logical_not = bytecode_enum == interpreter::Bytecode::kLogicalNot;
  if (!is_to_boolean && !is_to_boolean_not && !is_logical_not) return false;

  ReadOnlyRoots roots(isolate);
  Address value_address = SafeTaggedOrUndefined(
      isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
  Tagged<Object> value(value_address);

  bool truthy = false;
  if (IsTrue(value, roots)) {
    truthy = true;
  } else if (IsFalse(value, roots) || IsUndefined(value, roots) ||
             IsNull(value, roots)) {
    truthy = false;
  } else if (IsSmi(value)) {
    truthy = Smi::ToInt(value) != 0;
  } else {
    truthy = Object::BooleanValue(value, isolate);
  }

  bool result = is_to_boolean ? truthy : !truthy;
  *out_result = result ? roots.true_value().ptr() : roots.false_value().ptr();
  return true;
}

bool TryRunTypeOfBytecode(Isolate* isolate,
                          interpreter::Bytecode bytecode_enum,
                          Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kTypeOf) return false;

  Address input_address = SafeTaggedOrUndefined(
      isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
  HandleScope scope(isolate);
  DirectHandle<Object> input(Tagged<Object>(input_address), isolate);
  Handle<String> result = Object::TypeOf(isolate, input);
  *out_result = (*result).ptr();
  return true;
}

bool TryRunTestInBytecode(Isolate* isolate, Tagged<BytecodeArray> bytecode,
                          int bytecode_index,
                          interpreter::Bytecode bytecode_enum,
                          interpreter::OperandScale operand_scale,
                          Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kTestIn) return false;

  int32_t key_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  Address key_address = SafeTaggedOrUndefined(
      isolate, ReadInterpreterRegister(
                   interpreter::Register::FromOperand(key_operand)));
  Address object_address = SafeTaggedOrUndefined(
      isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);

  ReadOnlyRoots roots(isolate);
  if (!IsSafeTaggedHandleValue(object_address) ||
      !IsJSReceiver(Tagged<Object>(object_address))) {
    *out_result = roots.false_value().ptr();
    return true;
  }

  HandleScope scope(isolate);
  DirectHandle<Object> object =
      direct_handle(Tagged<Object>(object_address), isolate);
  DirectHandle<Object> key = direct_handle(Tagged<Object>(key_address),
                                           isolate);

  Tagged<Context> saved_context = isolate->context();
  Address context_address = CurrentInterpreterContext();
  bool switched_context = false;
  if (IsSafeTaggedHandleValue(context_address) &&
      IsContext(Tagged<Object>(context_address))) {
    isolate->set_context(Cast<Context>(Tagged<Object>(context_address)));
    switched_context = true;
  }

  DirectHandle<Object> result;
  MaybeDirectHandle<Object> maybe_result =
      Runtime::HasProperty(isolate, object, key);
  if (switched_context) isolate->set_context(saved_context);
  if (!maybe_result.ToHandle(&result)) {
    *out_result = ReadOnlyRoots(isolate).exception().ptr();
    return true;
  }
  *out_result = (*result).ptr();
  return true;
}

bool IsJSFunctionBuiltin(Isolate* isolate, DirectHandle<Object> callable,
                         Builtin builtin);
bool TryOrdinaryHasInstanceForWasm(Isolate* isolate, Address callable_address,
                                   Address object_address,
                                   Address* out_result);

bool TryRunTestInstanceOfBytecode(Isolate* isolate,
                                  Tagged<BytecodeArray> bytecode,
                                  int bytecode_index,
                                  interpreter::Bytecode bytecode_enum,
                                  interpreter::OperandScale operand_scale,
                                  Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kTestInstanceOf) return false;

  int32_t object_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  Address object_address = SafeTaggedOrUndefined(
      isolate, ReadInterpreterRegister(
                   interpreter::Register::FromOperand(object_operand)));
  Address callable_address = SafeTaggedOrUndefined(
      isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);

  ReadOnlyRoots roots(isolate);
  Tagged<Context> saved_context = isolate->context();
  Address context_address = CurrentInterpreterContext();
  bool switched_context = false;
  if (IsSafeTaggedHandleValue(context_address) &&
      IsContext(Tagged<Object>(context_address))) {
    isolate->set_context(Cast<Context>(Tagged<Object>(context_address)));
    switched_context = true;
  }

  HandleScope scope(isolate);
  DirectHandle<JSAny> object =
      direct_handle(Cast<JSAny>(Tagged<Object>(object_address)), isolate);
  DirectHandle<JSAny> callable =
      direct_handle(Cast<JSAny>(Tagged<Object>(callable_address)), isolate);

  if (IsJSReceiver(*callable)) {
    DirectHandle<Object> inst_of_handler;
    MaybeDirectHandle<Object> maybe_inst_of_handler = Object::GetMethod(
        isolate, Cast<JSReceiver>(callable),
        isolate->factory()->has_instance_symbol());
    if (!maybe_inst_of_handler.ToHandle(&inst_of_handler)) {
      if (switched_context) isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    if (IsUndefined(*inst_of_handler, isolate) ||
        IsJSFunctionBuiltin(isolate, inst_of_handler,
                            Builtin::kFunctionPrototypeHasInstance)) {
      Address ordinary_result = roots.exception().ptr();
      bool handled = TryOrdinaryHasInstanceForWasm(
          isolate, callable_address, object_address, &ordinary_result);
      if (handled) {
        if (switched_context) isolate->set_context(saved_context);
        *out_result = ordinary_result;
#ifdef __wasi__
        static int test_instanceof_ordinary_trace_count = 0;
        if (kTraceWasmFallbackDetails &&
            test_instanceof_ordinary_trace_count < 16) {
          ++test_instanceof_ordinary_trace_count;
          PrintF("WasmInterpreterEntryTrampoline: TestInstanceOf ordinary "
                 "handler detail");
          DumpRuntimeArg(" object", 0, object_address);
          DumpRuntimeArg(" callable", 0, callable_address);
          DumpRuntimeArg(" result", 0, *out_result);
          PrintF("\n");
        }
#endif
        return true;
      }
    }
  }

  DirectHandle<Object> result;
  MaybeDirectHandle<Object> maybe_result =
      Object::InstanceOf(isolate, object, callable);
  if (switched_context) isolate->set_context(saved_context);
  if (!maybe_result.ToHandle(&result)) {
    *out_result = roots.exception().ptr();
    return true;
  }

#ifdef __wasi__
  static int test_instanceof_false_trace_count = 0;
  if (IsFalse(*result, roots) && test_instanceof_false_trace_count < 16) {
    ++test_instanceof_false_trace_count;
    PrintF("WasmInterpreterEntryTrampoline: TestInstanceOf false detail");
    DumpRuntimeArg(" object", 0, object_address);
    DumpRuntimeArg(" callable", 0, callable_address);
    DumpRuntimeArg(" result", 0, (*result).ptr());
    PrintF("\n  current");
    Address current_function =
        g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
            StandardFrameConstants::kFunctionOffset)];
    DumpFunctionSourceForTrace(current_function);
    PrintF("\n  callable_detail");
    DumpFunctionSourceForTrace(callable_address);
    if (IsJSReceiver(*object) && IsJSReceiver(*callable)) {
      DirectHandle<JSReceiver> object_receiver = Cast<JSReceiver>(object);
      DirectHandle<JSReceiver> callable_receiver = Cast<JSReceiver>(callable);
      DirectHandle<JSPrototype> object_prototype;
      DirectHandle<Object> callable_prototype;
      MaybeDirectHandle<JSPrototype> maybe_object_prototype =
          JSReceiver::GetPrototype(isolate, object_receiver);
      MaybeDirectHandle<Object> maybe_callable_prototype =
          Object::GetProperty(isolate, callable_receiver,
                              isolate->factory()->prototype_string());
      bool have_object_prototype =
          maybe_object_prototype.ToHandle(&object_prototype);
      bool have_callable_prototype =
          maybe_callable_prototype.ToHandle(&callable_prototype);
      PrintF("\n  prototype_detail");
      if (have_object_prototype) {
        DumpRuntimeArg(" object_proto", 0, (*object_prototype).ptr());
      } else {
        PrintF(" object_proto=<lookup-failed>");
      }
      if (have_callable_prototype) {
        DumpRuntimeArg(" callable_proto", 0, (*callable_prototype).ptr());
      } else {
        PrintF(" callable_proto=<lookup-failed>");
      }
      if (have_object_prototype && have_callable_prototype) {
        Maybe<bool> direct_chain = JSReceiver::HasInPrototypeChain(
            isolate, object_receiver, callable_prototype);
        if (direct_chain.IsJust()) {
          PrintF(" direct_chain=%d same_first_proto=%d",
                 direct_chain.FromJust() ? 1 : 0,
                 *object_prototype == *callable_prototype ? 1 : 0);
        } else {
          PrintF(" direct_chain=<lookup-failed>");
        }
      }
    }
    PrintF("\n  object_props");
    DumpNamedDataPropertyForTrace(isolate, object_address, "id");
    DumpNamedDataPropertyForTrace(isolate, object_address, "filename");
    DumpNamedDataPropertyForTrace(isolate, object_address, "loaded");
    DumpNamedDataPropertyForTrace(isolate, object_address, "constructor");
    PrintF("\n");
  }
#endif

  *out_result = (*result).ptr();
  return true;
}

Address SafeTaggedOrUndefined(Isolate* isolate, Address value) {
  ReadOnlyRoots roots(isolate);
  if (!IsSafeTaggedHandleValue(value) &&
      !IsKnownReadOnlyRootValue(isolate, value)) {
    return roots.undefined_value().ptr();
  }
  Tagged<Object> object(value);
  if (IsException(object, isolate)) {
    if (isolate->has_exception()) isolate->clear_exception();
    isolate->clear_pending_message();
    return roots.undefined_value().ptr();
  }
  if (IsAnyHole(object)) {
    return roots.undefined_value().ptr();
  }
  return value;
}

Address SafeTaggedRootOrUndefined(Isolate* isolate, Address value) {
  ReadOnlyRoots roots(isolate);
  if (!IsSafeTaggedRootValue(isolate, value)) return roots.undefined_value().ptr();
  return SafeTaggedOrUndefined(isolate, value);
}

Address SafeRuntimeArgOrUndefined(Isolate* isolate, Address value) {
  ReadOnlyRoots roots(isolate);
  if (!IsSafeTaggedHandleValue(value) &&
      !IsKnownReadOnlyRootValue(isolate, value)) {
    return roots.undefined_value().ptr();
  }
  Tagged<Object> object(value);
  if (IsException(object, isolate)) {
    if (isolate->has_exception()) isolate->clear_exception();
    isolate->clear_pending_message();
    return roots.undefined_value().ptr();
  }
  return value;
}

Address NormalizeWasmInterpreterResult(Isolate* isolate, const char* label,
                                       Address result) {
  Tagged<Object> object(result);
  if (!IsException(object, isolate)) return result;

  bool had_exception = isolate->has_exception();
  if (kTraceWasmFallbackDetails) {
    PrintF("WasmInterpreterEntryTrampoline: suppressing exception sentinel "
           "from %s pending=%d\n",
           label, had_exception);
  }
  if (had_exception) isolate->clear_exception();
  isolate->clear_pending_message();
  return ReadOnlyRoots(isolate).undefined_value().ptr();
}

void PublishWasmInterpreterFallbackResult(Isolate* isolate, const char* label,
                                          Address* result) {
  *result = NormalizeWasmInterpreterResult(isolate, label, *result);
  if (*result == Smi::FromInt(123456789).ptr()) {
    PrintF("WASM32_SMI_PUBLISH_TRACE\n");
    g_trace_after_collection_fallback_steps = 64;
  }
  if (IsSafeTaggedHandleValue(*result) &&
      IsJSFunction(Tagged<Object>(*result)) &&
      SharedDebugNameEqualsAsciiForTrace(
          Wasm32JSFunctionShared(Cast<JSFunction>(Tagged<Object>(*result))),
          "W32TRACE")) {
    PrintF("WASM32_CLOSURE_TRACE\n");
    g_trace_after_collection_fallback_steps = 64;
  }
  int accumulator_slot = SlotFor(kInterpreterAccumulatorRegister);
  g_wasm_regs[accumulator_slot] = *result;
  MirrorWasmGCRegSlotForWrite(accumulator_slot, *result);
  int return_slot = SlotFor(kReturnRegister0);
  g_wasm_regs[return_slot] = *result;
  MirrorWasmGCRegSlotForWrite(return_slot, *result);
}

interpreter::Register RegisterFromListOperand(int32_t first_operand,
                                              int index) {
  return interpreter::Register::FromOperand(first_operand - index);
}

constexpr int kMaxWasmCallArgs = 64;
constexpr int kWasmJSEntryArgSlotBase = kWasmOutgoingArgSlotBase;

struct WasmSlotSnapshot {
  Address raw;
  int handle_index;
};

void SaveWasmSlot(Isolate* isolate, Address value, Address* handles,
                  int* handle_count, WasmSlotSnapshot* snapshot) {
  snapshot->raw = value;
  snapshot->handle_index = -1;
  if (!IsSafeTaggedRootValue(isolate, value)) return;
  if (HAS_SMI_TAG(value)) return;

  int index = *handle_count;
  DCHECK_LT(index, kWasmRegFileSize + kWasmInterpreterFrameSlots);
  snapshot->handle_index = index;
  handles[index] = value;
  *handle_count = index + 1;
}

Address RestoreWasmSlot(const Address* handles,
                        const WasmSlotSnapshot& snapshot) {
  if (snapshot.handle_index < 0) return snapshot.raw;
  return handles[snapshot.handle_index];
}

constexpr int kMaxWasmInterpreterSnapshotDepth = 256;

struct WasmInterpreterSnapshotStorage {
  Address handles[kWasmRegFileSize + kWasmInterpreterFrameSlots];
  Heap* registered_heap;
  StrongRootsEntry* roots_entry;
  int handle_count;
  WasmSlotSnapshot regs[kWasmRegFileSize];
  WasmSlotSnapshot frame[kWasmInterpreterFrameSlots];
  Address frame_pointer;
};

WasmInterpreterSnapshotStorage
    g_wasm_interpreter_snapshots[kMaxWasmInterpreterSnapshotDepth];
int g_wasm_interpreter_snapshot_depth = 0;
bool g_wasm_request_duplex_getter_returned = false;
int g_wasm_post_getter_heartbeat_count = 0;
int g_wasm_loop_window_count = 0;
int g_wasm_main_await_window_count = 0;

class WasmJSEntryDepthScope {
 public:
  WasmJSEntryDepthScope() : outermost_(g_wasm_js_entry_depth_++ == 0) {}
  ~WasmJSEntryDepthScope() { --g_wasm_js_entry_depth_; }

  bool outermost() const { return outermost_; }

 private:
  static int g_wasm_js_entry_depth_;
  bool outermost_;
};

int WasmJSEntryDepthScope::g_wasm_js_entry_depth_ = 0;

class WasmInterpreterStateSnapshot {
 public:
  explicit WasmInterpreterStateSnapshot(Isolate* isolate)
      : isolate_(isolate), storage_(nullptr), depth_(-1), restored_(false) {
    if (g_wasm_interpreter_snapshot_depth >=
        kMaxWasmInterpreterSnapshotDepth) {
      FATAL("wasm32 interpreter snapshot depth exceeded");
    }
    depth_ = g_wasm_interpreter_snapshot_depth++;
    storage_ = &g_wasm_interpreter_snapshots[depth_];
    Heap* heap = isolate_->heap();
    if (storage_->registered_heap != heap) {
      storage_->registered_heap = heap;
      storage_->roots_entry = nullptr;
    }
    Address undefined = ReadOnlyRoots(isolate_).undefined_value().ptr();
    for (Address& handle : storage_->handles) handle = undefined;
    if (storage_->roots_entry == nullptr) {
      storage_->roots_entry = heap->RegisterStrongRoots(
          "wasm32-interpreter-snapshot",
          FullObjectSlot(storage_->handles),
          FullObjectSlot(storage_->handles +
                         kWasmRegFileSize + kWasmInterpreterFrameSlots));
    }
    storage_->handle_count = 0;
    storage_->frame_pointer = g_wasm_current_frame_pointer;
    for (int i = 0; i < kWasmRegFileSize; ++i) {
      SaveWasmSlot(isolate_, g_wasm_regs[i], storage_->handles,
                   &storage_->handle_count, &storage_->regs[i]);
    }
    for (int i = 0; i < kWasmInterpreterFrameSlots; ++i) {
      SaveWasmSlot(isolate_, g_wasm_interpreter_frame[i], storage_->handles,
                   &storage_->handle_count, &storage_->frame[i]);
    }
  }

  ~WasmInterpreterStateSnapshot() {
    if (!restored_) Release();
  }

  void Restore() {
    if (restored_) return;
    for (int i = 0; i < kWasmRegFileSize; ++i) {
      g_wasm_regs[i] = RestoreWasmSlot(storage_->handles, storage_->regs[i]);
    }
    for (int i = 0; i < kWasmInterpreterFrameSlots; ++i) {
      g_wasm_interpreter_frame[i] =
          RestoreWasmSlot(storage_->handles, storage_->frame[i]);
    }
    g_wasm_current_frame_pointer = storage_->frame_pointer;
    Release();
  }

 private:
  void Release() {
    if (g_wasm_interpreter_snapshot_depth != depth_ + 1) {
      FATAL("wasm32 interpreter snapshot restore out of order");
    }
    Address undefined = ReadOnlyRoots(isolate_).undefined_value().ptr();
    for (int i = 0; i < storage_->handle_count; ++i) {
      storage_->handles[i] = undefined;
    }
    storage_->handle_count = 0;
    g_wasm_interpreter_snapshot_depth = depth_;
    restored_ = true;
    storage_ = nullptr;
  }

  Isolate* isolate_;
  WasmInterpreterSnapshotStorage* storage_;
  int depth_;
  bool restored_;
};

MaybeDirectHandle<Object> GetObjectPropertyPreservingWasmInterpreterState(
    Isolate* isolate, DirectHandle<JSAny> lookup_start_object,
    DirectHandle<Object> key) {
  return Runtime::GetObjectProperty(isolate, lookup_start_object, key);
}

bool AddCallArgument(Isolate* isolate, DirectHandle<Object>* args,
                     int* arg_count, Address value) {
  if (*arg_count >= kMaxWasmCallArgs) return false;
  value = SafeTaggedOrUndefined(isolate, value);
  args[*arg_count] = direct_handle(Tagged<Object>(value), isolate);
  *arg_count += 1;
  return true;
}

bool AddSpreadCallArguments(Isolate* isolate, DirectHandle<Object>* args,
                            int* arg_count, Address spread_value) {
  spread_value = SafeTaggedOrUndefined(isolate, spread_value);
  Tagged<Object> spread_object(spread_value);
  if (IsJSArray(spread_object)) {
    DirectHandle<JSArray> array =
        direct_handle(Cast<JSArray>(spread_object), isolate);
    uint32_t length = 0;
    if (!Object::ToArrayLength(array->length(), &length) ||
        length > static_cast<uint32_t>(kMaxWasmCallArgs - *arg_count)) {
      return false;
    }
    for (uint32_t i = 0; i < length; ++i) {
      DirectHandle<Object> element;
      if (!JSReceiver::GetElement(isolate, array, i).ToHandle(&element)) {
        return false;
      }
      args[*arg_count] = element;
      *arg_count += 1;
    }
    return true;
  }

  if (!IsJSReceiver(spread_object)) return false;
  DirectHandle<JSAny> iterable =
      direct_handle(Cast<JSAny>(spread_object), isolate);
  DirectHandle<Object> iterator_method;
  if (!Runtime::GetObjectProperty(isolate, iterable,
                                  isolate->factory()->iterator_symbol())
           .ToHandle(&iterator_method) ||
      !IsCallable(*iterator_method)) {
    return false;
  }

  auto call_no_args = [&](DirectHandle<Object> callable,
                          DirectHandle<Object> receiver,
                          DirectHandle<Object>* result) {
    WasmInterpreterStateSnapshot state(isolate);
    MaybeHandle<Object> maybe_result =
        Execution::Call(isolate, callable, receiver, {});
    bool succeeded = maybe_result.ToHandle(result);
    state.Restore();
    return succeeded;
  };

  DirectHandle<Object> iterator_object;
  if (!call_no_args(iterator_method, iterable, &iterator_object) ||
      !IsJSReceiver(*iterator_object)) {
    return false;
  }
  DirectHandle<JSAny> iterator = Cast<JSAny>(iterator_object);
  DirectHandle<Object> next_method;
  if (!Runtime::GetObjectProperty(isolate, iterator,
                                  isolate->factory()->next_string())
           .ToHandle(&next_method) ||
      !IsCallable(*next_method)) {
    return false;
  }

  for (;;) {
    DirectHandle<Object> next_result;
    if (!call_no_args(next_method, iterator, &next_result) ||
        !IsJSReceiver(*next_result)) {
      return false;
    }
    DirectHandle<JSAny> result_object = Cast<JSAny>(next_result);
    DirectHandle<Object> done;
    if (!Runtime::GetObjectProperty(isolate, result_object,
                                    isolate->factory()->done_string())
             .ToHandle(&done)) {
      return false;
    }
    if (Object::BooleanValue(*done, isolate)) return true;
    if (*arg_count >= kMaxWasmCallArgs) return false;

    DirectHandle<Object> value;
    if (!Runtime::GetObjectProperty(isolate, result_object,
                                    isolate->factory()->value_string())
             .ToHandle(&value)) {
      return false;
    }
    args[*arg_count] = value;
    *arg_count += 1;
  }
}

bool TryRunTypedArrayPrototypeSetBuiltin(Isolate* isolate,
                                         DirectHandle<Object> callable,
                                         DirectHandle<Object> receiver,
                                         int arg_count,
                                         DirectHandle<Object>* args,
                                         Address* out_result) {
  if (!IsJSFunctionBuiltin(isolate, callable,
                           Builtin::kTypedArrayPrototypeSet)) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  if (!IsJSTypedArray(*receiver) || arg_count < 1) {
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<JSTypedArray> target = Cast<JSTypedArray>(receiver);
  DirectHandle<Object> source = args[0];
  size_t offset = 0;
  if (arg_count > 1 && !IsUndefined(*args[1], isolate) &&
      !Object::ToIntegerIndex(*args[1], &offset)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  size_t source_length = 0;
  if (IsJSTypedArray(*source)) {
    source_length = Cast<JSTypedArray>(source)->GetLength();
  } else if (IsJSReceiver(*source)) {
    DirectHandle<Object> length_object;
    if (!Object::GetLengthFromArrayLike(isolate, Cast<JSReceiver>(source))
             .ToHandle(&length_object) ||
        !Object::ToIntegerIndex(*length_object, &source_length)) {
      *out_result = roots.exception().ptr();
      return true;
    }
  } else {
    *out_result = roots.exception().ptr();
    return true;
  }

  if (offset > target->GetLength() ||
      source_length > target->GetLength() - offset ||
      source_length > static_cast<size_t>(Smi::kMaxValue) ||
      offset > static_cast<size_t>(Smi::kMaxValue)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  Address runtime_args[4] = {
      Smi::FromInt(static_cast<int>(offset)).ptr(),
      Smi::FromInt(static_cast<int>(source_length)).ptr(),
      (*source).ptr(),
      (*target).ptr(),
  };
  WasmTemporaryRootScope runtime_roots(isolate, runtime_args, 4);
  Runtime_TypedArraySet(4, &runtime_roots.data()[3], isolate);
  *out_result = isolate->has_exception() ? roots.exception().ptr()
                                         : roots.undefined_value().ptr();
  return true;
}

bool TryCallJSFunctionDirect(Isolate* isolate, DirectHandle<Object> callable,
                             DirectHandle<Object> receiver, int arg_count,
                             DirectHandle<Object>* args,
                             Address* out_result) {
  if (!IsJSFunction(*callable) && !IsJSBoundFunction(*callable)) return false;
  if (arg_count > kMaxWasmCallArgs) return false;

  Address call_values[2] = {(*callable).ptr(), (*receiver).ptr()};
  Address arg_values[kMaxWasmCallArgs];
  for (int i = 0; i < arg_count; ++i) {
    arg_values[i] = (*args[i]).ptr();
  }
  WasmTemporaryRootScope call_roots(isolate, call_values, 2);
  WasmTemporaryRootScope arg_roots(isolate, arg_values, arg_count);
  Address* rooted_call = call_roots.data();
  Address* rooted_args = arg_roots.data();
  Address* argv[kMaxWasmCallArgs];
  for (int i = 0; i < arg_count; ++i) argv[i] = &rooted_args[i];

  ReadOnlyRoots roots(isolate);
  SaveContext save(isolate);
  *out_result = WasmJSEntry(isolate->isolate_data()->isolate_root(),
                            roots.undefined_value().ptr(), rooted_call[0],
                            rooted_call[1],
                            JSParameterCount(arg_count), argv);
  return true;
}

struct PendingWasmJSCall {
  bool pending = false;
  bool diagnostic = false;
  int source_position = -1;
  Address context = kNullAddress;
  Address callable = kNullAddress;
  Address receiver = kNullAddress;
  int arg_count = 0;
  Address args[kMaxWasmCallArgs];
};

Address RunPendingWasmJSCall(Isolate* isolate,
                             const PendingWasmJSCall& call) {
  DCHECK(call.pending);
  DCHECK_LE(call.arg_count, kMaxWasmCallArgs);

  // The previous bytecode iteration has already released its GC state scope.
  // Preserve the complete outer interpreter state while WasmJSEntry snapshots
  // and clears the global register file for the recursive call.
  WasmGCStateScope call_gc_state(isolate);
  Address call_values[3] = {call.context, call.callable, call.receiver};
  WasmTemporaryRootScope call_roots(isolate, call_values, 3);
  WasmTemporaryRootScope arg_roots(isolate, call.args, call.arg_count);
  Address* rooted_call = call_roots.data();
  Address* rooted_args = arg_roots.data();
  Address* argv[kMaxWasmCallArgs];
  for (int i = 0; i < call.arg_count; ++i) {
    argv[i] = &rooted_args[i];
  }

  SaveContext save_context(isolate);
  if (IsSafeTaggedHandleValue(rooted_call[0]) &&
      IsContext(Tagged<Object>(rooted_call[0]))) {
    isolate->set_context(Cast<Context>(Tagged<Object>(rooted_call[0])));
  }

  Address result = WasmJSEntry(
      isolate->isolate_data()->isolate_root(),
      ReadOnlyRoots(isolate).undefined_value().ptr(), rooted_call[1],
      rooted_call[2], JSParameterCount(call.arg_count),
      call.arg_count == 0 ? nullptr : argv);
  return result;
}

bool IsPercentSpecifierChar(uint16_t value) {
  switch (value) {
    case 'd':
    case 'f':
    case 'i':
    case 'j':
    case 'o':
    case 'O':
    case 's':
      return true;
    default:
      return false;
  }
}

bool IsNodePercentSpecifierPattern(Tagged<String> source) {
  constexpr int kPatternLength = sizeof(kNodePercentSpecifierPattern) - 1;
  if (source->length() != kPatternLength) return false;
  for (int i = 0; i < kPatternLength; ++i) {
    if (source->Get(i) != kNodePercentSpecifierPattern[i]) return false;
  }
  return true;
}

bool IsNodePercentSpecifierRegExp(Isolate* isolate,
                                  DirectHandle<JSReceiver> regexp) {
  if (IsJSRegExp(*regexp)) {
    Tagged<JSRegExp> regexp_value = Cast<JSRegExp>(*regexp);
    if ((regexp_value->flags() & JSRegExp::kGlobal) == 0) return false;
    return IsNodePercentSpecifierPattern(regexp_value->source());
  }

  DirectHandle<String> marker_name =
      isolate->factory()->InternalizeUtf8String(kWasm32RegExpPatternMarker);
  Handle<Object> marker_value =
      JSReceiver::GetDataProperty(isolate, regexp, marker_name);
  if (!IsString(*marker_value)) return false;
  if (!IsNodePercentSpecifierPattern(Cast<String>(*marker_value))) {
    return false;
  }

  DirectHandle<String> flags_name =
      isolate->factory()->InternalizeUtf8String(kWasm32RegExpFlagsMarker);
  Handle<Object> flags_value =
      JSReceiver::GetDataProperty(isolate, regexp, flags_name);
  return IsSmi(*flags_value) &&
         (Smi::ToInt(*flags_value) & JSRegExp::kGlobal) != 0;
}

int GetWasm32RegExpLastIndex(Isolate* isolate,
                             DirectHandle<JSReceiver> regexp) {
  if (IsJSRegExp(*regexp)) {
    Tagged<Object> last_index = Cast<JSRegExp>(*regexp)->last_index();
    return IsSmi(last_index) ? Smi::ToInt(last_index) : 0;
  }
  Handle<Object> last_index =
      JSReceiver::GetDataProperty(isolate, regexp,
                                  isolate->factory()->lastIndex_string());
  return IsSmi(*last_index) ? Smi::ToInt(*last_index) : 0;
}

void SetWasm32RegExpLastIndex(Isolate* isolate,
                              DirectHandle<JSReceiver> regexp, int value) {
  DirectHandle<Object> value_handle =
      direct_handle(Smi::FromInt(value), isolate);
  if (IsJSRegExp(*regexp)) {
    Cast<JSRegExp>(*regexp)->set_last_index(*value_handle,
                                            UPDATE_WRITE_BARRIER);
    return;
  }
  if (IsJSObject(*regexp)) {
    JSObject::SetOwnPropertyIgnoreAttributes(
        Cast<JSObject>(regexp), isolate->factory()->lastIndex_string(),
        value_handle, NONE)
        .Check();
  }
}

bool TryRunMissingRegExpPrototypeExecCall(Isolate* isolate,
                                          Address regexp_address,
                                          Address subject_address,
                                          Address* out_result) {
  if (!IsSafeTaggedHandleValue(regexp_address) ||
      !IsSafeTaggedHandleValue(subject_address)) {
    return false;
  }

  Tagged<Object> regexp_object(regexp_address);
  Tagged<Object> subject_object(subject_address);
  if (!IsJSReceiver(regexp_object) || !IsString(subject_object)) return false;

  HandleScope scope(isolate);
  WasmGCStateScope gc_state(isolate);
  SetCurrentIsolateScope current_isolate_scope(isolate);
  WasmInterpreterStateSnapshot state(isolate);
  ReadOnlyRoots roots(isolate);

  Tagged<Context> saved_context = isolate->context();
  Address context_address = CurrentInterpreterContext();
  bool switched_context = false;
  if (IsSafeTaggedHandleValue(context_address) &&
      IsContext(Tagged<Object>(context_address))) {
    isolate->set_context(Cast<Context>(Tagged<Object>(context_address)));
    switched_context = true;
  }

  DirectHandle<JSReceiver> regexp =
      direct_handle(Cast<JSReceiver>(regexp_object), isolate);
  if (!IsNodePercentSpecifierRegExp(isolate, regexp)) {
    if (switched_context) isolate->set_context(saved_context);
    state.Restore();
    return false;
  }
  DirectHandle<String> subject =
      direct_handle(Cast<String>(subject_object), isolate);
  int start_index = GetWasm32RegExpLastIndex(isolate, regexp);
  if (start_index < 0) start_index = 0;
  int subject_length = subject->length();
  if (start_index > subject_length) start_index = subject_length;

  int match_start = -1;
  for (int i = start_index; i + 1 < subject_length; ++i) {
    if (subject->Get(i) == '%' && IsPercentSpecifierChar(subject->Get(i + 1))) {
      match_start = i;
      break;
    }
  }

  if (match_start < 0) {
    SetWasm32RegExpLastIndex(isolate, regexp, 0);
    *out_result = roots.null_value().ptr();
    if (switched_context) isolate->set_context(saved_context);
    state.Restore();
    PrintF("WasmInterpreterEntryTrampoline: fallback missing "
           "RegExpPrototypeExec result=0x%x\n",
           static_cast<unsigned>(*out_result));
    return true;
  }

  int match_end = match_start + 2;
  SetWasm32RegExpLastIndex(isolate, regexp, match_end);
  DirectHandle<String> match =
      isolate->factory()->NewProperSubString(subject, match_start, match_end);
  DirectHandle<FixedArray> elements = isolate->factory()->NewFixedArray(1);
  elements->set(0, *match);
  DirectHandle<JSArray> result =
      isolate->factory()->NewJSArrayWithElements(elements, PACKED_ELEMENTS, 1);
  if (switched_context) isolate->set_context(saved_context);
  state.Restore();

  *out_result = (*result).ptr();
  PrintF("WasmInterpreterEntryTrampoline: fallback missing "
         "RegExpPrototypeExec result=0x%x\n",
         static_cast<unsigned>(*out_result));
  return true;
}

bool TryRunRegExpPrototypeExecDirect(Isolate* isolate,
                                     Tagged<JSFunction> function,
                                     Address receiver_address,
                                     Address subject_address,
                                     Address* out_result) {
  ReadOnlyRoots roots(isolate);
  if (!IsSafeTaggedHandleValue(receiver_address) ||
      !IsJSRegExp(Tagged<Object>(receiver_address))) {
    return false;
  }

  Tagged<Context> saved_context = isolate->context();
  isolate->set_context(Wasm32JSFunctionContext(function));

  HandleScope scope(isolate);
  DirectHandle<JSRegExp> regexp(
      Cast<JSRegExp>(Tagged<Object>(receiver_address)), isolate);
  DirectHandle<Object> subject_object(Tagged<Object>(subject_address),
                                      isolate);
  DirectHandle<String> subject;
  if (!Object::ToString(isolate, subject_object).ToHandle(&subject)) {
    isolate->set_context(saved_context);
    *out_result = roots.exception().ptr();
    return true;
  }
  subject = String::Flatten(isolate, subject);

  v8_flags.regexp_interpret_all = true;

  JSRegExp::Flags flags = regexp->flags();
  const bool global = (flags & JSRegExp::kGlobal) != 0;
  const bool sticky = (flags & JSRegExp::kSticky) != 0;
  const bool should_update_last_index = global || sticky;

  uint32_t last_index = 0;
  if (should_update_last_index) {
    DirectHandle<Object> last_index_object(regexp->last_index(), isolate);
    if (!Object::ToLength(isolate, last_index_object)
             .ToHandle(&last_index_object)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    last_index = PositiveNumberToUint32(*last_index_object);
  }

  DirectHandle<Object> match_indices_object(roots.null_value(), isolate);
  if (last_index <= static_cast<uint32_t>(subject->length())) {
    DirectHandle<RegExpMatchInfo> last_match_info =
        isolate->regexp_last_match_info();
    if (!RegExp::Exec_Single(isolate, regexp, subject, last_index,
                             last_match_info)
             .ToHandle(&match_indices_object)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
  }

  if (IsNull(*match_indices_object, isolate)) {
    if (should_update_last_index) {
      regexp->set_last_index(Smi::zero(), SKIP_WRITE_BARRIER);
    }
    isolate->set_context(saved_context);
    *out_result = roots.null_value().ptr();
    return true;
  }

  DirectHandle<RegExpMatchInfo> match_indices =
      Cast<RegExpMatchInfo>(match_indices_object);
  const int register_count = match_indices->number_of_capture_registers();
  const int result_count = register_count >> 1;
  DirectHandle<FixedArray> elements =
      isolate->factory()->NewFixedArray(result_count);
  for (int i = 0; i < result_count; ++i) {
    const int start =
        match_indices->capture(RegExpMatchInfo::capture_start_index(i));
    if (start < 0) {
      elements->set(i, roots.undefined_value());
      continue;
    }
    const int end =
        match_indices->capture(RegExpMatchInfo::capture_end_index(i));
    DirectHandle<String> capture =
        isolate->factory()->NewSubString(subject, start, end);
    elements->set(i, *capture);
  }

  const int match_start =
      match_indices->capture(RegExpMatchInfo::capture_start_index(0));
  const int match_end =
      match_indices->capture(RegExpMatchInfo::capture_end_index(0));
  if (should_update_last_index) {
    regexp->set_last_index(Smi::FromInt(match_end), SKIP_WRITE_BARRIER);
  }

  DirectHandle<JSArray> result =
      isolate->factory()->NewJSArrayWithElements(elements, PACKED_ELEMENTS,
                                                 result_count);
  DirectHandle<JSObject> result_object = Cast<JSObject>(result);
  DirectHandle<Object> maybe_names = isolate->factory()->undefined_value();
  DirectHandle<Object> groups = isolate->factory()->undefined_value();
  DirectHandle<RegExpData> data(regexp->data(isolate), isolate);
  if (RegExpData::TypeSupportsCaptures(data->type_tag())) {
    Tagged<Object> capture_name_map =
        Cast<IrRegExpData>(*data)->capture_name_map();
    if (IsFixedArray(capture_name_map)) {
      DirectHandle<FixedArray> names(Cast<FixedArray>(capture_name_map),
                                     isolate);
      maybe_names = names;
      DirectHandle<JSObject> group_object =
          isolate->factory()->NewJSObjectWithNullProto();
      const int named_capture_count = names->length() >> 1;
      for (int i = 0; i < named_capture_count; ++i) {
        DirectHandle<String> capture_name(
            Cast<String>(names->get(i * 2)), isolate);
        const int capture_index = Smi::ToInt(names->get(i * 2 + 1));
        DirectHandle<Object> capture_value(elements->get(capture_index),
                                           isolate);
        DirectHandle<Object> ignored;
        if (!JSObject::SetOwnPropertyIgnoreAttributes(
                 group_object, capture_name, capture_value, NONE)
                 .ToHandle(&ignored)) {
          isolate->set_context(saved_context);
          *out_result = roots.exception().ptr();
          return true;
        }
      }
      groups = group_object;
    }
  }

  auto set_result_property =
      [&](DirectHandle<Name> name, DirectHandle<Object> value) -> bool {
    DirectHandle<Object> ignored;
    return JSObject::SetOwnPropertyIgnoreAttributes(result_object, name, value,
                                                    NONE)
        .ToHandle(&ignored);
  };

  if (!set_result_property(isolate->factory()->index_string(),
                           direct_handle(Smi::FromInt(match_start), isolate)) ||
      !set_result_property(isolate->factory()->input_string(), subject) ||
      !set_result_property(isolate->factory()->groups_string(), groups)) {
    isolate->set_context(saved_context);
    *out_result = roots.exception().ptr();
    return true;
  }

  if ((flags & JSRegExp::kHasIndices) != 0) {
    DirectHandle<JSRegExpResultIndices> indices =
        JSRegExpResultIndices::BuildIndices(isolate, match_indices,
                                            maybe_names);
    if (!set_result_property(isolate->factory()->indices_string(), indices)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
  }

  *out_result = (*result).ptr();
  isolate->set_context(saved_context);
  if (kTraceWasmFallbackDetails) {
    PrintF("WasmJSEntry: fallback direct RegExpPrototypeExec result=0x%x\n",
           static_cast<unsigned>(*out_result));
  }
  return true;
}

bool TryConstructJSFunctionDirect(Isolate* isolate,
                                  DirectHandle<Object> constructor,
                                  DirectHandle<Object> new_target,
                                  int arg_count, DirectHandle<Object>* args,
                                  Address* out_result) {
  if (!IsJSFunction(*constructor)) return false;
  if (!IsJSReceiver(*new_target)) return false;
  if (arg_count > kMaxWasmCallArgs) return false;

  Address call_values[2];
  call_values[0] = (*constructor).ptr();
  call_values[1] = (*new_target).ptr();
  Address arg_values[kMaxWasmCallArgs];
  for (int i = 0; i < arg_count; ++i) {
    arg_values[i] = (*args[i]).ptr();
  }
  WasmTemporaryRootScope call_roots(isolate, call_values, 2);
  WasmTemporaryRootScope arg_roots(isolate, arg_values, arg_count);
  Address* rooted_call = call_roots.data();
  Address* rooted_args = arg_roots.data();
  Address* argv[64];
  for (int i = 0; i < arg_count; ++i) {
    argv[i] = &rooted_args[i];
  }

  ReadOnlyRoots roots(isolate);
  DirectHandle<JSFunction> ctor = direct_handle(
      Cast<JSFunction>(Tagged<Object>(rooted_call[0])), isolate);
  FunctionKind ctor_kind = Wasm32JSFunctionShared(*ctor)->kind();
  bool is_derived_constructor = IsDerivedConstructor(ctor_kind);
#ifdef __wasi__
  static int construct_js_function_trace_count = 0;
  bool trace_construct_js_function =
      kTraceWasmFallbackDetails &&
      (++construct_js_function_trace_count <= 64 ||
       (construct_js_function_trace_count % 256) == 0);
  if (trace_construct_js_function) {
    PrintF("TryConstructJSFunctionDirect: #%d kind=%s derived=%d argc=%d ",
           construct_js_function_trace_count, FunctionKind2String(ctor_kind),
           is_derived_constructor ? 1 : 0, arg_count);
    DumpRuntimeArg("constructor", 0, (*constructor).ptr());
    DumpRuntimeArg(" new_target", 0, (*new_target).ptr());
    PrintF("\n");
  }
#endif
  if (is_derived_constructor) {
    SaveContext save(isolate);
    Address raw_result = WasmJSEntry(isolate->isolate_data()->isolate_root(),
                                     rooted_call[1], rooted_call[0],
                                     roots.undefined_value().ptr(),
                                     JSParameterCount(arg_count), argv);
    if (isolate->has_exception() || raw_result == roots.exception().ptr()) {
      *out_result = roots.exception().ptr();
      return true;
    }

    Tagged<Object> result(raw_result);
    if (!IsJSReceiver(result)) {
#ifdef __wasi__
      if (trace_construct_js_function) {
        PrintF("TryConstructJSFunctionDirect: derived non-receiver result ");
        DumpRuntimeArg("result", 0, raw_result);
        PrintF("\n");
      }
#endif
      *out_result = roots.exception().ptr();
      return true;
    }
    *out_result = raw_result;
#ifdef __wasi__
    if (trace_construct_js_function) {
      PrintF("TryConstructJSFunctionDirect: derived result ");
      DumpRuntimeArg("result", 0, *out_result);
      PrintF("\n");
    }
#endif
    return true;
  }

  ctor = direct_handle(Cast<JSFunction>(Tagged<Object>(rooted_call[0])),
                       isolate);
  DirectHandle<JSReceiver> new_target_receiver = direct_handle(
      Cast<JSReceiver>(Tagged<Object>(rooted_call[1])), isolate);
  DirectHandle<JSObject> instance;
  if (!JSObject::New(ctor, new_target_receiver, {}).ToHandle(&instance)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  Address instance_value = (*instance).ptr();
  WasmTemporaryRootScope instance_root(isolate, &instance_value, 1);
  SaveContext save(isolate);
  Address raw_result = WasmJSEntry(isolate->isolate_data()->isolate_root(),
                                   rooted_call[1], rooted_call[0],
                                   instance_root.data()[0],
                                   JSParameterCount(arg_count), argv);
  if (isolate->has_exception() || raw_result == roots.exception().ptr()) {
    *out_result = roots.exception().ptr();
    return true;
  }

  Tagged<Object> result(raw_result);
  *out_result = IsJSReceiver(result) ? raw_result : instance_root.data()[0];
#ifdef __wasi__
  if (trace_construct_js_function) {
    PrintF("TryConstructJSFunctionDirect: base result ");
    DumpRuntimeArg("result", 0, *out_result);
    PrintF("\n");
  }
#endif
  return true;
}

bool IsJSFunctionBuiltin(Isolate* isolate, DirectHandle<Object> callable,
                         Builtin builtin) {
  if (!IsJSFunction(*callable)) return false;
  Tagged<JSFunction> function = Cast<JSFunction>(*callable);
  Tagged<Code> code = function->code(isolate);
  if (code->is_builtin() && code->builtin_id() == builtin) return true;
  Tagged<SharedFunctionInfo> shared = Wasm32JSFunctionShared(function);
  return shared->HasBuiltinId() && shared->builtin_id() == builtin;
}

bool TryRunStringPrototypeToStringBuiltin(Isolate* isolate,
                                          DirectHandle<Object> callable,
                                          DirectHandle<Object> receiver,
                                          Address* out_result) {
  bool is_to_string = IsJSFunctionBuiltin(
      isolate, callable, Builtin::kStringPrototypeToString);
  bool is_value_of = IsJSFunctionBuiltin(
      isolate, callable, Builtin::kStringPrototypeValueOf);
  if (!is_to_string && !is_value_of) return false;

  Tagged<Object> value = *receiver;
  if (IsJSPrimitiveWrapper(value)) {
    value = Cast<JSPrimitiveWrapper>(value)->value();
  }
  if (IsString(value)) {
    *out_result = value.ptr();
    return true;
  }

  HandleScope scope(isolate);
  const char* method = is_to_string ? "String.prototype.toString"
                                    : "String.prototype.valueOf";
  isolate->Throw(*isolate->factory()->NewTypeError(
      MessageTemplate::kNotGeneric,
      isolate->factory()->NewStringFromAsciiChecked(method),
      isolate->factory()->NewStringFromAsciiChecked("String")));
  *out_result = ReadOnlyRoots(isolate).exception().ptr();
  return true;
}

bool TryRunStringPrototypeToUpperCaseBuiltin(Isolate* isolate,
                                             DirectHandle<Object> callable,
                                             DirectHandle<Object> receiver,
                                             Address* out_result) {
#ifdef V8_INTL_SUPPORT
  bool is_to_upper = IsJSFunctionBuiltin(
      isolate, callable, Builtin::kStringPrototypeToUpperCaseIntl);
#else
  bool is_to_upper = IsJSFunctionBuiltin(
      isolate, callable, Builtin::kStringPrototypeToUpperCase);
#endif
  if (!is_to_upper) {
    return false;
  }
  if (!IsString(*receiver)) return false;

  Tagged<String> input = Cast<String>(*receiver);
  if (input->length() != 1) return false;
  uint16_t code = input->Get(0);
  if (code > 0x7f) return false;
  if (code >= 'a' && code <= 'z') code -= 'a' - 'A';

  DirectHandle<String> result =
      isolate->factory()->LookupSingleCharacterStringFromCode(code);
  *out_result = (*result).ptr();
  if (kTraceWasmFallbackDetails) {
    PrintF("WasmInterpreterEntryTrampoline: fallback "
           "StringPrototypeToUpperCase result=0x%x\n",
           static_cast<unsigned>(*out_result));
  }
  return true;
}

bool TryReadRelativeSmiIndex(DirectHandle<Object> value, uint32_t length,
                             uint32_t* out_index) {
  if (!IsSmi(*value)) return false;
  int index = Smi::ToInt(*value);
  if (index < 0) {
    int64_t relative = static_cast<int64_t>(length) + index;
    *out_index = relative > 0 ? static_cast<uint32_t>(relative) : 0;
  } else {
    uint32_t relative = static_cast<uint32_t>(index);
    *out_index = relative < length ? relative : length;
  }
  return true;
}

bool TryRunStringPrototypeSliceBuiltinWithArgs(
    Isolate* isolate, DirectHandle<Object> callable,
    DirectHandle<Object> receiver, int argc, DirectHandle<Object> arg0,
    DirectHandle<Object> arg1, Address* out_result) {
  if (!IsJSFunctionBuiltin(isolate, callable,
                           Builtin::kStringPrototypeSlice)) {
    if (kTraceWasmFallbackDetails && IsJSFunction(*callable) &&
        IsString(*receiver)) {
      Tagged<JSFunction> function = Cast<JSFunction>(*callable);
      Tagged<Code> code = function->code(isolate);
      Tagged<SharedFunctionInfo> shared = Wasm32JSFunctionShared(function);
      PrintF("WasmInterpreterEntryTrampoline: StringPrototypeSlice miss "
             "code_is_builtin=%d code_builtin=%d shared_has_builtin=%d "
             "shared_builtin=%d ",
             code->is_builtin(), static_cast<int>(code->builtin_id()),
             shared->HasBuiltinId(),
             shared->HasBuiltinId() ? static_cast<int>(shared->builtin_id())
                                    : -1);
      DumpRuntimeArg("receiver", 0, (*receiver).ptr());
      PrintF("\n");
    }
    return false;
  }
  if (!IsString(*receiver)) return false;

  DirectHandle<String> input = Cast<String>(receiver);
  uint32_t length = static_cast<uint32_t>(input->length());
  ReadOnlyRoots roots(isolate);

  uint32_t start = 0;
  if (argc > 0 && !IsUndefined(*arg0, roots) &&
      !TryReadRelativeSmiIndex(arg0, length, &start)) {
    return false;
  }

  uint32_t end = length;
  if (argc > 1 && !IsUndefined(*arg1, roots) &&
      !TryReadRelativeSmiIndex(arg1, length, &end)) {
    return false;
  }

  DirectHandle<String> result =
      end <= start ? isolate->factory()->empty_string()
                   : isolate->factory()->NewProperSubString(input, start, end);
  *out_result = (*result).ptr();
  if (kTraceWasmFallbackDetails) {
    PrintF("WasmInterpreterEntryTrampoline: fallback StringPrototypeSlice "
           "start=%u end=%u result=0x%x\n",
           start, end, static_cast<unsigned>(*out_result));
  }
  return true;
}

bool TryRunStringPrototypeSliceBuiltin(Isolate* isolate,
                                       DirectHandle<Object> callable,
                                       DirectHandle<Object> receiver,
                                       int arg_count,
                                       DirectHandle<Object>* args,
                                       Address* out_result) {
  ReadOnlyRoots roots(isolate);
  DirectHandle<Object> undefined = direct_handle(roots.undefined_value(), isolate);
  DirectHandle<Object> arg0 = arg_count > 0 ? args[0] : undefined;
  DirectHandle<Object> arg1 = arg_count > 1 ? args[1] : undefined;
  return TryRunStringPrototypeSliceBuiltinWithArgs(
      isolate, callable, receiver, arg_count, arg0, arg1, out_result);
}

bool TryRunArrayForEachBuiltin(Isolate* isolate, DirectHandle<Object> callable,
                               DirectHandle<Object> receiver,
                               int arg_count, DirectHandle<Object>* args,
                               Address* out_result) {
  if (!IsJSFunction(*callable)) return false;
  Tagged<Code> code = Cast<JSFunction>(*callable)->code(isolate);
  if (!code->is_builtin() || code->builtin_id() != Builtin::kArrayForEach) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  if (arg_count == 0 || !IsCallable(*args[0])) {
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmInterpreterEntryTrampoline: ArrayForEach callback invalid ");
      if (arg_count > 0) DumpRuntimeArg("callback", 0, (*args[0]).ptr());
      PrintF("\n");
    }
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<JSReceiver> object;
  if (!Object::ToObject(isolate, receiver, "Array.prototype.forEach")
           .ToHandle(&object)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<Object> length_object;
  if (!Object::GetLengthFromArrayLike(isolate, object).ToHandle(&length_object)) {
    *out_result = roots.exception().ptr();
    return true;
  }
  double raw_length = Object::NumberValue(*length_object);
  uint32_t length = raw_length > static_cast<double>(0xffffffffu)
                        ? 0xffffffffu
                        : static_cast<uint32_t>(raw_length);

  DirectHandle<JSReceiver> protected_object = object;
  DirectHandle<Object> protected_callback = args[0];
  DirectHandle<Object> protected_this =
      arg_count > 1 ? args[1] : direct_handle(roots.undefined_value(), isolate);

  for (uint32_t index = 0; index < length; ++index) {
    HandleScope iteration_scope(isolate);
    DirectHandle<JSReceiver> current_object = protected_object;
    Maybe<bool> maybe_has_element =
        JSReceiver::HasElement(isolate, current_object, index);
    if (maybe_has_element.IsNothing()) {
      *out_result = roots.exception().ptr();
      return true;
    }
    if (!maybe_has_element.FromJust()) continue;

    DirectHandle<Object> element;
    if (!JSReceiver::GetElement(isolate, current_object, index)
             .ToHandle(&element)) {
      *out_result = roots.exception().ptr();
        return true;
    }

    DirectHandle<Object> callback_args[3];
    callback_args[0] = element;
    callback_args[1] = isolate->factory()->NewNumberFromUint(index);
    callback_args[2] = current_object;

    WasmInterpreterStateSnapshot state(isolate);
    DirectHandle<Object> callback = protected_callback;
    DirectHandle<Object> this_arg = protected_this;
    Address callback_result = roots.exception().ptr();
    bool direct_call = TryCallJSFunctionDirect(isolate, callback, this_arg, 3,
                                               callback_args,
                                               &callback_result);
    MaybeHandle<Object> maybe_result;
    if (!direct_call) {
      maybe_result = Execution::Call(
          isolate, callback, this_arg,
          ZoneVector<const DirectHandle<Object>>(callback_args, 3));
    }
    state.Restore();
    DirectHandle<Object> ignored;
    if (!direct_call && !maybe_result.ToHandle(&ignored)) {
      *out_result = roots.exception().ptr();
      return true;
    }
    if (direct_call && IsException(Tagged<Object>(callback_result), isolate)) {
      *out_result = callback_result;
      return true;
    }
  }

  *out_result = roots.undefined_value().ptr();
  return true;
}

bool TryRunArraySliceBuiltin(Isolate* isolate, DirectHandle<Object> callable,
                             DirectHandle<Object> receiver, int arg_count,
                             DirectHandle<Object>* args,
                             Address* out_result) {
  if (!IsJSFunctionBuiltin(isolate, callable,
                           Builtin::kArrayPrototypeSlice)) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  DirectHandle<JSReceiver> object;
  if (!Object::ToObject(isolate, receiver, "Array.prototype.slice")
           .ToHandle(&object)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<Object> length_object;
  if (!Object::GetLengthFromArrayLike(isolate, object).ToHandle(&length_object)) {
    *out_result = roots.exception().ptr();
    return true;
  }
  double raw_length = Object::NumberValue(*length_object);
  if (raw_length < 0 || raw_length > FixedArray::kMaxLength) {
    *out_result = roots.exception().ptr();
    return true;
  }
  int length = static_cast<int>(raw_length);

  auto read_index = [&](int arg_index, int default_value, int* out_index) {
    if (arg_index >= arg_count || IsUndefined(*args[arg_index], isolate)) {
      *out_index = default_value;
      return true;
    }
    DirectHandle<Number> number;
    if (!Object::ToInteger(isolate, args[arg_index]).ToHandle(&number)) {
      return false;
    }
    double relative = Object::NumberValue(*number);
    if (relative < 0) {
      double from_end = static_cast<double>(length) + relative;
      *out_index = from_end <= 0 ? 0 : static_cast<int>(from_end);
    } else if (relative >= length) {
      *out_index = length;
    } else {
      *out_index = static_cast<int>(relative);
    }
    return true;
  };

  int start = 0;
  int end = length;
  if (!read_index(0, 0, &start) || !read_index(1, length, &end)) {
    *out_result = roots.exception().ptr();
    return true;
  }
  int result_length = end > start ? end - start : 0;
  DirectHandle<FixedArray> elements;
  if (!isolate->factory()
           ->TryNewFixedArray(result_length)
           .ToHandle(&elements)) {
    *out_result = roots.exception().ptr();
    return true;
  }
  for (int index = 0; index < result_length; ++index) {
    elements->set(index, roots.the_hole_value());
  }

  for (int index = 0; index < result_length; ++index) {
    uint32_t source_index = static_cast<uint32_t>(start + index);
    Maybe<bool> maybe_has = JSReceiver::HasElement(isolate, object, source_index);
    if (maybe_has.IsNothing()) {
      *out_result = roots.exception().ptr();
      return true;
    }
    if (!maybe_has.FromJust()) continue;
    DirectHandle<Object> element;
    if (!JSReceiver::GetElement(isolate, object, source_index)
             .ToHandle(&element)) {
      *out_result = roots.exception().ptr();
      return true;
    }
    elements->set(index, *element);
  }

  DirectHandle<JSArray> result = isolate->factory()->NewJSArrayWithElements(
      elements, HOLEY_ELEMENTS, result_length);
  *out_result = (*result).ptr();
  return true;
}

bool TryRunArrayFilterBuiltin(Isolate* isolate, DirectHandle<Object> callable,
                              DirectHandle<Object> receiver, int arg_count,
                              DirectHandle<Object>* args,
                              Address* out_result) {
  if (!IsJSFunctionBuiltin(isolate, callable, Builtin::kArrayFilter)) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  if (arg_count == 0 || !IsCallable(*args[0])) {
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmInterpreterEntryTrampoline: ArrayFilter callback invalid ");
      if (arg_count > 0) DumpRuntimeArg("callback", 0, (*args[0]).ptr());
      PrintF("\n");
    }
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<JSReceiver> object;
  if (!Object::ToObject(isolate, receiver, "Array.prototype.filter")
           .ToHandle(&object)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<Object> length_object;
  if (!Object::GetLengthFromArrayLike(isolate, object).ToHandle(&length_object)) {
    *out_result = roots.exception().ptr();
    return true;
  }
  double raw_length = Object::NumberValue(*length_object);
  if (raw_length < 0 || raw_length > FixedArray::kMaxLength) {
    *out_result = roots.exception().ptr();
    return true;
  }
  uint32_t length = static_cast<uint32_t>(raw_length);

  DirectHandle<FixedArray> elements;
  if (!isolate->factory()
           ->TryNewFixedArray(static_cast<int>(length))
           .ToHandle(&elements)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<JSReceiver> protected_object = object;
  DirectHandle<Object> protected_callback = args[0];
  DirectHandle<Object> protected_this =
      arg_count > 1 ? args[1] : direct_handle(roots.undefined_value(), isolate);

  int result_length = 0;
  for (uint32_t index = 0; index < length; ++index) {
    HandleScope iteration_scope(isolate);
    DirectHandle<JSReceiver> current_object = protected_object;
    Maybe<bool> maybe_has_element =
        JSReceiver::HasElement(isolate, current_object, index);
    if (maybe_has_element.IsNothing()) {
      *out_result = roots.exception().ptr();
      return true;
    }
    if (!maybe_has_element.FromJust()) continue;

    DirectHandle<Object> element;
    if (!JSReceiver::GetElement(isolate, current_object, index)
             .ToHandle(&element)) {
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<Object> callback_args[3];
    callback_args[0] = element;
    callback_args[1] = isolate->factory()->NewNumberFromUint(index);
    callback_args[2] = current_object;

    WasmInterpreterStateSnapshot state(isolate);
    DirectHandle<Object> callback = protected_callback;
    DirectHandle<Object> this_arg = protected_this;
    Address callback_result = roots.exception().ptr();
    bool direct_call = TryCallJSFunctionDirect(isolate, callback, this_arg, 3,
                                               callback_args,
                                               &callback_result);
    MaybeHandle<Object> maybe_result;
    DirectHandle<Object> callback_value;
    if (direct_call) {
      if (IsException(Tagged<Object>(callback_result), isolate)) {
        state.Restore();
        *out_result = callback_result;
        return true;
      }
      callback_value = direct_handle(Tagged<Object>(callback_result), isolate);
    } else {
      maybe_result = Execution::Call(
          isolate, callback, this_arg,
          ZoneVector<const DirectHandle<Object>>(callback_args, 3));
      if (!maybe_result.ToHandle(&callback_value)) {
        state.Restore();
        *out_result = roots.exception().ptr();
        return true;
      }
    }
    state.Restore();

    if (Object::BooleanValue(*callback_value, isolate)) {
      elements->set(result_length++, *element);
    }
  }

  DirectHandle<JSArray> result = isolate->factory()->NewJSArrayWithElements(
      elements, PACKED_ELEMENTS, result_length);
  *out_result = (*result).ptr();
  if (kTraceWasmFallbackDetails) {
    PrintF("WasmInterpreterEntryTrampoline: fallback ArrayFilter "
           "length=%u result_length=%d result=0x%x\n",
           length, result_length, static_cast<unsigned>(*out_result));
  }
  return true;
}

bool TryRunArrayMapBuiltin(Isolate* isolate, DirectHandle<Object> callable,
                           DirectHandle<Object> receiver, int arg_count,
                           DirectHandle<Object>* args,
                           Address* out_result) {
  if (!IsJSFunctionBuiltin(isolate, callable, Builtin::kArrayMap)) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  if (arg_count == 0 || !IsCallable(*args[0])) {
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmInterpreterEntryTrampoline: ArrayMap callback invalid ");
      if (arg_count > 0) DumpRuntimeArg("callback", 0, (*args[0]).ptr());
      PrintF("\n");
    }
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<JSReceiver> object;
  if (!Object::ToObject(isolate, receiver, "Array.prototype.map")
           .ToHandle(&object)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<Object> length_object;
  if (!Object::GetLengthFromArrayLike(isolate, object)
           .ToHandle(&length_object)) {
    *out_result = roots.exception().ptr();
    return true;
  }
  double raw_length = Object::NumberValue(*length_object);
  if (raw_length < 0 || raw_length > FixedArray::kMaxLength) {
    *out_result = roots.exception().ptr();
    return true;
  }
  uint32_t length = static_cast<uint32_t>(raw_length);

  DirectHandle<FixedArray> elements;
  if (!isolate->factory()
           ->TryNewFixedArray(static_cast<int>(length))
           .ToHandle(&elements)) {
    *out_result = roots.exception().ptr();
    return true;
  }
  for (uint32_t index = 0; index < length; ++index) {
    elements->set(static_cast<int>(index), roots.the_hole_value());
  }

  DirectHandle<JSReceiver> protected_object = object;
  DirectHandle<Object> protected_callback = args[0];
  DirectHandle<Object> protected_this =
      arg_count > 1 ? args[1] : direct_handle(roots.undefined_value(), isolate);

  for (uint32_t index = 0; index < length; ++index) {
    HandleScope iteration_scope(isolate);
    DirectHandle<JSReceiver> current_object = protected_object;
    Maybe<bool> maybe_has_element =
        JSReceiver::HasElement(isolate, current_object, index);
    if (maybe_has_element.IsNothing()) {
      *out_result = roots.exception().ptr();
      return true;
    }
    if (!maybe_has_element.FromJust()) continue;

    DirectHandle<Object> element;
    if (!JSReceiver::GetElement(isolate, current_object, index)
             .ToHandle(&element)) {
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<Object> callback_args[3];
    callback_args[0] = element;
    callback_args[1] = isolate->factory()->NewNumberFromUint(index);
    callback_args[2] = current_object;

    WasmInterpreterStateSnapshot state(isolate);
    DirectHandle<Object> callback = protected_callback;
    DirectHandle<Object> this_arg = protected_this;
    Address callback_result = roots.exception().ptr();
    bool direct_call = TryCallJSFunctionDirect(isolate, callback, this_arg, 3,
                                               callback_args,
                                               &callback_result);
    MaybeHandle<Object> maybe_result;
    DirectHandle<Object> mapped_value;
    if (direct_call) {
      if (IsException(Tagged<Object>(callback_result), isolate)) {
        state.Restore();
        *out_result = callback_result;
        return true;
      }
      mapped_value = direct_handle(Tagged<Object>(callback_result), isolate);
    } else {
      maybe_result = Execution::Call(
          isolate, callback, this_arg,
          ZoneVector<const DirectHandle<Object>>(callback_args, 3));
      if (!maybe_result.ToHandle(&mapped_value)) {
        state.Restore();
        *out_result = roots.exception().ptr();
        return true;
      }
    }
    state.Restore();

    elements->set(static_cast<int>(index), *mapped_value);
  }

  DirectHandle<JSArray> result = isolate->factory()->NewJSArrayWithElements(
      elements, HOLEY_ELEMENTS, static_cast<int>(length));
  *out_result = (*result).ptr();
  if (kTraceWasmFallbackDetails) {
    PrintF("WasmInterpreterEntryTrampoline: fallback ArrayMap "
           "length=%u result=0x%x\n",
           length, static_cast<unsigned>(*out_result));
  }
  return true;
}

bool TryRunArrayReduceBuiltin(Isolate* isolate, DirectHandle<Object> callable,
                              DirectHandle<Object> receiver, int arg_count,
                              DirectHandle<Object>* args,
                              Address* out_result) {
  if (!IsJSFunctionBuiltin(isolate, callable, Builtin::kArrayReduce)) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  if (arg_count == 0 || !IsCallable(*args[0])) {
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmInterpreterEntryTrampoline: ArrayReduce callback invalid ");
      if (arg_count > 0) DumpRuntimeArg("callback", 0, (*args[0]).ptr());
      PrintF("\n");
    }
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<JSReceiver> object;
  if (!Object::ToObject(isolate, receiver, "Array.prototype.reduce")
           .ToHandle(&object)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<Object> length_object;
  if (!Object::GetLengthFromArrayLike(isolate, object)
           .ToHandle(&length_object)) {
    *out_result = roots.exception().ptr();
    return true;
  }
  double raw_length = Object::NumberValue(*length_object);
  if (raw_length < 0 || raw_length > FixedArray::kMaxLength) {
    *out_result = roots.exception().ptr();
    return true;
  }
  uint32_t length = static_cast<uint32_t>(raw_length);

  DirectHandle<JSReceiver> protected_object = object;
  DirectHandle<Object> protected_callback = args[0];
  DirectHandle<FixedArray> accumulator_holder =
      isolate->factory()->NewFixedArray(1);
  accumulator_holder->set(
      0, arg_count > 1 ? *args[1] : roots.the_hole_value());
  bool has_accumulator = arg_count > 1;

  for (uint32_t index = 0; index < length; ++index) {
    HandleScope iteration_scope(isolate);
    DirectHandle<JSReceiver> current_object = protected_object;
    Maybe<bool> maybe_has_element =
        JSReceiver::HasElement(isolate, current_object, index);
    if (maybe_has_element.IsNothing()) {
      *out_result = roots.exception().ptr();
      return true;
    }
    if (!maybe_has_element.FromJust()) continue;

    DirectHandle<Object> element;
    if (!JSReceiver::GetElement(isolate, current_object, index)
             .ToHandle(&element)) {
      *out_result = roots.exception().ptr();
      return true;
    }

    if (!has_accumulator) {
      accumulator_holder->set(0, *element);
      has_accumulator = true;
      continue;
    }

    DirectHandle<Object> accumulator =
        direct_handle(accumulator_holder->get(0), isolate);
    DirectHandle<Object> callback_args[4];
    callback_args[0] = accumulator;
    callback_args[1] = element;
    callback_args[2] = isolate->factory()->NewNumberFromUint(index);
    callback_args[3] = current_object;

    WasmInterpreterStateSnapshot state(isolate);
    DirectHandle<Object> callback = protected_callback;
    DirectHandle<Object> this_arg =
        direct_handle(roots.undefined_value(), isolate);
    Address callback_result = roots.exception().ptr();
    bool direct_call = TryCallJSFunctionDirect(isolate, callback, this_arg, 4,
                                               callback_args,
                                               &callback_result);
    MaybeHandle<Object> maybe_result;
    DirectHandle<Object> next_accumulator;
    if (direct_call) {
      if (IsException(Tagged<Object>(callback_result), isolate)) {
        state.Restore();
        *out_result = callback_result;
        return true;
      }
      next_accumulator =
          direct_handle(Tagged<Object>(callback_result), isolate);
    } else {
      maybe_result = Execution::Call(
          isolate, callback, this_arg,
          ZoneVector<const DirectHandle<Object>>(callback_args, 4));
      if (!maybe_result.ToHandle(&next_accumulator)) {
        state.Restore();
        *out_result = roots.exception().ptr();
        return true;
      }
    }
    state.Restore();

    accumulator_holder->set(0, *next_accumulator);
  }

  if (!has_accumulator) {
    isolate->Throw(*isolate->factory()->NewTypeError(
        MessageTemplate::kReduceNoInitial,
        isolate->factory()->NewStringFromAsciiChecked(
            "Array.prototype.reduce")));
    *out_result = roots.exception().ptr();
    return true;
  }

  *out_result = accumulator_holder->get(0).ptr();
  if (kTraceWasmFallbackDetails) {
    PrintF("WasmInterpreterEntryTrampoline: fallback ArrayReduce "
           "length=%u result=0x%x\n",
           length, static_cast<unsigned>(*out_result));
  }
  return true;
}

DirectHandle<Object> NormalizeCollectionKey(Isolate* isolate,
                                            DirectHandle<Object> key) {
  if (IsHeapNumber(*key) && Object::NumberValue(*key) == 0.0) {
    return direct_handle(Smi::zero(), isolate);
  }
  return key;
}

bool TryRunSetPrototypeAddBuiltin(Isolate* isolate,
                                  DirectHandle<Object> callable,
                                  DirectHandle<Object> receiver,
                                  int arg_count, DirectHandle<Object>* args,
                                  Address* out_result) {
  if (!IsJSFunctionBuiltin(isolate, callable, Builtin::kSetPrototypeAdd)) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  if (!IsJSSet(*receiver)) {
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmInterpreterEntryTrampoline: SetPrototypeAdd receiver "
             "invalid ");
      DumpRuntimeArg("receiver", 0, (*receiver).ptr());
      PrintF("\n");
    }
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<Object> key =
      arg_count > 0 ? args[0] : direct_handle(roots.undefined_value(), isolate);
  key = NormalizeCollectionKey(isolate, key);

  DirectHandle<JSSet> set = Cast<JSSet>(receiver);
  Tagged<Object> raw_table = set->table();
  DirectHandle<OrderedHashSet> table;
  if (!IsOrderedHashSet(raw_table) ||
      raw_table == roots.empty_ordered_hash_set()) {
    table = isolate->factory()->NewOrderedHashSet();
    set->set_table(*table);
  } else {
    table = direct_handle(Cast<OrderedHashSet>(raw_table), isolate);
  }
  static int set_add_entry_trace_count = 0;
  bool trace_set_add_entry =
      kTraceWasmFallbackDetails && set_add_entry_trace_count < 24;
  if (trace_set_add_entry) {
    ++set_add_entry_trace_count;
    PrintF("WasmInterpreterEntryTrampoline: SetPrototypeAdd entry count=%d "
           "set=0x%x table=0x%x elements=%d deleted=%d buckets=%d "
           "capacity=%d used=%d ",
           set_add_entry_trace_count, static_cast<unsigned>((*set).ptr()),
           static_cast<unsigned>((*table).ptr()), table->NumberOfElements(),
           table->NumberOfDeletedElements(), table->NumberOfBuckets(),
           table->Capacity(), table->UsedCapacity());
    DumpRuntimeArg("key", 0, (*key).ptr());
    PrintF("\n");
  }
  int32_t key_hash = Object::GetOrCreateHash(*key, isolate).value();
  if (trace_set_add_entry) {
    PrintF("WasmInterpreterEntryTrampoline: SetPrototypeAdd hash count=%d "
           "hash=%d table=0x%x elements=%d deleted=%d buckets=%d\n",
           set_add_entry_trace_count, key_hash,
           static_cast<unsigned>((*table).ptr()), table->NumberOfElements(),
           table->NumberOfDeletedElements(), table->NumberOfBuckets());
  }
  MaybeDirectHandle<OrderedHashSet> table_candidate =
      OrderedHashSet::Add(isolate, table, key);
  if (!table_candidate.ToHandle(&table)) {
    *out_result = roots.exception().ptr();
    return true;
  }
  if (trace_set_add_entry) {
    PrintF("WasmInterpreterEntryTrampoline: SetPrototypeAdd after Add "
           "count=%d table=0x%x elements=%d deleted=%d buckets=%d "
           "capacity=%d used=%d\n",
           set_add_entry_trace_count, static_cast<unsigned>((*table).ptr()),
           table->NumberOfElements(), table->NumberOfDeletedElements(),
           table->NumberOfBuckets(), table->Capacity(), table->UsedCapacity());
  }

  set->set_table(*table);
  *out_result = (*receiver).ptr();
  static int set_add_trace_count = 0;
  if (kTraceWasmFallbackDetails && set_add_trace_count < 24) {
    ++set_add_trace_count;
    if (set_add_trace_count == 1) {
      g_trace_after_collection_fallback_steps = 80;
    }
    PrintF("WasmInterpreterEntryTrampoline: fallback SetPrototypeAdd "
           "count=%d ",
           set_add_trace_count);
    DumpRuntimeArg("receiver", 0, (*receiver).ptr());
    PrintF(" ");
    DumpRuntimeArg("key", 0, (*key).ptr());
    PrintF(" result=0x%x\n",
           static_cast<unsigned>(*out_result));
  }
  return true;
}

bool TryCompareForArrayToSorted(Isolate* isolate,
                                DirectHandle<Object> comparefn,
                                DirectHandle<Object> left,
                                DirectHandle<Object> right,
                                bool* out_less) {
  ReadOnlyRoots roots(isolate);
  *out_less = false;

  if (IsUndefined(*left, roots)) {
    return true;
  }
  if (IsUndefined(*right, roots)) {
    *out_less = true;
    return true;
  }

  if (!IsUndefined(*comparefn, roots)) {
    DirectHandle<Object> compare_args[2] = {left, right};
    DirectHandle<Object> this_arg(roots.undefined_value(), isolate);
    WasmInterpreterStateSnapshot state(isolate);
    Address direct_result = roots.exception().ptr();
    bool direct_call = TryCallJSFunctionDirect(isolate, comparefn, this_arg, 2,
                                               compare_args, &direct_result);
    MaybeHandle<Object> maybe_result;
    DirectHandle<Object> compare_result;
    if (direct_call) {
      if (IsException(Tagged<Object>(direct_result), isolate)) {
        state.Restore();
        return false;
      }
      compare_result = direct_handle(Tagged<Object>(direct_result), isolate);
    } else {
      maybe_result = Execution::Call(
          isolate, comparefn, this_arg,
          ZoneVector<const DirectHandle<Object>>(compare_args, 2));
      if (!maybe_result.ToHandle(&compare_result)) {
        state.Restore();
        return false;
      }
    }
    state.Restore();

    DirectHandle<Number> number_result;
    if (!Object::ToNumber(isolate, compare_result).ToHandle(&number_result)) {
      return false;
    }
    double value = Object::NumberValue(*number_result);
    *out_less = value < 0;
    return true;
  }

  DirectHandle<String> left_string;
  if (!Object::ToString(isolate, left).ToHandle(&left_string)) return false;
  DirectHandle<String> right_string;
  if (!Object::ToString(isolate, right).ToHandle(&right_string)) return false;
  ComparisonResult result = String::Compare(isolate, left_string, right_string);
  *out_less = result == ComparisonResult::kLessThan;
  return true;
}

bool TryRunMapPrototypeSetBuiltin(Isolate* isolate,
                                  DirectHandle<Object> callable,
                                  DirectHandle<Object> receiver,
                                  int arg_count, DirectHandle<Object>* args,
                                  Address* out_result) {
  if (!IsJSFunctionBuiltin(isolate, callable, Builtin::kMapPrototypeSet)) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  if (!IsJSMap(*receiver)) {
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmInterpreterEntryTrampoline: MapPrototypeSet receiver "
             "invalid ");
      DumpRuntimeArg("receiver", 0, (*receiver).ptr());
      PrintF("\n");
    }
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<Object> key =
      arg_count > 0 ? args[0] : direct_handle(roots.undefined_value(), isolate);
  DirectHandle<Object> value =
      arg_count > 1 ? args[1] : direct_handle(roots.undefined_value(), isolate);
  key = NormalizeCollectionKey(isolate, key);

  DirectHandle<JSMap> map = Cast<JSMap>(receiver);
  Tagged<Object> raw_table = map->table();
  Handle<OrderedHashMap> table;
  if (!IsOrderedHashMap(raw_table) ||
      raw_table == roots.empty_ordered_hash_map()) {
    table = isolate->factory()->NewOrderedHashMap();
    map->set_table(*table);
  } else {
    table = handle(Cast<OrderedHashMap>(raw_table), isolate);
  }
  InternalIndex entry = table->FindEntry(isolate, *key);
  if (entry.is_found()) {
    table->SetEntry(entry, *key, *value);
  } else {
    MaybeHandle<OrderedHashMap> table_candidate =
        OrderedHashMap::Add(isolate, table, key, value);
    if (!table_candidate.ToHandle(&table)) {
      *out_result = roots.exception().ptr();
      return true;
    }
    map->set_table(*table);
  }

  *out_result = (*receiver).ptr();
  static int map_set_trace_count = 0;
  if (kTraceWasmFallbackDetails && map_set_trace_count < 24) {
    ++map_set_trace_count;
    if (map_set_trace_count == 1) {
      g_trace_after_collection_fallback_steps = 80;
    }
    PrintF("WasmInterpreterEntryTrampoline: fallback MapPrototypeSet "
           "count=%d ",
           map_set_trace_count);
    DumpRuntimeArg("receiver", 0, (*receiver).ptr());
    PrintF(" ");
    DumpRuntimeArg("key", 0, (*key).ptr());
    PrintF(" ");
    DumpRuntimeArg("value", 0, (*value).ptr());
    PrintF(" result=0x%x\n", static_cast<unsigned>(*out_result));
  }
  return true;
}

bool TryRunSetPrototypeHasBuiltin(Isolate* isolate,
                                  DirectHandle<Object> callable,
                                  DirectHandle<Object> receiver,
                                  int arg_count, DirectHandle<Object>* args,
                                  Address* out_result) {
  if (!IsJSFunctionBuiltin(isolate, callable, Builtin::kSetPrototypeHas)) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  if (!IsJSSet(*receiver)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<Object> key =
      arg_count > 0 ? args[0] : direct_handle(roots.undefined_value(), isolate);
  key = NormalizeCollectionKey(isolate, key);

  bool found = false;
  Tagged<Object> raw_table = Cast<JSSet>(*receiver)->table();
  if (IsOrderedHashSet(raw_table)) {
    Tagged<OrderedHashSet> table = Cast<OrderedHashSet>(raw_table);
    found = table->FindEntry(isolate, *key).is_found();
  }

  *out_result =
      found ? roots.true_value().ptr() : roots.false_value().ptr();
  if (kTraceWasmFallbackDetails) {
    PrintF("WasmInterpreterEntryTrampoline: fallback SetPrototypeHas "
           "found=%d result=0x%x\n",
           found, static_cast<unsigned>(*out_result));
  }
  return true;
}

bool TryRunMapPrototypeHasOrGetBuiltin(Isolate* isolate,
                                       DirectHandle<Object> callable,
                                       DirectHandle<Object> receiver,
                                       int arg_count,
                                       DirectHandle<Object>* args,
                                       Address* out_result) {
  bool is_has =
      IsJSFunctionBuiltin(isolate, callable, Builtin::kMapPrototypeHas);
  bool is_get =
      IsJSFunctionBuiltin(isolate, callable, Builtin::kMapPrototypeGet);
  if (!is_has && !is_get) return false;

  ReadOnlyRoots roots(isolate);
  if (!IsJSMap(*receiver)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<Object> key =
      arg_count > 0 ? args[0] : direct_handle(roots.undefined_value(), isolate);
  key = NormalizeCollectionKey(isolate, key);

  bool found = false;
  Tagged<Object> value = roots.undefined_value();
  Tagged<Object> raw_table = Cast<JSMap>(*receiver)->table();
  if (IsOrderedHashMap(raw_table)) {
    Tagged<OrderedHashMap> table = Cast<OrderedHashMap>(raw_table);
    InternalIndex entry = table->FindEntry(isolate, *key);
    found = entry.is_found();
    if (found && is_get) value = table->ValueAt(entry);
  }

  if (is_has) {
    *out_result =
        found ? roots.true_value().ptr() : roots.false_value().ptr();
  } else {
    *out_result = value.ptr();
  }
  if (kTraceWasmFallbackDetails) {
    PrintF("WasmInterpreterEntryTrampoline: fallback %s found=%d "
           "result=0x%x\n",
           is_has ? "MapPrototypeHas" : "MapPrototypeGet", found,
           static_cast<unsigned>(*out_result));
  }
  return true;
}

bool TryRunWeakCollectionSetBuiltin(Isolate* isolate,
                                    DirectHandle<Object> callable,
                                    DirectHandle<Object> receiver,
                                    int arg_count, DirectHandle<Object>* args,
                                    Address* out_result) {
  bool is_weak_map_set =
      IsJSFunctionBuiltin(isolate, callable, Builtin::kWeakMapPrototypeSet);
  bool is_weak_set_add =
      IsJSFunctionBuiltin(isolate, callable, Builtin::kWeakSetPrototypeAdd);
  if (!is_weak_map_set && !is_weak_set_add) return false;

  ReadOnlyRoots roots(isolate);
  if ((is_weak_map_set && !IsJSWeakMap(*receiver)) ||
      (is_weak_set_add && !IsJSWeakSet(*receiver))) {
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmInterpreterEntryTrampoline: WeakCollectionSet receiver "
             "invalid ");
      DumpRuntimeArg("receiver", 0, (*receiver).ptr());
      PrintF("\n");
    }
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<Object> key =
      arg_count > 0 ? args[0] : direct_handle(roots.undefined_value(), isolate);
  DirectHandle<Object> value =
      is_weak_map_set
          ? (arg_count > 1 ? args[1]
                           : direct_handle(roots.undefined_value(), isolate))
          : direct_handle(roots.true_value(), isolate);
  if (!Object::CanBeHeldWeakly(*key)) {
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmInterpreterEntryTrampoline: WeakCollectionSet key invalid ");
      DumpRuntimeArg("key", 0, (*key).ptr());
      PrintF("\n");
    }
    *out_result = roots.exception().ptr();
    return true;
  }

  int32_t hash = Object::GetOrCreateHash(*key, isolate).value();
  DirectHandle<JSWeakCollection> collection =
      Cast<JSWeakCollection>(receiver);
  JSWeakCollection::Set(collection, key, value, hash);

  *out_result = (*receiver).ptr();
  static int weak_set_trace_count = 0;
  if (kTraceWasmFallbackDetails && weak_set_trace_count < 24) {
    ++weak_set_trace_count;
    if (weak_set_trace_count == 1) {
      g_trace_after_collection_fallback_steps = 80;
    }
    PrintF("WasmInterpreterEntryTrampoline: fallback %s count=%d ",
           is_weak_map_set ? "WeakMapPrototypeSet" : "WeakSetPrototypeAdd",
           weak_set_trace_count);
    DumpRuntimeArg("receiver", 0, (*receiver).ptr());
    PrintF(" ");
    DumpRuntimeArg("key", 0, (*key).ptr());
    PrintF(" ");
    DumpRuntimeArg("value", 0, (*value).ptr());
    PrintF(" hash=%d result=0x%x\n", hash,
           static_cast<unsigned>(*out_result));
  }
  return true;
}

bool TryRunReflectOwnKeysBuiltin(Isolate* isolate,
                                 DirectHandle<Object> callable,
                                 int arg_count, DirectHandle<Object>* args,
                                 Address* out_result) {
  if (!IsJSFunction(*callable)) return false;

  Tagged<JSFunction> function = Cast<JSFunction>(*callable);
  Tagged<Code> code = function->code(isolate);
  if (!code->is_builtin() || code->builtin_id() != Builtin::kReflectOwnKeys) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  if (arg_count != 1 || !IsJSReceiver(*args[0])) {
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmInterpreterEntryTrampoline: ReflectOwnKeys target invalid");
      if (arg_count > 0) {
        DumpRuntimeArg("target", 0, (*args[0]).ptr());
      }
      PrintF("\n");
    }
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<JSReceiver> target =
      direct_handle(Cast<JSReceiver>(*args[0]), isolate);
  DirectHandle<FixedArray> keys;
  if (!KeyAccumulator::GetKeys(isolate, target, KeyCollectionMode::kOwnOnly,
                               ALL_PROPERTIES,
                               GetKeysConversion::kConvertToString)
           .ToHandle(&keys)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<JSArray> result =
      isolate->factory()->NewJSArrayWithElements(keys);
  *out_result = (*result).ptr();
  if (kTraceWasmFallbackDetails) {
    PrintF("WasmInterpreterEntryTrampoline: fallback ReflectOwnKeys "
           "result=0x%x\n",
           static_cast<unsigned>(*out_result));
  }
  return true;
}

bool TryRunReflectGetBuiltin(Isolate* isolate, DirectHandle<Object> callable,
                             int arg_count, DirectHandle<Object>* args,
                             Address* out_result) {
  if (!IsJSFunctionBuiltin(isolate, callable, Builtin::kReflectGet)) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  if (arg_count == 0 || !IsJSReceiver(*args[0])) {
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<JSReceiver> target = Cast<JSReceiver>(args[0]);
  DirectHandle<Object> key =
      arg_count > 1 ? args[1]
                    : direct_handle(roots.undefined_value(), isolate);
  DirectHandle<JSAny> receiver =
      arg_count > 2 ? Cast<JSAny>(args[2]) : target;
  DirectHandle<Name> name;
  if (!Object::ToName(isolate, key).ToHandle(&name)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  PropertyKey lookup_key(isolate, name);
  LookupIterator it(isolate, receiver, lookup_key, target);
  DirectHandle<Object> result;
  if (!Object::GetProperty(&it).ToHandle(&result)) {
    *out_result = roots.exception().ptr();
    return true;
  }
  *out_result = (*result).ptr();
  return true;
}

bool TryRunReflectGetPrototypeOfBuiltin(Isolate* isolate,
                                        DirectHandle<Object> callable,
                                        int arg_count,
                                        DirectHandle<Object>* args,
                                        Address* out_result) {
  if (!IsJSFunctionBuiltin(isolate, callable, Builtin::kReflectGetPrototypeOf)) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  if (arg_count != 1 || !IsJSReceiver(*args[0])) {
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmInterpreterEntryTrampoline: "
             "ReflectGetPrototypeOf target invalid");
      if (arg_count > 0) DumpRuntimeArg("target", 0, (*args[0]).ptr());
      PrintF("\n");
    }
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<JSReceiver> target =
      direct_handle(Cast<JSReceiver>(*args[0]), isolate);
  DirectHandle<JSPrototype> prototype;
  if (!JSReceiver::GetPrototype(isolate, target).ToHandle(&prototype)) {
    *out_result = roots.exception().ptr();
    return true;
  }
  *out_result = (*prototype).ptr();
  if (kTraceWasmFallbackDetails) {
    PrintF("WasmInterpreterEntryTrampoline: fallback ReflectGetPrototypeOf "
           "result=0x%x\n",
           static_cast<unsigned>(*out_result));
  }
  return true;
}

bool TryRunReflectGetOwnPropertyDescriptorBuiltin(
    Isolate* isolate, DirectHandle<Object> callable,
    int arg_count, DirectHandle<Object>* args, Address* out_result) {
  if (!IsJSFunctionBuiltin(isolate, callable,
                           Builtin::kReflectGetOwnPropertyDescriptor)) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  if (arg_count < 2 || !IsJSReceiver(*args[0])) {
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmInterpreterEntryTrampoline: "
             "ReflectGetOwnPropertyDescriptor target invalid");
      if (arg_count > 0) DumpRuntimeArg("target", 0, (*args[0]).ptr());
      PrintF("\n");
    }
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<JSReceiver> target =
      direct_handle(Cast<JSReceiver>(*args[0]), isolate);
  DirectHandle<Name> name;
  if (!Object::ToName(isolate, args[1]).ToHandle(&name)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  PropertyDescriptor desc;
  Maybe<bool> found =
      JSReceiver::GetOwnPropertyDescriptor(isolate, target, name, &desc);
  if (found.IsNothing()) {
    *out_result = roots.exception().ptr();
    return true;
  }
  if (!found.FromJust()) {
    *out_result = roots.undefined_value().ptr();
    return true;
  }

  DirectHandle<Object> result = desc.ToObject(isolate);
  *out_result = (*result).ptr();
  if (kTraceWasmFallbackDetails) {
    PrintF("WasmInterpreterEntryTrampoline: fallback "
           "ReflectGetOwnPropertyDescriptor result=0x%x\n",
           static_cast<unsigned>(*out_result));
  }
  return true;
}

bool TryRunArrayIteratorPrototypeNextBuiltin(Isolate* isolate,
                                             DirectHandle<Object> callable,
                                             DirectHandle<Object> receiver,
                                             Address* out_result) {
  if (!IsJSFunction(*callable)) return false;

  Tagged<JSFunction> function = Cast<JSFunction>(*callable);
  Tagged<Code> code = function->code(isolate);
  if (!code->is_builtin() ||
      code->builtin_id() != Builtin::kArrayIteratorPrototypeNext) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  if (!IsJSArrayIterator(*receiver)) {
    PrintF("WasmInterpreterEntryTrampoline: ArrayIterator.next receiver "
           "invalid ");
    DumpRuntimeArg("receiver", 0, (*receiver).ptr());
    PrintF("\n");
    *out_result = roots.exception().ptr();
    return true;
  }

  Tagged<JSArrayIterator> iterator = Cast<JSArrayIterator>(*receiver);
  Tagged<JSReceiver> iterated_object = iterator->iterated_object();
  Tagged<Number> next_index = iterator->next_index();

  uint32_t index = 0;
  if (!Object::ToArrayLength(next_index, &index)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  uint32_t length = 0;
  if (IsJSArray(iterated_object)) {
    if (!Object::ToArrayLength(Cast<JSArray>(iterated_object)->length(),
                               &length)) {
      *out_result = roots.exception().ptr();
      return true;
    }
  } else {
    DirectHandle<JSReceiver> object = direct_handle(iterated_object, isolate);
    DirectHandle<Object> length_key = isolate->factory()->length_string();
    DirectHandle<Object> length_object;
    if (!Runtime::GetObjectProperty(isolate, object, length_key)
             .ToHandle(&length_object) ||
        !Object::ToArrayLength(*length_object, &length)) {
      *out_result = roots.exception().ptr();
      return true;
    }
  }

  DirectHandle<Object> value = isolate->factory()->undefined_value();
  bool done = index >= length;
  if (done) {
    iterator->set_next_index(*isolate->factory()->NewNumber(kMaxUInt32));
  } else {
    iterator->set_next_index(*isolate->factory()->NewNumberFromUint(index + 1));
    switch (iterator->kind()) {
      case IterationKind::kKeys:
        value = isolate->factory()->NewNumberFromUint(index);
        break;
      case IterationKind::kValues:
        if (!JSReceiver::GetElement(isolate, direct_handle(iterated_object,
                                                           isolate),
                                    index)
                 .ToHandle(&value)) {
          *out_result = roots.exception().ptr();
          return true;
        }
        break;
      case IterationKind::kEntries: {
        DirectHandle<Object> element;
        if (!JSReceiver::GetElement(isolate, direct_handle(iterated_object,
                                                           isolate),
                                    index)
                 .ToHandle(&element)) {
          *out_result = roots.exception().ptr();
          return true;
        }
        DirectHandle<FixedArray> entry = isolate->factory()->NewFixedArray(2);
        entry->set(0, *isolate->factory()->NewNumberFromUint(index));
        entry->set(1, *element);
        value = isolate->factory()->NewJSArrayWithElements(entry);
        break;
      }
    }
  }

  DirectHandle<JSIteratorResult> result =
      isolate->factory()->NewJSIteratorResult(value, done);
  *out_result = (*result).ptr();
  static int array_iterator_next_trace_count = 0;
  if (kTraceWasmFallbackDetails &&
      (array_iterator_next_trace_count < 96 || done ||
       (index != 0 && (index % 256) == 0) ||
       (array_iterator_next_trace_count != 0 &&
        (array_iterator_next_trace_count % 2048) == 0))) {
    PrintF("WasmInterpreterEntryTrampoline: ArrayIterator.next trace "
           "count=%d index=%u length=%u done=%d kind=%d iter=0x%x "
           "next_index=0x%x value=0x%x result=0x%x\n",
           array_iterator_next_trace_count,
           index, length, done, static_cast<int>(iterator->kind()),
           static_cast<unsigned>((*receiver).ptr()),
           static_cast<unsigned>(iterator->next_index().ptr()),
           static_cast<unsigned>((*value).ptr()),
           static_cast<unsigned>(*out_result));
  }
  ++array_iterator_next_trace_count;
  if (kTraceWasmFallbackDetails) {
    PrintF("WasmInterpreterEntryTrampoline: fallback ArrayIterator.next "
           "done=%d result=0x%x\n",
           done, static_cast<unsigned>(*out_result));
  }
  return true;
}

bool TryRunMapIteratorPrototypeNextBuiltin(Isolate* isolate,
                                           DirectHandle<Object> callable,
                                           DirectHandle<Object> receiver,
                                           Address* out_result) {
  if (!IsJSFunctionBuiltin(isolate, callable,
                           Builtin::kMapIteratorPrototypeNext)) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  if (!IsJSMapIterator(*receiver)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<JSMapIterator> iterator = Cast<JSMapIterator>(receiver);
  bool done = !iterator->HasMore();
  DirectHandle<Object> value = isolate->factory()->undefined_value();
  if (!done) {
    DirectHandle<Object> key(iterator->CurrentKey(), isolate);
    DirectHandle<Object> current_value(iterator->CurrentValue(), isolate);
    InstanceType instance_type = iterator->map()->instance_type();
    if (instance_type == JS_MAP_KEY_ITERATOR_TYPE) {
      value = key;
    } else if (instance_type == JS_MAP_VALUE_ITERATOR_TYPE) {
      value = current_value;
    } else {
      DirectHandle<FixedArray> pair = isolate->factory()->NewFixedArray(2);
      pair->set(0, *key);
      pair->set(1, *current_value);
      value = isolate->factory()->NewJSArrayWithElements(pair);
    }
    iterator->MoveNext();
  }

  DirectHandle<JSIteratorResult> result =
      isolate->factory()->NewJSIteratorResult(value, done);
  *out_result = (*result).ptr();
  return true;
}

bool TryRunSetIteratorPrototypeNextBuiltin(Isolate* isolate,
                                           DirectHandle<Object> callable,
                                           DirectHandle<Object> receiver,
                                           Address* out_result) {
  if (!IsJSFunctionBuiltin(isolate, callable,
                           Builtin::kSetIteratorPrototypeNext)) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  if (!IsJSSetIterator(*receiver)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<JSSetIterator> iterator = Cast<JSSetIterator>(receiver);
  bool done = !iterator->HasMore();
  DirectHandle<Object> value = isolate->factory()->undefined_value();
  if (!done) {
    DirectHandle<Object> current_value(iterator->CurrentKey(), isolate);
    if (iterator->map()->instance_type() == JS_SET_KEY_VALUE_ITERATOR_TYPE) {
      DirectHandle<FixedArray> pair = isolate->factory()->NewFixedArray(2);
      pair->set(0, *current_value);
      pair->set(1, *current_value);
      value = isolate->factory()->NewJSArrayWithElements(pair);
    } else {
      value = current_value;
    }
    iterator->MoveNext();
  }

  DirectHandle<JSIteratorResult> result =
      isolate->factory()->NewJSIteratorResult(value, done);
  *out_result = (*result).ptr();
  return true;
}

bool TryRunObjectPrototypeHasOwnPropertyBuiltin(
    Isolate* isolate, DirectHandle<Object> callable,
    DirectHandle<Object> receiver, int arg_count,
    DirectHandle<Object>* args, Address* out_result) {
  if (!IsJSFunctionBuiltin(isolate, callable,
                           Builtin::kObjectPrototypeHasOwnProperty)) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  DirectHandle<JSReceiver> object;
  if (!Object::ToObject(isolate, receiver,
                        "Object.prototype.hasOwnProperty")
           .ToHandle(&object)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<Object> key =
      arg_count > 0 ? args[0]
                    : direct_handle(roots.undefined_value(), isolate);
  DirectHandle<Name> name;
  if (!Object::ToName(isolate, key).ToHandle(&name)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  Maybe<bool> result = JSReceiver::HasOwnProperty(isolate, object, name);
  if (result.IsNothing()) {
    *out_result = roots.exception().ptr();
    return true;
  }
  *out_result =
      (*isolate->factory()->ToBoolean(result.FromJust())).ptr();
  return true;
}

bool TryRunArrayConstructorBuiltin(Isolate* isolate,
                                   DirectHandle<Object> constructor,
                                   DirectHandle<Object> new_target,
                                   int arg_count,
                                   DirectHandle<Object>* args,
                                   Address* out_result);

bool TryRunCallBytecode(Isolate* isolate, Tagged<BytecodeArray> bytecode,
                        int bytecode_index,
                        interpreter::Bytecode bytecode_enum,
                        interpreter::OperandScale operand_scale,
                        Tagged<JSFunction> current_function,
                        Address* out_result,
                        PendingWasmJSCall* pending_call) {
  bool is_supported_call = false;
  bool receiver_is_implicit_undefined = false;
  bool uses_register_list = false;
  bool has_spread = false;
  int fixed_arg_count = 0;

  switch (bytecode_enum) {
    case interpreter::Bytecode::kCallAnyReceiver:
    case interpreter::Bytecode::kCallProperty:
      is_supported_call = true;
      uses_register_list = true;
      break;
    case interpreter::Bytecode::kCallProperty0:
      is_supported_call = true;
      fixed_arg_count = 0;
      break;
    case interpreter::Bytecode::kCallProperty1:
      is_supported_call = true;
      fixed_arg_count = 1;
      break;
    case interpreter::Bytecode::kCallProperty2:
      is_supported_call = true;
      fixed_arg_count = 2;
      break;
    case interpreter::Bytecode::kCallUndefinedReceiver:
      is_supported_call = true;
      receiver_is_implicit_undefined = true;
      uses_register_list = true;
      break;
    case interpreter::Bytecode::kCallUndefinedReceiver0:
      is_supported_call = true;
      receiver_is_implicit_undefined = true;
      fixed_arg_count = 0;
      break;
    case interpreter::Bytecode::kCallUndefinedReceiver1:
      is_supported_call = true;
      receiver_is_implicit_undefined = true;
      fixed_arg_count = 1;
      break;
    case interpreter::Bytecode::kCallUndefinedReceiver2:
      is_supported_call = true;
      receiver_is_implicit_undefined = true;
      fixed_arg_count = 2;
      break;
    case interpreter::Bytecode::kCallWithSpread:
      is_supported_call = true;
      uses_register_list = true;
      has_spread = true;
      break;
    default:
      break;
  }
  if (!is_supported_call) return false;

  ReadOnlyRoots roots(isolate);
  int call_source_position = bytecode->SourcePosition(bytecode_index);
  bool diagnostic_call = call_source_position >= 7531400 &&
                         call_source_position <= 7531800;
  bool trace_collection_call = g_trace_after_collection_fallback_steps > 0;
  bool trace_fs_utils_ownkeys_call =
      kTraceWasmFallbackDetails && bytecode->length() == 2762 &&
      bytecode_index >= 1230 && bytecode_index <= 1305;
  bool trace_eval_call =
#ifdef __wasi__
      kTraceWasmFallbackDetails &&
      FunctionMatchesWasmEvalTraceNeedle(
          Wasm32JSFunctionShared(current_function));
#else
      false;
#endif
  bool trace_bootstrap_realm_call =
#ifdef __wasi__
      kTraceWasmFallbackDetails &&
      FunctionScriptNameContainsAsciiForTrace(
          Wasm32JSFunctionShared(current_function),
          "node:internal/bootstrap/realm");
#else
      false;
#endif
  bool trace_call_details =
      trace_collection_call || trace_fs_utils_ownkeys_call || trace_eval_call ||
      trace_bootstrap_realm_call;
  if (trace_call_details) {
    PrintF("TryRunCallBytecode: enter bytecode=%s index=%d scale=%d\n",
           interpreter::Bytecodes::ToString(bytecode_enum), bytecode_index,
           static_cast<int>(operand_scale));
  }
  int32_t callable_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  Address callable_address = SafeTaggedOrUndefined(
      isolate, ReadInterpreterRegister(
                   interpreter::Register::FromOperand(callable_operand)));
#ifdef __wasi__
  diagnostic_call = diagnostic_call ||
                    (Wasm32JSFunctionShared(current_function)->StartPosition() ==
                         483118 &&
                     (bytecode_index == 2195 || bytecode_index == 2225 ||
                      bytecode_index == 2267)) ||
                    (Wasm32JSFunctionShared(current_function)->StartPosition() ==
                         0 &&
                     bytecode->length() > 200000 &&
                     bytecode_index == 135512);
#endif
#ifdef __wasi__
  if (bytecode_enum == interpreter::Bytecode::kCallUndefinedReceiver0 &&
      operand_scale == interpreter::OperandScale::kDouble) {
    static int wide_call0_trace_count = 0;
    if (wide_call0_trace_count < 128) {
      ++wide_call0_trace_count;
      PrintF("WASM32_WIDE_CALL0 #%d index=%d operand=%d callable=0x%x",
             wide_call0_trace_count, bytecode_index, callable_operand,
             static_cast<unsigned>(callable_address));
      if (IsSafeTaggedHandleValue(callable_address) &&
          IsJSFunction(Tagged<Object>(callable_address))) {
        Tagged<SharedFunctionInfo> call_shared = Wasm32JSFunctionShared(
            Cast<JSFunction>(Tagged<Object>(callable_address)));
        PrintF(" start=%d name=", call_shared->StartPosition());
        DumpNameForTrace(call_shared->Name());
      }
      PrintF("\n");
    }
  }
#endif
  if (IsSafeTaggedHandleValue(callable_address) &&
      IsJSFunction(Tagged<Object>(callable_address))) {
    diagnostic_call = diagnostic_call || SharedDebugNameEqualsAsciiForTrace(
                                             Wasm32JSFunctionShared(Cast<JSFunction>(
                                                 Tagged<Object>(callable_address))),
                                             "McQ");
  }
  if (diagnostic_call) {
    int target_start = -1;
    int target_compiled = -1;
    int target_builtin = -1;
    std::unique_ptr<char[]> target_name;
    if (IsSafeTaggedHandleValue(callable_address) &&
        IsJSFunction(Tagged<Object>(callable_address))) {
      Tagged<JSFunction> call_function =
          Cast<JSFunction>(Tagged<Object>(callable_address));
      Tagged<SharedFunctionInfo> call_shared =
          Wasm32JSFunctionShared(call_function);
      target_start = call_shared->StartPosition();
      target_compiled = call_function->is_compiled(isolate) ? 1 : 0;
      Tagged<Code> call_code = call_function->code(isolate);
      target_builtin = call_code->is_builtin()
                           ? static_cast<int>(call_code->builtin_id())
                           : -1;
      target_name = call_shared->DebugNameCStr();
    }
    std::fprintf(stderr,
                 "WASM32_CALL_DIAG_REAL source=%d index=%d opcode=%s "
                 "callable=0x%x target_start=%d target_name=%s "
                 "compiled=%d builtin=%d\n",
                 call_source_position, bytecode_index,
                 interpreter::Bytecodes::ToString(bytecode_enum),
                 static_cast<unsigned>(callable_address), target_start,
                 target_name ? target_name.get() : "<non-js-function>",
                 target_compiled, target_builtin);
    std::fflush(stderr);
    PrintF("WASM32_CALL_DIAG source=%d index=%d opcode=%s callable=0x%x",
           call_source_position, bytecode_index,
           interpreter::Bytecodes::ToString(bytecode_enum),
           static_cast<unsigned>(callable_address));
    if (IsSafeTaggedHandleValue(callable_address) &&
        IsJSFunction(Tagged<Object>(callable_address))) {
      Tagged<SharedFunctionInfo> call_shared = Wasm32JSFunctionShared(
          Cast<JSFunction>(Tagged<Object>(callable_address)));
      PrintF(" target_start=%d name=", call_shared->StartPosition());
      DumpNameForTrace(call_shared->Name());
    }
    PrintF("\n");
  }
  if (trace_call_details) {
    PrintF("TryRunCallBytecode: callable_operand=%d ", callable_operand);
    DumpRuntimeArg("callable", 0, callable_address);
    DumpInterpreterRegisterValue(
        "callable_reg", 0, interpreter::Register::FromOperand(callable_operand));
    if (IsSafeTaggedHandleValue(callable_address) &&
        IsJSFunction(Tagged<Object>(callable_address))) {
      Tagged<JSFunction> trace_function =
          Cast<JSFunction>(Tagged<Object>(callable_address));
      Tagged<Code> trace_code = trace_function->code(isolate);
      Tagged<SharedFunctionInfo> trace_shared =
          Wasm32JSFunctionShared(trace_function);
      PrintF(" jsfn code_builtin=%d builtin_id=%d builtin_name=%s sfi=0x%x name=",
             trace_code->is_builtin() ? 1 : 0,
             static_cast<int>(trace_code->builtin_id()),
             trace_code->is_builtin() ? Builtins::name(trace_code->builtin_id())
                                      : "<none>",
             static_cast<unsigned>(trace_shared.ptr()));
      DumpNameForTrace(trace_shared->Name());
    }
    PrintF("\n");
  }
  if (kTraceWasmCallBytecode) {
    interpreter::Register callable_reg =
        interpreter::Register::FromOperand(callable_operand);
    PrintF("WasmInterpreterEntryTrampoline: call detail bytecode=%s "
           "callable_operand=%d callable_index=%d ",
           interpreter::Bytecodes::ToString(bytecode_enum), callable_operand,
           callable_reg.index());
    DumpRuntimeArg("callable", 0, callable_address);
    PrintF("\n");
  }
  if (!IsCallable(Tagged<Object>(callable_address))) {
    if (bytecode_enum == interpreter::Bytecode::kCallUndefinedReceiver2) {
      int32_t regexp_operand =
          ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                    operand_scale);
      int32_t subject_operand =
          ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 2,
                                    operand_scale);
      Address regexp_address = SafeTaggedOrUndefined(
          isolate, ReadInterpreterRegister(
                       interpreter::Register::FromOperand(regexp_operand)));
      Address subject_address = SafeTaggedOrUndefined(
          isolate, ReadInterpreterRegister(
                       interpreter::Register::FromOperand(subject_operand)));
      if (TryRunMissingRegExpPrototypeExecCall(isolate, regexp_address,
                                               subject_address, out_result)) {
        return true;
      }
    }
    if (kTraceWasmCallBytecode) {
      PrintF("WasmInterpreterEntryTrampoline: call target is not callable "
             "bytecode=%s ",
             interpreter::Bytecodes::ToString(bytecode_enum));
      DumpRuntimeArg("target", 0, callable_address);
      PrintF("\n");
    }
    *out_result = roots.undefined_value().ptr();
    return true;
  }
  HandleScope scope(isolate);
  if (bytecode_enum == interpreter::Bytecode::kCallProperty1) {
    int32_t receiver_operand =
        ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                  operand_scale);
    int32_t arg_operand =
        ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 2,
                                  operand_scale);
    Address early_receiver_address = SafeTaggedOrUndefined(
        isolate, ReadInterpreterRegister(
                     interpreter::Register::FromOperand(receiver_operand)));
    Address early_arg_address = SafeTaggedOrUndefined(
        isolate, ReadInterpreterRegister(
                     interpreter::Register::FromOperand(arg_operand)));
    if (trace_call_details) {
      PrintF("TryRunCallBytecode: early receiver_operand=%d arg_operand=%d ",
             receiver_operand, arg_operand);
      DumpRuntimeArg("receiver", 0, early_receiver_address);
      PrintF(" ");
      DumpRuntimeArg("arg", 0, early_arg_address);
      PrintF("\n");
    }
    DirectHandle<Object> early_callable =
        direct_handle(Tagged<Object>(callable_address), isolate);
    DirectHandle<Object> early_receiver =
        direct_handle(Tagged<Object>(early_receiver_address), isolate);
    DirectHandle<Object> early_arg =
        direct_handle(Tagged<Object>(early_arg_address), isolate);
    DirectHandle<Object> undefined =
        direct_handle(roots.undefined_value(), isolate);

    Tagged<Context> saved_context = isolate->context();
    Address context_address = CurrentInterpreterContext();
    bool switched_context = false;
    if (IsSafeTaggedHandleValue(context_address) &&
        IsContext(Tagged<Object>(context_address))) {
      isolate->set_context(Cast<Context>(Tagged<Object>(context_address)));
      switched_context = true;
    }
    if (TryRunStringPrototypeSliceBuiltinWithArgs(
            isolate, early_callable, early_receiver, 1, early_arg, undefined,
            out_result)) {
      if (switched_context) isolate->set_context(saved_context);
      return true;
    }
    if (switched_context) isolate->set_context(saved_context);
  }

  DirectHandle<Object> args[kMaxWasmCallArgs];
  int arg_count = 0;
  Address receiver_address = roots.undefined_value().ptr();
  if (uses_register_list) {
    int32_t first_arg_operand =
        ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                  operand_scale);
    uint32_t reg_count =
        ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 2,
                                    operand_scale);
    if (kTraceWasmCallBytecode) {
      PrintF("WasmInterpreterEntryTrampoline: call reglist first_operand=%d "
             "reg_count=%u implicit_receiver=%d",
             first_arg_operand, reg_count, receiver_is_implicit_undefined);
    }
    if (!receiver_is_implicit_undefined) {
      if (reg_count == 0 || (has_spread && reg_count < 2)) {
        if (kTraceWasmCallBytecode) {
          PrintF("WasmInterpreterEntryTrampoline: call missing receiver "
                 "bytecode=%s\n",
                 interpreter::Bytecodes::ToString(bytecode_enum));
        }
        *out_result = roots.undefined_value().ptr();
        return true;
      }
      receiver_address = SafeTaggedOrUndefined(
          isolate, ReadInterpreterRegister(
                       RegisterFromListOperand(first_arg_operand, 0)));
      if (kTraceWasmCallBytecode) {
        DumpInterpreterRegisterValue(
            "receiver", 0, RegisterFromListOperand(first_arg_operand, 0));
      }
      for (uint32_t i = 1; i < reg_count; ++i) {
        if (kTraceWasmCallBytecode) {
          DumpInterpreterRegisterValue(
              "arg", static_cast<int>(i - 1),
              RegisterFromListOperand(first_arg_operand, i));
        }
        Address argument = ReadInterpreterRegister(
            RegisterFromListOperand(first_arg_operand, i));
        bool added = has_spread && i + 1 == reg_count
                         ? AddSpreadCallArguments(isolate, args, &arg_count,
                                                  argument)
                         : AddCallArgument(isolate, args, &arg_count, argument);
        if (!added) {
          *out_result = roots.exception().ptr();
          return true;
        }
      }
    } else {
      for (uint32_t i = 0; i < reg_count; ++i) {
        if (kTraceWasmCallBytecode) {
          DumpInterpreterRegisterValue(
              "arg", static_cast<int>(i),
              RegisterFromListOperand(first_arg_operand, i));
        }
        if (!AddCallArgument(
                isolate, args, &arg_count,
                ReadInterpreterRegister(
                    RegisterFromListOperand(first_arg_operand, i)))) {
          *out_result = roots.exception().ptr();
          return true;
        }
      }
    }
    if (kTraceWasmCallBytecode) PrintF("\n");
  } else {
    if (!receiver_is_implicit_undefined) {
      int32_t receiver_operand =
          ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                    operand_scale);
      receiver_address = SafeTaggedOrUndefined(
          isolate, ReadInterpreterRegister(
                       interpreter::Register::FromOperand(receiver_operand)));
      if (trace_call_details) {
        DumpInterpreterRegisterValue(
            "receiver_reg", 0,
            interpreter::Register::FromOperand(receiver_operand));
      }
      if (kTraceWasmCallBytecode) {
        DumpInterpreterRegisterValue(
            "receiver", 0, interpreter::Register::FromOperand(receiver_operand));
      }
    }
    for (int i = 0; i < fixed_arg_count; ++i) {
      int operand_index = receiver_is_implicit_undefined ? 1 + i : 2 + i;
      int32_t arg_operand =
          ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum,
                                    operand_index, operand_scale);
      if (kTraceWasmCallBytecode) {
        DumpInterpreterRegisterValue(
            "arg", i, interpreter::Register::FromOperand(arg_operand));
      }
      if (trace_call_details) {
        DumpInterpreterRegisterValue(
            "arg_reg", i, interpreter::Register::FromOperand(arg_operand));
      }
      if (!AddCallArgument(
              isolate, args, &arg_count,
              ReadInterpreterRegister(interpreter::Register::FromOperand(
                  arg_operand)))) {
        *out_result = roots.exception().ptr();
        return true;
      }
    }
    if (kTraceWasmCallBytecode) PrintF("\n");
  }
  if (trace_call_details) {
    PrintF("TryRunCallBytecode: decoded receiver=");
    DumpRuntimeArg("receiver", 0, receiver_address);
    PrintF(" arg_count=%d", arg_count);
    for (int i = 0; i < arg_count; ++i) {
      PrintF(" ");
      DumpRuntimeArg("arg", i, (*args[i]).ptr());
    }
    PrintF("\n");
  }
  if (arg_count > 0 && IsString(*args[0]) &&
      StringContainsAsciiForTrace(*args[0], "CLAUDE_MCQ callable")) {
    g_trace_after_collection_fallback_steps = 64;
  }

  DirectHandle<Object> callable =
      direct_handle(Tagged<Object>(callable_address), isolate);
  DirectHandle<Object> receiver =
      direct_handle(Tagged<Object>(receiver_address), isolate);
  Tagged<Context> saved_context = isolate->context();
  Address context_address = CurrentInterpreterContext();
  bool switched_context = false;
  if (IsSafeTaggedHandleValue(context_address) &&
      IsContext(Tagged<Object>(context_address))) {
    isolate->set_context(Cast<Context>(Tagged<Object>(context_address)));
    switched_context = true;
  }

  if (TryRunArrayConstructorBuiltin(isolate, callable, callable, arg_count,
                                    args, out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }

  if (TryRunObjectPrototypeHasOwnPropertyBuiltin(
          isolate, callable, receiver, arg_count, args, out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }

  if (TryRunArrayForEachBuiltin(isolate, callable, receiver, arg_count, args,
                                out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if (TryRunArraySliceBuiltin(isolate, callable, receiver, arg_count, args,
                              out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if (TryRunArrayFilterBuiltin(isolate, callable, receiver, arg_count, args,
                               out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if (TryRunArrayMapBuiltin(isolate, callable, receiver, arg_count, args,
                            out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if (TryRunArrayReduceBuiltin(isolate, callable, receiver, arg_count, args,
                               out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if (TryRunTypedArrayPrototypeSetBuiltin(
          isolate, callable, receiver, arg_count, args, out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if (trace_fs_utils_ownkeys_call) {
    PrintF("fsutils-ownkeys: before ReflectOwnKeys fallback ");
    DumpRuntimeArg("callable", 0, (*callable).ptr());
    if (arg_count > 0) DumpRuntimeArg("arg", 0, (*args[0]).ptr());
    PrintF("\n");
  }
  if (TryRunReflectOwnKeysBuiltin(isolate, callable, arg_count, args,
                                  out_result)) {
    if (trace_fs_utils_ownkeys_call) {
      PrintF("fsutils-ownkeys: ReflectOwnKeys fallback result ");
      DumpRuntimeArg("result", 0, *out_result);
      PrintF("\n");
    }
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if (TryRunReflectGetBuiltin(isolate, callable, arg_count, args, out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if (TryRunReflectGetPrototypeOfBuiltin(isolate, callable, arg_count, args,
                                         out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if (TryRunReflectGetOwnPropertyDescriptorBuiltin(
          isolate, callable, arg_count, args, out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if (TryRunArrayIteratorPrototypeNextBuiltin(isolate, callable, receiver,
                                              out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if (TryRunMapIteratorPrototypeNextBuiltin(isolate, callable, receiver,
                                            out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if (TryRunSetIteratorPrototypeNextBuiltin(isolate, callable, receiver,
                                            out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if (TryRunStringPrototypeToStringBuiltin(isolate, callable, receiver,
                                           out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if (TryRunStringPrototypeToUpperCaseBuiltin(isolate, callable, receiver,
                                              out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if (TryRunStringPrototypeSliceBuiltin(isolate, callable, receiver, arg_count,
                                        args, out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if (TryRunSetPrototypeHasBuiltin(isolate, callable, receiver, arg_count, args,
                                   out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if (TryRunMapPrototypeHasOrGetBuiltin(isolate, callable, receiver, arg_count,
                                        args, out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if (trace_collection_call) {
    PrintF("TryRunCallBytecode: before MapPrototypeSet/SetPrototypeAdd "
           "fallbacks\n");
  }
  if (TryRunMapPrototypeSetBuiltin(isolate, callable, receiver, arg_count, args,
                                   out_result)) {
    if (trace_collection_call) {
      PrintF("TryRunCallBytecode: MapPrototypeSet fallback returned 0x%x\n",
             static_cast<unsigned>(*out_result));
    }
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if (TryRunSetPrototypeAddBuiltin(isolate, callable, receiver, arg_count, args,
                                   out_result)) {
    if (trace_collection_call) {
      PrintF("TryRunCallBytecode: SetPrototypeAdd fallback returned 0x%x\n",
             static_cast<unsigned>(*out_result));
    }
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }
  if (TryRunWeakCollectionSetBuiltin(isolate, callable, receiver, arg_count,
                                     args, out_result)) {
    if (trace_collection_call) {
      PrintF("TryRunCallBytecode: WeakCollection fallback returned 0x%x\n",
             static_cast<unsigned>(*out_result));
    }
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }

  if (trace_call_details) {
    PrintF("TryRunCallBytecode: before direct/generic call\n");
  }
  if ((IsJSFunction(*callable) || IsJSBoundFunction(*callable)) &&
      arg_count <= kMaxWasmCallArgs) {
    pending_call->pending = true;
    pending_call->diagnostic = diagnostic_call;
    pending_call->source_position = call_source_position;
    pending_call->context = context_address;
    pending_call->callable = (*callable).ptr();
    pending_call->receiver = (*receiver).ptr();
    pending_call->arg_count = arg_count;
    for (int i = 0; i < arg_count; ++i) {
      pending_call->args[i] = (*args[i]).ptr();
    }
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }

  WasmInterpreterStateSnapshot state(isolate);
  Address result_address = roots.exception().ptr();
  bool generic_call_empty = false;
  DirectHandle<Object> result;
  MaybeHandle<Object> maybe_result = Execution::Call(
      isolate, callable, receiver,
      ZoneVector<const DirectHandle<Object>>(args, arg_count));
  if (maybe_result.ToHandle(&result)) {
    result_address = (*result).ptr();
  } else {
    generic_call_empty = true;
  }
  if (switched_context) isolate->set_context(saved_context);
  state.Restore();

  *out_result = result_address;
  if (trace_call_details) {
    PrintF("TryRunCallBytecode: direct/generic return direct=%d empty=%d "
           "has_exception=%d ",
           0, generic_call_empty ? 1 : 0,
           isolate->has_exception() ? 1 : 0);
    DumpRuntimeArg("result", 0, result_address);
    PrintF("\n");
  }
  return true;
}

bool TryRunCollectionConstructorBuiltin(Isolate* isolate,
                                        DirectHandle<Object> constructor,
                                        DirectHandle<Object> new_target,
                                        int arg_count,
                                        DirectHandle<Object>* args,
                                        Address* out_result) {
  bool is_set =
      IsJSFunctionBuiltin(isolate, constructor, Builtin::kSetConstructor);
  bool is_map =
      IsJSFunctionBuiltin(isolate, constructor, Builtin::kMapConstructor);
  bool is_weak_map =
      IsJSFunctionBuiltin(isolate, constructor, Builtin::kWeakMapConstructor);
  bool is_weak_set =
      IsJSFunctionBuiltin(isolate, constructor, Builtin::kWeakSetConstructor);
  if (!is_set && !is_map && !is_weak_map && !is_weak_set) return false;

  ReadOnlyRoots roots(isolate);
  if (!IsJSFunction(*constructor) || !IsJSReceiver(*new_target)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<JSFunction> ctor = Cast<JSFunction>(constructor);
  DirectHandle<JSReceiver> new_target_receiver = Cast<JSReceiver>(new_target);
  DirectHandle<JSObject> instance;
  if (!JSObject::New(ctor, new_target_receiver, {}).ToHandle(&instance)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<Object> iterable =
      arg_count > 0 ? args[0] : direct_handle(roots.undefined_value(), isolate);

  if (is_set) {
    if (!IsJSSet(*instance)) {
      *out_result = roots.exception().ptr();
      return true;
    }
    DirectHandle<JSSet> set = Cast<JSSet>(instance);
    JSSet::Initialize(set, isolate);
    if (IsUndefined(*iterable, roots) || IsNull(*iterable, roots)) {
      *out_result = (*set).ptr();
      return true;
    }

    DirectHandle<JSReceiver> iterable_object;
    if (!Object::ToObject(isolate, iterable, "Set constructor")
             .ToHandle(&iterable_object)) {
      *out_result = roots.exception().ptr();
      return true;
    }
    DirectHandle<Object> length_object;
    if (!Object::GetLengthFromArrayLike(isolate, iterable_object)
             .ToHandle(&length_object)) {
      *out_result = roots.exception().ptr();
      return true;
    }
    double raw_length = Object::NumberValue(*length_object);
    if (raw_length < 0 || raw_length > FixedArray::kMaxLength) {
      *out_result = roots.exception().ptr();
      return true;
    }

    Handle<OrderedHashSet> table =
        handle(Cast<OrderedHashSet>(set->table()), isolate);
    uint32_t length = static_cast<uint32_t>(raw_length);
    for (uint32_t index = 0; index < length; ++index) {
      Maybe<bool> maybe_has_element =
          JSReceiver::HasElement(isolate, iterable_object, index);
      if (maybe_has_element.IsNothing()) {
        *out_result = roots.exception().ptr();
        return true;
      }
      if (!maybe_has_element.FromJust()) continue;

      DirectHandle<Object> key;
      if (!JSReceiver::GetElement(isolate, iterable_object, index)
               .ToHandle(&key)) {
        *out_result = roots.exception().ptr();
        return true;
      }
      key = NormalizeCollectionKey(isolate, key);

      MaybeHandle<OrderedHashSet> table_candidate =
          OrderedHashSet::Add(isolate, table, key);
      if (!table_candidate.ToHandle(&table)) {
        *out_result = roots.exception().ptr();
        return true;
      }
      set->set_table(*table);
    }
    *out_result = (*set).ptr();
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmInterpreterEntryTrampoline: fallback SetConstructor "
             "length=%u result=0x%x\n",
             length, static_cast<unsigned>(*out_result));
    }
    return true;
  }

  if (is_map) {
    if (!IsJSMap(*instance)) {
      *out_result = roots.exception().ptr();
      return true;
    }
    DirectHandle<JSMap> map = Cast<JSMap>(instance);
    JSMap::Initialize(map, isolate);
    if (IsUndefined(*iterable, roots) || IsNull(*iterable, roots)) {
      *out_result = (*map).ptr();
      return true;
    }

    DirectHandle<JSReceiver> iterable_object;
    if (!Object::ToObject(isolate, iterable, "Map constructor")
             .ToHandle(&iterable_object)) {
      *out_result = roots.exception().ptr();
      return true;
    }
    DirectHandle<Object> length_object;
    if (!Object::GetLengthFromArrayLike(isolate, iterable_object)
             .ToHandle(&length_object)) {
      *out_result = roots.exception().ptr();
      return true;
    }
    double raw_length = Object::NumberValue(*length_object);
    if (raw_length < 0 || raw_length > FixedArray::kMaxLength) {
      *out_result = roots.exception().ptr();
      return true;
    }

    Handle<OrderedHashMap> table =
        handle(Cast<OrderedHashMap>(map->table()), isolate);
    uint32_t length = static_cast<uint32_t>(raw_length);
    for (uint32_t index = 0; index < length; ++index) {
      Maybe<bool> maybe_has_element =
          JSReceiver::HasElement(isolate, iterable_object, index);
      if (maybe_has_element.IsNothing()) {
        *out_result = roots.exception().ptr();
        return true;
      }
      if (!maybe_has_element.FromJust()) continue;

      DirectHandle<Object> entry_object;
      if (!JSReceiver::GetElement(isolate, iterable_object, index)
               .ToHandle(&entry_object)) {
        *out_result = roots.exception().ptr();
        return true;
      }
      if (!IsJSReceiver(*entry_object)) {
        *out_result = roots.exception().ptr();
        return true;
      }
      DirectHandle<JSReceiver> entry = Cast<JSReceiver>(entry_object);
      DirectHandle<Object> key;
      DirectHandle<Object> value;
      if (!JSReceiver::GetElement(isolate, entry, 0).ToHandle(&key) ||
          !JSReceiver::GetElement(isolate, entry, 1).ToHandle(&value)) {
        *out_result = roots.exception().ptr();
        return true;
      }
      key = NormalizeCollectionKey(isolate, key);

      InternalIndex existing = table->FindEntry(isolate, *key);
      if (existing.is_found()) {
        table->SetEntry(existing, *key, *value);
      } else {
        MaybeHandle<OrderedHashMap> table_candidate =
            OrderedHashMap::Add(isolate, table, key, value);
        if (!table_candidate.ToHandle(&table)) {
          *out_result = roots.exception().ptr();
          return true;
        }
        map->set_table(*table);
      }
    }
    *out_result = (*map).ptr();
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmInterpreterEntryTrampoline: fallback MapConstructor "
             "length=%u result=0x%x\n",
             length, static_cast<unsigned>(*out_result));
    }
    return true;
  }

  if ((is_weak_map && !IsJSWeakMap(*instance)) ||
      (is_weak_set && !IsJSWeakSet(*instance))) {
    *out_result = roots.exception().ptr();
    return true;
  }
  DirectHandle<JSWeakCollection> weak_collection =
      Cast<JSWeakCollection>(instance);
  JSWeakCollection::Initialize(weak_collection, isolate);
  if (IsUndefined(*iterable, roots) || IsNull(*iterable, roots)) {
    *out_result = (*weak_collection).ptr();
    return true;
  }

  DirectHandle<JSReceiver> iterable_object;
  if (!Object::ToObject(isolate, iterable,
                        is_weak_map ? "WeakMap constructor"
                                    : "WeakSet constructor")
           .ToHandle(&iterable_object)) {
    *out_result = roots.exception().ptr();
    return true;
  }
  DirectHandle<Object> length_object;
  if (!Object::GetLengthFromArrayLike(isolate, iterable_object)
           .ToHandle(&length_object)) {
    *out_result = roots.exception().ptr();
    return true;
  }
  double raw_length = Object::NumberValue(*length_object);
  if (raw_length < 0 || raw_length > FixedArray::kMaxLength) {
    *out_result = roots.exception().ptr();
    return true;
  }

  uint32_t length = static_cast<uint32_t>(raw_length);
  for (uint32_t index = 0; index < length; ++index) {
    Maybe<bool> maybe_has_element =
        JSReceiver::HasElement(isolate, iterable_object, index);
    if (maybe_has_element.IsNothing()) {
      *out_result = roots.exception().ptr();
      return true;
    }
    if (!maybe_has_element.FromJust()) continue;

    DirectHandle<Object> key;
    DirectHandle<Object> value;
    if (is_weak_map) {
      DirectHandle<Object> entry_object;
      if (!JSReceiver::GetElement(isolate, iterable_object, index)
               .ToHandle(&entry_object) ||
          !IsJSReceiver(*entry_object)) {
        *out_result = roots.exception().ptr();
        return true;
      }
      DirectHandle<JSReceiver> entry = Cast<JSReceiver>(entry_object);
      if (!JSReceiver::GetElement(isolate, entry, 0).ToHandle(&key) ||
          !JSReceiver::GetElement(isolate, entry, 1).ToHandle(&value)) {
        *out_result = roots.exception().ptr();
        return true;
      }
    } else {
      if (!JSReceiver::GetElement(isolate, iterable_object, index)
               .ToHandle(&key)) {
        *out_result = roots.exception().ptr();
        return true;
      }
      value = direct_handle(roots.true_value(), isolate);
    }

    if (!Object::CanBeHeldWeakly(*key)) {
      *out_result = roots.exception().ptr();
      return true;
    }
    int32_t hash = Object::GetOrCreateHash(*key, isolate).value();
    JSWeakCollection::Set(weak_collection, key, value, hash);
  }
  *out_result = (*weak_collection).ptr();
  if (kTraceWasmFallbackDetails) {
    PrintF("WasmInterpreterEntryTrampoline: fallback %s length=%u "
           "result=0x%x\n",
           is_weak_map ? "WeakMapConstructor" : "WeakSetConstructor", length,
           static_cast<unsigned>(*out_result));
  }
  return true;
}

bool TryRunProxyConstructorBuiltin(Isolate* isolate,
                                   DirectHandle<Object> constructor,
                                   DirectHandle<Object> new_target,
                                   int arg_count,
                                   DirectHandle<Object>* args,
                                   Address* out_result) {
  if (!IsJSFunctionBuiltin(isolate, constructor, Builtin::kProxyConstructor)) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  if (IsUndefined(*new_target, roots)) {
    isolate->Throw(*isolate->factory()->NewTypeError(
        MessageTemplate::kConstructorNotFunction,
        isolate->factory()->Proxy_string()));
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<Object> target =
      arg_count > 0 ? args[0] : direct_handle(roots.undefined_value(), isolate);
  DirectHandle<Object> handler =
      arg_count > 1 ? args[1] : direct_handle(roots.undefined_value(), isolate);

  DirectHandle<JSProxy> proxy;
  if (!JSProxy::New(isolate, target, handler).ToHandle(&proxy)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  *out_result = (*proxy).ptr();
  if (kTraceWasmFallbackDetails) {
    PrintF("WasmInterpreterEntryTrampoline: fallback ProxyConstructor "
           "result=0x%x\n",
           static_cast<unsigned>(*out_result));
  }
  return true;
}

bool TryRunRegExpConstructorBuiltin(Isolate* isolate,
                                    DirectHandle<Object> constructor,
                                    DirectHandle<Object> new_target,
                                    int arg_count,
                                    DirectHandle<Object>* args,
                                    Address* out_result) {
  if (!IsJSFunctionBuiltin(isolate, constructor,
                           Builtin::kRegExpConstructor)) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  if (!IsJSFunction(*constructor) || !IsJSReceiver(*new_target)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<Object> pattern =
      arg_count > 0 ? args[0] : direct_handle(roots.undefined_value(), isolate);
  DirectHandle<Object> flags =
      arg_count > 1 ? args[1] : direct_handle(roots.undefined_value(), isolate);

  DirectHandle<String> source;
  DirectHandle<String> flags_string;
  if (IsJSRegExp(*pattern)) {
    DirectHandle<JSRegExp> pattern_regexp = Cast<JSRegExp>(pattern);
    source = direct_handle(pattern_regexp->source(), isolate);
    if (IsUndefined(*flags, roots)) {
      flags_string = JSRegExp::StringFromFlags(isolate, pattern_regexp->flags());
    }
  }

  if (source.is_null()) {
    if (IsUndefined(*pattern, roots)) {
      source = isolate->factory()->empty_string();
    } else if (!Object::ToString(isolate, pattern).ToHandle(&source)) {
      *out_result = roots.exception().ptr();
      return true;
    }
  }

  if (flags_string.is_null()) {
    if (IsUndefined(*flags, roots)) {
      flags_string = isolate->factory()->empty_string();
    } else if (!Object::ToString(isolate, flags).ToHandle(&flags_string)) {
      *out_result = roots.exception().ptr();
      return true;
    }
  }

  DirectHandle<JSFunction> ctor = Cast<JSFunction>(constructor);
  DirectHandle<JSReceiver> new_target_receiver = Cast<JSReceiver>(new_target);
  DirectHandle<JSObject> instance;
  if (!JSObject::New(ctor, new_target_receiver, {}).ToHandle(&instance) ||
      !IsJSRegExp(*instance)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<JSRegExp> regexp = Cast<JSRegExp>(instance);
  regexp->clear_data();
  if (!JSRegExp::Initialize(regexp, source, flags_string).ToHandle(&regexp)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  *out_result = (*regexp).ptr();
  if (kTraceWasmFallbackDetails) {
    PrintF("WasmInterpreterEntryTrampoline: fallback RegExpConstructor "
           "result=0x%x\n",
           static_cast<unsigned>(*out_result));
  }
  return true;
}

bool TryRunArrayConstructorBuiltin(Isolate* isolate,
                                   DirectHandle<Object> constructor,
                                   DirectHandle<Object> new_target,
                                   int arg_count,
                                   DirectHandle<Object>* args,
                                   Address* out_result) {
  if (!IsJSFunctionBuiltin(isolate, constructor, Builtin::kArrayConstructor) &&
      !IsJSFunctionBuiltin(isolate, constructor,
                           Builtin::kArrayConstructorImpl)) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  if (!IsJSFunction(*constructor) || !IsJSReceiver(*new_target)) {
    *out_result = roots.exception().ptr();
    return true;
  }

  WasmGCStateScope gc_state(isolate);
  SetCurrentIsolateScope current_isolate_scope(isolate);

  Handle<JSArray> array;
  if (arg_count == 0) {
    array = isolate->factory()->NewJSArray(0);
  } else if (arg_count == 1 && IsNumber(*args[0])) {
    uint32_t length = 0;
    if (!Object::ToArrayLength(*args[0], &length)) {
      isolate->Throw(*isolate->factory()->NewRangeError(
          MessageTemplate::kInvalidArrayLength));
      *out_result = roots.exception().ptr();
      return true;
    }

    if (length <= JSArray::kMaxFastArrayLength) {
      int fast_length = static_cast<int>(length);
      array = isolate->factory()->NewJSArray(
          HOLEY_ELEMENTS, fast_length, fast_length,
          ArrayStorageAllocationMode::INITIALIZE_ARRAY_ELEMENTS_WITH_HOLE);
    } else {
      array = isolate->factory()->NewJSArray(0);
      if (JSArray::SetLength(isolate, array, length).IsNothing()) {
        *out_result = roots.exception().ptr();
        return true;
      }
    }
  } else {
    DirectHandle<FixedArray> elements =
        isolate->factory()->NewFixedArray(arg_count);
    for (int index = 0; index < arg_count; ++index) {
      elements->set(index, *args[index]);
    }
    array = isolate->factory()->NewJSArrayWithElements(elements, PACKED_ELEMENTS,
                                                       arg_count);
  }

  *out_result = (*array).ptr();
  if (kTraceWasmFallbackDetails) {
    PrintF("WasmInterpreterEntryTrampoline: fallback ArrayConstructor "
           "argc=%d result=0x%x\n",
           arg_count, static_cast<unsigned>(*out_result));
  }
  return true;
}

bool TryGetWasm32TypedArrayElementsKind(Tagged<JSFunction> function,
                                        ElementsKind* out_kind) {
  Tagged<NativeContext> context = function->native_context();
#define MATCH_TYPED_ARRAY(accessor, kind) \
  if (function == context->accessor()) {   \
    *out_kind = kind;                      \
    return true;                           \
  }
  MATCH_TYPED_ARRAY(uint8_array_fun, UINT8_ELEMENTS)
  MATCH_TYPED_ARRAY(int8_array_fun, INT8_ELEMENTS)
  MATCH_TYPED_ARRAY(uint16_array_fun, UINT16_ELEMENTS)
  MATCH_TYPED_ARRAY(int16_array_fun, INT16_ELEMENTS)
  MATCH_TYPED_ARRAY(uint32_array_fun, UINT32_ELEMENTS)
  MATCH_TYPED_ARRAY(int32_array_fun, INT32_ELEMENTS)
  MATCH_TYPED_ARRAY(biguint64_array_fun, BIGUINT64_ELEMENTS)
  MATCH_TYPED_ARRAY(bigint64_array_fun, BIGINT64_ELEMENTS)
  MATCH_TYPED_ARRAY(uint8_clamped_array_fun, UINT8_CLAMPED_ELEMENTS)
  MATCH_TYPED_ARRAY(float32_array_fun, FLOAT32_ELEMENTS)
  MATCH_TYPED_ARRAY(float64_array_fun, FLOAT64_ELEMENTS)
  MATCH_TYPED_ARRAY(float16_array_fun, FLOAT16_ELEMENTS)
#undef MATCH_TYPED_ARRAY

  Tagged<Object> name_object = function->shared()->Name();
  if (!IsString(name_object)) return false;
  Tagged<String> name = Cast<String>(name_object);
#define MATCH_TYPED_ARRAY_NAME(literal, kind)                   \
  do {                                                          \
    constexpr char expected[] = literal;                        \
    constexpr int expected_length = sizeof(expected) - 1;       \
    if (name->length() == expected_length) {                     \
      bool matches = true;                                      \
      for (int i = 0; i < expected_length; ++i) {                \
        if (name->Get(i) != static_cast<uint16_t>(expected[i])) {\
          matches = false;                                      \
          break;                                                \
        }                                                       \
      }                                                         \
      if (matches) {                                            \
        *out_kind = kind;                                       \
        return true;                                            \
      }                                                         \
    }                                                           \
  } while (false)
  MATCH_TYPED_ARRAY_NAME("Uint8Array", UINT8_ELEMENTS);
  MATCH_TYPED_ARRAY_NAME("Int8Array", INT8_ELEMENTS);
  MATCH_TYPED_ARRAY_NAME("Uint16Array", UINT16_ELEMENTS);
  MATCH_TYPED_ARRAY_NAME("Int16Array", INT16_ELEMENTS);
  MATCH_TYPED_ARRAY_NAME("Uint32Array", UINT32_ELEMENTS);
  MATCH_TYPED_ARRAY_NAME("Int32Array", INT32_ELEMENTS);
  MATCH_TYPED_ARRAY_NAME("BigUint64Array", BIGUINT64_ELEMENTS);
  MATCH_TYPED_ARRAY_NAME("BigInt64Array", BIGINT64_ELEMENTS);
  MATCH_TYPED_ARRAY_NAME("Uint8ClampedArray", UINT8_CLAMPED_ELEMENTS);
  MATCH_TYPED_ARRAY_NAME("Float32Array", FLOAT32_ELEMENTS);
  MATCH_TYPED_ARRAY_NAME("Float64Array", FLOAT64_ELEMENTS);
  MATCH_TYPED_ARRAY_NAME("Float16Array", FLOAT16_ELEMENTS);
#undef MATCH_TYPED_ARRAY_NAME
  return false;
}

bool TryRunTypedArrayConstructorBuiltin(Isolate* isolate,
                                        DirectHandle<Object> constructor,
                                        DirectHandle<Object> new_target,
                                        int arg_count,
                                        DirectHandle<Object>* args,
                                        Address* out_result) {
  if (!IsJSFunction(*constructor) || !IsJSReceiver(*new_target)) {
    return false;
  }
  DirectHandle<JSFunction> function = Cast<JSFunction>(constructor);
  ElementsKind elements_kind;
  if (!TryGetWasm32TypedArrayElementsKind(*function, &elements_kind)) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  DirectHandle<Object> source =
      arg_count > 0 ? args[0] : direct_handle(roots.undefined_value(), isolate);

  ExternalArrayType array_type;
  size_t element_size = 0;
  Factory::TypeAndSizeForElementsKind(elements_kind, &array_type,
                                      &element_size);
  size_t length = 0;
  size_t byte_offset = 0;
  bool copy_from_source = false;
  DirectHandle<JSArrayBuffer> buffer;
  if (IsUndefined(*source, isolate)) {
    length = 0;
  } else if (IsNumber(*source)) {
    if (!Object::ToIntegerIndex(*source, &length)) return false;
  } else if (IsJSArrayBuffer(*source)) {
    buffer = Cast<JSArrayBuffer>(source);
    if (arg_count > 1 && !IsUndefined(*args[1], isolate) &&
        !Object::ToIntegerIndex(*args[1], &byte_offset)) {
      *out_result = roots.exception().ptr();
      return true;
    }
    size_t buffer_byte_length = buffer->byte_length();
    if (byte_offset > buffer_byte_length ||
        byte_offset % element_size != 0) {
      *out_result = roots.exception().ptr();
      return true;
    }
    if (arg_count > 2 && !IsUndefined(*args[2], isolate)) {
      if (!Object::ToIntegerIndex(*args[2], &length) ||
          length > (buffer_byte_length - byte_offset) / element_size) {
        *out_result = roots.exception().ptr();
        return true;
      }
    } else {
      size_t remaining = buffer_byte_length - byte_offset;
      if (remaining % element_size != 0) {
        *out_result = roots.exception().ptr();
        return true;
      }
      length = remaining / element_size;
    }
  } else if (IsJSReceiver(*source)) {
    DirectHandle<Object> length_object;
    if (!Object::GetLengthFromArrayLike(isolate, Cast<JSReceiver>(source))
             .ToHandle(&length_object)) {
      *out_result = roots.exception().ptr();
      return true;
    }
    if (!Object::ToIntegerIndex(*length_object, &length)) return false;
    copy_from_source = true;
  } else {
    return false;
  }

  if (element_size == 0 || length > JSTypedArray::kMaxByteLength / element_size ||
      length > static_cast<size_t>(kMaxInt)) {
    *out_result = roots.exception().ptr();
    return true;
  }
  size_t byte_length = length * element_size;

  WasmGCStateScope gc_state(isolate);
  SetCurrentIsolateScope current_isolate_scope(isolate);

  if (buffer.is_null()) {
    if (!isolate->factory()
             ->NewJSArrayBufferAndBackingStore(
                 byte_length, InitializedFlag::kZeroInitialized)
             .ToHandle(&buffer)) {
      *out_result = roots.exception().ptr();
      return true;
    }
  }
  DirectHandle<JSTypedArray> typed_array =
      isolate->factory()->NewJSTypedArray(array_type, buffer, byte_offset,
                                          length);

  if (IsJSFunction(*new_target) && *new_target != *constructor) {
    DirectHandle<JSFunction> derived = Cast<JSFunction>(new_target);
    Address prototype_address =
        Wasm32JSFunctionPrototypeAddress(isolate, derived);
    if (IsSafeTaggedHandleValue(prototype_address) &&
        IsJSReceiver(Tagged<Object>(prototype_address)) &&
        JSObject::SetPrototype(
            isolate, typed_array,
            direct_handle(Tagged<Object>(prototype_address), isolate), false,
            kDontThrow)
            .IsNothing()) {
      *out_result = roots.exception().ptr();
      return true;
    }
  }

  if (copy_from_source && length > 0) {
    Address copy_args[3] = {
        Smi::FromInt(static_cast<int>(length)).ptr(),
        (*source).ptr(),
        (*typed_array).ptr(),
    };
    WasmTemporaryRootScope copy_roots(isolate, copy_args, 3);
    Address copy_result =
        Runtime_TypedArrayCopyElements(3, &copy_roots.data()[2], isolate);
    if (isolate->has_exception()) {
      *out_result = roots.exception().ptr();
      return true;
    }
  }

  *out_result = (*typed_array).ptr();
  if (kTraceWasmFallbackDetails) {
    PrintF("WasmInterpreterEntryTrampoline: fallback TypedArrayConstructor "
           "kind=%d length=%zu copy=%d result=0x%x\n",
           static_cast<int>(elements_kind), length, copy_from_source,
           static_cast<unsigned>(*out_result));
  }
  return true;
}

bool TryRunConstructBytecode(Isolate* isolate, Tagged<BytecodeArray> bytecode,
                             int bytecode_index,
                             interpreter::Bytecode bytecode_enum,
                             interpreter::OperandScale operand_scale,
                             Address* out_result) {
  bool forward_all_args =
      bytecode_enum == interpreter::Bytecode::kConstructForwardAllArgs;
  bool has_spread =
      bytecode_enum == interpreter::Bytecode::kConstructWithSpread;
  if (bytecode_enum != interpreter::Bytecode::kConstruct &&
      !forward_all_args && !has_spread) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  int32_t constructor_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  Address constructor_address = SafeTaggedOrUndefined(
      isolate, ReadInterpreterRegister(
                   interpreter::Register::FromOperand(constructor_operand)));
  Address new_target_address = SafeTaggedOrUndefined(
      isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
  if (!IsConstructor(Tagged<Object>(constructor_address)) ||
      !IsConstructor(Tagged<Object>(new_target_address))) {
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<Object> args[kMaxWasmCallArgs];
  int arg_count = 0;
  int32_t first_arg_operand = 0;
  uint32_t reg_count = 0;
  if (forward_all_args) {
    int argc_with_receiver = static_cast<int>(
        g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
            StandardFrameConstants::kArgCOffset)]);
    reg_count = static_cast<uint32_t>(
        argc_with_receiver > kJSArgcReceiverSlots
            ? argc_with_receiver - kJSArgcReceiverSlots
            : 0);
  } else {
    first_arg_operand =
        ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                  operand_scale);
    reg_count =
        ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 2,
                                    operand_scale);
  }
  if (reg_count > kMaxWasmCallArgs) {
    *out_result = roots.exception().ptr();
    return true;
  }
  if (has_spread && reg_count == 0) {
    *out_result = roots.exception().ptr();
    return true;
  }
  for (uint32_t i = 0; i < reg_count; ++i) {
    Address argument =
        forward_all_args
            ? ReadInterpreterRegister(
                  interpreter::Register::FromParameterIndex(1 + i))
            : ReadInterpreterRegister(
                  RegisterFromListOperand(first_arg_operand, i));
    bool added = has_spread && i + 1 == reg_count
                     ? AddSpreadCallArguments(isolate, args, &arg_count,
                                              argument)
                     : AddCallArgument(isolate, args, &arg_count, argument);
    if (!added) {
      *out_result = roots.exception().ptr();
      return true;
    }
  }

  DirectHandle<Object> constructor =
      direct_handle(Tagged<Object>(constructor_address), isolate);
  DirectHandle<Object> new_target =
      direct_handle(Tagged<Object>(new_target_address), isolate);
  Tagged<Context> saved_context = isolate->context();
  Address context_address = CurrentInterpreterContext();
  bool switched_context = false;
  if (IsSafeTaggedHandleValue(context_address) &&
      IsContext(Tagged<Object>(context_address))) {
    isolate->set_context(Cast<Context>(Tagged<Object>(context_address)));
    switched_context = true;
  }

  if (TryRunArrayConstructorBuiltin(isolate, constructor, new_target, arg_count,
                                    args, out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }

  if (TryRunTypedArrayConstructorBuiltin(isolate, constructor, new_target,
                                         arg_count, args, out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }

  if (TryRunRegExpConstructorBuiltin(isolate, constructor, new_target,
                                     arg_count, args, out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }

  if (TryRunProxyConstructorBuiltin(isolate, constructor, new_target, arg_count,
                                    args, out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }

  if (TryRunCollectionConstructorBuiltin(isolate, constructor, new_target,
                                         arg_count, args, out_result)) {
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }

  Address result_address = roots.exception().ptr();
  if (!TryConstructJSFunctionDirect(isolate, constructor, new_target, arg_count,
                                    args, &result_address)) {
    DirectHandle<JSReceiver> result;
    MaybeDirectHandle<JSReceiver> maybe_result =
        Execution::New(isolate, constructor, new_target,
                       ZoneVector<const DirectHandle<Object>>(args, arg_count));
    if (maybe_result.ToHandle(&result)) {
      result_address = (*result).ptr();
    }
  }
  if (switched_context) isolate->set_context(saved_context);

  *out_result = result_address;
  return true;
}

bool TryRunFindNonDefaultConstructorOrConstructBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum, interpreter::OperandScale operand_scale,
    Address* out_result) {
  if (bytecode_enum !=
      interpreter::Bytecode::kFindNonDefaultConstructorOrConstruct) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  int32_t this_function_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  int32_t new_target_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                operand_scale);
  int32_t output_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 2,
                                operand_scale);
  Address preserved_accumulator =
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];

  Address this_function_address = SafeTaggedOrUndefined(
      isolate, ReadInterpreterRegister(
                   interpreter::Register::FromOperand(this_function_operand)));
  Address new_target_address = SafeTaggedOrUndefined(
      isolate, ReadInterpreterRegister(
                   interpreter::Register::FromOperand(new_target_operand)));
  if (!IsJSFunction(Tagged<Object>(this_function_address)) ||
      !IsJSReceiver(Tagged<Object>(new_target_address))) {
    StoreInterpreterRegister(interpreter::Register::FromOperand(output_operand),
                             roots.false_value().ptr());
    StoreInterpreterRegister(
        interpreter::Register::FromOperand(output_operand - 1),
        roots.undefined_value().ptr());
    *out_result = preserved_accumulator;
    return true;
  }

  Tagged<Object> constructor =
      Cast<JSFunction>(Tagged<Object>(this_function_address))
          ->map()
          ->prototype();
  HandleScope scope(isolate);
  for (int depth = 0; depth < 32; ++depth) {
    if (!IsJSFunction(constructor)) {
      StoreInterpreterRegister(interpreter::Register::FromOperand(output_operand),
                               roots.false_value().ptr());
      StoreInterpreterRegister(
          interpreter::Register::FromOperand(output_operand - 1),
          SafeTaggedOrUndefined(isolate, constructor.ptr()));
      *out_result = preserved_accumulator;
      return true;
    }

    Tagged<JSFunction> constructor_function = Cast<JSFunction>(constructor);
    Tagged<SharedFunctionInfo> shared =
        Wasm32JSFunctionShared(constructor_function);
    if (shared->requires_instance_members_initializer() ||
        Wasm32JSFunctionContext(constructor_function)
            ->scope_info()
            ->ClassScopeHasPrivateBrand()) {
      StoreInterpreterRegister(interpreter::Register::FromOperand(output_operand),
                               roots.false_value().ptr());
      StoreInterpreterRegister(
          interpreter::Register::FromOperand(output_operand - 1),
          constructor.ptr());
      *out_result = preserved_accumulator;
      return true;
    }

    FunctionKind kind = shared->kind();
    if (kind == FunctionKind::kDefaultBaseConstructor) {
      DirectHandle<JSFunction> ctor(constructor_function, isolate);
      DirectHandle<JSReceiver> new_target(
          Cast<JSReceiver>(Tagged<Object>(new_target_address)), isolate);
      DirectHandle<JSObject> instance;
      if (!JSObject::New(ctor, new_target, {}).ToHandle(&instance)) {
        *out_result = roots.exception().ptr();
        return true;
      }
      StoreInterpreterRegister(interpreter::Register::FromOperand(output_operand),
                               roots.true_value().ptr());
      StoreInterpreterRegister(
          interpreter::Register::FromOperand(output_operand - 1),
          (*instance).ptr());
      *out_result = preserved_accumulator;
      return true;
    }

    if (kind != FunctionKind::kDefaultDerivedConstructor) {
      StoreInterpreterRegister(interpreter::Register::FromOperand(output_operand),
                               roots.false_value().ptr());
      StoreInterpreterRegister(
          interpreter::Register::FromOperand(output_operand - 1),
          constructor.ptr());
      *out_result = preserved_accumulator;
      return true;
    }

    constructor = constructor_function->map()->prototype();
  }

  StoreInterpreterRegister(interpreter::Register::FromOperand(output_operand),
                           roots.false_value().ptr());
  StoreInterpreterRegister(interpreter::Register::FromOperand(output_operand - 1),
                           SafeTaggedOrUndefined(isolate, constructor.ptr()));
  *out_result = preserved_accumulator;
  return true;
}

bool TryRunAddBytecode(Isolate* isolate, Tagged<BytecodeArray> bytecode,
                       int bytecode_index,
                       interpreter::Bytecode bytecode_enum,
                       interpreter::OperandScale operand_scale,
                       Address* out_result) {
  Address lhs_address = kNullAddress;
  Address rhs_address = kNullAddress;
  if (bytecode_enum == interpreter::Bytecode::kAdd ||
      bytecode_enum ==
          interpreter::Bytecode::kAdd_LhsIsStringConstant_Internalize) {
    int32_t lhs_operand =
        ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
    lhs_address = ReadInterpreterRegister(
        interpreter::Register::FromOperand(lhs_operand));
    rhs_address = g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
  } else if (bytecode_enum == interpreter::Bytecode::kAddSmi) {
    int32_t rhs =
        ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
    lhs_address = g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
    rhs_address = Smi::FromInt(rhs).ptr();
  } else if (bytecode_enum == interpreter::Bytecode::kSubSmi) {
    int32_t rhs =
        ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
    Address lhs = SafeTaggedOrUndefined(
        isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
    HandleScope scope(isolate);
    DirectHandle<Object> lhs_object(Tagged<Object>(lhs), isolate);
    DirectHandle<Object> lhs_numeric;
    if (!Object::ToNumeric(isolate, lhs_object).ToHandle(&lhs_numeric)) {
      *out_result = ReadOnlyRoots(isolate).exception().ptr();
      return true;
    }
    if (IsBigInt(*lhs_numeric)) {
      isolate->Throw(*isolate->factory()->NewTypeError(
          MessageTemplate::kBigIntMixedTypes));
      *out_result = ReadOnlyRoots(isolate).exception().ptr();
      return true;
    }
    double result = Object::NumberValue(*lhs_numeric) - rhs;
    *out_result = (*isolate->factory()->NewNumber(result)).ptr();
    return true;
  } else if (bytecode_enum == interpreter::Bytecode::kModSmi) {
    int32_t rhs =
        ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
    Address lhs = SafeTaggedOrUndefined(
        isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
    HandleScope scope(isolate);
    DirectHandle<Object> lhs_object(Tagged<Object>(lhs), isolate);
    DirectHandle<Object> lhs_numeric;
    if (!Object::ToNumeric(isolate, lhs_object).ToHandle(&lhs_numeric)) {
      *out_result = ReadOnlyRoots(isolate).exception().ptr();
      return true;
    }
    if (IsBigInt(*lhs_numeric)) {
      isolate->Throw(*isolate->factory()->NewTypeError(
          MessageTemplate::kBigIntMixedTypes));
      *out_result = ReadOnlyRoots(isolate).exception().ptr();
      return true;
    }
    double result = Modulo(Object::NumberValue(*lhs_numeric), rhs);
    *out_result = (*isolate->factory()->NewNumber(result)).ptr();
    return true;
  } else if (bytecode_enum == interpreter::Bytecode::kSub) {
    int32_t lhs_operand =
        ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
    Address lhs = SafeTaggedOrUndefined(
        isolate, ReadInterpreterRegister(
                     interpreter::Register::FromOperand(lhs_operand)));
    Address rhs = SafeTaggedOrUndefined(
        isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
    HandleScope scope(isolate);
    DirectHandle<Object> lhs_object(Tagged<Object>(lhs), isolate);
    DirectHandle<Object> rhs_object(Tagged<Object>(rhs), isolate);
    DirectHandle<Object> lhs_numeric;
    DirectHandle<Object> rhs_numeric;
    if (!Object::ToNumeric(isolate, lhs_object).ToHandle(&lhs_numeric) ||
        !Object::ToNumeric(isolate, rhs_object).ToHandle(&rhs_numeric)) {
      *out_result = ReadOnlyRoots(isolate).exception().ptr();
      return true;
    }
    if (IsBigInt(*lhs_numeric) || IsBigInt(*rhs_numeric)) {
      if (!IsBigInt(*lhs_numeric) || !IsBigInt(*rhs_numeric)) {
        isolate->Throw(*isolate->factory()->NewTypeError(
            MessageTemplate::kBigIntMixedTypes));
        *out_result = ReadOnlyRoots(isolate).exception().ptr();
        return true;
      }
      Handle<BigInt> result;
      if (!BigInt::Subtract(isolate, Cast<BigInt>(lhs_numeric),
                            Cast<BigInt>(rhs_numeric))
               .ToHandle(&result)) {
        *out_result = ReadOnlyRoots(isolate).exception().ptr();
        return true;
      }
      *out_result = (*result).ptr();
      return true;
    }
    double result = Object::NumberValue(*lhs_numeric) -
                    Object::NumberValue(*rhs_numeric);
    *out_result = (*isolate->factory()->NewNumber(result)).ptr();
    return true;
  } else if (bytecode_enum == interpreter::Bytecode::kDivSmi) {
    int32_t rhs =
        ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
    Address lhs = SafeTaggedOrUndefined(
        isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
    HandleScope scope(isolate);
    DirectHandle<Object> lhs_object(Tagged<Object>(lhs), isolate);
    DirectHandle<Object> lhs_numeric;
    if (!Object::ToNumeric(isolate, lhs_object).ToHandle(&lhs_numeric)) {
      *out_result = ReadOnlyRoots(isolate).exception().ptr();
      return true;
    }
    if (IsBigInt(*lhs_numeric)) {
      isolate->Throw(*isolate->factory()->NewTypeError(
          MessageTemplate::kBigIntMixedTypes));
      *out_result = ReadOnlyRoots(isolate).exception().ptr();
      return true;
    }
    double result = Object::NumberValue(*lhs_numeric) /
                    static_cast<double>(rhs);
    *out_result = (*isolate->factory()->NewNumber(result)).ptr();
    return true;
  } else if (bytecode_enum == interpreter::Bytecode::kDiv) {
    int32_t lhs_operand =
        ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
    Address lhs = SafeTaggedOrUndefined(
        isolate, ReadInterpreterRegister(
                     interpreter::Register::FromOperand(lhs_operand)));
    Address rhs = SafeTaggedOrUndefined(
        isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
    HandleScope scope(isolate);
    DirectHandle<Object> lhs_object(Tagged<Object>(lhs), isolate);
    DirectHandle<Object> rhs_object(Tagged<Object>(rhs), isolate);
    DirectHandle<Object> lhs_numeric;
    DirectHandle<Object> rhs_numeric;
    if (!Object::ToNumeric(isolate, lhs_object).ToHandle(&lhs_numeric) ||
        !Object::ToNumeric(isolate, rhs_object).ToHandle(&rhs_numeric)) {
      *out_result = ReadOnlyRoots(isolate).exception().ptr();
      return true;
    }
    if (IsBigInt(*lhs_numeric) || IsBigInt(*rhs_numeric)) {
      if (!IsBigInt(*lhs_numeric) || !IsBigInt(*rhs_numeric)) {
        isolate->Throw(*isolate->factory()->NewTypeError(
            MessageTemplate::kBigIntMixedTypes));
        *out_result = ReadOnlyRoots(isolate).exception().ptr();
        return true;
      }
      Handle<BigInt> result;
      if (!BigInt::Divide(isolate, Cast<BigInt>(lhs_numeric),
                          Cast<BigInt>(rhs_numeric))
               .ToHandle(&result)) {
        *out_result = ReadOnlyRoots(isolate).exception().ptr();
        return true;
      }
      *out_result = (*result).ptr();
      return true;
    }
    double result = Object::NumberValue(*lhs_numeric) /
                    Object::NumberValue(*rhs_numeric);
    *out_result = (*isolate->factory()->NewNumber(result)).ptr();
    return true;
  } else if (bytecode_enum == interpreter::Bytecode::kMulSmi) {
    int32_t rhs =
        ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
    Address lhs = SafeTaggedOrUndefined(
        isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
    HandleScope scope(isolate);
    DirectHandle<Object> lhs_object(Tagged<Object>(lhs), isolate);
    DirectHandle<Object> lhs_numeric;
    if (!Object::ToNumeric(isolate, lhs_object).ToHandle(&lhs_numeric)) {
      *out_result = ReadOnlyRoots(isolate).exception().ptr();
      return true;
    }
    if (IsBigInt(*lhs_numeric)) {
      isolate->Throw(*isolate->factory()->NewTypeError(
          MessageTemplate::kBigIntMixedTypes));
      *out_result = ReadOnlyRoots(isolate).exception().ptr();
      return true;
    }
    double result = Object::NumberValue(*lhs_numeric) *
                    static_cast<double>(rhs);
    *out_result = (*isolate->factory()->NewNumber(result)).ptr();
    return true;
  } else if (bytecode_enum == interpreter::Bytecode::kExp) {
    int32_t lhs_operand =
        ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
    Address lhs = SafeTaggedOrUndefined(
        isolate, ReadInterpreterRegister(
                     interpreter::Register::FromOperand(lhs_operand)));
    Address rhs = SafeTaggedOrUndefined(
        isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
    HandleScope scope(isolate);
    DirectHandle<Object> lhs_object(Tagged<Object>(lhs), isolate);
    DirectHandle<Object> rhs_object(Tagged<Object>(rhs), isolate);
    DirectHandle<Object> lhs_numeric;
    DirectHandle<Object> rhs_numeric;
    if (!Object::ToNumeric(isolate, lhs_object).ToHandle(&lhs_numeric) ||
        !Object::ToNumeric(isolate, rhs_object).ToHandle(&rhs_numeric)) {
      *out_result = ReadOnlyRoots(isolate).exception().ptr();
      return true;
    }
    if (IsBigInt(*lhs_numeric) || IsBigInt(*rhs_numeric)) {
      isolate->Throw(*isolate->factory()->NewTypeError(
          MessageTemplate::kBigIntMixedTypes));
      *out_result = ReadOnlyRoots(isolate).exception().ptr();
      return true;
    }
    double result = std::pow(Object::NumberValue(*lhs_numeric),
                             Object::NumberValue(*rhs_numeric));
    *out_result = (*isolate->factory()->NewNumber(result)).ptr();
    return true;
  } else {
    return false;
  }

  lhs_address = SafeTaggedOrUndefined(isolate, lhs_address);
  rhs_address = SafeTaggedOrUndefined(isolate, rhs_address);

  Address args[2] = {rhs_address, lhs_address};
  *out_result = Runtime_Add(2, &args[1], isolate);
  return true;
}

bool TryRunIncDecBytecode(Isolate* isolate,
                          interpreter::Bytecode bytecode_enum,
                          Address* out_result) {
  bool increment = bytecode_enum == interpreter::Bytecode::kInc;
  bool decrement = bytecode_enum == interpreter::Bytecode::kDec;
  if (!increment && !decrement) return false;

  Address input_address = SafeTaggedOrUndefined(
      isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
  HandleScope scope(isolate);
  DirectHandle<Object> input =
      direct_handle(Tagged<Object>(input_address), isolate);
  DirectHandle<Object> numeric;
  if (!Object::ToNumeric(isolate, input).ToHandle(&numeric)) {
    *out_result = ReadOnlyRoots(isolate).exception().ptr();
    return true;
  }

  if (IsBigInt(*numeric)) {
    DirectHandle<BigInt> value = Cast<BigInt>(numeric);
    DirectHandle<BigInt> one = BigInt::FromInt64(isolate, 1);
    Handle<BigInt> result;
    MaybeHandle<BigInt> maybe_result =
        increment ? BigInt::Add(isolate, value, one)
                  : BigInt::Subtract(isolate, value, one);
    if (!maybe_result.ToHandle(&result)) {
      *out_result = ReadOnlyRoots(isolate).exception().ptr();
      return true;
    }
    *out_result = (*result).ptr();
    return true;
  }

  double value = Object::NumberValue(*numeric);
  *out_result =
      (*isolate->factory()->NewNumber(value + (increment ? 1.0 : -1.0))).ptr();
  return true;
}

bool TryRunBitwiseSmiBytecode(Isolate* isolate, Tagged<BytecodeArray> bytecode,
                              int bytecode_index,
                              interpreter::Bytecode bytecode_enum,
                              interpreter::OperandScale operand_scale,
                              Address* out_result) {
  bool generic_bitwise_or =
      bytecode_enum == interpreter::Bytecode::kBitwiseOr;
  bool generic_bitwise_and =
      bytecode_enum == interpreter::Bytecode::kBitwiseAnd;
  bool bitwise_or = bytecode_enum == interpreter::Bytecode::kBitwiseOrSmi ||
                    generic_bitwise_or;
  bool bitwise_xor = bytecode_enum == interpreter::Bytecode::kBitwiseXorSmi;
  bool bitwise_and = bytecode_enum == interpreter::Bytecode::kBitwiseAndSmi ||
                     generic_bitwise_and;
  bool shift_left = bytecode_enum == interpreter::Bytecode::kShiftLeftSmi;
  bool shift_right = bytecode_enum == interpreter::Bytecode::kShiftRightSmi;
  bool shift_right_logical =
      bytecode_enum == interpreter::Bytecode::kShiftRightLogicalSmi;
  if (!bitwise_or && !bitwise_xor && !bitwise_and && !shift_left &&
      !shift_right && !shift_right_logical) {
    return false;
  }

  int32_t rhs = 0;
  Address lhs_address = kNullAddress;
  Address rhs_address = kNullAddress;
  bool generic_bitwise = generic_bitwise_or || generic_bitwise_and;
  if (generic_bitwise) {
    int32_t lhs_operand =
        ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
    lhs_address = SafeTaggedOrUndefined(
        isolate, ReadInterpreterRegister(
                     interpreter::Register::FromOperand(lhs_operand)));
    rhs_address = SafeTaggedOrUndefined(
        isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
  } else {
    rhs = ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                    operand_scale);
    lhs_address = SafeTaggedOrUndefined(
        isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
  }
  HandleScope scope(isolate);
  DirectHandle<Object> lhs =
      direct_handle(Tagged<Object>(lhs_address), isolate);

  DirectHandle<Number> lhs_number;
  if (shift_right_logical) {
    if (!Object::ToUint32(isolate, lhs).ToHandle(&lhs_number)) {
      *out_result = ReadOnlyRoots(isolate).exception().ptr();
      return true;
    }
    uint32_t left = NumberToUint32(*lhs_number);
    uint32_t count = static_cast<uint32_t>(rhs) & 0x1f;
    DirectHandle<Number> result =
        isolate->factory()->NewNumberFromUint(left >> count);
    *out_result = (*result).ptr();
    return true;
  }

  if (!Object::ToInt32(isolate, lhs).ToHandle(&lhs_number)) {
    *out_result = ReadOnlyRoots(isolate).exception().ptr();
    return true;
  }
  if (generic_bitwise) {
    DirectHandle<Object> rhs_object(
        Tagged<Object>(rhs_address), isolate);
    DirectHandle<Number> rhs_number;
    if (!Object::ToInt32(isolate, rhs_object).ToHandle(&rhs_number)) {
      *out_result = ReadOnlyRoots(isolate).exception().ptr();
      return true;
    }
    rhs = NumberToInt32(*rhs_number);
  }
  int32_t left = NumberToInt32(*lhs_number);
  int32_t result_value = 0;
  if (bitwise_or) {
    result_value = left | rhs;
  } else if (bitwise_xor) {
    result_value = left ^ rhs;
  } else if (bitwise_and) {
    result_value = left & rhs;
  } else {
    uint32_t count = static_cast<uint32_t>(rhs) & 0x1f;
    if (shift_left) {
      result_value = static_cast<int32_t>(static_cast<uint32_t>(left) << count);
    } else {
      result_value = left >> count;
    }
  }

  DirectHandle<Number> result =
      isolate->factory()->NewNumberFromInt(result_value);
  *out_result = (*result).ptr();
  return true;
}

bool TryRunBitwiseNotBytecode(Isolate* isolate,
                              interpreter::Bytecode bytecode_enum,
                              Address* out_result) {
  bool negate = bytecode_enum == interpreter::Bytecode::kNegate;
  if (bytecode_enum != interpreter::Bytecode::kBitwiseNot && !negate) {
    return false;
  }

  Address input_address = SafeTaggedOrUndefined(
      isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
  HandleScope scope(isolate);
  DirectHandle<Object> input(Tagged<Object>(input_address), isolate);
  if (negate) {
    DirectHandle<Object> numeric;
    if (!Object::ToNumeric(isolate, input).ToHandle(&numeric)) {
      *out_result = ReadOnlyRoots(isolate).exception().ptr();
      return true;
    }
    if (IsBigInt(*numeric)) {
      *out_result =
          (*BigInt::UnaryMinus(isolate, Cast<BigInt>(numeric))).ptr();
      return true;
    }
    *out_result =
        (*isolate->factory()->NewNumber(-Object::NumberValue(*numeric))).ptr();
    return true;
  }

  DirectHandle<Number> number;
  if (!Object::ToInt32(isolate, input).ToHandle(&number)) {
    *out_result = ReadOnlyRoots(isolate).exception().ptr();
    return true;
  }
  DirectHandle<Number> result =
      isolate->factory()->NewNumberFromInt(~NumberToInt32(*number));
  *out_result = (*result).ptr();
  return true;
}

bool TryReadJumpOffset(Tagged<BytecodeArray> bytecode, int bytecode_index,
                       interpreter::Bytecode bytecode_enum,
                       interpreter::OperandScale operand_scale,
                       int operand_index, intptr_t* out_offset) {
  if (interpreter::Bytecodes::IsJumpConstant(bytecode_enum)) {
    uint32_t constant_index =
        ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum,
                                    operand_index, operand_scale);
    Tagged<TrustedFixedArray> constant_pool = bytecode->constant_pool();
    if (constant_index >= static_cast<uint32_t>(constant_pool->length())) {
      PrintF("WasmInterpreterEntryTrampoline: bad jump constant index=%u "
             "length=%d\n",
             constant_index, constant_pool->length());
      return false;
    }
    Tagged<Object> offset_object = constant_pool->get(constant_index);
    if (!IsSmi(offset_object)) {
      PrintF("WasmInterpreterEntryTrampoline: jump constant is not Smi ");
      DumpRuntimeArg("value", 0, offset_object.ptr());
      PrintF("\n");
      return false;
    }
    *out_offset = Smi::ToInt(offset_object);
    return true;
  }

  *out_offset = static_cast<intptr_t>(ReadBytecodeUnsignedOperand(
      bytecode, bytecode_index, bytecode_enum, operand_index, operand_scale));
  return true;
}

bool TryRunForInBytecode(Isolate* isolate, Tagged<BytecodeArray> bytecode,
                         int bytecode_index,
                         interpreter::Bytecode bytecode_enum,
                         interpreter::OperandScale operand_scale,
                         Address* out_result) {
  ReadOnlyRoots roots(isolate);
  if (bytecode_enum != interpreter::Bytecode::kToObject &&
      bytecode_enum != interpreter::Bytecode::kForInEnumerate &&
      bytecode_enum != interpreter::Bytecode::kForInPrepare &&
      bytecode_enum != interpreter::Bytecode::kForInNext &&
      bytecode_enum != interpreter::Bytecode::kForInStep) {
    return false;
  }

  if (bytecode_enum == interpreter::Bytecode::kToObject) {
    int32_t destination_operand = ReadBytecodeSignedOperand(
        bytecode, bytecode_index, bytecode_enum, 0, operand_scale);
    Address accumulator = SafeTaggedOrUndefined(
        isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
    HandleScope scope(isolate);
    WasmGCStateScope gc_state(isolate);
    SetCurrentIsolateScope current_isolate_scope(isolate);
    SaveContext save_context(isolate);
    Address context = CurrentInterpreterContext();
    if (IsSafeTaggedHandleValue(context) &&
        IsContext(Tagged<Object>(context))) {
      isolate->set_context(Cast<Context>(Tagged<Object>(context)));
    }
    DirectHandle<Object> value(Tagged<Object>(accumulator), isolate);
    DirectHandle<JSReceiver> object;
    if (!Object::ToObject(isolate, value).ToHandle(&object)) {
      *out_result = roots.exception().ptr();
      return true;
    }
    StoreInterpreterRegister(
        interpreter::Register::FromOperand(destination_operand),
        (*object).ptr());
    *out_result = accumulator;
    return true;
  }

  if (bytecode_enum == interpreter::Bytecode::kForInEnumerate) {
    int32_t receiver_operand = ReadBytecodeSignedOperand(
        bytecode, bytecode_index, bytecode_enum, 0, operand_scale);
    Address receiver = SafeTaggedOrUndefined(
        isolate, ReadInterpreterRegister(
                     interpreter::Register::FromOperand(receiver_operand)));
    if (!IsJSReceiver(Tagged<Object>(receiver))) {
      *out_result = roots.exception().ptr();
      return true;
    }

    WasmGCStateScope gc_state(isolate);
    SetCurrentIsolateScope current_isolate_scope(isolate);
    SaveContext save_context(isolate);
    Address context = CurrentInterpreterContext();
    if (IsSafeTaggedHandleValue(context) &&
        IsContext(Tagged<Object>(context))) {
      isolate->set_context(Cast<Context>(Tagged<Object>(context)));
    }
    Address args[1] = {receiver};
    WasmTemporaryRootScope args_roots(isolate, args, 1);
    *out_result = Runtime_ForInEnumerate(1, args_roots.data(), isolate);
    return true;
  }

  if (bytecode_enum == interpreter::Bytecode::kForInPrepare) {
    int32_t cache_info_operand = ReadBytecodeSignedOperand(
        bytecode, bytecode_index, bytecode_enum, 0, operand_scale);
    Address enumerator = SafeTaggedOrUndefined(
        isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
    Address cache_type = enumerator;
    Address cache_array = roots.empty_fixed_array().ptr();
    int cache_length = 0;

    Tagged<Object> enumerator_object(enumerator);
    if (IsMap(enumerator_object)) {
      Tagged<Map> map = Cast<Map>(enumerator_object);
      cache_length = map->EnumLength();
      if (cache_length == kInvalidEnumCacheSentinel) {
        *out_result = roots.exception().ptr();
        return true;
      }
      cache_array =
          map->instance_descriptors(isolate)->enum_cache()->keys().ptr();
    } else if (IsFixedArray(enumerator_object)) {
      Tagged<FixedArray> keys = Cast<FixedArray>(enumerator_object);
      cache_array = keys.ptr();
      cache_length = keys->length();
    } else {
      *out_result = roots.exception().ptr();
      return true;
    }

    StoreInterpreterRegister(
        RegisterFromListOperand(cache_info_operand, 0), cache_type);
    StoreInterpreterRegister(
        RegisterFromListOperand(cache_info_operand, 1), cache_array);
    StoreInterpreterRegister(
        RegisterFromListOperand(cache_info_operand, 2),
        Smi::FromInt(cache_length).ptr());
    *out_result = Smi::zero().ptr();
    return true;
  }

  if (bytecode_enum == interpreter::Bytecode::kForInNext) {
    int32_t receiver_operand = ReadBytecodeSignedOperand(
        bytecode, bytecode_index, bytecode_enum, 0, operand_scale);
    int32_t index_operand = ReadBytecodeSignedOperand(
        bytecode, bytecode_index, bytecode_enum, 1, operand_scale);
    int32_t cache_info_operand = ReadBytecodeSignedOperand(
        bytecode, bytecode_index, bytecode_enum, 2, operand_scale);
    Address receiver = SafeTaggedOrUndefined(
        isolate, ReadInterpreterRegister(
                     interpreter::Register::FromOperand(receiver_operand)));
    Address index_value = SafeTaggedOrUndefined(
        isolate, ReadInterpreterRegister(
                     interpreter::Register::FromOperand(index_operand)));
    Address cache_type = SafeTaggedOrUndefined(
        isolate, ReadInterpreterRegister(
                     RegisterFromListOperand(cache_info_operand, 0)));
    Address cache_array = SafeTaggedOrUndefined(
        isolate, ReadInterpreterRegister(
                     RegisterFromListOperand(cache_info_operand, 1)));

    if (!IsJSReceiver(Tagged<Object>(receiver)) ||
        !IsSmi(Tagged<Object>(index_value)) ||
        !IsFixedArray(Tagged<Object>(cache_array))) {
      *out_result = roots.exception().ptr();
      return true;
    }
    int index = Smi::ToInt(Tagged<Smi>(index_value));
    Tagged<FixedArray> keys = Cast<FixedArray>(Tagged<Object>(cache_array));
    if (index < 0 || index >= keys->length()) {
      *out_result = roots.exception().ptr();
      return true;
    }
    Address key = keys->get(index).ptr();
    Tagged<JSReceiver> receiver_object =
        Cast<JSReceiver>(Tagged<Object>(receiver));
    if (receiver_object->map(isolate).ptr() == cache_type) {
      *out_result = key;
      return true;
    }

    WasmGCStateScope gc_state(isolate);
    SetCurrentIsolateScope current_isolate_scope(isolate);
    SaveContext save_context(isolate);
    Address context = CurrentInterpreterContext();
    if (IsSafeTaggedHandleValue(context) &&
        IsContext(Tagged<Object>(context))) {
      isolate->set_context(Cast<Context>(Tagged<Object>(context)));
    }
    Address args[2] = {key, receiver};
    WasmTemporaryRootScope args_roots(isolate, args, 2);
    Address present =
        Runtime_ForInHasProperty(2, &args_roots.data()[1], isolate);
    *out_result = IsTrue(Tagged<Object>(present), roots)
                      ? args_roots.data()[0]
                      : roots.undefined_value().ptr();
    return true;
  }

  if (bytecode_enum == interpreter::Bytecode::kForInStep) {
    int32_t index_operand = ReadBytecodeSignedOperand(
        bytecode, bytecode_index, bytecode_enum, 0, operand_scale);
    interpreter::Register index_register =
        interpreter::Register::FromOperand(index_operand);
    Address index_value = SafeTaggedOrUndefined(
        isolate, ReadInterpreterRegister(index_register));
    if (!IsSmi(Tagged<Object>(index_value))) {
      *out_result = roots.exception().ptr();
      return true;
    }
    Address next =
        Smi::FromInt(Smi::ToInt(Tagged<Smi>(index_value)) + 1).ptr();
    StoreInterpreterRegister(index_register, next);
    *out_result = next;
    return true;
  }

  return false;
}

bool TryRunJumpBytecode(Isolate* isolate, Tagged<BytecodeArray> bytecode,
                        int bytecode_index,
                        interpreter::Bytecode bytecode_enum,
                        interpreter::OperandScale operand_scale,
                        Address current_offset, Address* out_next_offset) {
  if (!interpreter::Bytecodes::IsJump(bytecode_enum)) return false;

  intptr_t relative_jump = 0;
  if (!TryReadJumpOffset(bytecode, bytecode_index, bytecode_enum, operand_scale,
                         0, &relative_jump)) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  Address accumulator_address = SafeTaggedOrUndefined(
      isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
  Tagged<Object> accumulator(accumulator_address);
  auto wasm_to_boolean = [isolate, roots](Tagged<Object> value) {
    if (IsTrue(value, roots)) return true;
    if (IsFalse(value, roots) || IsUndefined(value, roots) ||
        IsNull(value, roots)) {
      return false;
    }
    if (IsSmi(value)) return Smi::ToInt(value) != 0;
    return Object::BooleanValue(value, isolate);
  };
  bool should_jump = false;

  switch (bytecode_enum) {
    case interpreter::Bytecode::kJump:
    case interpreter::Bytecode::kJumpConstant:
      should_jump = true;
      break;
    case interpreter::Bytecode::kJumpLoop:
      should_jump = true;
      break;
    case interpreter::Bytecode::kJumpIfTrue:
    case interpreter::Bytecode::kJumpIfTrueConstant:
      should_jump = IsTrue(accumulator, roots);
      break;
    case interpreter::Bytecode::kJumpIfFalse:
    case interpreter::Bytecode::kJumpIfFalseConstant:
      should_jump = IsFalse(accumulator, roots);
      break;
    case interpreter::Bytecode::kJumpIfToBooleanTrue:
    case interpreter::Bytecode::kJumpIfToBooleanTrueConstant:
      should_jump = wasm_to_boolean(accumulator);
      break;
    case interpreter::Bytecode::kJumpIfToBooleanFalse:
    case interpreter::Bytecode::kJumpIfToBooleanFalseConstant:
      should_jump = !wasm_to_boolean(accumulator);
      break;
    case interpreter::Bytecode::kJumpIfNull:
    case interpreter::Bytecode::kJumpIfNullConstant:
      should_jump = IsNull(accumulator, roots);
      break;
    case interpreter::Bytecode::kJumpIfNotNull:
    case interpreter::Bytecode::kJumpIfNotNullConstant:
      should_jump = !IsNull(accumulator, roots);
      break;
    case interpreter::Bytecode::kJumpIfUndefined:
    case interpreter::Bytecode::kJumpIfUndefinedConstant:
      should_jump = IsUndefined(accumulator, roots);
      break;
    case interpreter::Bytecode::kJumpIfNotUndefined:
    case interpreter::Bytecode::kJumpIfNotUndefinedConstant:
      should_jump = !IsUndefined(accumulator, roots);
      break;
    case interpreter::Bytecode::kJumpIfUndefinedOrNull:
    case interpreter::Bytecode::kJumpIfUndefinedOrNullConstant:
      should_jump = IsUndefined(accumulator, roots) ||
                    IsNull(accumulator, roots);
      break;
    case interpreter::Bytecode::kJumpIfJSReceiver:
    case interpreter::Bytecode::kJumpIfJSReceiverConstant:
      should_jump = IsJSReceiver(accumulator);
      break;
    case interpreter::Bytecode::kJumpIfForInDone:
    case interpreter::Bytecode::kJumpIfForInDoneConstant: {
      int32_t index_operand =
          ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                    operand_scale);
      int32_t cache_length_operand =
          ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 2,
                                    operand_scale);
      Address index_address = ReadInterpreterRegister(
          interpreter::Register::FromOperand(index_operand));
      Address cache_length_address = ReadInterpreterRegister(
          interpreter::Register::FromOperand(cache_length_operand));
      should_jump = index_address == cache_length_address;
      break;
    }
    default:
      return false;
  }

  Address next_offset = current_offset +
                        interpreter::Bytecodes::Size(bytecode_enum,
                                                     operand_scale);
  if (should_jump) {
    next_offset = bytecode_enum == interpreter::Bytecode::kJumpLoop
                      ? current_offset - static_cast<Address>(relative_jump)
                      : current_offset + static_cast<Address>(relative_jump);
  }

  Address bytecode_start = BytecodeArray::kHeaderSize - kHeapObjectTag;
  Address bytecode_limit =
      bytecode_start + static_cast<Address>(bytecode->length());
  if (next_offset < bytecode_start || next_offset >= bytecode_limit) {
    PrintF("WasmInterpreterEntryTrampoline: jump target OOB bytecode=%s "
           "current=0x%x relative=%d target=0x%x range=[0x%x,0x%x)\n",
           interpreter::Bytecodes::ToString(bytecode_enum),
           static_cast<unsigned>(current_offset),
           static_cast<int>(relative_jump), static_cast<unsigned>(next_offset),
           static_cast<unsigned>(bytecode_start),
           static_cast<unsigned>(bytecode_limit));
    return false;
  }

  *out_next_offset = next_offset;
  return true;
}

bool TryRunSetNamedPropertyBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kSetNamedProperty &&
      bytecode_enum != interpreter::Bytecode::kDefineNamedOwnProperty) {
    return false;
  }

  int32_t object_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  uint32_t name_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                  operand_scale);
  uint32_t slot_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 2,
                                  operand_scale);

  Tagged<TrustedFixedArray> constant_pool = bytecode->constant_pool();
  if (name_index >= static_cast<uint32_t>(constant_pool->length())) {
    PrintF("WasmInterpreterEntryTrampoline: bad store name index=%u "
           "length=%d\n",
           name_index, constant_pool->length());
    return false;
  }
  Tagged<Object> name_object = constant_pool->get(name_index);
  if (!IsName(name_object)) {
    PrintF("WasmInterpreterEntryTrampoline: store constant is not Name "
           "index=%u ",
           name_index);
    DumpRuntimeArg("value", 0, name_object.ptr());
    PrintF("\n");
    return false;
  }
  bool trace_reflect_own_keys_name =
      kTraceWasmFallbackDetails &&
      IsReflectOwnKeysTraceName(isolate, name_object);
  bool trace_module_state_name =
      kTraceWasmFallbackDetails &&
      IsBuiltinModuleStateTraceName(isolate, name_object);

  Address object_address = ReadInterpreterRegister(
      interpreter::Register::FromOperand(object_operand));
  Address value_address =
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
  ReadOnlyRoots roots(isolate);
  if (trace_reflect_own_keys_name) {
    PrintF("WasmInterpreterEntryTrampoline: named store trace before "
           "bytecode=%s index=%d object_operand=%d name=",
           interpreter::Bytecodes::ToString(bytecode_enum), bytecode_index,
           object_operand);
    DumpNameForTrace(name_object);
    DumpRuntimeArg(" object", 0, object_address);
    DumpRuntimeArg(" value", 0, value_address);
    PrintF("\n");
  }
  if (!IsJSAnyForWasmPropertyLookup(isolate, object_address)) {
    PrintF("WasmInterpreterEntryTrampoline: named store receiver is not JSAny ");
    DumpRuntimeArg("receiver", 0, object_address);
    PrintF(" ");
    DumpRuntimeArg("name", 0, name_object.ptr());
    PrintF("\n");
    *out_result = IsSafeTaggedRootValue(isolate, value_address)
                      ? value_address
                      : roots.undefined_value().ptr();
    return true;
  }
  if (!IsSafeTaggedRootValue(isolate, value_address)) {
    value_address = roots.undefined_value().ptr();
  }

  HandleScope scope(isolate);
  WasmGCStateScope gc_state(isolate);
  SetCurrentIsolateScope current_isolate_scope(isolate);

  Handle<JSAny> object =
      handle(Cast<JSAny>(Tagged<Object>(object_address)), isolate);
  Handle<Name> name = handle(Cast<Name>(name_object), isolate);
  DirectHandle<Object> value =
      direct_handle(Tagged<Object>(value_address), isolate);

  bool trace_module_store = false;
  if (trace_module_state_name && g_module_property_store_trace_count < 128 &&
      IsJSReceiver(*object)) {
    DirectHandle<JSReceiver> receiver(Cast<JSReceiver>(*object), isolate);
    DirectHandle<Name> id_name =
        isolate->factory()->InternalizeUtf8String("id");
    Handle<Object> module_id =
        JSReceiver::GetDataProperty(isolate, receiver, id_name);
    if (IsString(*module_id)) {
      ++g_module_property_store_trace_count;
      trace_module_store = true;
      PrintF("WasmInterpreterEntryTrampoline: module property store before "
             "count=%d bytecode=%s index=%d source_pos=%d stmt_pos=%d "
             "object_operand=%d slot=%u name=",
             g_module_property_store_trace_count,
             interpreter::Bytecodes::ToString(bytecode_enum), bytecode_index,
             bytecode->SourcePosition(bytecode_index),
             bytecode->SourceStatementPosition(bytecode_index), object_operand,
             slot_index);
      DumpNameForTrace(name_object);
      PrintStringPreviewForTrace(" name_string", name_object, 0, 64);
      DumpRuntimeArg(" object", 0, object_address);
      DumpNamedDataPropertyForTrace(isolate, object_address, "id");
      DumpNamedDataPropertyForTrace(isolate, object_address, "loaded");
      DumpNamedDataPropertyForTrace(isolate, object_address, "loading");
      DumpNamedDataPropertyForTrace(isolate, object_address, "exports");
      DumpRuntimeArg(" value", 0, (*value).ptr());
      Address function_address =
          g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
              StandardFrameConstants::kFunctionOffset)];
      DumpFunctionSourceForTrace(function_address);
      PrintF("\n");
      if (value_address == roots.undefined_value().ptr() &&
          g_module_undefined_store_trace_count < 12) {
        ++g_module_undefined_store_trace_count;
        PrintF("WasmInterpreterEntryTrampoline: module undefined store "
               "diagnostic count=%d bytecode=%s index=%d",
               g_module_undefined_store_trace_count,
               interpreter::Bytecodes::ToString(bytecode_enum),
               bytecode_index);
        DumpRuntimeArg(" acc", 0,
                       g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
        DumpRuntimeArg(" root_true", 0, roots.true_value().ptr());
        DumpRuntimeArg(" root_false", 0, roots.false_value().ptr());
        DumpRuntimeArg(" root_undefined", 0, roots.undefined_value().ptr());
        DumpRuntimeArg(" root_exception", 0, roots.exception().ptr());
        DumpRuntimeArg(" frame_offset", 0,
                       g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
                           InterpreterFrameConstants::kBytecodeOffsetFromFp)]);
        DumpRuntimeArg(" reg_offset", 0,
                       g_wasm_regs[SlotFor(kInterpreterBytecodeOffsetRegister)]);
        PrintF("\n");
        DumpBytecodeWindowForTrace(bytecode, bytecode_index, 32, 48,
                                   "module-undefined-store");
        int logged_parameter_count =
            bytecode->parameter_count() < 8 ? bytecode->parameter_count() : 8;
        for (int parameter_index = 0;
             parameter_index < logged_parameter_count; ++parameter_index) {
          DumpRuntimeArg(
              "module.p", parameter_index,
              ReadInterpreterRegister(
                  interpreter::Register::FromParameterIndex(parameter_index)));
        }
        PrintF("\n");
        int logged_register_count =
            bytecode->register_count() < 16 ? bytecode->register_count() : 16;
        for (int register_index = 0; register_index < logged_register_count;
             ++register_index) {
          DumpRuntimeArg("module.l", register_index,
                         ReadInterpreterRegister(
                             interpreter::Register(register_index)));
        }
        PrintF("\n");
      }
    }
  }

  Tagged<Context> saved_context = isolate->context();
  Address context_address = CurrentInterpreterContext();
  bool switched_context = false;
  if (IsSafeTaggedHandleValue(context_address) &&
      IsContext(Tagged<Object>(context_address))) {
    isolate->set_context(Cast<Context>(Tagged<Object>(context_address)));
    switched_context = true;
  }

  MaybeDirectHandle<Object> maybe_result;
  if (bytecode_enum == interpreter::Bytecode::kSetNamedProperty &&
      IsJSArray(*object) &&
      Name::Equals(isolate, name, isolate->factory()->length_string())) {
    uint32_t length = 0;
    if (!Object::ToArrayLength(*value, &length)) {
      isolate->Throw(*isolate->factory()->NewRangeError(
          MessageTemplate::kInvalidArrayLength));
      if (switched_context) isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    Handle<JSArray> array = Cast<JSArray>(object);
    if (JSArray::SetLength(isolate, array, length).IsNothing()) {
      if (switched_context) isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    if (switched_context) isolate->set_context(saved_context);
    *out_result = value_address;
    return true;
  }
  if (bytecode_enum == interpreter::Bytecode::kDefineNamedOwnProperty) {
    maybe_result =
        Runtime::DefineObjectOwnProperty(isolate, object, name, value,
                                         StoreOrigin::kNamed);
  } else {
    maybe_result =
        Runtime::SetObjectProperty(isolate, object, name, value,
                                   StoreOrigin::kNamed, Just(kThrowOnError));
  }
  DirectHandle<Object> result;
  if (!maybe_result.ToHandle(&result)) {
    if (switched_context) isolate->set_context(saved_context);
    *out_result = ReadOnlyRoots(isolate).exception().ptr();
    if (trace_module_store) {
      PrintF("WasmInterpreterEntryTrampoline: module property store exception "
             "bytecode=%s index=%d ",
             interpreter::Bytecodes::ToString(bytecode_enum), bytecode_index);
      DumpNamedDataPropertyForTrace(isolate, object_address, "id");
      DumpNamedDataPropertyForTrace(isolate, object_address, "loaded");
      DumpNamedDataPropertyForTrace(isolate, object_address, "loading");
      DumpNamedDataPropertyForTrace(isolate, object_address, "exports");
      PrintF("\n");
    }
    return true;
  }
  if (switched_context) isolate->set_context(saved_context);
  *out_result = (*result).ptr();
  if (trace_module_store) {
    PrintF("WasmInterpreterEntryTrampoline: module property store after "
           "bytecode=%s index=%d ",
           interpreter::Bytecodes::ToString(bytecode_enum), bytecode_index);
    DumpRuntimeArg("result", 0, *out_result);
    DumpNamedDataPropertyForTrace(isolate, object_address, "id");
    DumpNamedDataPropertyForTrace(isolate, object_address, "loaded");
    DumpNamedDataPropertyForTrace(isolate, object_address, "loading");
    DumpNamedDataPropertyForTrace(isolate, object_address, "exports");
    PrintF("\n");
  }
  if (trace_reflect_own_keys_name) {
    PrintF("WasmInterpreterEntryTrampoline: named store trace after "
           "bytecode=%s index=%d ",
           interpreter::Bytecodes::ToString(bytecode_enum), bytecode_index);
    DumpRuntimeArg("result", 0, *out_result);
    PrintF("\n");
  }
  return true;
}

bool TryRunSetKeyedPropertyBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kSetKeyedProperty) return false;

  int32_t object_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  int32_t key_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                operand_scale);

  Address object_address = ReadInterpreterRegister(
      interpreter::Register::FromOperand(object_operand));
  Address key_address =
      ReadInterpreterRegister(interpreter::Register::FromOperand(key_operand));
  Address value_address =
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
  ReadOnlyRoots roots(isolate);
  if (!IsSafeJSAnyForWasmPropertyLookup(isolate, object_address)) {
    PrintF("WasmInterpreterEntryTrampoline: keyed store receiver is not safe JSAny ");
    DumpRuntimeArg("receiver", 0, object_address);
    PrintF(" ");
    DumpRuntimeArg("key", 0, key_address);
    PrintF(" ");
    DumpRuntimeArg("value", 0, value_address);
    PrintF("\n");
    *out_result = roots.exception().ptr();
    return true;
  }

  object_address = SafeTaggedOrUndefined(isolate, object_address);
  key_address = SafeTaggedOrUndefined(isolate, key_address);
  value_address = SafeTaggedOrUndefined(isolate, value_address);

  Tagged<Object> object_object(object_address);
  if (!IsJSReceiver(object_object)) {
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmInterpreterEntryTrampoline: SetKeyedProperty primitive "
             "receiver bytecode_index=%d object_operand=%d key_operand=%d",
             bytecode_index, object_operand, key_operand);
      DumpRuntimeArg(" receiver", 0, object_address);
      DumpRuntimeArg(" key", 0, key_address);
      DumpRuntimeArg(" value", 0, value_address);
      PrintF("\n");
    }
    if (kTraceWasmFallbackDetails && !g_dumped_set_keyed_primitive_receiver) {
      g_dumped_set_keyed_primitive_receiver = true;
      PrintF("WasmInterpreterEntryTrampoline: SetKeyedProperty bytecode "
             "window length=%d regs=%d params=%d\n",
             bytecode->length(), bytecode->register_count(),
             bytecode->parameter_count());
      interpreter::OperandScale dump_scale = interpreter::OperandScale::kSingle;
      for (int index = 0; index < bytecode->length();) {
        uint8_t dump_opcode = bytecode->get(index);
        interpreter::Bytecode dump_bytecode =
            interpreter::Bytecodes::FromByte(dump_opcode);
        int dump_size = interpreter::Bytecodes::Size(dump_bytecode, dump_scale);
        if (index >= bytecode_index - 96 && index <= bytecode_index + 16) {
          PrintF("  bc index=%d op=0x%x(%s) scale=%d size=%d",
                 index, static_cast<unsigned>(dump_opcode),
                 interpreter::Bytecodes::ToString(dump_bytecode),
                 static_cast<int>(dump_scale), dump_size);
          int operand_count =
              interpreter::Bytecodes::NumberOfOperands(dump_bytecode);
          for (int operand_index = 0; operand_index < operand_count;
               ++operand_index) {
            uint32_t operand_u = ReadBytecodeUnsignedOperand(
                bytecode, index, dump_bytecode, operand_index, dump_scale);
            int32_t operand_s = ReadBytecodeSignedOperand(
                bytecode, index, dump_bytecode, operand_index, dump_scale);
            PrintF(" #%d=u0x%x/s%d", operand_index, operand_u, operand_s);
          }
          PrintF(" raw:");
          for (int raw_index = 0; raw_index < dump_size; ++raw_index) {
            PrintF(" %02x", bytecode->get(index + raw_index));
          }
          PrintF("\n");
        }
        if (interpreter::Bytecodes::IsPrefixScalingBytecode(dump_bytecode)) {
          dump_scale =
              interpreter::Bytecodes::PrefixBytecodeToOperandScale(dump_bytecode);
          index += interpreter::Bytecodes::Size(
              dump_bytecode, interpreter::OperandScale::kSingle);
          continue;
        }
        dump_scale = interpreter::OperandScale::kSingle;
        index += dump_size;
      }
      Tagged<TrustedFixedArray> constant_pool = bytecode->constant_pool();
      int constant_count = constant_pool->length();
      int constant_dump_count = constant_count < 32 ? constant_count : 32;
      PrintF("WasmInterpreterEntryTrampoline: SetKeyedProperty constants "
             "length=%d dump=%d\n",
             constant_count, constant_dump_count);
      for (int constant_index = 0; constant_index < constant_dump_count;
           ++constant_index) {
        DumpRuntimeArg("constant", constant_index,
                       constant_pool->get(constant_index).ptr());
      }
      PrintF("\n");
      DumpInterpreterRegisterValue(
          "setkeyed.object.reg", 0,
          interpreter::Register::FromOperand(object_operand));
      DumpInterpreterRegisterValue(
          "setkeyed.key.reg", 0,
          interpreter::Register::FromOperand(key_operand));
      PrintF("\n");
      int register_count = bytecode->register_count();
      int register_dump_count = register_count < 32 ? register_count : 32;
      for (int register_index = 0; register_index < register_dump_count;
           ++register_index) {
        DumpInterpreterRegisterValue(
            "setkeyed.local", register_index,
            interpreter::Register(register_index));
      }
      PrintF("\n");
      int parameter_dump_count =
          bytecode->parameter_count() < 8 ? bytecode->parameter_count() : 8;
      for (int parameter_index = 0; parameter_index < parameter_dump_count;
           ++parameter_index) {
        DumpInterpreterRegisterValue(
            "setkeyed.param", parameter_index,
            interpreter::Register::FromParameterIndex(parameter_index));
      }
      PrintF("\n");
      DumpRuntimeArg("setkeyed.accumulator", 0,
                     g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
      DumpRuntimeArg("setkeyed.current_context", 0, CurrentInterpreterContext());
      DumpRuntimeArg("setkeyed.frame_context", 0,
                     g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
                         StandardFrameConstants::kContextOffset)]);
      PrintF("\n");
    }
  }

  ++g_set_keyed_property_trace_count;
  if (kTraceWasmFallbackDetails) {
    PrintF("WasmInterpreterEntryTrampoline: keyed store before "
           "count=%d index=%d source_pos=%d stmt_pos=%d object_operand=%d "
           "key_operand=%d ",
           g_set_keyed_property_trace_count, bytecode_index,
           bytecode->SourcePosition(bytecode_index),
           bytecode->SourceStatementPosition(bytecode_index), object_operand,
           key_operand);
    DumpRuntimeArg("object", 0, object_address);
    DumpRuntimeArg(" key", 0, key_address);
    DumpRuntimeArg(" value", 0, value_address);
    DumpRuntimeArg(" context", 0, CurrentInterpreterContext());
    PrintF("\n");
    if (g_set_keyed_property_trace_count <= 64) {
      Address function_address =
          g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
              StandardFrameConstants::kFunctionOffset)];
      DumpFunctionSourceForTrace(function_address);
      PrintF("\n");
      DumpBytecodeWindowForTrace(bytecode, bytecode_index, 48, 48,
                                 "set-keyed-before");
    }
  }

  WasmGCStateScope gc_state(isolate);
  SetCurrentIsolateScope current_isolate_scope(isolate);

  Tagged<Context> saved_context = isolate->context();
  Address context_address = CurrentInterpreterContext();
  bool switched_context = false;
  if (IsSafeTaggedHandleValue(context_address) &&
      IsContext(Tagged<Object>(context_address))) {
    isolate->set_context(Cast<Context>(Tagged<Object>(context_address)));
    switched_context = true;
  }

  HandleScope scope(isolate);
  DirectHandle<JSAny> object =
      direct_handle(Cast<JSAny>(Tagged<Object>(object_address)), isolate);
  DirectHandle<Object> key = direct_handle(Tagged<Object>(key_address), isolate);
  DirectHandle<Object> value =
      direct_handle(Tagged<Object>(value_address), isolate);

  if (IsJSObject(*object) && IsSymbol(*key) &&
      Cast<Symbol>(*key)->IsPrivate() &&
      IsKnownReadOnlyRootValue(isolate, value_address)) {
    DirectHandle<JSObject> receiver = Cast<JSObject>(object);
    if (!receiver->map(isolate)->is_dictionary_map()) {
      JSObject::NormalizeProperties(isolate, receiver,
                                    CLEAR_INOBJECT_PROPERTIES, 1,
                                    "wasm32 private root keyed store");
      if (kTraceWasmFallbackDetails) {
        PrintF("WasmInterpreterEntryTrampoline: keyed store normalized "
               "private-root receiver count=%d index=%d ",
               g_set_keyed_property_trace_count, bytecode_index);
        DumpRuntimeArg("object", 0, (*receiver).ptr());
        DumpRuntimeArg(" key", 0, (*key).ptr());
        DumpRuntimeArg(" value", 0, (*value).ptr());
        PrintF("\n");
      }
    }
  }

  MaybeDirectHandle<Object> maybe_result =
      Runtime::SetObjectProperty(isolate, object, key, value,
                                 StoreOrigin::kMaybeKeyed, Just(kThrowOnError));
  DirectHandle<Object> result;
  if (!maybe_result.ToHandle(&result)) {
    if (switched_context) isolate->set_context(saved_context);
    *out_result = roots.exception().ptr();
    return true;
  }
  if (switched_context) isolate->set_context(saved_context);
  *out_result = (*result).ptr();
  return true;
}

bool TryRunDefineKeyedOwnPropertyBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kDefineKeyedOwnProperty) {
    return false;
  }

  int32_t object_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  int32_t key_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                operand_scale);
  uint32_t flags =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 2,
                                  operand_scale);

  Address object_address = ReadInterpreterRegister(
      interpreter::Register::FromOperand(object_operand));
  Address key_address =
      ReadInterpreterRegister(interpreter::Register::FromOperand(key_operand));
  Address value_address =
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
  ReadOnlyRoots roots(isolate);

  if (!IsSafeJSAnyForWasmPropertyLookup(isolate, object_address)) {
    PrintF("WasmInterpreterEntryTrampoline: define-keyed receiver invalid");
    DumpRuntimeArg(" receiver", 0, object_address);
    DumpRuntimeArg(" key", 0, key_address);
    DumpRuntimeArg(" value", 0, value_address);
    PrintF("\n");
    *out_result = IsSafeTaggedRootValue(isolate, value_address)
                      ? value_address
                      : roots.undefined_value().ptr();
    return true;
  }

  object_address = SafeTaggedOrUndefined(isolate, object_address);
  key_address = SafeTaggedOrUndefined(isolate, key_address);
  value_address = SafeTaggedOrUndefined(isolate, value_address);

  WasmGCStateScope gc_state(isolate);
  SetCurrentIsolateScope current_isolate_scope(isolate);
  HandleScope scope(isolate);

  Tagged<Context> saved_context = isolate->context();
  Address context_address = CurrentInterpreterContext();
  bool switched_context = false;
  if (IsSafeTaggedHandleValue(context_address) &&
      IsContext(Tagged<Object>(context_address))) {
    isolate->set_context(Cast<Context>(Tagged<Object>(context_address)));
    switched_context = true;
  }

  Tagged<Object> object_object(object_address);
  Tagged<Object> key_object(key_address);
  Tagged<Object> value_object(value_address);
  DirectHandle<JSAny> object =
      direct_handle(Cast<JSAny>(object_object), isolate);
  DirectHandle<Object> key = direct_handle(key_object, isolate);
  DirectHandle<Object> value = direct_handle(value_object, isolate);

  bool key_is_private_name = false;
  bool key_is_private_brand = false;
  Address key_description = roots.undefined_value().ptr();
  if (IsSymbol(key_object)) {
    Tagged<Symbol> key_symbol = Cast<Symbol>(key_object);
    key_is_private_name = key_symbol->is_private_name();
    key_is_private_brand = key_symbol->is_private_brand();
    key_description = key_symbol->description().ptr();
  }

  if (key_is_private_name) {
    bool object_is_receiver =
        IsSafeTaggedRootValue(isolate, object_address) &&
        IsJSReceiver(object_object);
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmInterpreterEntryTrampoline: define-keyed private "
             "flags=%u object_operand=%d key_operand=%d object_is_receiver=%d "
             "private_brand=%d",
             flags, object_operand, key_operand, object_is_receiver,
             key_is_private_brand);
      DumpRuntimeArg(" object", 0, object_address);
      DumpRuntimeArg(" key", 0, key_address);
      DumpRuntimeArg(" key_desc", 0, key_description);
      DumpRuntimeArg(" value", 0, value_address);
      DumpRuntimeArg(" current_context", 0, CurrentInterpreterContext());
      PrintF("\n");
    }

    if (!object_is_receiver) {
      DirectHandle<String> name_string =
          IsString(Tagged<Object>(key_description), isolate)
              ? direct_handle(Cast<String>(Tagged<Object>(key_description)),
                              isolate)
              : isolate->factory()->empty_string();
      isolate->Throw(*isolate->factory()->NewTypeError(
          MessageTemplate::kNonObjectPrivateNameAccess, name_string, object));
      if (switched_context) isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
  }

  if ((flags & static_cast<uint32_t>(
                   DefineKeyedOwnPropertyFlag::kSetFunctionName)) != 0 &&
      IsJSFunction(*value) && IsName(*key)) {
    DirectHandle<JSFunction> function = Cast<JSFunction>(value);
    if (!Wasm32JSFunctionShared(*function)->HasSharedName()) {
      DirectHandle<Name> name = Cast<Name>(key);
      if (!JSFunction::SetName(isolate, function, name,
                               isolate->factory()->empty_string())) {
        if (switched_context) isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
    }
  }

  MaybeDirectHandle<Object> maybe_result =
      Runtime::DefineObjectOwnProperty(isolate, object, key, value,
                                       StoreOrigin::kMaybeKeyed);
  DirectHandle<Object> result;
  if (!maybe_result.ToHandle(&result)) {
    if (switched_context) isolate->set_context(saved_context);
    *out_result = roots.exception().ptr();
    return true;
  }
  if (switched_context) isolate->set_context(saved_context);
  *out_result = (*result).ptr();
  return true;
}

bool TryRunDeletePropertyBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kDeletePropertyStrict &&
      bytecode_enum != interpreter::Bytecode::kDeletePropertySloppy) {
    return false;
  }

  int32_t object_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  Address object_address = ReadInterpreterRegister(
      interpreter::Register::FromOperand(object_operand));
  Address key_address = g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];

  object_address = SafeTaggedOrUndefined(isolate, object_address);
  key_address = SafeTaggedOrUndefined(isolate, key_address);

  WasmGCStateScope gc_state(isolate);
  SetCurrentIsolateScope current_isolate_scope(isolate);

  Tagged<Context> saved_context = isolate->context();
  Address context_address = CurrentInterpreterContext();
  bool switched_context = false;
  if (IsSafeTaggedHandleValue(context_address) &&
      IsContext(Tagged<Object>(context_address))) {
    isolate->set_context(Cast<Context>(Tagged<Object>(context_address)));
    switched_context = true;
  }

  HandleScope scope(isolate);
  DirectHandle<Object> object =
      direct_handle(Tagged<Object>(object_address), isolate);
  DirectHandle<Object> key = direct_handle(Tagged<Object>(key_address), isolate);
  Tagged<Object> key_object = *key;
  if (kTraceWasmFallbackDetails &&
      (IsSymbol(key_object) || kTraceWasmFallbackDetails) &&
      g_delete_property_trace_count < 64) {
    ++g_delete_property_trace_count;
    bool key_is_private_name = false;
    bool key_is_private_brand = false;
    Address key_description = ReadOnlyRoots(isolate).undefined_value().ptr();
    if (IsSymbol(key_object)) {
      Tagged<Symbol> key_symbol = Cast<Symbol>(key_object);
      key_is_private_name = key_symbol->is_private_name();
      key_is_private_brand = key_symbol->is_private_brand();
      key_description = key_symbol->description().ptr();
    }
    PrintF("WasmInterpreterEntryTrampoline: delete property trace count=%d "
           "bytecode=%s index=%d source_pos=%d stmt_pos=%d object_operand=%d "
           "strict=%d private_name=%d private_brand=%d",
           g_delete_property_trace_count,
           interpreter::Bytecodes::ToString(bytecode_enum), bytecode_index,
           bytecode->SourcePosition(bytecode_index),
           bytecode->SourceStatementPosition(bytecode_index), object_operand,
           bytecode_enum == interpreter::Bytecode::kDeletePropertyStrict ? 1 : 0,
           key_is_private_name, key_is_private_brand);
    DumpRuntimeArg(" object", 0, object_address);
    DumpRuntimeArg(" key", 0, key_address);
    DumpRuntimeArg(" key_desc", 0, key_description);
    Address function_address =
        g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
            StandardFrameConstants::kFunctionOffset)];
    DumpFunctionSourceForTrace(function_address);
    PrintF("\n");
  }

  DirectHandle<JSReceiver> receiver;
  if (!Object::ToObject(isolate, object).ToHandle(&receiver)) {
    if (switched_context) isolate->set_context(saved_context);
    *out_result = ReadOnlyRoots(isolate).exception().ptr();
    return true;
  }

  LanguageMode language_mode_enum =
      bytecode_enum == interpreter::Bytecode::kDeletePropertyStrict
          ? LanguageMode::kStrict
          : LanguageMode::kSloppy;
  Maybe<bool> maybe_result;
  if (IsName(*key)) {
    PropertyKey lookup_key(isolate, Cast<Name>(key));
    maybe_result = JSReceiver::DeletePropertyOrElement(
        isolate, receiver, lookup_key, language_mode_enum);
  } else {
    maybe_result = Runtime::DeleteObjectProperty(isolate, receiver, key,
                                                 language_mode_enum);
  }
  if (maybe_result.IsNothing()) {
    if (switched_context) isolate->set_context(saved_context);
    *out_result = ReadOnlyRoots(isolate).exception().ptr();
    return true;
  }

  Address result = isolate->heap()->ToBoolean(maybe_result.FromJust()).ptr();
  if (switched_context) isolate->set_context(saved_context);
  *out_result = result;
  return true;
}

bool TryRunCloneObjectBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kCloneObject) return false;

  int32_t source_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  uint32_t encoded_flags =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                  operand_scale);
  int flags = interpreter::CreateObjectLiteralFlags::FlagsBits::decode(
      static_cast<uint8_t>(encoded_flags));

  Address source_address = SafeTaggedRootOrUndefined(
      isolate,
      ReadInterpreterRegister(interpreter::Register::FromOperand(source_operand)));

  WasmGCStateScope gc_state(isolate);
  SetCurrentIsolateScope current_isolate_scope(isolate);

  Tagged<Context> saved_context = isolate->context();
  Address context_address = CurrentInterpreterContext();
  bool switched_context = false;
  if (IsSafeTaggedRootValue(isolate, context_address) &&
      IsContext(Tagged<Object>(context_address))) {
    isolate->set_context(Cast<Context>(Tagged<Object>(context_address)));
    switched_context = true;
  }

  Address args[2] = {Smi::FromInt(flags).ptr(), source_address};
  WasmTemporaryRootScope args_roots(isolate, args, 2);
  Address result =
      Runtime_CloneObjectIC_Slow(2, &args_roots.data()[1], isolate);
  if (switched_context) isolate->set_context(saved_context);

  *out_result = result;
  return true;
}

bool TryRunCreateArrayLiteralBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kCreateArrayLiteral) return false;

  uint32_t elements_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
  uint32_t literal_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                  operand_scale);
  uint32_t flags =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 2,
                                  operand_scale);

  Tagged<TrustedFixedArray> constant_pool = bytecode->constant_pool();
  if (elements_index >= static_cast<uint32_t>(constant_pool->length())) {
    PrintF("WasmInterpreterEntryTrampoline: bad array literal index=%u "
           "length=%d\n",
           elements_index, constant_pool->length());
    return false;
  }
  Tagged<Object> elements_object = constant_pool->get(elements_index);
  if (!IsArrayBoilerplateDescription(elements_object)) {
    PrintF("WasmInterpreterEntryTrampoline: array literal constant invalid "
           "index=%u ",
           elements_index);
    DumpRuntimeArg("value", 0, elements_object.ptr());
    PrintF("\n");
    return false;
  }

  Address feedback_address =
      g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
          InterpreterFrameConstants::kFeedbackVectorFromFp)];
  if (!IsFeedbackVectorAddress(feedback_address)) {
    feedback_address = ReadOnlyRoots(isolate).undefined_value().ptr();
  }

  Address args[4] = {
      Smi::FromInt(static_cast<int>(flags)).ptr(),
      elements_object.ptr(),
      TaggedIndex::FromIntptr(static_cast<intptr_t>(literal_index)).ptr(),
      feedback_address,
  };
  *out_result = Runtime_CreateArrayLiteral(4, &args[3], isolate);
  return true;
}

bool TryRunCreateRestParameterBytecode(
    Isolate* isolate, interpreter::Bytecode bytecode_enum,
    Address* out_result) {
  bool create_unmapped_arguments =
      bytecode_enum == interpreter::Bytecode::kCreateUnmappedArguments;
  bool create_mapped_arguments =
      bytecode_enum == interpreter::Bytecode::kCreateMappedArguments;
  if (bytecode_enum != interpreter::Bytecode::kCreateRestParameter &&
      !create_unmapped_arguments && !create_mapped_arguments) {
    return false;
  }

  Address function_address =
      g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
          StandardFrameConstants::kFunctionOffset)];
  if (!IsSafeTaggedHandleValue(function_address) ||
      !IsJSFunction(Tagged<Object>(function_address))) {
    PrintF("WasmInterpreterEntryTrampoline: rest parameter function invalid ");
    DumpRuntimeArg("function", 0, function_address);
    PrintF("\n");
    *out_result = ReadOnlyRoots(isolate).undefined_value().ptr();
    return true;
  }

  Tagged<JSFunction> function =
      Cast<JSFunction>(Tagged<Object>(function_address));
  int start_index = (create_unmapped_arguments || create_mapped_arguments)
                        ? 0
                        : Wasm32JSFunctionShared(function)
                              ->internal_formal_parameter_count_without_receiver();
  int argc_with_receiver = static_cast<int>(
      g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
          StandardFrameConstants::kArgCOffset)]);
  int actual_argc = argc_with_receiver - kJSArgcReceiverSlots;
  if (actual_argc < 0) actual_argc = 0;
  int rest_count = actual_argc > start_index ? actual_argc - start_index : 0;

  WasmGCStateScope gc_state(isolate);
  SetCurrentIsolateScope current_isolate_scope(isolate);

  Tagged<Context> saved_context = isolate->context();
  Address context_address = CurrentInterpreterContext();
  bool switched_context = false;
  if (IsSafeTaggedHandleValue(context_address) &&
      IsContext(Tagged<Object>(context_address))) {
    isolate->set_context(Cast<Context>(Tagged<Object>(context_address)));
    switched_context = true;
  }

  HandleScope scope(isolate);
  DirectHandle<FixedArray> elements =
      isolate->factory()->NewFixedArray(rest_count);
  {
    DisallowGarbageCollection no_gc;
    WriteBarrierMode mode = elements->GetWriteBarrierMode(no_gc);
    for (int i = 0; i < rest_count; ++i) {
      Address value = SafeRuntimeArgOrUndefined(
          isolate,
          ReadInterpreterRegister(
              interpreter::Register::FromParameterIndex(1 + start_index + i)));
      elements->set(i, Tagged<Object>(value), mode);
    }
  }
  if (create_unmapped_arguments || create_mapped_arguments) {
    DirectHandle<JSFunction> callee(function, isolate);
    DirectHandle<JSObject> result =
        isolate->factory()->NewArgumentsObject(callee, rest_count);
    if (rest_count > 0) result->set_elements(*elements);
    *out_result = (*result).ptr();
  } else {
    DirectHandle<JSArray> result = isolate->factory()->NewJSArrayWithElements(
        elements, PACKED_ELEMENTS, rest_count);
    *out_result = (*result).ptr();
  }
  if (switched_context) isolate->set_context(saved_context);
  return true;
}

bool TryRunCreateEmptyArrayLiteralBytecode(
    Isolate* isolate, interpreter::Bytecode bytecode_enum,
    Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kCreateEmptyArrayLiteral) {
    return false;
  }

  Tagged<Context> saved_context = isolate->context();
  Address context_address = CurrentInterpreterContext();
  bool switched_context = false;
  if (IsSafeTaggedHandleValue(context_address) &&
      IsContext(Tagged<Object>(context_address))) {
    isolate->set_context(Cast<Context>(Tagged<Object>(context_address)));
    switched_context = true;
  }

  HandleScope scope(isolate);
  Handle<JSArray> result = isolate->factory()->NewJSArray(0);
  *out_result = (*result).ptr();
  if (switched_context) isolate->set_context(saved_context);
  return true;
}

bool TryRunCreateArrayFromIterableBytecode(
    Isolate* isolate, interpreter::Bytecode bytecode_enum,
    Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kCreateArrayFromIterable) {
    return false;
  }

  ReadOnlyRoots roots(isolate);
  Address iterable_address =
      SafeTaggedOrUndefined(
          isolate, g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
  Tagged<Object> iterable(iterable_address);

  Tagged<Context> saved_context = isolate->context();
  Address context_address = CurrentInterpreterContext();
  bool switched_context = false;
  if (IsSafeTaggedHandleValue(context_address) &&
      IsContext(Tagged<Object>(context_address))) {
    isolate->set_context(Cast<Context>(Tagged<Object>(context_address)));
    switched_context = true;
  }

  HandleScope scope(isolate);
  if (IsJSArray(iterable)) {
    DirectHandle<JSArray> source(Cast<JSArray>(iterable), isolate);
    uint32_t length = 0;
    if (!Object::ToArrayLength(source->length(), &length) ||
        length > static_cast<uint32_t>(FixedArray::kMaxLength)) {
      if (switched_context) isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<FixedArray> elements =
        isolate->factory()->NewFixedArray(static_cast<int>(length));
    for (uint32_t index = 0; index < length; ++index) {
      DirectHandle<Object> value;
      if (!JSReceiver::GetElement(isolate, source, index).ToHandle(&value)) {
        if (switched_context) isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
      elements->set(static_cast<int>(index), *value);
    }
    DirectHandle<JSArray> result = isolate->factory()->NewJSArrayWithElements(
        elements, PACKED_ELEMENTS, static_cast<int>(length));
    *out_result = (*result).ptr();
    if (switched_context) isolate->set_context(saved_context);
    return true;
  }

  DirectHandle<Object> values[kMaxWasmCallArgs];
  int value_count = 0;
  if (!AddSpreadCallArguments(isolate, values, &value_count,
                              iterable_address)) {
    if (switched_context) isolate->set_context(saved_context);
    *out_result = roots.exception().ptr();
    return true;
  }
  DirectHandle<FixedArray> elements =
      isolate->factory()->NewFixedArray(value_count);
  for (int index = 0; index < value_count; ++index) {
    elements->set(index, *values[index]);
  }
  DirectHandle<JSArray> result = isolate->factory()->NewJSArrayWithElements(
      elements, PACKED_ELEMENTS, value_count);
  *out_result = (*result).ptr();
  if (switched_context) isolate->set_context(saved_context);
  return true;
}

bool TryRunCreateRegExpLiteralBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kCreateRegExpLiteral) {
    return false;
  }

  uint32_t pattern_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
  uint32_t literal_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                  operand_scale);
  uint32_t flags =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 2,
                                  operand_scale);

  Tagged<TrustedFixedArray> constant_pool = bytecode->constant_pool();
  if (pattern_index >= static_cast<uint32_t>(constant_pool->length())) {
    PrintF("WasmInterpreterEntryTrampoline: bad regexp literal index=%u "
           "length=%d\n",
           pattern_index, constant_pool->length());
    return false;
  }
  Tagged<Object> pattern_object = constant_pool->get(pattern_index);
  if (!IsString(pattern_object)) {
    PrintF("WasmInterpreterEntryTrampoline: regexp literal pattern invalid "
           "index=%u ",
           pattern_index);
    DumpRuntimeArg("value", 0, pattern_object.ptr());
    PrintF("\n");
    return false;
  }

  if ((flags & JSRegExp::kGlobal) != 0 &&
      IsNodePercentSpecifierPattern(Cast<String>(pattern_object))) {
    HandleScope scope(isolate);
    WasmGCStateScope gc_state(isolate);
    SetCurrentIsolateScope current_isolate_scope(isolate);
    DirectHandle<String> pattern =
        direct_handle(Cast<String>(pattern_object), isolate);
    DirectHandle<JSObject> regexp_marker =
        NewWasm32PlainJSObject(isolate);
    DirectHandle<String> pattern_marker =
        isolate->factory()->InternalizeUtf8String(kWasm32RegExpPatternMarker);
    DirectHandle<String> flags_marker =
        isolate->factory()->InternalizeUtf8String(kWasm32RegExpFlagsMarker);
    DirectHandle<Object> flags_value =
        direct_handle(Smi::FromInt(static_cast<int>(flags)), isolate);
    DirectHandle<Object> zero = direct_handle(Smi::zero(), isolate);
    JSObject::AddProperty(isolate, regexp_marker, pattern_marker, pattern,
                          NONE);
    JSObject::AddProperty(isolate, regexp_marker, flags_marker, flags_value,
                          NONE);
    JSObject::AddProperty(isolate, regexp_marker,
                          isolate->factory()->lastIndex_string(), zero, NONE);
    *out_result = (*regexp_marker).ptr();
    return true;
  }

  Address feedback_address =
      g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
          InterpreterFrameConstants::kFeedbackVectorFromFp)];
  if (!IsFeedbackVectorAddress(feedback_address)) {
    feedback_address = ReadOnlyRoots(isolate).undefined_value().ptr();
  }

  Address args[4] = {
      Smi::FromInt(static_cast<int>(flags)).ptr(),
      pattern_object.ptr(),
      TaggedIndex::FromIntptr(static_cast<intptr_t>(literal_index)).ptr(),
      feedback_address,
  };
  *out_result = Runtime_CreateRegExpLiteral(4, &args[3], isolate);
  return true;
}

bool TryRunStaInArrayLiteralBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kStaInArrayLiteral) return false;

  int32_t array_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  int32_t index_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                operand_scale);
  uint32_t slot_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 2,
                                  operand_scale);

  Address array_address =
      ReadInterpreterRegister(interpreter::Register::FromOperand(array_operand));
  Address index_address =
      ReadInterpreterRegister(interpreter::Register::FromOperand(index_operand));
  Address value_address =
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
#ifdef __wasi__
  if (bytecode->length() > 200000 && bytecode_index == 211665) {
    std::fprintf(stderr,
                 "WASM32_MAIN_ARRAY_STORE array=0x%x index=0x%x value=0x%x "
                 "array_plausible=%d array_map=%d is_array=%d "
                 "index_plausible=%d index_map=%d is_number=%d\n",
                 static_cast<unsigned>(array_address),
                 static_cast<unsigned>(index_address),
                 static_cast<unsigned>(value_address),
                 IsPlausibleTaggedValue(array_address) ? 1 : 0,
                 HasReadableHeapObjectMap(array_address) ? 1 : 0,
                 IsPlausibleTaggedValue(array_address) &&
                         HasReadableHeapObjectMap(array_address) &&
                         IsJSArray(Tagged<Object>(array_address))
                     ? 1
                     : 0,
                 IsPlausibleTaggedValue(index_address) ? 1 : 0,
                 HAS_SMI_TAG(index_address) ||
                         HasReadableHeapObjectMap(index_address)
                     ? 1
                     : 0,
                 IsPlausibleTaggedValue(index_address) &&
                         (HAS_SMI_TAG(index_address) ||
                          HasReadableHeapObjectMap(index_address)) &&
                         IsNumber(Tagged<Object>(index_address))
                     ? 1
                     : 0);
    std::fflush(stderr);
  }
#endif

  if (!IsPlausibleTaggedValue(array_address) ||
      !HasReadableHeapObjectMap(array_address) ||
      !IsJSArray(Tagged<Object>(array_address))) {
    PrintF("WasmInterpreterEntryTrampoline: array literal store receiver "
           "invalid ");
    DumpRuntimeArg("array", 0, array_address);
    PrintF("\n");
    return false;
  }
  if (!IsPlausibleTaggedValue(index_address) ||
      (!HAS_SMI_TAG(index_address) && !HasReadableHeapObjectMap(index_address)) ||
      !IsNumber(Tagged<Object>(index_address))) {
    PrintF("WasmInterpreterEntryTrampoline: array literal store index invalid ");
    DumpRuntimeArg("index", 0, index_address);
    PrintF("\n");
    return false;
  }
  if (!IsSafeTaggedRootValue(isolate, value_address)) {
    value_address = ReadOnlyRoots(isolate).undefined_value().ptr();
  }

  Address feedback_address =
      g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
          InterpreterFrameConstants::kFeedbackVectorFromFp)];
  HandleScope scope(isolate);
  Handle<FeedbackVector> vector;
  if (IsFeedbackVectorAddress(feedback_address)) {
    vector =
        handle(Cast<FeedbackVector>(Tagged<Object>(feedback_address)), isolate);
  }

  FeedbackSlot vector_slot = FeedbackVector::ToSlot(slot_index);
  DirectHandle<JSArray> array =
      direct_handle(Cast<JSArray>(Tagged<Object>(array_address)), isolate);
  Handle<Object> index = handle(Tagged<Object>(index_address), isolate);
  DirectHandle<Object> value =
      direct_handle(Tagged<Object>(value_address), isolate);
  StoreInArrayLiteralIC ic(isolate, vector, vector_slot);

  DirectHandle<Object> result;
  if (!ic.Store(array, index, value).ToHandle(&result)) {
    *out_result = ReadOnlyRoots(isolate).exception().ptr();
    return true;
  }
  *out_result = (*result).ptr();
  return true;
}

bool TryCreateWasm32ObjectLiteral(
    Isolate* isolate, DirectHandle<ObjectBoilerplateDescription> description,
    int flags, int depth, DirectHandle<JSObject>* out_object);

bool TryCreateWasm32ArrayLiteral(
    Isolate* isolate, DirectHandle<ArrayBoilerplateDescription> description,
    int depth, DirectHandle<JSArray>* out_array) {
  if (depth > 8) return false;
  ElementsKind elements_kind = description->elements_kind();
  Handle<FixedArrayBase> constant_elements(
      description->constant_elements(isolate), isolate);
  Handle<FixedArrayBase> copied_elements;
  if (IsDoubleElementsKind(elements_kind)) {
    copied_elements =
        isolate->factory()->CopyFixedDoubleArray(
            Cast<FixedDoubleArray>(constant_elements));
  } else if (IsSmiOrObjectElementsKind(elements_kind)) {
    Handle<FixedArray> object_elements =
        isolate->factory()->CopyFixedArray(Cast<FixedArray>(constant_elements));
    for (int i = 0; i < object_elements->length(); ++i) {
      Tagged<Object> element = object_elements->get(i);
      if (!IsHeapObject(element)) continue;
      Tagged<HeapObject> heap_element = Cast<HeapObject>(element);
      if (IsObjectBoilerplateDescription(heap_element)) {
        DirectHandle<JSObject> nested;
        DirectHandle<ObjectBoilerplateDescription> nested_description(
            Cast<ObjectBoilerplateDescription>(heap_element), isolate);
        if (!TryCreateWasm32ObjectLiteral(isolate, nested_description,
                                          nested_description->flags(),
                                          depth + 1, &nested)) {
          return false;
        }
        object_elements->set(i, *nested);
      } else if (IsArrayBoilerplateDescription(heap_element, isolate)) {
        DirectHandle<JSArray> nested;
        DirectHandle<ArrayBoilerplateDescription> nested_description(
            Cast<ArrayBoilerplateDescription>(heap_element), isolate);
        if (!TryCreateWasm32ArrayLiteral(isolate, nested_description,
                                         depth + 1, &nested)) {
          return false;
        }
        object_elements->set(i, *nested);
      }
    }
    copied_elements = object_elements;
  } else {
    return false;
  }

  *out_array = isolate->factory()->NewJSArrayWithElements(
      copied_elements, elements_kind, copied_elements->length());
  return true;
}

bool TryCreateWasm32ObjectLiteral(
    Isolate* isolate, DirectHandle<ObjectBoilerplateDescription> description,
    int flags, int depth, DirectHandle<JSObject>* out_object) {
  if (depth > 8) return false;

  Tagged<Context> native_context_value = isolate->context();
  if (!TryResolveWasm32NativeContext(isolate->context(), &native_context_value) ||
      !IsNativeContext(native_context_value)) {
    return false;
  }
  DirectHandle<NativeContext> native_context(
      Cast<NativeContext>(native_context_value), isolate);
  bool use_fast_elements = (flags & ObjectLiteral::kFastElements) != 0;
  bool has_null_prototype = (flags & ObjectLiteral::kHasNullPrototype) != 0;
  int number_of_properties = description->backing_store_size();

  DirectHandle<Map> map =
      has_null_prototype
          ? direct_handle(native_context->slow_object_with_null_prototype_map(),
                          isolate)
          : isolate->factory()->ObjectLiteralMapFromCache(
                native_context, number_of_properties);
  Handle<JSObject> object = isolate->factory()->NewFastOrSlowJSObjectFromMap(
      map, number_of_properties);
  if (!use_fast_elements) JSObject::NormalizeElements(isolate, object);

  int property_count = description->boilerplate_properties_count();

  for (int index = 0; index < property_count; ++index) {
    DirectHandle<Object> key(description->name(index), isolate);
    Handle<Object> value(description->value(index), isolate);

    if (IsHeapObject(*value)) {
      Tagged<HeapObject> heap_value = Cast<HeapObject>(*value);
      if (IsObjectBoilerplateDescription(heap_value)) {
        DirectHandle<JSObject> nested;
        DirectHandle<ObjectBoilerplateDescription> nested_description(
            Cast<ObjectBoilerplateDescription>(heap_value), isolate);
        if (!TryCreateWasm32ObjectLiteral(isolate, nested_description,
                                          nested_description->flags(),
                                          depth + 1, &nested)) {
          return false;
        }
        value = handle(*nested, isolate);
      } else if (IsArrayBoilerplateDescription(heap_value, isolate)) {
        DirectHandle<JSArray> nested;
        DirectHandle<ArrayBoilerplateDescription> nested_description(
            Cast<ArrayBoilerplateDescription>(heap_value), isolate);
        if (!TryCreateWasm32ArrayLiteral(isolate, nested_description,
                                         depth + 1, &nested)) {
          return false;
        }
        value = handle(*nested, isolate);
      }
    }

    if (IsUninitialized(*value, isolate)) {
      value = handle(Smi::zero(), isolate);
    }

    uint32_t element_index = 0;
    MaybeDirectHandle<Object> maybe_result;
    if (Object::ToArrayIndex(*key, &element_index)) {
      maybe_result = JSObject::SetOwnElementIgnoreAttributes(
          object, element_index, value, NONE);
    } else if (IsString(*key)) {
      maybe_result = JSObject::SetOwnPropertyIgnoreAttributes(
          object, Cast<String>(key), value, NONE);
    } else {
      return false;
    }
    DirectHandle<Object> ignored;
    if (!maybe_result.ToHandle(&ignored)) return false;
  }

  *out_object = object;
  return true;
}

bool TryRunCreateObjectLiteralBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kCreateObjectLiteral) {
    return false;
  }

  uint32_t description_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale);
  uint32_t literal_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                  operand_scale);
  uint32_t flags =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 2,
                                  operand_scale);

  Tagged<TrustedFixedArray> constant_pool = bytecode->constant_pool();
  if (description_index >= static_cast<uint32_t>(constant_pool->length())) {
    PrintF("WasmInterpreterEntryTrampoline: bad object literal index=%u "
           "length=%d\n",
           description_index, constant_pool->length());
    return false;
  }
  Tagged<Object> description_object = constant_pool->get(description_index);
  if (!IsObjectBoilerplateDescription(description_object)) {
    PrintF("WasmInterpreterEntryTrampoline: object literal constant invalid "
           "index=%u ",
           description_index);
    DumpRuntimeArg("value", 0, description_object.ptr());
    PrintF("\n");
    return false;
  }

  USE(literal_index);
  HandleScope scope(isolate);
  WasmGCStateScope gc_state(isolate);
  SetCurrentIsolateScope current_isolate_scope(isolate);
  Tagged<Context> saved_context = isolate->context();
  Address context_address = CurrentInterpreterContext();
  bool switched_context = false;
  if (IsSafeTaggedHandleValue(context_address) &&
      IsContext(Tagged<Object>(context_address))) {
    isolate->set_context(Cast<Context>(Tagged<Object>(context_address)));
    switched_context = true;
  }

  USE(literal_index);
  int runtime_flags = interpreter::CreateObjectLiteralFlags::FlagsBits::decode(
      static_cast<uint8_t>(flags));
  DirectHandle<ObjectBoilerplateDescription> description(
      Cast<ObjectBoilerplateDescription>(description_object), isolate);
  DirectHandle<JSObject> object;
  if (TryCreateWasm32ObjectLiteral(isolate, description, runtime_flags, 0,
                                   &object)) {
    if (switched_context) isolate->set_context(saved_context);
    *out_result = (*object).ptr();
    return true;
  }

  Address maybe_vector =
      g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
          InterpreterFrameConstants::kFeedbackVectorFromFp)];
  if (!IsFeedbackVector(Tagged<Object>(maybe_vector))) {
    maybe_vector = ReadOnlyRoots(isolate).undefined_value().ptr();
  }
  Address args[4] = {Smi::FromInt(runtime_flags).ptr(),
                     description_object.ptr(),
                     TaggedIndex::FromIntptr(
                         static_cast<intptr_t>(literal_index)).ptr(),
                     maybe_vector};
  WasmTemporaryRootScope args_roots(isolate, args, 4);
  Address runtime_result =
      Runtime_CreateObjectLiteral(4, &args_roots.data()[3], isolate);
  if (IsSafeTaggedHandleValue(runtime_result) &&
      IsJSObject(Tagged<Object>(runtime_result))) {
    if (switched_context) isolate->set_context(saved_context);
    *out_result = runtime_result;
    return true;
  }
  if (switched_context) isolate->set_context(saved_context);
  return false;
}

bool TryRunCreateEmptyObjectLiteralBytecode(
    Isolate* isolate, interpreter::Bytecode bytecode_enum,
    Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kCreateEmptyObjectLiteral) {
    return false;
  }

  Tagged<Context> saved_context = isolate->context();
  Address context_address = CurrentInterpreterContext();
  bool switched_context = false;
  if (IsSafeTaggedHandleValue(context_address) &&
      IsContext(Tagged<Object>(context_address))) {
    isolate->set_context(Cast<Context>(Tagged<Object>(context_address)));
    switched_context = true;
  }

  HandleScope scope(isolate);
  Handle<JSObject> result = NewWasm32PlainJSObject(isolate);
  *out_result = (*result).ptr();
  if (switched_context) isolate->set_context(saved_context);
  return true;
}

bool TryRunDefineKeyedOwnPropertyInLiteralBytecode(
    Isolate* isolate, Tagged<BytecodeArray> bytecode, int bytecode_index,
    interpreter::Bytecode bytecode_enum,
    interpreter::OperandScale operand_scale, Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kDefineKeyedOwnPropertyInLiteral) {
    return false;
  }

  int32_t object_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  int32_t name_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 1,
                                operand_scale);
  uint32_t flags =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 2,
                                  operand_scale);
  uint32_t slot_index =
      ReadBytecodeUnsignedOperand(bytecode, bytecode_index, bytecode_enum, 3,
                                  operand_scale);

  Address object_address =
      ReadInterpreterRegister(interpreter::Register::FromOperand(object_operand));
  Address name_address =
      ReadInterpreterRegister(interpreter::Register::FromOperand(name_operand));
  Address value_address =
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
  ReadOnlyRoots roots(isolate);

  if (!IsPlausibleTaggedValue(object_address) ||
      !HasReadableHeapObjectMap(object_address) ||
      !IsJSReceiver(Tagged<Object>(object_address))) {
    PrintF("WasmInterpreterEntryTrampoline: keyed literal receiver invalid ");
    DumpRuntimeArg("object", 0, object_address);
    PrintF(" ");
    DumpRuntimeArg("name", 0, name_address);
    PrintF("\n");
    *out_result = IsSafeTaggedRootValue(isolate, value_address)
                      ? value_address
                      : roots.undefined_value().ptr();
    return true;
  }

  if (!IsSafeTaggedRootValue(isolate, name_address)) {
    name_address = roots.undefined_value().ptr();
  }
  if (!IsSafeTaggedRootValue(isolate, value_address)) {
    value_address = roots.undefined_value().ptr();
  }

  Tagged<Object> name_object(name_address);
  if (IsTheHole(name_object, roots) || IsUninitialized(name_object, roots)) {
    name_address = roots.undefined_value().ptr();
  }

  // The wasm32 interpreter frame can carry placeholder feedback state while
  // bootstrapping. Avoid the optional FeedbackNexus update and let the runtime
  // perform the actual literal define.
  Address feedback_address = roots.undefined_value().ptr();

  Address args[6] = {
      TaggedIndex::FromIntptr(static_cast<intptr_t>(slot_index)).ptr(),
      feedback_address,
      Smi::FromInt(static_cast<int>(flags)).ptr(),
      value_address,
      name_address,
      object_address,
  };
  *out_result = Runtime_DefineKeyedOwnPropertyInLiteral(6, &args[5], isolate);
  return true;
}

bool TryRunGetIteratorBytecode(Isolate* isolate, Tagged<BytecodeArray> bytecode,
                               int bytecode_index,
                               interpreter::Bytecode bytecode_enum,
                               interpreter::OperandScale operand_scale,
                               Address* out_result) {
  if (bytecode_enum != interpreter::Bytecode::kGetIterator) return false;

  int32_t receiver_operand =
      ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                operand_scale);
  Address receiver_address = ReadInterpreterRegister(
      interpreter::Register::FromOperand(receiver_operand));
  ReadOnlyRoots roots(isolate);
#ifdef __wasi__
  if (bytecode->length() > 200000 && bytecode_index == 232103) {
    std::fprintf(stderr,
                 "WASM32_MAIN_GETITER receiver=0x%x is_smi=%d is_null=%d "
                 "is_array=%d is_map_iter=%d is_set_iter=%d source=%d\n",
                 static_cast<unsigned>(receiver_address),
                 HAS_SMI_TAG(receiver_address) ? 1 : 0,
                 IsNullOrUndefined(Tagged<Object>(receiver_address), isolate)
                     ? 1
                     : 0,
                 IsJSArray(Tagged<Object>(receiver_address)) ? 1 : 0,
                 IsJSMapIterator(Tagged<Object>(receiver_address)) ? 1 : 0,
                 IsJSSetIterator(Tagged<Object>(receiver_address)) ? 1 : 0,
                 bytecode->SourcePosition(bytecode_index));
    std::fflush(stderr);
  }
#endif
  if (!IsJSAnyForWasmPropertyLookup(isolate, receiver_address)) {
    int bytecode_size = interpreter::Bytecodes::Size(bytecode_enum,
                                                     operand_scale);
    PrintF("WasmInterpreterEntryTrampoline: iterator receiver invalid "
           "bytecode_index=%d receiver_operand=%d receiver_reg_index=%d "
           "regs=%d params=%d source_pos=%d stmt_pos=%d size=%d raw:",
           bytecode_index, receiver_operand,
           interpreter::Register::FromOperand(receiver_operand).index(),
           bytecode->register_count(), bytecode->parameter_count(),
           bytecode->SourcePosition(bytecode_index),
           bytecode->SourceStatementPosition(bytecode_index), bytecode_size);
    for (int raw_index = 0; raw_index < bytecode_size; ++raw_index) {
      PrintF(" %02x", bytecode->get(bytecode_index + raw_index));
    }
    PrintF(" ");
    DumpRuntimeArg("receiver", 0, receiver_address);
    Address function_address =
        g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
            StandardFrameConstants::kFunctionOffset)];
    DumpRuntimeArg("function", 0, function_address);
    if (IsSafeTaggedHandleValue(function_address) &&
        IsJSFunction(Tagged<Object>(function_address))) {
      Tagged<SharedFunctionInfo> shared =
          Cast<JSFunction>(Tagged<Object>(function_address))->shared();
      PrintF(" sfi=0x%x start=%d end=%d literal_id=%d",
             static_cast<unsigned>(shared.ptr()), shared->StartPosition(),
             shared->EndPosition(), shared->function_literal_id());
    }
    DumpRuntimeArg("acc", 0,
                   g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
    DumpRuntimeArg("context", 0, CurrentInterpreterContext());
    int logged_parameter_count =
        bytecode->parameter_count() < 8 ? bytecode->parameter_count() : 8;
    for (int parameter_index = 0; parameter_index < logged_parameter_count;
         ++parameter_index) {
      DumpRuntimeArg(
          "p", parameter_index,
          ReadInterpreterRegister(
              interpreter::Register::FromParameterIndex(parameter_index)));
    }
    int logged_register_count =
        bytecode->register_count() < 16 ? bytecode->register_count() : 16;
    for (int register_index = 0; register_index < logged_register_count;
         ++register_index) {
      DumpRuntimeArg("l", register_index,
                     ReadInterpreterRegister(
                         interpreter::Register(register_index)));
    }
    PrintF("\n");
    *out_result = roots.exception().ptr();
    return true;
  }

  Tagged<Object> receiver_object(receiver_address);
  if (IsNullOrUndefined(receiver_object, isolate)) {
    PrintF("WASM32_GETITER_NULL bytecode_index=%d source_pos=%d stmt_pos=%d "
           "receiver_operand=%d receiver_reg=%d\n",
           bytecode_index, bytecode->SourcePosition(bytecode_index),
           bytecode->SourceStatementPosition(bytecode_index), receiver_operand,
           interpreter::Register::FromOperand(receiver_operand).index());
    Address function_address =
        g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
            StandardFrameConstants::kFunctionOffset)];
    if (IsSafeTaggedHandleValue(function_address) &&
        IsJSFunction(Tagged<Object>(function_address))) {
      Tagged<SharedFunctionInfo> shared =
          Wasm32JSFunctionShared(Cast<JSFunction>(Tagged<Object>(function_address)));
      PrintF("WASM32_GETITER_NULL bytecode_index=%d source_pos=%d stmt_pos=%d "
             "receiver_operand=%d receiver_reg=%d start=%d end=%d "
             "literal_id=%d\n",
             bytecode_index, bytecode->SourcePosition(bytecode_index),
             bytecode->SourceStatementPosition(bytecode_index),
             receiver_operand,
             interpreter::Register::FromOperand(receiver_operand).index(),
             shared->StartPosition(), shared->EndPosition(),
             shared->function_literal_id());
    }
    *out_result = roots.undefined_value().ptr();
    return true;
  }
  HandleScope scope(isolate);
  Handle<JSAny> receiver = handle(Cast<JSAny>(receiver_object), isolate);
  if (IsJSMapIterator(receiver_object) || IsJSSetIterator(receiver_object)) {
    *out_result = receiver_object.ptr();
    return true;
  }
  if (IsJSArray(receiver_object)) {
    DirectHandle<Map> iterator_map(
        isolate->native_context()->initial_array_iterator_map(), isolate);
    DirectHandle<JSArrayIterator> iterator = Cast<JSArrayIterator>(
        isolate->factory()->NewJSObjectFromMap(iterator_map));
    iterator->set_iterated_object(Cast<JSReceiver>(receiver_object));
    iterator->set_next_index(Smi::zero());
    iterator->set_kind(IterationKind::kValues);
    *out_result = (*iterator).ptr();
    return true;
  }
  if (IsJSMap(receiver_object)) {
    DirectHandle<Map> iterator_map(
        isolate->native_context()->map_key_value_iterator_map(), isolate);
    DirectHandle<JSMapIterator> iterator = Cast<JSMapIterator>(
        isolate->factory()->NewJSObjectFromMap(iterator_map));
    iterator->set_table(Cast<JSMap>(receiver_object)->table());
    iterator->set_index(Smi::zero());
    *out_result = (*iterator).ptr();
    return true;
  }

  DirectHandle<Name> iterator_symbol = isolate->factory()->iterator_symbol();
  DirectHandle<Object> method;
  if (!Runtime::GetObjectProperty(isolate, receiver, iterator_symbol)
           .ToHandle(&method)) {
    *out_result = roots.exception().ptr();
    return true;
  }
  if (!IsCallable(*method)) {
    PrintF("WasmInterpreterEntryTrampoline: iterator method not callable ");
    DumpRuntimeArg("receiver", 0, (*receiver).ptr());
    DumpRuntimeArg("method", 0, (*method).ptr());
    PrintF("\n");
    if (!g_dumped_get_iterator_method_failure) {
      g_dumped_get_iterator_method_failure = true;
      int bytecode_size = interpreter::Bytecodes::Size(bytecode_enum,
                                                       operand_scale);
      PrintF("WasmInterpreterEntryTrampoline: GetIterator method failure "
             "bytecode_index=%d receiver_operand=%d receiver_reg_index=%d "
             "regs=%d params=%d source_pos=%d stmt_pos=%d size=%d raw:",
             bytecode_index, receiver_operand,
             interpreter::Register::FromOperand(receiver_operand).index(),
             bytecode->register_count(), bytecode->parameter_count(),
             bytecode->SourcePosition(bytecode_index),
             bytecode->SourceStatementPosition(bytecode_index), bytecode_size);
      for (int raw_index = 0; raw_index < bytecode_size; ++raw_index) {
        PrintF(" %02x", bytecode->get(bytecode_index + raw_index));
      }
      Address function_address =
          g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
              StandardFrameConstants::kFunctionOffset)];
      DumpRuntimeArg(" function", 0, function_address);
      if (IsSafeTaggedHandleValue(function_address) &&
          IsJSFunction(Tagged<Object>(function_address))) {
        Tagged<SharedFunctionInfo> shared =
            Cast<JSFunction>(Tagged<Object>(function_address))->shared();
        std::unique_ptr<char[]> debug_name = shared->DebugNameCStr();
        PrintF(" sfi=0x%x name=\"%s\" start=%d end=%d literal_id=%d",
               static_cast<unsigned>(shared.ptr()), debug_name.get(),
               shared->StartPosition(), shared->EndPosition(),
               shared->function_literal_id());
        Tagged<Object> script_object = shared->script();
        if (IsScript(script_object)) {
          Tagged<Script> script = Cast<Script>(script_object);
          PrintStringPreviewForTrace(" script_name", script->name());
          if (IsString(script->source())) {
            Tagged<String> source = Cast<String>(script->source());
            int source_start = shared->StartPosition() - 80;
            if (source_start < 0) source_start = 0;
            PrintStringPreviewForTrace(" source",
                                       source,
                                       static_cast<uint32_t>(source_start),
                                       420);
          }
        }
      }
      DumpRuntimeArg(" acc", 0,
                     g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
      DumpRuntimeArg(" context", 0, CurrentInterpreterContext());
      PrintF("\n");

      int window_start = bytecode_index - 80;
      if (window_start < 0) window_start = 0;
      int window_end = bytecode_index + 48;
      if (window_end > bytecode->length()) window_end = bytecode->length();
      interpreter::OperandScale dump_scale = interpreter::OperandScale::kSingle;
      for (int index = 0; index < bytecode->length();) {
        uint8_t dump_opcode = bytecode->get(index);
        interpreter::Bytecode dump_bytecode =
            interpreter::Bytecodes::FromByte(dump_opcode);
        int dump_size = interpreter::Bytecodes::Size(dump_bytecode,
                                                     dump_scale);
        if (index >= window_start && index <= window_end) {
          PrintF("  getiter bc index=%d op=0x%x(%s) scale=%d size=%d",
                 index, static_cast<unsigned>(dump_opcode),
                 interpreter::Bytecodes::ToString(dump_bytecode),
                 static_cast<int>(dump_scale), dump_size);
          int operand_count =
              interpreter::Bytecodes::NumberOfOperands(dump_bytecode);
          for (int operand_index = 0; operand_index < operand_count;
               ++operand_index) {
            uint32_t operand_u = ReadBytecodeUnsignedOperand(
                bytecode, index, dump_bytecode, operand_index, dump_scale);
            int32_t operand_s = ReadBytecodeSignedOperand(
                bytecode, index, dump_bytecode, operand_index, dump_scale);
            PrintF(" #%d=u0x%x/s%d", operand_index, operand_u, operand_s);
          }
          PrintF(" raw:");
          for (int raw_index = 0; raw_index < dump_size; ++raw_index) {
            PrintF(" %02x", bytecode->get(index + raw_index));
          }
          PrintF("\n");
        }
        if (interpreter::Bytecodes::IsPrefixScalingBytecode(dump_bytecode)) {
          dump_scale =
              interpreter::Bytecodes::PrefixBytecodeToOperandScale(dump_bytecode);
          index += interpreter::Bytecodes::Size(
              dump_bytecode, interpreter::OperandScale::kSingle);
          continue;
        }
        dump_scale = interpreter::OperandScale::kSingle;
        index += dump_size;
      }

      int logged_parameter_count =
          bytecode->parameter_count() < 8 ? bytecode->parameter_count() : 8;
      for (int parameter_index = 0; parameter_index < logged_parameter_count;
           ++parameter_index) {
        DumpRuntimeArg(
            "getiter.p", parameter_index,
            ReadInterpreterRegister(
                interpreter::Register::FromParameterIndex(parameter_index)));
      }
      int logged_register_count =
          bytecode->register_count() < 24 ? bytecode->register_count() : 24;
      for (int register_index = 0; register_index < logged_register_count;
           ++register_index) {
        DumpRuntimeArg("getiter.l", register_index,
                       ReadInterpreterRegister(
                           interpreter::Register(register_index)));
      }
      PrintF("\n");

      Tagged<TrustedFixedArray> constant_pool = bytecode->constant_pool();
      int constant_count = constant_pool->length();
      int constant_dump_count = constant_count < 24 ? constant_count : 24;
      PrintF("WasmInterpreterEntryTrampoline: GetIterator constants "
             "length=%d dump=%d",
             constant_count, constant_dump_count);
      for (int constant_index = 0; constant_index < constant_dump_count;
           ++constant_index) {
        Tagged<Object> constant = constant_pool->get(constant_index);
        DumpRuntimeArg("getiter.const", constant_index, constant.ptr());
        PrintStringPreviewForTrace("string", constant);
      }
      PrintF("\n");
    }
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<Object> iterator;
  if (!Execution::Call(isolate, method, receiver, {}).ToHandle(&iterator)) {
    *out_result = roots.exception().ptr();
    return true;
  }
  if (!IsJSReceiver(*iterator)) {
    PrintF("WasmInterpreterEntryTrampoline: iterator result is not receiver ");
    DumpRuntimeArg("iterator", 0, (*iterator).ptr());
    PrintF("\n");
    *out_result = roots.exception().ptr();
    return true;
  }
  *out_result = (*iterator).ptr();
  return true;
}

Address NamedLoadKeyFromRuntimeArgs(Address* argv, int argc) {
  Address key = RuntimeVisibleArg(argv, argc, 1);
  if (!IsNameAddress(key) && argc > 2) {
    Address alternate = RuntimeVisibleArg(argv, argc, 2);
    if (IsNameAddress(alternate)) key = alternate;
  }
  return key;
}

Address KeyedLoadKeyFromRuntimeArgs(Address* argv, int argc) {
  Address key = RuntimeVisibleArg(argv, argc, 1);
  if (argc > 2 && HAS_SMI_TAG(key) && IsTaggedIndex(Tagged<Object>(key))) {
    Address alternate = RuntimeVisibleArg(argv, argc, 2);
    if (IsNameAddress(alternate)) key = alternate;
  }
  return key;
}

bool TryFallbackICMiss(Isolate* isolate, Runtime::FunctionId function_id,
                       Address* argv, int argc, Address* out_result) {
  if (argc != 4) return false;
  if (function_id != Runtime::kLoadIC_Miss &&
      function_id != Runtime::kKeyedLoadIC_Miss) {
    return false;
  }

  Address maybe_vector = RuntimeVisibleArg(argv, argc, 3);
  if (IsFeedbackVectorAddress(maybe_vector)) return false;

  HandleScope scope(isolate);
  Address receiver_address = RuntimeVisibleArg(argv, argc, 0);
  ReadOnlyRoots roots(isolate);
  if (!IsJSAnyForWasmPropertyLookup(isolate, receiver_address)) {
    PrintF("WasmRuntimeCallFromGenerated: IC fallback receiver invalid");
    DumpRuntimeArg("receiver", 0, receiver_address);
    PrintF("\n");
    *out_result = roots.undefined_value().ptr();
    return true;
  }
  Tagged<Object> receiver_object(receiver_address);
  DirectHandle<JSAny> receiver =
      direct_handle(Cast<JSAny>(receiver_object), isolate);

  Address key_address;
  if (function_id == Runtime::kLoadIC_Miss) {
    key_address = NamedLoadKeyFromRuntimeArgs(argv, argc);
    if (!IsNameAddress(key_address)) {
      PrintF("WasmRuntimeCallFromGenerated: LoadIC fallback missing Name key");
      DumpRuntimeArg("key", 0, key_address);
      PrintF("\n");
      *out_result = roots.exception().ptr();
      return true;
    }
  } else {
    key_address = KeyedLoadKeyFromRuntimeArgs(argv, argc);
    if (!IsSafeTaggedHandleValue(key_address)) {
      key_address = roots.undefined_value().ptr();
    }
  }

  DirectHandle<Object> key = direct_handle(Tagged<Object>(key_address), isolate);
  MaybeDirectHandle<Object> maybe_result =
      GetObjectPropertyPreservingWasmInterpreterState(isolate, receiver, key);

  DirectHandle<Object> result;
  if (!maybe_result.ToHandle(&result)) {
    *out_result = roots.exception().ptr();
    return true;
  }
  *out_result = (*result).ptr();
  return true;
}

bool TryOrdinaryHasInstanceForWasm(Isolate* isolate, Address callable_address,
                                   Address object_address,
                                   Address* out_result) {
  ReadOnlyRoots roots(isolate);
  callable_address = SafeTaggedOrUndefined(isolate, callable_address);
  object_address = SafeTaggedOrUndefined(isolate, object_address);
  if (!IsSafeTaggedHandleValue(callable_address) ||
      !IsSafeTaggedHandleValue(object_address)) {
    return false;
  }

  Tagged<Object> callable_object(callable_address);
  Tagged<Object> object_object(object_address);
  if (!IsCallable(callable_object)) {
    *out_result = roots.false_value().ptr();
    return true;
  }
  if (!IsJSReceiver(object_object)) {
    *out_result = roots.false_value().ptr();
    return true;
  }

  if (!IsJSFunction(callable_object)) return false;

  HandleScope scope(isolate);
  DirectHandle<JSFunction> function =
      direct_handle(Cast<JSFunction>(callable_object), isolate);
  if (!function->has_prototype_slot()) return false;
  DirectHandle<Object> prototype(
      Tagged<Object>(Wasm32JSFunctionPrototypeAddress(isolate, function)),
      isolate);
  if (!IsJSReceiver(*prototype)) {
    isolate->Throw(*isolate->factory()->NewTypeError(
        MessageTemplate::kInstanceofNonobjectProto, prototype));
    *out_result = roots.exception().ptr();
    return true;
  }

  DirectHandle<JSReceiver> object =
      direct_handle(Cast<JSReceiver>(object_object), isolate);
  Maybe<bool> result =
      JSReceiver::HasInPrototypeChain(isolate, object, prototype);
  if (result.IsNothing()) {
    *out_result = roots.exception().ptr();
    return true;
  }
  *out_result = (*isolate->factory()->ToBoolean(result.FromJust())).ptr();
  return true;
}

bool TryFallbackGeneratedRuntime(Isolate* isolate,
                                 Runtime::FunctionId function_id,
                                 Address* argv, int argc,
                                 Address* out_result) {
  if (function_id == Runtime::kOrdinaryHasInstance && argc == 2) {
    return TryOrdinaryHasInstanceForWasm(
        isolate, RuntimeVisibleArg(argv, argc, 0),
        RuntimeVisibleArg(argv, argc, 1), out_result);
  }

  if (function_id == Runtime::kGetDerivedMap && argc == 3) {
    ReadOnlyRoots roots(isolate);
    Address visible_args[3] = {
        RuntimeVisibleArg(argv, argc, 0),
        RuntimeVisibleArg(argv, argc, 1),
        RuntimeVisibleArg(argv, argc, 2),
    };
    int target_index = -1;
    int new_target_index = -1;
    for (int i = 0; i < 3; ++i) {
      Address target_address = SafeRuntimeArgOrUndefined(isolate,
                                                         visible_args[i]);
      if (!IsSafeTaggedHandleValue(target_address) ||
          !IsJSFunction(Tagged<Object>(target_address))) {
        continue;
      }
      for (int j = 0; j < 3; ++j) {
        if (j == i) continue;
        Address new_target_address = SafeRuntimeArgOrUndefined(
            isolate, visible_args[j]);
        if (IsSafeTaggedHandleValue(new_target_address) &&
            IsJSReceiver(Tagged<Object>(new_target_address))) {
          target_index = i;
          new_target_index = j;
          break;
        }
      }
      if (target_index >= 0) break;
    }

    if (target_index < 0 || new_target_index < 0) {
      PrintF("WasmRuntimeCallFromGenerated: GetDerivedMap args invalid");
      DumpCurrentRuntimeCallStateForTrace("state");
      for (int i = 0; i < 3; ++i) DumpRuntimeArg("arg", i, visible_args[i]);
      DumpRuntimeArg(" js_target", 0,
                     g_wasm_regs[SlotFor(kJavaScriptCallTargetRegister)]);
      DumpRuntimeArg(" js_new_target", 0,
                     g_wasm_regs[SlotFor(kJavaScriptCallNewTargetRegister)]);
      DumpRuntimeArg(" js_argc", 0,
                     g_wasm_regs[SlotFor(kJavaScriptCallArgCountRegister)]);
      for (int i = 0; i < 8; ++i) {
        DumpRuntimeArg(" out", i, g_wasm_regs[kWasmOutgoingArgSlotBase + i]);
      }
      for (int i = 0; i < 6; ++i) {
        DumpRuntimeArg(" genarg", i, ReadGeneratedJSArgument(i));
      }
      for (int i = 0; i < 8; ++i) {
        DumpRuntimeArg(" reg", i, g_wasm_regs[i]);
      }
      PrintF("\n");
      *out_result = roots.exception().ptr();
      return true;
    }

    HandleScope scope(isolate);
    DirectHandle<JSFunction> target = direct_handle(
        Cast<JSFunction>(Tagged<Object>(SafeRuntimeArgOrUndefined(
            isolate, visible_args[target_index]))),
        isolate);
    DirectHandle<JSReceiver> new_target = direct_handle(
        Cast<JSReceiver>(Tagged<Object>(SafeRuntimeArgOrUndefined(
            isolate, visible_args[new_target_index]))),
        isolate);
    DirectHandle<Map> map;
    if (!JSFunction::GetDerivedMap(isolate, target, new_target)
             .ToHandle(&map)) {
      *out_result = roots.exception().ptr();
      return true;
    }
    *out_result = (*map).ptr();
    return true;
  }

  if (function_id != Runtime::kJSReceiverGetPrototypeOf) return false;
  if (argc != 1) return false;

  ReadOnlyRoots roots(isolate);
  Address receiver_address = RuntimeVisibleArg(argv, argc, 0);
  if (!IsSafeTaggedHandleValue(receiver_address) ||
      !IsJSReceiver(Tagged<Object>(receiver_address))) {
    Address generated_arg0 = ReadGeneratedJSArgument(0);
    if (IsSafeTaggedHandleValue(generated_arg0) &&
        IsJSReceiver(Tagged<Object>(generated_arg0))) {
      if (kTraceWasmFallbackDetails) {
        PrintF("WasmRuntimeCallFromGenerated: JSReceiverGetPrototypeOf "
               "recovered receiver from JS arg0");
        DumpRuntimeArg("receiver", 0, generated_arg0);
        PrintF("\n");
      }
      receiver_address = generated_arg0;
    }
  }
  if (!IsSafeTaggedHandleValue(receiver_address) ||
      !IsJSReceiver(Tagged<Object>(receiver_address))) {
    PrintF("WasmRuntimeCallFromGenerated: JSReceiverGetPrototypeOf "
           "receiver invalid");
    DumpRuntimeArg("receiver", 0, receiver_address);
    PrintF("\n");
    *out_result = roots.exception().ptr();
    return true;
  }

  HandleScope scope(isolate);
  DirectHandle<JSReceiver> receiver =
      direct_handle(Cast<JSReceiver>(Tagged<Object>(receiver_address)),
                    isolate);
  DirectHandle<JSPrototype> prototype;
#ifdef __wasi__
  if (kTraceWasmFallbackDetails) {
    PrintF("WasmRuntimeCallFromGenerated: JSReceiverGetPrototypeOf before get ");
    DumpRuntimeArg("receiver", 0, receiver_address);
    PrintF("\n");
  }
#endif
  if (!JSReceiver::GetPrototype(isolate, receiver).ToHandle(&prototype)) {
#ifdef __wasi__
    PrintF("WasmRuntimeCallFromGenerated: JSReceiverGetPrototypeOf exception "
           "has_exception=%d\n",
           isolate->has_exception() ? 1 : 0);
#endif
    *out_result = roots.exception().ptr();
    return true;
  }
  *out_result = (*prototype).ptr();
#ifdef __wasi__
  if (kTraceWasmFallbackDetails) {
    PrintF("WasmRuntimeCallFromGenerated: JSReceiverGetPrototypeOf result ");
    DumpRuntimeArg("prototype", 0, *out_result);
    PrintF(" has_exception=%d\n", isolate->has_exception() ? 1 : 0);
  }
#endif
  return true;
}

bool IsBytecodeBudgetInterruptRuntime(Runtime::FunctionId function_id) {
  switch (function_id) {
    case Runtime::kBytecodeBudgetInterrupt_Ignition:
    case Runtime::kBytecodeBudgetInterruptWithStackCheck_Ignition:
    case Runtime::kBytecodeBudgetInterrupt_Sparkplug:
    case Runtime::kBytecodeBudgetInterruptWithStackCheck_Sparkplug:
    case Runtime::kBytecodeBudgetInterrupt_Maglev:
    case Runtime::kBytecodeBudgetInterruptWithStackCheck_Maglev:
      return true;
    default:
      return false;
  }
}

Address WasmBytecodeHandlerEntry(Isolate* isolate, uint8_t opcode,
                                 interpreter::OperandScale operand_scale,
                                 Builtin* out_builtin) {
  interpreter::Bytecode bytecode = interpreter::Bytecodes::FromByte(opcode);
  Tagged<Code> handler =
      isolate->interpreter()->GetBytecodeHandler(bytecode, operand_scale);
  Builtin builtin = handler->builtin_id();
  if (out_builtin != nullptr) *out_builtin = builtin;
  void* fn = WasmBuiltinFuncref(builtin);
  return fn == nullptr ? kNullAddress : reinterpret_cast<Address>(fn);
}

MaybeDirectHandle<Object> Wasm32CreateDynamicFunction(
    Isolate* isolate, Tagged<JSFunction> function, Address new_target,
    int actual_argc, Address* argv, const char* token) {
#ifdef __wasi__
  static int dynamic_function_trace_count = 0;
  int trace_dynamic_function_index = ++dynamic_function_trace_count;
  bool trace_dynamic_function =
      kTraceWasmFallbackDetails && trace_dynamic_function_index <= 16;
  if (trace_dynamic_function) {
    PrintF("Wasm32CreateDynamicFunction enter #%d argc=%d token=%s ",
           trace_dynamic_function_index, actual_argc, token);
    DumpRuntimeArg("function", 0, function.ptr());
    DumpRuntimeArg("new_target", 0, new_target);
    PrintF("\n");
  }
#endif
  DirectHandle<JSFunction> target = direct_handle(function, isolate);
  DirectHandle<JSObject> target_global_proxy(target->global_proxy(), isolate);
  if (!Builtins::AllowDynamicFunction(isolate, target, target_global_proxy)) {
    isolate->CountUsage(v8::Isolate::kFunctionConstructorReturnedUndefined);
    isolate->Throw(*isolate->factory()->NewTypeError(MessageTemplate::kNoAccess));
    return MaybeDirectHandle<Object>();
  }

  Tagged<Context> function_context = Wasm32JSFunctionContext(function);
  DirectHandle<NativeContext> native_context =
      IsNativeContext(function_context)
          ? direct_handle(Cast<NativeContext>(function_context), isolate)
          : direct_handle(function_context->native_context(), isolate);

  DirectHandle<String> source;
  int parameters_end_pos = kNoSourcePosition;
  {
    IncrementalStringBuilder builder(isolate);
    builder.AppendCharacter('(');
    builder.AppendCString(token);
    builder.AppendCStringLiteral(" anonymous(");
    if (actual_argc > 1) {
      for (int i = 0; i < actual_argc - 1; ++i) {
        if (i > 0) builder.AppendCharacter(',');
        DirectHandle<Object> argument(
            Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[i])), isolate);
        DirectHandle<String> parameter;
        if (!Object::ToString(isolate, argument).ToHandle(&parameter)) {
          return MaybeDirectHandle<Object>();
        }
        parameter = String::Flatten(isolate, parameter);
        builder.AppendString(parameter);
      }
    }
    builder.AppendCharacter('\n');
    parameters_end_pos = builder.Length();
    builder.AppendCStringLiteral(") {\n");
    if (actual_argc > 0) {
      DirectHandle<Object> body_object(
          Tagged<Object>(
              SafeTaggedOrUndefined(isolate, argv[actual_argc - 1])),
          isolate);
      DirectHandle<String> body;
      if (!Object::ToString(isolate, body_object).ToHandle(&body)) {
        return MaybeDirectHandle<Object>();
      }
#ifdef __wasi__
      if (trace_dynamic_function) {
        PrintStringPreviewForTrace(" body", *body, 0, 120);
        PrintF("\n");
      }
#endif
      builder.AppendString(body);
    }
    builder.AppendCStringLiteral("\n})");
    if (!builder.Finish().ToHandle(&source)) {
      return MaybeDirectHandle<Object>();
    }
  }
#ifdef __wasi__
  if (trace_dynamic_function) {
    PrintF("Wasm32CreateDynamicFunction source #%d len=%d params_end=%d",
           trace_dynamic_function_index, source->length(), parameters_end_pos);
    PrintStringPreviewForTrace(" source", *source, 0, 160);
    PrintF("\n");
  }
#endif

  bool is_code_like = true;
  for (int i = 0; i < actual_argc; ++i) {
    DirectHandle<Object> argument(
        Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[i])), isolate);
    if (!Object::IsCodeLike(*argument, isolate)) {
      is_code_like = false;
      break;
    }
  }
#ifdef __wasi__
  if (trace_dynamic_function) {
    PrintF("Wasm32CreateDynamicFunction before compile #%d code_like=%d\n",
           trace_dynamic_function_index, is_code_like ? 1 : 0);
  }
#endif

  if (actual_argc <= 1 &&
      (actual_argc == 0 ||
       IsUndefined(Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[0])),
                   isolate))) {
    DirectHandle<JSFunction> empty_function(native_context->empty_function(),
                                            isolate);
#ifdef __wasi__
    if (trace_dynamic_function) {
      PrintF("Wasm32CreateDynamicFunction empty fast-path #%d ",
             trace_dynamic_function_index);
      DumpRuntimeArg("function", 0, (*empty_function).ptr());
      PrintF("\n");
    }
#endif
    return empty_function;
  }

  DirectHandle<JSFunction> compiled_function;
  if (!Compiler::GetFunctionFromString(
           isolate, native_context, indirect_handle(source, isolate),
           parameters_end_pos, is_code_like)
           .ToHandle(&compiled_function)) {
    return MaybeDirectHandle<Object>();
  }
#ifdef __wasi__
  if (trace_dynamic_function) {
    PrintF("Wasm32CreateDynamicFunction after compile #%d ",
           trace_dynamic_function_index);
    DumpRuntimeArg("compiled", 0, (*compiled_function).ptr());
    PrintF("\n");
  }
#endif

  DirectHandle<Object> result;
  if (!Execution::Call(isolate, compiled_function, target_global_proxy, {})
           .ToHandle(&result)) {
    return MaybeDirectHandle<Object>();
  }
#ifdef __wasi__
  if (trace_dynamic_function) {
    PrintF("Wasm32CreateDynamicFunction after call #%d ",
           trace_dynamic_function_index);
    DumpRuntimeArg("result", 0, (*result).ptr());
    PrintF("\n");
  }
#endif
  compiled_function = Cast<JSFunction>(result);
  compiled_function->shared()->set_name_should_print_as_anonymous(true);

  DirectHandle<Object> unchecked_new_target(
      Tagged<Object>(SafeTaggedOrUndefined(isolate, new_target)), isolate);
  if (!IsUndefined(*unchecked_new_target, isolate) &&
      !unchecked_new_target.is_identical_to(target)) {
    if (!IsJSReceiver(*unchecked_new_target)) {
      return MaybeDirectHandle<Object>();
    }
    DirectHandle<JSReceiver> constructor_new_target =
        Cast<JSReceiver>(unchecked_new_target);
    DirectHandle<Map> initial_map;
    if (!JSFunction::GetDerivedMap(isolate, target, constructor_new_target)
             .ToHandle(&initial_map)) {
      return MaybeDirectHandle<Object>();
    }

    DirectHandle<SharedFunctionInfo> shared_info(compiled_function->shared(),
                                                 isolate);
    DirectHandle<Map> map =
        Map::AsLanguageMode(isolate, initial_map, shared_info);
    DirectHandle<Context> context(compiled_function->context(), isolate);
    compiled_function =
        Factory::JSFunctionBuilder{isolate, shared_info, context}
            .set_map(map)
            .set_allocation_type(AllocationType::kYoung)
            .Build();
  }

#ifdef __wasi__
  if (trace_dynamic_function) {
    PrintF("Wasm32CreateDynamicFunction return #%d ",
           trace_dynamic_function_index);
    DumpRuntimeArg("function", 0, (*compiled_function).ptr());
    PrintF("\n");
  }
#endif
  return compiled_function;
}

bool TryRunGeneratorResumeBuiltin(Isolate* isolate, Builtin builtin,
                                  Address receiver, int actual_argc,
                                  Address* argv, Address* out_result) {
  JSGeneratorObject::ResumeMode resume_mode;
  switch (builtin) {
    case Builtin::kGeneratorPrototypeNext:
    case Builtin::kAsyncModuleEvaluate:
      resume_mode = JSGeneratorObject::ResumeMode::kNext;
      break;
    case Builtin::kGeneratorPrototypeReturn:
      resume_mode = JSGeneratorObject::ResumeMode::kReturn;
      break;
    case Builtin::kGeneratorPrototypeThrow:
      resume_mode = JSGeneratorObject::ResumeMode::kThrow;
      break;
    default:
      return false;
  }

  ReadOnlyRoots roots(isolate);
  Address receiver_address = SafeTaggedOrUndefined(isolate, receiver);
  if (!IsSafeTaggedHandleValue(receiver_address) ||
      (!IsJSGeneratorObject(Tagged<Object>(receiver_address)) &&
       !IsJSAsyncFunctionObject(Tagged<Object>(receiver_address)) &&
       !IsJSAsyncGeneratorObject(Tagged<Object>(receiver_address)))) {
    return false;
  }

  HandleScope scope(isolate);
  Handle<JSGeneratorObject> generator(
      Cast<JSGeneratorObject>(Tagged<Object>(receiver_address)), isolate);
  Address value_address =
      actual_argc > 0 ? SafeTaggedOrUndefined(isolate, argv[0])
                      : roots.undefined_value().ptr();
  Handle<Object> value(Tagged<Object>(value_address), isolate);

  int continuation = generator->continuation();
  if (continuation == JSGeneratorObject::kGeneratorClosed) {
    if (resume_mode == JSGeneratorObject::ResumeMode::kThrow) {
      isolate->Throw(*value);
      *out_result = roots.exception().ptr();
      return true;
    }
    DirectHandle<Object> closed_value =
        resume_mode == JSGeneratorObject::ResumeMode::kReturn
            ? direct_handle(*value, isolate)
            : direct_handle(roots.undefined_value(), isolate);
    *out_result =
        (*isolate->factory()->NewJSIteratorResult(closed_value, true)).ptr();
    return true;
  }
  if (continuation < JSGeneratorObject::kGeneratorClosed) {
    return false;
  }

  generator->set_input_or_debug_pos(*value);
  generator->set_resume_mode(resume_mode);
  Handle<JSFunction> target(generator->function(), isolate);
  Handle<JSAny> generator_receiver(generator->receiver(), isolate);

  Address root = g_wasm_regs[kWasmRegRoot];
  if (root == kNullAddress) root = g_wasm_regs[SlotFor(kRootRegister)];
  Address result = WasmJSEntry(
      root, generator->ptr(), target->ptr(), (*generator_receiver).ptr(),
      JSParameterCount(0), nullptr);
  if (isolate->has_exception() || result == roots.exception().ptr()) {
    generator->set_continuation(JSGeneratorObject::kGeneratorClosed);
    *out_result = roots.exception().ptr();
    return true;
  }

  Handle<Object> result_handle(Tagged<Object>(result), isolate);
  if (generator->continuation() == JSGeneratorObject::kGeneratorExecuting) {
    generator->set_continuation(JSGeneratorObject::kGeneratorClosed);
    *out_result = isolate->factory()
                      ->NewJSIteratorResult(
                          direct_handle(*result_handle, isolate), true)
                      ->ptr();
  } else {
    *out_result = (*result_handle).ptr();
  }
  return true;
}

bool ResumeWasmAsyncFunctionAwait(
    Isolate* isolate, Tagged<JSFunction> function, Address value_address,
    JSGeneratorObject::ResumeMode resume_mode, Address* out_result) {
  ReadOnlyRoots roots(isolate);
  Tagged<Context> context = Wasm32JSFunctionContext(function);
  Tagged<Object> extension = context->extension();
  if (!IsJSAsyncFunctionObject(extension)) return false;

  HandleScope scope(isolate);
  Handle<JSAsyncFunctionObject> async_function(
      Cast<JSAsyncFunctionObject>(extension), isolate);
  value_address = SafeTaggedOrUndefined(isolate, value_address);
  Handle<Object> value(Tagged<Object>(value_address), isolate);
  async_function->set_input_or_debug_pos(*value);
  async_function->set_resume_mode(resume_mode);

  Handle<JSFunction> target(async_function->function(), isolate);
  Handle<JSAny> generator_receiver(async_function->receiver(), isolate);
#ifdef __wasi__
  PrintF("WASM32_AWAIT_RESUME mode=%d continuation=%d promise_state=%d",
         static_cast<int>(resume_mode), async_function->continuation(),
         static_cast<int>(async_function->promise()->status()));
  DumpRuntimeArg("value", 0, (*value).ptr());
  PrintStringPreviewForTrace("value_string", *value, 0, 240);
  PrintF("\n");
#endif
  Address root = g_wasm_regs[kWasmRegRoot];
  if (root == kNullAddress) root = g_wasm_regs[SlotFor(kRootRegister)];
  Address result = WasmJSEntry(
      root, async_function->ptr(), target->ptr(), (*generator_receiver).ptr(),
      JSParameterCount(0), nullptr);
#ifdef __wasi__
  PrintF("WASM32_AWAIT_RESULT result=0x%x has_exception=%d "
         "promise_state=%d",
         static_cast<unsigned>(result), isolate->has_exception() ? 1 : 0,
         static_cast<int>(async_function->promise()->status()));
  if (isolate->has_exception()) {
    Tagged<Object> exception = isolate->exception();
    DumpRuntimeArg("exception", 0, exception.ptr());
    PrintStringPreviewForTrace("exception_string", exception, 0, 240);
    if (IsJSReceiver(exception)) {
      DumpNamedDataPropertyForTrace(isolate, exception.ptr(), "name");
      DumpNamedDataPropertyForTrace(isolate, exception.ptr(), "message");
      DumpNamedDataPropertyForTrace(isolate, exception.ptr(), "code");
    }
  }
  if (async_function->promise()->status() != Promise::kPending) {
    Tagged<Object> promise_result = async_function->promise()->result();
    DumpRuntimeArg("promise_result", 0, promise_result.ptr());
    PrintStringPreviewForTrace("promise_string", promise_result, 0, 240);
    if (IsJSReceiver(promise_result)) {
      DumpNamedDataPropertyForTrace(isolate, promise_result.ptr(), "name");
      DumpNamedDataPropertyForTrace(isolate, promise_result.ptr(), "message");
      DumpNamedDataPropertyForTrace(isolate, promise_result.ptr(), "code");
    }
  }
  PrintF("\n");
#endif
  if (isolate->has_exception() || result == roots.exception().ptr()) {
    *out_result = roots.exception().ptr();
  } else {
    *out_result = roots.undefined_value().ptr();
  }
  return true;
}

extern "C" bool Wasm32TryResumeAsyncFunctionAwait(
    Isolate* isolate, Address handler_address, Address value_address,
    bool rejected, Address* out_result) {
  if (!IsSafeTaggedHandleValue(handler_address) ||
      !IsJSFunction(Tagged<Object>(handler_address))) {
    return false;
  }
  Tagged<JSFunction> function =
      Cast<JSFunction>(Tagged<Object>(handler_address));
  Tagged<SharedFunctionInfo> shared = Wasm32JSFunctionShared(function);
  if (!shared->HasBuiltinId()) return false;

  JSGeneratorObject::ResumeMode resume_mode;
  switch (shared->builtin_id()) {
    case Builtin::kAsyncFunctionAwaitResolveClosure:
      resume_mode = JSGeneratorObject::kNext;
      break;
    case Builtin::kAsyncFunctionAwaitRejectClosure:
      resume_mode = JSGeneratorObject::kThrow;
      break;
    default:
      return false;
  }
  USE(rejected);
  return ResumeWasmAsyncFunctionAwait(
      isolate, function, value_address, resume_mode, out_result);
}

extern "C" Address Wasm32CallMicrotaskFunction(Isolate* isolate,
                                                Address callable_address) {
  ReadOnlyRoots roots(isolate);
  if (!IsSafeTaggedHandleValue(callable_address) ||
      (!IsJSFunction(Tagged<Object>(callable_address)) &&
       !IsJSBoundFunction(Tagged<Object>(callable_address)))) {
    return roots.exception().ptr();
  }
  HandleScope scope(isolate);
  Handle<Object> callable(Tagged<Object>(callable_address), isolate);
  return WasmJSEntry(isolate->isolate_data()->isolate_root(),
                     roots.undefined_value().ptr(), (*callable).ptr(),
                     roots.undefined_value().ptr(), JSParameterCount(0),
                     nullptr);
}

extern "C" bool Wasm32TryRunPromiseAllElementClosure(
    Isolate* isolate, Address handler_address, Address value_address,
    bool rejected, Address* out_result) {
  if (!IsSafeTaggedHandleValue(handler_address) ||
      !IsJSFunction(Tagged<Object>(handler_address))) {
    return false;
  }
  Tagged<JSFunction> function =
      Cast<JSFunction>(Tagged<Object>(handler_address));
  if (Wasm32JSFunctionShared(function) !=
      *isolate->factory()->promise_all_resolve_element_closure_shared_fun()) {
    return false;
  }

  constexpr int kPromiseAllRemainingSlot = 0;
  constexpr int kPromiseAllValuesSlot = 1;
  constexpr int kPromiseAllAggregateSlot = 2;
  constexpr int kPromiseAllIndexSlot = Context::MIN_CONTEXT_EXTENDED_SLOTS;
  Tagged<Context> context = Wasm32JSFunctionContext(function);
  Tagged<Object> state_object = context->extension();
  Tagged<Object> index_object = context->GetNoCell(kPromiseAllIndexSlot);
  if (!IsFixedArray(state_object) || !IsSmi(index_object)) return false;

  int index = Smi::ToInt(Cast<Smi>(index_object));
  ReadOnlyRoots roots(isolate);
  if (index < 0) {
    *out_result = roots.undefined_value().ptr();
    return true;
  }
  context->SetNoCell(kPromiseAllIndexSlot, Smi::FromInt(-1));

  HandleScope scope(isolate);
  DirectHandle<FixedArray> state(Cast<FixedArray>(state_object), isolate);
  Tagged<Object> aggregate_object = state->get(kPromiseAllAggregateSlot);
  Tagged<Object> values_object = state->get(kPromiseAllValuesSlot);
  if (!IsJSPromise(aggregate_object) || !IsFixedArray(values_object)) {
    return false;
  }
  DirectHandle<JSPromise> aggregate(Cast<JSPromise>(aggregate_object), isolate);
  DirectHandle<Object> value(
      Tagged<Object>(SafeTaggedOrUndefined(isolate, value_address)), isolate);
  if (rejected) {
    state->set(kPromiseAllRemainingSlot, Smi::zero());
    JSPromise::Reject(aggregate, value);
  } else {
    Tagged<Object> remaining_object = state->get(kPromiseAllRemainingSlot);
    if (!IsSmi(remaining_object)) return false;
    int remaining = Smi::ToInt(Cast<Smi>(remaining_object));
    if (remaining <= 0) {
      *out_result = roots.undefined_value().ptr();
      return true;
    }
    DirectHandle<FixedArray> values(Cast<FixedArray>(values_object), isolate);
    if (index >= values->length()) return false;
    values->set(index, *value);
    --remaining;
    state->set(kPromiseAllRemainingSlot, Smi::FromInt(remaining));
    if (remaining == 0) {
      DirectHandle<JSArray> result =
          isolate->factory()->NewJSArrayWithElements(
              values, PACKED_ELEMENTS, values->length());
      DirectHandle<Object> resolve_result;
      if (!JSPromise::Resolve(aggregate, result).ToHandle(&resolve_result)) {
        *out_result = roots.exception().ptr();
        return true;
      }
    }
  }
  *out_result = roots.undefined_value().ptr();
  return true;
}

bool TryRunAsyncFunctionAwaitClosureBuiltin(
    Isolate* isolate, Builtin builtin, Tagged<JSFunction> function,
    int actual_argc, Address* argv, Address* out_result) {
  JSGeneratorObject::ResumeMode resume_mode;
  switch (builtin) {
    case Builtin::kAsyncFunctionAwaitResolveClosure:
      resume_mode = JSGeneratorObject::ResumeMode::kNext;
      break;
    case Builtin::kAsyncFunctionAwaitRejectClosure:
      resume_mode = JSGeneratorObject::ResumeMode::kThrow;
      break;
    default:
      return false;
  }
  Address value_address =
      actual_argc > 0 ? argv[0] : ReadOnlyRoots(isolate).undefined_value().ptr();
  return ResumeWasmAsyncFunctionAwait(isolate, function, value_address,
                                      resume_mode, out_result);
}

bool TryFallbackJSEntryBuiltin(Isolate* isolate, Builtin builtin,
                               Tagged<JSFunction> function_value,
                               Address receiver_value,
                               Address new_target_value, int actual_argc,
                               Address* argv_values, Address* out_result) {
  if (actual_argc < 0 || actual_argc > kMaxWasmCallArgs) return false;
  HandleScope function_scope(isolate);
  Handle<JSFunction> function = handle(function_value, isolate);
  Address call_values[2] = {receiver_value, new_target_value};
  WasmTemporaryRootScope call_roots(isolate, call_values, 2);
  WasmTemporaryRootScope arg_roots(isolate, argv_values, actual_argc);
  Address& receiver = call_roots.data()[0];
  Address& new_target = call_roots.data()[1];
  Address* argv = arg_roots.data();

  ReadOnlyRoots roots(isolate);
  if (builtin == Builtin::kMapPrototypeKeys ||
      builtin == Builtin::kMapPrototypeValues ||
      builtin == Builtin::kMapPrototypeEntries) {
    Tagged<Object> receiver_object(
        SafeTaggedOrUndefined(isolate, receiver));
    if (!IsJSMap(receiver_object)) return false;

    HandleScope scope(isolate);
    DirectHandle<Map> iterator_map;
    if (builtin == Builtin::kMapPrototypeKeys) {
      iterator_map = direct_handle(
          isolate->native_context()->map_key_iterator_map(), isolate);
    } else if (builtin == Builtin::kMapPrototypeValues) {
      iterator_map = direct_handle(
          isolate->native_context()->map_value_iterator_map(), isolate);
    } else {
      iterator_map = direct_handle(
          isolate->native_context()->map_key_value_iterator_map(), isolate);
    }
    DirectHandle<JSMapIterator> iterator = Cast<JSMapIterator>(
        isolate->factory()->NewJSObjectFromMap(iterator_map));
    iterator->set_table(Cast<JSMap>(receiver_object)->table());
    iterator->set_index(Smi::zero());
    *out_result = (*iterator).ptr();
#ifdef __wasi__
    if (builtin == Builtin::kMapPrototypeValues) {
      std::fprintf(stderr, "WASM32_MAP_VALUES_RESULT result=0x%x\n",
                   static_cast<unsigned>(*out_result));
      std::fflush(stderr);
    }
#endif
    return true;
  }
  if (builtin == Builtin::kSetPrototypeValues ||
      builtin == Builtin::kSetPrototypeEntries) {
    Tagged<Object> receiver_object(
        SafeTaggedOrUndefined(isolate, receiver));
    if (!IsJSSet(receiver_object)) return false;

    HandleScope scope(isolate);
    DirectHandle<Map> iterator_map = direct_handle(
        builtin == Builtin::kSetPrototypeEntries
            ? isolate->native_context()->set_key_value_iterator_map()
            : isolate->native_context()->set_value_iterator_map(),
        isolate);
    DirectHandle<JSSetIterator> iterator = Cast<JSSetIterator>(
        isolate->factory()->NewJSObjectFromMap(iterator_map));
    iterator->set_table(Cast<JSSet>(receiver_object)->table());
    iterator->set_index(Smi::zero());
    *out_result = (*iterator).ptr();
    return true;
  }
  if (builtin == Builtin::kArrayIteratorPrototypeNext ||
      builtin == Builtin::kMapIteratorPrototypeNext ||
      builtin == Builtin::kSetIteratorPrototypeNext) {
    HandleScope scope(isolate);
    DirectHandle<Object> callable(*function, isolate);
    DirectHandle<Object> iterator(
        Tagged<Object>(SafeTaggedOrUndefined(isolate, receiver)), isolate);
    if (builtin == Builtin::kArrayIteratorPrototypeNext) {
      return TryRunArrayIteratorPrototypeNextBuiltin(
          isolate, callable, iterator, out_result);
    }
    if (builtin == Builtin::kMapIteratorPrototypeNext) {
      return TryRunMapIteratorPrototypeNextBuiltin(isolate, callable, iterator,
                                                   out_result);
    }
    return TryRunSetIteratorPrototypeNextBuiltin(isolate, callable, iterator,
                                                 out_result);
  }
  if (builtin == Builtin::kFunctionPrototypeCall && actual_argc >= 1 &&
      IsSafeTaggedHandleValue(receiver) &&
      IsJSFunction(Tagged<Object>(receiver))) {
    HandleScope scope(isolate);
    DirectHandle<Object> target(
        Cast<JSFunction>(Tagged<Object>(receiver)), isolate);
    if (IsJSFunctionBuiltin(isolate, target,
                            Builtin::kTypedArrayPrototypeSet)) {
      DirectHandle<Object> target_receiver(
          Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[0])), isolate);
      DirectHandle<Object> call_args[kMaxWasmCallArgs];
      int call_argc = actual_argc - 1;
      if (call_argc > kMaxWasmCallArgs) return false;
      for (int i = 0; i < call_argc; ++i) {
        call_args[i] = direct_handle(
            Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[i + 1])),
            isolate);
      }
      return TryRunTypedArrayPrototypeSetBuiltin(
          isolate, target, target_receiver, call_argc, call_args, out_result);
    }
  }
#ifdef __wasi__
  static int wasm_try_fallback_api_entry_count = 0;
  if ((builtin == Builtin::kHandleApiCallOrConstruct ||
       builtin == Builtin::kCallApiCallbackGeneric ||
       builtin == Builtin::kCallApiCallbackOptimizedNoProfiling ||
       builtin == Builtin::kCallApiCallbackOptimized) &&
      kTraceWasmFallbackDetails &&
      wasm_try_fallback_api_entry_count < 64) {
    ++wasm_try_fallback_api_entry_count;
    Tagged<SharedFunctionInfo> entry_shared = Wasm32JSFunctionShared(*function);
    PrintF("TryFallbackJSEntryBuiltin: API entry #%d builtin=%s "
           "actual_argc=%d is_api=%d function=0x%x\n",
           wasm_try_fallback_api_entry_count, Builtins::name(builtin),
           actual_argc, entry_shared->IsApiFunction() ? 1 : 0,
           static_cast<unsigned>((*function).ptr()));
  }
#endif
  if (TryRunGeneratorResumeBuiltin(isolate, builtin, receiver, actual_argc,
                                   argv, out_result)) {
    return true;
  }
  if (TryRunAsyncFunctionAwaitClosureBuiltin(
          isolate, builtin, *function, actual_argc, argv, out_result)) {
    return true;
  }
  if (builtin == Builtin::kCallAsyncModuleFulfilled ||
      builtin == Builtin::kCallAsyncModuleRejected) {
#ifdef __wasi__
    Address callback_argument =
        actual_argc > 0 ? SafeTaggedOrUndefined(isolate, argv[0])
                        : roots.undefined_value().ptr();
    std::fprintf(stderr,
                 "WASM32_ASYNC_MODULE_CALLBACK builtin=%s argc=%d arg0=0x%x "
                 "undefined=0x%x\n",
                 Builtins::name(builtin), actual_argc,
                 static_cast<unsigned>(callback_argument),
                 static_cast<unsigned>(roots.undefined_value().ptr()));
    std::fflush(stderr);
#endif
    Tagged<Context> saved_context = isolate->context();
    Tagged<Context> callback_context = Wasm32JSFunctionContext(*function);
    isolate->set_context(callback_context);

    HandleScope scope(isolate);
    Tagged<Object> module_value = callback_context->GetNoCell(
        SourceTextModule::ExecuteAsyncModuleContextSlots::kModule);
    if (!IsSourceTextModule(module_value)) {
      *out_result = roots.exception().ptr();
      isolate->set_context(saved_context);
      return true;
    }

    Handle<SourceTextModule> module(
        Cast<SourceTextModule>(module_value), isolate);
    if (builtin == Builtin::kCallAsyncModuleFulfilled) {
      Maybe<bool> completed =
          SourceTextModule::AsyncModuleExecutionFulfilled(isolate, module);
      *out_result = completed.IsNothing() ? roots.exception().ptr()
                                          : roots.undefined_value().ptr();
    } else {
      Address exception_address =
          actual_argc > 0 ? SafeTaggedOrUndefined(isolate, argv[0])
                          : roots.undefined_value().ptr();
      DirectHandle<Object> exception(Tagged<Object>(exception_address),
                                     isolate);
      SourceTextModule::AsyncModuleExecutionRejected(isolate, module,
                                                     exception);
      *out_result = roots.undefined_value().ptr();
    }
    isolate->set_context(saved_context);
    return true;
  }
  if (builtin == Builtin::kCompileLazy) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    Address compile_values[3] = {(*function).ptr(), receiver,
                                 saved_context.ptr()};
    WasmTemporaryRootScope compile_roots(isolate, compile_values, 3);
    WasmTemporaryRootScope compile_args_roots(isolate, argv, actual_argc);
    Address code = kNullAddress;
    if (Wasm32JSFunctionShared(*function)->HasBytecodeArray()) {
      function->UpdateCode(
          isolate, *BUILTIN_CODE(isolate, InterpreterEntryTrampoline));
      code = function->code(isolate)->ptr();
    } else {
      const Runtime::Function* compile_lazy =
          Runtime::FunctionForId(Runtime::kCompileLazy);
      using RuntimeEntry = Address (*)(int, Address*, Isolate*);
      code = reinterpret_cast<RuntimeEntry>(compile_lazy->entry)(
          1, compile_roots.data(), isolate);
    }
    if (isolate->has_exception() || code == roots.exception().ptr()) {
      isolate->set_context(
          Cast<Context>(Tagged<Object>(compile_roots.data()[2])));
      *out_result = roots.exception().ptr();
      return true;
    }

    Address nested_values[kWasmMaxOutgoingArgSlots == 0
                              ? 1
                              : kWasmMaxOutgoingArgSlots];
    Address* nested_argv[kWasmMaxOutgoingArgSlots == 0
                             ? 1
                             : kWasmMaxOutgoingArgSlots];
    int nested_argc = actual_argc;
    if (nested_argc > kWasmMaxOutgoingArgSlots) {
      nested_argc = kWasmMaxOutgoingArgSlots;
    }
    for (int i = 0; i < nested_argc; ++i) {
      nested_values[i] = compile_args_roots.data()[i];
      nested_argv[i] = &nested_values[i];
    }

    Address root = g_wasm_regs[kWasmRegRoot];
    if (root == kNullAddress) root = g_wasm_regs[SlotFor(kRootRegister)];
    Address compiled_function = compile_roots.data()[0];
    Address compiled_receiver = compile_roots.data()[1];
    *out_result = WasmJSEntry(root, new_target, compiled_function,
                              compiled_receiver,
                              nested_argc + kJSArgcReceiverSlots, nested_argv);
    isolate->set_context(
        Cast<Context>(Tagged<Object>(compile_roots.data()[2])));
    return true;
  }

  if (builtin == Builtin::kFunctionPrototypeHasInstance) {
    Address value = actual_argc > 0 ? argv[0] : roots.undefined_value().ptr();
    if (TryOrdinaryHasInstanceForWasm(isolate, receiver, value, out_result)) {
      return true;
    }
  }

  if (builtin == Builtin::kFunctionConstructor) {
    Tagged<Context> saved_context = isolate->context();
    Tagged<Context> function_context = Wasm32JSFunctionContext(*function);
    isolate->set_context(function_context);

    HandleScope scope(isolate);
    DirectHandle<Object> result;
    if (!Wasm32CreateDynamicFunction(isolate, *function, new_target,
                                     actual_argc, argv, "function")
             .ToHandle(&result)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    *out_result = (*result).ptr();
    isolate->set_context(saved_context);
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmJSEntry: fallback FunctionConstructor result=0x%x\n",
             static_cast<unsigned>(*out_result));
    }
    return true;
  }

  if (builtin == Builtin::kIsTraceCategoryEnabled) {
    *out_result = roots.false_value().ptr();
    return true;
  }

  if (builtin == Builtin::kConsoleLog) {
    Tagged<Context> saved_context = isolate->context();
    Tagged<Context> function_context = Wasm32JSFunctionContext(*function);
    Tagged<Context> native_context = function_context;
    if (TryResolveWasm32NativeContext(function_context, &native_context)) {
      isolate->set_context(native_context);
    } else {
      isolate->set_context(function_context);
    }

    HandleScope scope(isolate);
    for (int i = 0; i < actual_argc; ++i) {
      DirectHandle<Object> input(
          Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[i])), isolate);
      DirectHandle<String> string;
      if (!Object::ToString(isolate, input).ToHandle(&string)) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
      string = String::Flatten(isolate, string);
      size_t text_length = 0;
      std::unique_ptr<char[]> text = string->ToCString(
          0, string->length(), &text_length);
      if (i != 0) std::fputc(' ', stdout);
      if (text_length != 0) {
        std::fwrite(text.get(), 1, text_length, stdout);
      }
    }
    std::fputc('\n', stdout);
    std::fflush(stdout);

    *out_result = roots.undefined_value().ptr();
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kDateNow) {
    HandleScope scope(isolate);
    DirectHandle<Number> now = isolate->factory()->NewNumberFromInt64(
        JSDate::CurrentTimeValue(isolate));
    *out_result = (*now).ptr();
    return true;
  }

  if (builtin == Builtin::kMathMin || builtin == Builtin::kMathMax) {
    HandleScope scope(isolate);
    const bool is_min = builtin == Builtin::kMathMin;
    double result = is_min ? V8_INFINITY : -V8_INFINITY;
    for (int i = 0; i < actual_argc; ++i) {
      DirectHandle<Object> input(
          Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[i])), isolate);
      DirectHandle<Number> number;
      if (!Object::ToNumber(isolate, input).ToHandle(&number)) {
        *out_result = roots.exception().ptr();
        return true;
      }

      const double value = Object::NumberValue(*number);
      if (std::isnan(value)) {
        result = std::numeric_limits<double>::quiet_NaN();
        break;
      }
      if ((is_min && value < result) || (!is_min && value > result)) {
        result = value;
      } else if (value == 0 && result == 0) {
        if (is_min && std::signbit(value)) result = -0.0;
        if (!is_min && !std::signbit(value)) result = 0.0;
      }
    }

    DirectHandle<Number> number = isolate->factory()->NewNumber(result);
    *out_result = (*number).ptr();
    return true;
  }

  if (builtin == Builtin::kNumberConstructor) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<Number> number = direct_handle(Smi::zero(), isolate);
    if (actual_argc > 0) {
      DirectHandle<Object> input(
          Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[0])), isolate);
      if (IsBigInt(*input)) {
        number = BigInt::ToNumber(isolate, Cast<BigInt>(input));
      } else if (!Object::ToNumber(isolate, input).ToHandle(&number)) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
    }

    Address new_target_address = SafeTaggedOrUndefined(isolate, new_target);
    if (IsUndefined(Tagged<Object>(new_target_address), isolate)) {
      *out_result = (*number).ptr();
      isolate->set_context(saved_context);
      return true;
    }
    if (!IsSafeTaggedHandleValue(new_target_address) ||
        !IsJSReceiver(Tagged<Object>(new_target_address))) {
      isolate->set_context(saved_context);
      return false;
    }

    DirectHandle<JSFunction> target = direct_handle(*function, isolate);
    DirectHandle<JSReceiver> constructor_new_target =
        direct_handle(Cast<JSReceiver>(Tagged<Object>(new_target_address)),
                      isolate);
    DirectHandle<Map> map;
    if (!JSFunction::GetDerivedMap(isolate, target, constructor_new_target)
             .ToHandle(&map)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    DirectHandle<JSObject> object =
        isolate->factory()->NewFastOrSlowJSObjectFromMap(map);
    DirectHandle<JSPrimitiveWrapper> wrapper =
        Cast<JSPrimitiveWrapper>(object);
    wrapper->set_value(Cast<JSAny>(*number));
    *out_result = (*wrapper).ptr();
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kNumberPrototypeValueOf) {
    Tagged<Object> value(SafeTaggedOrUndefined(isolate, receiver));
    if (IsNumber(value)) {
      *out_result = value.ptr();
      return true;
    }
    if (IsJSPrimitiveWrapper(value)) {
      Tagged<Object> wrapped = Cast<JSPrimitiveWrapper>(value)->value();
      if (IsNumber(wrapped)) {
        *out_result = wrapped.ptr();
        return true;
      }
    }
  }

  if (builtin == Builtin::kNumberIsFinite ||
      builtin == Builtin::kNumberIsInteger ||
      builtin == Builtin::kNumberIsNaN ||
      builtin == Builtin::kNumberIsSafeInteger) {
    Tagged<Object> input(
        actual_argc > 0 ? SafeTaggedOrUndefined(isolate, argv[0])
                        : roots.undefined_value().ptr());
    bool result = false;
    if (IsNumber(input)) {
      double value = Object::NumberValue(input);
      if (builtin == Builtin::kNumberIsFinite) {
        result = std::isfinite(value);
      } else if (builtin == Builtin::kNumberIsNaN) {
        result = std::isnan(value);
      } else {
        result = std::isfinite(value) && std::trunc(value) == value;
        if (builtin == Builtin::kNumberIsSafeInteger) {
          result = result && std::abs(value) <= 9007199254740991.0;
        }
      }
    }
    *out_result = result ? roots.true_value().ptr() : roots.false_value().ptr();
    return true;
  }

  if (builtin == Builtin::kStringConstructor) {
    Address input_address = actual_argc > 0
                                ? SafeTaggedOrUndefined(isolate, argv[0])
                                : roots.undefined_value().ptr();
    Address new_target_address = SafeTaggedOrUndefined(isolate, new_target);
    bool is_function_call =
        IsUndefined(Tagged<Object>(new_target_address), isolate);

    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<String> string;
    if (actual_argc == 0) {
      string = isolate->factory()->empty_string();
    } else if (is_function_call && IsSymbol(Tagged<Object>(input_address))) {
      Address runtime_arg = input_address;
      const Runtime::Function* symbol_descriptive_string =
          Runtime::FunctionForId(Runtime::kSymbolDescriptiveString);
      using RuntimeEntry = Address (*)(int, Address*, Isolate*);
      Address result = reinterpret_cast<RuntimeEntry>(
          symbol_descriptive_string->entry)(1, &runtime_arg, isolate);
      if (isolate->has_exception() || result == roots.exception().ptr()) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
      string = direct_handle(Cast<String>(Tagged<Object>(result)), isolate);
    } else {
      DirectHandle<Object> input =
          direct_handle(Tagged<Object>(input_address), isolate);
      if (!Object::ToString(isolate, input).ToHandle(&string)) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
    }

    if (is_function_call) {
      *out_result = (*string).ptr();
      isolate->set_context(saved_context);
      return true;
    }
    if (!IsSafeTaggedHandleValue(new_target_address) ||
        !IsJSReceiver(Tagged<Object>(new_target_address))) {
      isolate->set_context(saved_context);
      return false;
    }

    DirectHandle<JSFunction> target = direct_handle(*function, isolate);
    DirectHandle<JSReceiver> constructor_new_target =
        direct_handle(Cast<JSReceiver>(Tagged<Object>(new_target_address)),
                      isolate);
    DirectHandle<Map> map;
    if (!JSFunction::GetDerivedMap(isolate, target, constructor_new_target)
             .ToHandle(&map)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    DirectHandle<JSObject> object =
        isolate->factory()->NewFastOrSlowJSObjectFromMap(map);
    DirectHandle<JSPrimitiveWrapper> wrapper =
        Cast<JSPrimitiveWrapper>(object);
    wrapper->set_value(Cast<JSAny>(*string));
    *out_result = (*wrapper).ptr();
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kObjectGetPrototypeOf) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<Object> target(
        Tagged<Object>(actual_argc > 0
                           ? SafeTaggedOrUndefined(isolate, argv[0])
                           : roots.undefined_value().ptr()),
        isolate);
    DirectHandle<JSReceiver> object;
    if (!Object::ToObject(isolate, target, "Object.getPrototypeOf")
             .ToHandle(&object)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<JSPrototype> prototype;
    if (!JSReceiver::GetPrototype(isolate, object).ToHandle(&prototype)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    *out_result = (*prototype).ptr();
    isolate->set_context(saved_context);
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmJSEntry: fallback ObjectGetPrototypeOf result=0x%x\n",
             static_cast<unsigned>(*out_result));
    }
    return true;
  }

  if (builtin == Builtin::kArrayForEach) {
    if (actual_argc > kMaxWasmCallArgs) {
      *out_result = roots.exception().ptr();
      return true;
    }

    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<Object> callable = direct_handle(*function, isolate);
    DirectHandle<Object> receiver_object(
        Tagged<Object>(SafeTaggedOrUndefined(isolate, receiver)), isolate);
    DirectHandle<Object> args[kMaxWasmCallArgs];
    for (int i = 0; i < actual_argc; ++i) {
      args[i] = direct_handle(
          Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[i])), isolate);
    }

    bool handled = TryRunArrayForEachBuiltin(
        isolate, callable, receiver_object, actual_argc, args, out_result);
    isolate->set_context(saved_context);
    return handled;
  }

  if (builtin == Builtin::kBooleanConstructor) {
    Address input_address = actual_argc > 0
                                ? SafeTaggedOrUndefined(isolate, argv[0])
                                : roots.undefined_value().ptr();
    Tagged<Object> input(input_address);
    bool bool_value = false;
    if (IsTrue(input, roots)) {
      bool_value = true;
    } else if (IsFalse(input, roots) || IsUndefined(input, roots) ||
               IsNull(input, roots)) {
      bool_value = false;
    } else if (IsSmi(input)) {
      bool_value = Smi::ToInt(input) != 0;
    } else {
      bool_value = Object::BooleanValue(input, isolate);
    }
    Address boolean_address =
        bool_value ? roots.true_value().ptr() : roots.false_value().ptr();

    Address new_target_address = SafeTaggedOrUndefined(isolate, new_target);
    if (IsUndefined(Tagged<Object>(new_target_address), isolate)) {
      *out_result = boolean_address;
      return true;
    }
    if (!IsSafeTaggedHandleValue(new_target_address) ||
        !IsJSReceiver(Tagged<Object>(new_target_address))) {
      return false;
    }

    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<JSFunction> target = direct_handle(*function, isolate);
    DirectHandle<JSReceiver> constructor_new_target =
        direct_handle(Cast<JSReceiver>(Tagged<Object>(new_target_address)),
                      isolate);
    DirectHandle<Map> map;
    if (!JSFunction::GetDerivedMap(isolate, target, constructor_new_target)
             .ToHandle(&map)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    DirectHandle<JSObject> object =
        isolate->factory()->NewFastOrSlowJSObjectFromMap(map);
    DirectHandle<JSPrimitiveWrapper> wrapper =
        Cast<JSPrimitiveWrapper>(object);
    wrapper->set_value(Cast<JSAny>(Tagged<Object>(boolean_address)));
    *out_result = (*wrapper).ptr();
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kRegExpPrototypeTest) {
    Address subject_address =
        actual_argc > 0 ? SafeTaggedOrUndefined(isolate, argv[0])
                        : roots.undefined_value().ptr();
    Address exec_result = roots.exception().ptr();
    if (!TryRunRegExpPrototypeExecDirect(isolate, *function, receiver,
                                         subject_address, &exec_result)) {
      return false;
    }
    if (isolate->has_exception() || exec_result == roots.exception().ptr()) {
      *out_result = roots.exception().ptr();
    } else {
      *out_result = IsNull(Tagged<Object>(exec_result), isolate)
                        ? roots.false_value().ptr()
                        : roots.true_value().ptr();
    }
    return true;
  }

  if (builtin == Builtin::kRegExpPrototypeExec) {
    Address receiver_address = SafeTaggedOrUndefined(isolate, receiver);
    Address subject_address = actual_argc > 0
                                  ? SafeTaggedOrUndefined(isolate, argv[0])
                                  : roots.undefined_value().ptr();
    if (TryRunMissingRegExpPrototypeExecCall(isolate, receiver_address,
                                             subject_address, out_result)) {
      return true;
    }
    if (!IsSafeTaggedHandleValue(receiver_address) ||
        !IsJSRegExp(Tagged<Object>(receiver_address))) {
#ifdef __wasi__
      PrintF("WasmJSEntry: RegExpPrototypeExec non-RegExp receiver ");
      DumpRuntimeArg("receiver", 0, receiver_address);
      DumpRuntimeArg(" subject", 0, subject_address);
      PrintF(" throwing TypeError\n");
      static int regexp_non_receiver_trace_count = 0;
      if (regexp_non_receiver_trace_count < 12) {
        ++regexp_non_receiver_trace_count;
        PrintF("WasmJSEntry: regexp diagnostic argc=%d receiver=0x%x "
               "subject=0x%x\n",
               actual_argc, static_cast<unsigned>(receiver_address),
               static_cast<unsigned>(subject_address));
        PrintF("  regexp target");
        DumpFunctionSourceForTrace((*function).ptr());
        PrintF("\n  current_frame");
        Address current_function =
            g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
                StandardFrameConstants::kFunctionOffset)];
        DumpFunctionSourceForTrace(current_function);
        PrintF("\n");
        DumpCurrentInterpreterBytecodeForTrace("regexp.current");
        PrintF("  receiver_props");
        DumpNamedDataPropertyForTrace(isolate, receiver_address,
                                      kWasm32RegExpPatternMarker);
        DumpNamedDataPropertyForTrace(isolate, receiver_address,
                                      kWasm32RegExpFlagsMarker);
        DumpNamedDataPropertyForTrace(isolate, receiver_address, "lastIndex");
        DumpNamedDataPropertyForTrace(isolate, receiver_address, "constructor");
        PrintF("\n");
        for (int i = 0; i < 4; ++i) {
          DumpRuntimeArg(
              "  regexp.param", i,
              ReadInterpreterRegister(
                  interpreter::Register::FromParameterIndex(i)));
        }
        PrintF("\n");
        for (int i = 0; i < 8; ++i) {
          DumpRuntimeArg("  regexp.local", i,
                         ReadInterpreterRegister(interpreter::Register(i)));
        }
        PrintF("\n");
      }
#endif
      Tagged<Context> saved_context = isolate->context();
      isolate->set_context(Wasm32JSFunctionContext(*function));
      HandleScope scope(isolate);
      DirectHandle<Object> receiver_object =
          IsSafeTaggedHandleValue(receiver_address)
              ? direct_handle(Tagged<Object>(receiver_address), isolate)
              : direct_handle(roots.undefined_value(), isolate);
      isolate->Throw(*isolate->factory()->NewTypeError(
          MessageTemplate::kIncompatibleMethodReceiver,
          isolate->factory()->NewStringFromAsciiChecked(
              "RegExp.prototype.exec"),
          receiver_object));
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    return TryRunRegExpPrototypeExecDirect(isolate, *function, receiver_address,
                                           subject_address, out_result);
  }

  if (builtin == Builtin::kErrorConstructor) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<JSFunction> target = direct_handle(*function, isolate);
    DirectHandle<Object> new_target_handle(
        Tagged<Object>(SafeTaggedOrUndefined(isolate, new_target)), isolate);
    DirectHandle<Object> message =
        actual_argc > 0
            ? direct_handle(Tagged<Object>(SafeTaggedOrUndefined(isolate,
                                                                 argv[0])),
                            isolate)
            : direct_handle(roots.undefined_value(), isolate);
    DirectHandle<Object> options =
        actual_argc > 1
            ? direct_handle(Tagged<Object>(SafeTaggedOrUndefined(isolate,
                                                                 argv[1])),
                            isolate)
            : direct_handle(roots.undefined_value(), isolate);

    DirectHandle<JSObject> result;
    if (!ErrorUtils::Construct(isolate, target, new_target_handle, message,
                               options)
             .ToHandle(&result)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

#ifdef __wasi__
    PrintF("WasmJSEntry: fallback ErrorConstructor result ");
    DumpRuntimeArg("result", 0, (*result).ptr());
    PrintF("\n");
#endif
    *out_result = (*result).ptr();
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kGlobalDecodeURI ||
      builtin == Builtin::kGlobalDecodeURIComponent ||
      builtin == Builtin::kGlobalEncodeURI ||
      builtin == Builtin::kGlobalEncodeURIComponent) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<Object> argument =
        actual_argc > 0
            ? direct_handle(
                  Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[0])),
                  isolate)
            : direct_handle(roots.undefined_value(), isolate);
    DirectHandle<String> string;
    if (!Object::ToString(isolate, argument).ToHandle(&string)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    MaybeDirectHandle<String> maybe_result;
    if (builtin == Builtin::kGlobalDecodeURI) {
      maybe_result = Uri::DecodeUri(isolate, string);
    } else if (builtin == Builtin::kGlobalDecodeURIComponent) {
      maybe_result = Uri::DecodeUriComponent(isolate, string);
    } else if (builtin == Builtin::kGlobalEncodeURI) {
      maybe_result = Uri::EncodeUri(isolate, string);
    } else {
      maybe_result = Uri::EncodeUriComponent(isolate, string);
    }

    DirectHandle<String> result;
    if (!maybe_result.ToHandle(&result)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    *out_result = (*result).ptr();
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kPromiseCapabilityDefaultResolve ||
      builtin == Builtin::kPromiseCapabilityDefaultReject) {
    Tagged<Context> function_context = Wasm32JSFunctionContext(*function);
    HandleScope scope(isolate);
    DirectHandle<Context> resolving_context(function_context, isolate);
    Tagged<Object> promise_object =
        resolving_context->GetNoCell(PromiseBuiltins::kPromiseSlot);
    if (!IsJSPromise(promise_object)) {
      *out_result = roots.exception().ptr();
      return true;
    }

    if (Object::BooleanValue(
            resolving_context->GetNoCell(
                PromiseBuiltins::kAlreadyResolvedSlot),
            isolate)) {
      *out_result = roots.undefined_value().ptr();
      return true;
    }
    resolving_context->SetNoCell(PromiseBuiltins::kAlreadyResolvedSlot,
                                 roots.true_value());

    DirectHandle<JSPromise> promise(Cast<JSPromise>(promise_object), isolate);
    DirectHandle<Object> value =
        actual_argc > 0
            ? direct_handle(
                  Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[0])),
                  isolate)
            : direct_handle(roots.undefined_value(), isolate);
    if (builtin == Builtin::kPromiseCapabilityDefaultReject) {
#ifdef __wasi__
      if (IsUndefined(*value, roots)) {
        std::fprintf(stderr,
                     "WASM32_REJECT_UNDEFINED source=capability promise=0x%x "
                     "function=0x%x\n",
                     static_cast<unsigned>((*promise).ptr()),
                     static_cast<unsigned>((*function).ptr()));
        std::fflush(stderr);
      }
#endif
      JSPromise::Reject(promise, value);
    } else {
      DirectHandle<Object> resolve_result;
      if (!JSPromise::Resolve(promise, value).ToHandle(&resolve_result)) {
        *out_result = roots.exception().ptr();
        return true;
      }
    }
    *out_result = roots.undefined_value().ptr();
    return true;
  }

  if (builtin == Builtin::kPromiseAll) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));
    HandleScope scope(isolate);
    DirectHandle<Object> iterable =
        actual_argc > 0
            ? direct_handle(
                  Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[0])),
                  isolate)
            : direct_handle(roots.undefined_value(), isolate);
    if (!IsJSArray(*iterable)) {
      isolate->Throw(*isolate->factory()->NewTypeError(
          MessageTemplate::kNotIterable, iterable));
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<JSArray> input = Cast<JSArray>(iterable);
    uint32_t length = 0;
    if (!Object::ToArrayLength(input->length(), &length) ||
        length > static_cast<uint32_t>(FixedArray::kMaxLength)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    Handle<JSPromise> aggregate = isolate->factory()->NewJSPromise();
    DirectHandle<FixedArray> values =
        isolate->factory()->NewFixedArray(static_cast<int>(length));
    constexpr int kPromiseAllRemainingSlot = 0;
    constexpr int kPromiseAllValuesSlot = 1;
    constexpr int kPromiseAllAggregateSlot = 2;
    constexpr int kPromiseAllIndexSlot =
        Context::MIN_CONTEXT_EXTENDED_SLOTS;
    DirectHandle<FixedArray> state = isolate->factory()->NewFixedArray(3);
    state->set(kPromiseAllRemainingSlot, Smi::zero());
    state->set(kPromiseAllValuesSlot, *values);
    state->set(kPromiseAllAggregateSlot, *aggregate);
    int remaining = 0;
    DirectHandle<NativeContext> native_context = isolate->native_context();
    DirectHandle<SharedFunctionInfo> element_info =
        isolate->factory()->promise_all_resolve_element_closure_shared_fun();

    for (uint32_t i = 0; i < length; ++i) {
      DirectHandle<Object> element;
      if (!JSReceiver::GetElement(isolate, input, i).ToHandle(&element)) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
      if (!IsJSPromise(*element)) {
        values->set(static_cast<int>(i), *element);
        continue;
      }
      DirectHandle<JSPromise> promise = Cast<JSPromise>(element);
      if (promise->status() == Promise::kFulfilled) {
        values->set(static_cast<int>(i), promise->reactions_or_result());
        continue;
      }
      if (promise->status() == Promise::kRejected) {
        DirectHandle<Object> reason(promise->reactions_or_result(), isolate);
        JSPromise::Reject(aggregate, reason);
        *out_result = (*aggregate).ptr();
        isolate->set_context(saved_context);
        return true;
      }

      ++remaining;
      state->set(kPromiseAllRemainingSlot, Smi::FromInt(remaining));
      DirectHandle<Context> element_context =
          isolate->factory()->NewBuiltinContext(
              native_context, kPromiseAllIndexSlot + 1);
      element_context->set_extension(*state);
      element_context->SetNoCell(kPromiseAllIndexSlot,
                                 Smi::FromInt(static_cast<int>(i)));
      DirectHandle<JSFunction> element_closure =
          Factory::JSFunctionBuilder{isolate, element_info, element_context}
              .Build();
      DirectHandle<Object> then_args[] = {element_closure, element_closure};
      if (Execution::CallBuiltin(isolate, isolate->promise_then(), promise,
                                 base::VectorOf(then_args))
              .is_null()) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
    }

    if (remaining == 0) {
      DirectHandle<JSArray> result =
          isolate->factory()->NewJSArrayWithElements(
              values, PACKED_ELEMENTS, static_cast<int>(length));
      DirectHandle<Object> resolve_result;
      if (!JSPromise::Resolve(aggregate, result).ToHandle(&resolve_result)) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
    }

    *out_result = (*aggregate).ptr();
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kPromiseConstructor) {
    Tagged<Context> saved_context = isolate->context();
    Tagged<Context> function_context = Wasm32JSFunctionContext(*function);
    isolate->set_context(function_context);
    HandleScope scope(isolate);

    DirectHandle<Object> executor =
        actual_argc > 0
            ? direct_handle(
                  Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[0])),
                  isolate)
            : direct_handle(roots.undefined_value(), isolate);
    if (!IsCallable(*executor)) {
      isolate->Throw(*isolate->factory()->NewTypeError(
          MessageTemplate::kResolverNotAFunction, executor));
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    Handle<JSPromise> promise = isolate->factory()->NewJSPromise();
    DirectHandle<NativeContext> native_context = isolate->native_context();
    DirectHandle<Context> resolving_context =
        isolate->factory()->NewBuiltinContext(
            native_context, PromiseBuiltins::kPromiseContextLength);
    resolving_context->SetNoCell(PromiseBuiltins::kPromiseSlot, *promise);
    resolving_context->SetNoCell(PromiseBuiltins::kAlreadyResolvedSlot,
                                 roots.false_value());
    resolving_context->SetNoCell(PromiseBuiltins::kDebugEventSlot,
                                 roots.true_value());

    DirectHandle<SharedFunctionInfo> resolve_info =
        isolate->factory()->promise_capability_default_resolve_shared_fun();
    DirectHandle<SharedFunctionInfo> reject_info =
        isolate->factory()->promise_capability_default_reject_shared_fun();
    Handle<JSFunction> resolve =
        Factory::JSFunctionBuilder{isolate, resolve_info, resolving_context}
            .Build();
    Handle<JSFunction> reject =
        Factory::JSFunctionBuilder{isolate, reject_info, resolving_context}
            .Build();

    Address executor_args[2] = {(*resolve).ptr(), (*reject).ptr()};
    Address* executor_argv[2] = {&executor_args[0], &executor_args[1]};
    Address call_result = WasmJSEntry(
        isolate->isolate_data()->isolate_root(), roots.undefined_value().ptr(),
        (*executor).ptr(), roots.undefined_value().ptr(), JSParameterCount(2),
        executor_argv);
    if (isolate->has_exception()) {
      DirectHandle<Object> reason(isolate->exception(), isolate);
      isolate->clear_exception();
      isolate->clear_pending_message();
      bool already_resolved = Object::BooleanValue(
          resolving_context->GetNoCell(
              PromiseBuiltins::kAlreadyResolvedSlot),
          isolate);
      if (!already_resolved) {
        resolving_context->SetNoCell(
            PromiseBuiltins::kAlreadyResolvedSlot, roots.true_value());
        resolving_context->SetNoCell(PromiseBuiltins::kDebugEventSlot,
                                     roots.false_value());
        JSPromise::Reject(promise, reason, false);
      }
    }
    USE(call_result);

    *out_result = (*promise).ptr();
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kPromiseResolveTrampoline ||
      builtin == Builtin::kPromiseReject) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<Object> value =
        actual_argc > 0
            ? direct_handle(
                  Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[0])),
                  isolate)
            : direct_handle(roots.undefined_value(), isolate);
    Handle<JSPromise> promise = isolate->factory()->NewJSPromise();
    if (builtin == Builtin::kPromiseReject) {
      JSPromise::Reject(promise, value);
    } else {
      DirectHandle<Object> resolve_result;
      if (!JSPromise::Resolve(promise, value).ToHandle(&resolve_result)) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
    }

    *out_result = (*promise).ptr();
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kPromiseValueThunkFinally ||
      builtin == Builtin::kPromiseThrowerFinally) {
    Tagged<Context> function_context = Wasm32JSFunctionContext(*function);
    Tagged<Object> value = function_context->GetNoCell(
        PromiseBuiltins::PromiseValueThunkOrReasonContextSlot::kValueSlot);
    if (builtin == Builtin::kPromiseThrowerFinally) {
      isolate->Throw(value);
      *out_result = roots.exception().ptr();
    } else {
      *out_result = value.ptr();
    }
    return true;
  }

  if (builtin == Builtin::kPromiseThenFinally ||
      builtin == Builtin::kPromiseCatchFinally) {
    HandleScope scope(isolate);
    Tagged<Context> function_context = Wasm32JSFunctionContext(*function);
    DirectHandle<Object> on_finally(
        function_context->GetNoCell(
            PromiseBuiltins::PromiseFinallyContextSlot::kOnFinallySlot),
        isolate);
    DirectHandle<Object> undefined(roots.undefined_value(), isolate);
    DirectHandle<Object> finally_result;
    {
      WasmInterpreterStateSnapshot state(isolate);
      MaybeHandle<Object> maybe_result =
          Execution::Call(isolate, on_finally, undefined, {});
      bool succeeded = maybe_result.ToHandle(&finally_result);
      state.Restore();
      if (!succeeded) {
        *out_result = roots.exception().ptr();
        return true;
      }
    }

    Handle<JSPromise> finally_promise = isolate->factory()->NewJSPromise();
    DirectHandle<Object> resolve_result;
    if (!JSPromise::Resolve(finally_promise, finally_result)
             .ToHandle(&resolve_result)) {
      *out_result = roots.exception().ptr();
      return true;
    }

    Address original_value =
        actual_argc > 0 ? SafeTaggedOrUndefined(isolate, argv[0])
                        : roots.undefined_value().ptr();
    DirectHandle<NativeContext> native_context = isolate->native_context();
    DirectHandle<Context> value_context =
        isolate->factory()->NewBuiltinContext(
            native_context,
            PromiseBuiltins::kPromiseValueThunkOrReasonContextLength);
    value_context->SetNoCell(
        PromiseBuiltins::PromiseValueThunkOrReasonContextSlot::kValueSlot,
        Tagged<Object>(original_value));
    DirectHandle<SharedFunctionInfo> continuation_info =
        builtin == Builtin::kPromiseThenFinally
            ? isolate->factory()->promise_value_thunk_finally_shared_fun()
            : isolate->factory()->promise_thrower_finally_shared_fun();
    Handle<JSFunction> continuation =
        Factory::JSFunctionBuilder{isolate, continuation_info, value_context}
            .Build();

    Address then_args[1] = {(*continuation).ptr()};
    return TryFallbackJSEntryBuiltin(
        isolate, Builtin::kPromisePrototypeThen, *isolate->promise_then(),
        (*finally_promise).ptr(), roots.undefined_value().ptr(), 1, then_args,
        out_result);
  }

  if (builtin == Builtin::kPromisePrototypeFinally) {
    HandleScope scope(isolate);
    Address promise_address = SafeTaggedOrUndefined(isolate, receiver);
    if (!IsJSPromise(Tagged<Object>(promise_address))) {
      isolate->Throw(*isolate->factory()->NewTypeError(
          MessageTemplate::kIncompatibleMethodReceiver,
          isolate->factory()->NewStringFromAsciiChecked(
              "Promise.prototype.finally"),
          direct_handle(Tagged<Object>(promise_address), isolate)));
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<Object> on_finally =
        actual_argc > 0
            ? direct_handle(
                  Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[0])),
                  isolate)
            : direct_handle(roots.undefined_value(), isolate);
    DirectHandle<Object> then_finally = on_finally;
    DirectHandle<Object> catch_finally = on_finally;
    if (IsCallable(*on_finally)) {
      DirectHandle<NativeContext> native_context = isolate->native_context();
      DirectHandle<Context> finally_context =
          isolate->factory()->NewBuiltinContext(
              native_context, PromiseBuiltins::kPromiseFinallyContextLength);
      finally_context->SetNoCell(
          PromiseBuiltins::PromiseFinallyContextSlot::kOnFinallySlot,
          *on_finally);
      finally_context->SetNoCell(
          PromiseBuiltins::PromiseFinallyContextSlot::kConstructorSlot,
          native_context->promise_function());
      then_finally =
          Factory::JSFunctionBuilder{
              isolate,
              isolate->factory()->promise_then_finally_shared_fun(),
              finally_context}
              .Build();
      catch_finally =
          Factory::JSFunctionBuilder{
              isolate,
              isolate->factory()->promise_catch_finally_shared_fun(),
              finally_context}
              .Build();
    }

    Address then_args[2] = {(*then_finally).ptr(), (*catch_finally).ptr()};
    return TryFallbackJSEntryBuiltin(
        isolate, Builtin::kPromisePrototypeThen, *isolate->promise_then(),
        promise_address, roots.undefined_value().ptr(), 2, then_args,
        out_result);
  }

  if (builtin == Builtin::kPromisePrototypeThen) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    Address promise_address = SafeTaggedOrUndefined(isolate, receiver);
    if (!IsJSPromise(Tagged<Object>(promise_address))) {
      isolate->Throw(*isolate->factory()->NewTypeError(
          MessageTemplate::kIncompatibleMethodReceiver,
          isolate->factory()->NewStringFromAsciiChecked(
              "Promise.prototype.then"),
          direct_handle(Tagged<Object>(promise_address), isolate)));
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<JSPromise> promise(
        Cast<JSPromise>(Tagged<Object>(promise_address)), isolate);
    DirectHandle<Object> on_fulfilled =
        actual_argc > 0
            ? direct_handle(
                  Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[0])),
                  isolate)
            : direct_handle(roots.undefined_value(), isolate);
    DirectHandle<Object> on_rejected =
        actual_argc > 1
            ? direct_handle(
                  Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[1])),
                  isolate)
            : direct_handle(roots.undefined_value(), isolate);
    if (!IsCallable(*on_fulfilled)) {
      on_fulfilled = direct_handle(roots.undefined_value(), isolate);
    }
    if (!IsCallable(*on_rejected)) {
      on_rejected = direct_handle(roots.undefined_value(), isolate);
    }

    Handle<JSPromise> result_promise = isolate->factory()->NewJSPromise();
    if (promise->status() == Promise::kPending) {
      DirectHandle<PromiseReaction> reaction = Cast<PromiseReaction>(
          isolate->factory()->NewStruct(PROMISE_REACTION_TYPE));
      reaction->set_next(Cast<UnionOf<Smi, PromiseReaction>>(
          promise->reactions_or_result()));
      reaction->set_reject_handler(
          Cast<UnionOf<Undefined, JSCallable>>(*on_rejected));
      reaction->set_fulfill_handler(
          Cast<UnionOf<Undefined, JSCallable>>(*on_fulfilled));
      reaction->set_promise_or_capability(*result_promise);
#ifdef V8_ENABLE_CONTINUATION_PRESERVED_EMBEDDER_DATA
      reaction->set_continuation_preserved_embedder_data(
          roots.undefined_value());
#endif
      promise->set_reactions_or_result(*reaction);
    } else {
      bool fulfilled = promise->status() == Promise::kFulfilled;
      DirectHandle<Object> handler = fulfilled ? on_fulfilled : on_rejected;
      DirectHandle<Object> secondary = fulfilled ? on_rejected : on_fulfilled;
      DirectHandle<NativeContext> handler_context;
      if (!IsJSReceiver(*handler) ||
          !JSReceiver::GetContextForMicrotask(Cast<JSReceiver>(handler))
               .ToHandle(&handler_context)) {
        if (!IsJSReceiver(*secondary) ||
            !JSReceiver::GetContextForMicrotask(Cast<JSReceiver>(secondary))
                 .ToHandle(&handler_context)) {
          handler_context = isolate->native_context();
        }
      }

      DirectHandle<PromiseReactionJobTask> task;
      if (fulfilled) {
        task = Cast<PromiseFulfillReactionJobTask>(
            isolate->factory()->NewStruct(
                PROMISE_FULFILL_REACTION_JOB_TASK_TYPE));
      } else {
        task = Cast<PromiseRejectReactionJobTask>(
            isolate->factory()->NewStruct(
                PROMISE_REJECT_REACTION_JOB_TASK_TYPE));
      }
      task->set_argument(promise->reactions_or_result());
      task->set_context(*handler_context);
      task->set_handler(Cast<UnionOf<Undefined, JSCallable>>(*handler));
      task->set_promise_or_capability(*result_promise);
#ifdef V8_ENABLE_CONTINUATION_PRESERVED_EMBEDDER_DATA
      task->set_continuation_preserved_embedder_data(
          roots.undefined_value());
#endif
      MicrotaskQueue* queue = handler_context->microtask_queue();
      if (queue != nullptr) queue->EnqueueMicrotask(*task);
    }
    promise->set_has_handler(true);

    *out_result = (*result_promise).ptr();
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kArrayIsArray) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<Object> argument =
        actual_argc > 0
            ? direct_handle(
                  Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[0])),
                  isolate)
            : direct_handle(roots.undefined_value(), isolate);
    Maybe<bool> result = Object::IsArray(argument);
    if (result.IsNothing()) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    *out_result = isolate->heap()->ToBoolean(result.FromJust()).ptr();
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kStringPrototypeCharAt) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<Object> receiver_object(
        Tagged<Object>(SafeTaggedOrUndefined(isolate, receiver)), isolate);
    DirectHandle<String> receiver_string;
    if (!Object::ToString(isolate, receiver_object)
             .ToHandle(&receiver_string)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    double position = 0;
    if (actual_argc > 0) {
      DirectHandle<Object> position_object(
          Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[0])), isolate);
      DirectHandle<Number> position_number;
      if (!Object::ToInteger(isolate, position_object)
               .ToHandle(&position_number)) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
      position = Object::NumberValue(*position_number);
    }

    if (position < 0 || position >= receiver_string->length()) {
      *out_result = roots.empty_string().ptr();
    } else {
      int index = static_cast<int>(position);
      DirectHandle<String> result = isolate->factory()->NewSubString(
          receiver_string, index, index + 1);
      *out_result = (*result).ptr();
    }
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kStringPrototypeIsWellFormed ||
      builtin == Builtin::kStringPrototypeToWellFormed) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));
    HandleScope scope(isolate);
    DirectHandle<Object> receiver_object(
        Tagged<Object>(SafeTaggedOrUndefined(isolate, receiver)), isolate);
    DirectHandle<String> source;
    if (!Object::ToString(isolate, receiver_object).ToHandle(&source)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    source = String::Flatten(isolate, source);
    bool well_formed = String::IsWellFormedUnicode(isolate, source);
    if (builtin == Builtin::kStringPrototypeIsWellFormed) {
      *out_result = isolate->heap()->ToBoolean(well_formed).ptr();
    } else if (well_formed) {
      *out_result = (*source).ptr();
    } else {
      const int length = source->length();
      DirectHandle<SeqTwoByteString> result =
          isolate->factory()->NewRawTwoByteString(length).ToHandleChecked();
      DisallowGarbageCollection no_gc;
      String::FlatContent contents = source->GetFlatContent(no_gc);
      const uint16_t* source_data = contents.ToUC16Vector().begin();
      uint16_t* result_data = result->GetChars(no_gc);
      unibrow::Utf16::ReplaceUnpairedSurrogates(source_data, result_data,
                                                length);
      *out_result = (*result).ptr();
    }
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kStringPrototypeCharCodeAt) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<Object> receiver_object(
        Tagged<Object>(SafeTaggedOrUndefined(isolate, receiver)), isolate);
    DirectHandle<String> receiver_string;
    if (!Object::ToString(isolate, receiver_object)
             .ToHandle(&receiver_string)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    receiver_string = String::Flatten(isolate, receiver_string);

    int position = 0;
    if (actual_argc > 0) {
      DirectHandle<Object> position_object(
          Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[0])), isolate);
      DirectHandle<Number> position_number;
      if (!Object::ToInteger(isolate, position_object)
               .ToHandle(&position_number)) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
      double position_double = Object::NumberValue(*position_number);
      if (position_double > 0) {
        position = position_double > receiver_string->length()
                       ? receiver_string->length()
                       : static_cast<int>(position_double);
      }
    }

    if (position < 0 || position >= receiver_string->length()) {
      *out_result = roots.nan_value().ptr();
    } else {
      *out_result = Smi::FromInt(receiver_string->Get(position)).ptr();
    }
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kStringPrototypeIndexOf) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<Object> receiver_object(
        Tagged<Object>(SafeTaggedOrUndefined(isolate, receiver)), isolate);
    DirectHandle<String> input;
    if (!Object::ToString(isolate, receiver_object).ToHandle(&input)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<Object> search_object =
        actual_argc > 0
            ? direct_handle(
                  Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[0])),
                  isolate)
            : direct_handle(roots.undefined_value(), isolate);
    DirectHandle<String> search;
    if (!Object::ToString(isolate, search_object).ToHandle(&search)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    int start = 0;
    if (actual_argc > 1) {
      DirectHandle<Object> position_object(
          Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[1])), isolate);
      if (!IsUndefined(*position_object, roots)) {
        DirectHandle<Number> position_number;
        if (!Object::ToInteger(isolate, position_object)
                 .ToHandle(&position_number)) {
          isolate->set_context(saved_context);
          *out_result = roots.exception().ptr();
          return true;
        }
        double position = Object::NumberValue(*position_number);
        if (position > 0) {
          start = position >= input->length() ? input->length()
                                               : static_cast<int>(position);
        }
      }
    }

    int result = String::IndexOf(isolate, input, search, start);
    *out_result = Smi::FromInt(result).ptr();
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kStringPrototypeSlice) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<Object> receiver_object(
        Tagged<Object>(SafeTaggedOrUndefined(isolate, receiver)), isolate);
    DirectHandle<String> input;
    if (!Object::ToString(isolate, receiver_object).ToHandle(&input)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    input = String::Flatten(isolate, input);
    const int length = input->length();

    auto read_slice_index = [&](int arg_index, int default_value,
                                int* out_index) -> bool {
      if (actual_argc <= arg_index) {
        *out_index = default_value;
        return true;
      }
      DirectHandle<Object> arg(
          Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[arg_index])),
          isolate);
      if (IsUndefined(*arg, roots)) {
        *out_index = default_value;
        return true;
      }
      DirectHandle<Number> number;
      if (!Object::ToInteger(isolate, arg).ToHandle(&number)) return false;
      double relative = Object::NumberValue(*number);
      if (relative < 0) {
        double from_end = static_cast<double>(length) + relative;
        *out_index = from_end <= 0 ? 0 : static_cast<int>(from_end);
      } else if (relative >= length) {
        *out_index = length;
      } else {
        *out_index = static_cast<int>(relative);
      }
      return true;
    };

    int start = 0;
    int end = length;
    if (!read_slice_index(0, 0, &start) ||
        !read_slice_index(1, length, &end)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<String> result =
        end <= start ? isolate->factory()->empty_string()
                     : isolate->factory()->NewProperSubString(input, start, end);
    *out_result = (*result).ptr();
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kHandleApiCallOrConstruct ||
      builtin == Builtin::kCallApiCallbackGeneric ||
      builtin == Builtin::kCallApiCallbackOptimizedNoProfiling ||
      builtin == Builtin::kCallApiCallbackOptimized) {
    Tagged<SharedFunctionInfo> shared = Wasm32JSFunctionShared(*function);
#ifdef __wasi__
    static int api_fallback_probe_count = 0;
    if (kTraceWasmFallbackDetails && api_fallback_probe_count < 16) {
      Tagged<Object> data = shared->GetUntrustedData();
      PrintF("WasmJSEntry: API fallback probe builtin=%s is_api=%d ",
             Builtins::name(builtin), shared->IsApiFunction() ? 1 : 0);
      DumpRuntimeArg("function", 0, (*function).ptr());
      DumpRuntimeArg(" data", 0, data.ptr());
      PrintF(" is_fti=%d\n", IsFunctionTemplateInfo(data) ? 1 : 0);
      api_fallback_probe_count++;
    }
#endif
    if (!shared->IsApiFunction()) {
#ifdef __wasi__
      static int wasm_non_api_handle_api_trace_count = 0;
      if (kTraceWasmFallbackDetails &&
          wasm_non_api_handle_api_trace_count < 64) {
        ++wasm_non_api_handle_api_trace_count;
        PrintF("WasmJSEntry: API fallback rejected non-api #%d builtin=%s ",
               wasm_non_api_handle_api_trace_count, Builtins::name(builtin));
        DumpRuntimeArg("function", 0, (*function).ptr());
        PrintF(" sfi=0x%x name=", static_cast<unsigned>(shared.ptr()));
        DumpNameForTrace(shared->Name());
        PrintF(" kind=%d has_api_data=%d data=",
               static_cast<int>(shared->kind()),
               IsFunctionTemplateInfo(shared->GetUntrustedData()) ? 1 : 0);
        DumpRuntimeArg("data", 0, shared->GetUntrustedData().ptr());
        PrintF("\n");
      }
#endif
      return false;
    }

    Tagged<Context> saved_context = isolate->context();
    Tagged<Context> function_context = Wasm32JSFunctionContext(*function);
    Tagged<Context> api_context = function_context;
    bool using_caller_context = false;
    Address caller_context_address = CurrentInterpreterContext();
    if (IsReadableTaggedHeapObject(caller_context_address,
                                   HeapObject::kMapOffset +
                                       sizeof(Address)) &&
        HasReadableHeapObjectMap(caller_context_address) &&
        IsContext(Tagged<Object>(caller_context_address))) {
      api_context = Cast<Context>(Tagged<Object>(caller_context_address));
      using_caller_context = true;
    }
    Tagged<Context> api_native_context = api_context;
    if (!TryResolveWasm32NativeContext(api_context, &api_native_context) &&
        !TryResolveWasm32NativeContext(function_context, &api_native_context) &&
        !TryResolveWasm32NativeContext(saved_context, &api_native_context)) {
      *out_result = roots.exception().ptr();
      return true;
    }
    static int api_context_trace_count = 0;
    if (kTraceWasmFallbackDetails && api_context_trace_count < 16) {
      PrintF("WasmJSEntry: API fallback context source=%s raw=0x%x "
             "native=0x%x raw_type=%d native_type=%d function_context=0x%x\n",
             using_caller_context ? "caller" : "function",
             static_cast<unsigned>(api_context.ptr()),
             static_cast<unsigned>(api_native_context.ptr()),
             IsHeapObject(api_context) ? api_context->map()->instance_type()
                                       : -1,
             IsHeapObject(api_native_context)
                 ? api_native_context->map()->instance_type()
                 : -1,
             static_cast<unsigned>(function_context.ptr()));
      api_context_trace_count++;
    }
    v8::Locker locker(reinterpret_cast<v8::Isolate*>(isolate));
    WasmGCStateScope gc_state(isolate);
    SetCurrentIsolateScope current_isolate_scope(isolate);
    isolate->set_context(api_native_context);
    HandleScope scope(isolate);
    RepairWasm32NodeEnvironmentRealm(api_native_context);
    Handle<FunctionTemplateInfo> rooted_function_template(
        shared->api_func_data(), isolate);
    DirectHandle<FunctionTemplateInfo> function_template(
        rooted_function_template);
    Address new_target_address = SafeTaggedOrUndefined(isolate, new_target);
    bool is_construct =
        !IsUndefined(Tagged<Object>(new_target_address), roots);
    if (is_construct &&
        (!IsSafeTaggedHandleValue(new_target_address) ||
         !IsJSReceiver(Tagged<Object>(new_target_address)))) {
      isolate->set_context(saved_context);
      return false;
    }
    Handle<Object> rooted_api_receiver(
        Tagged<Object>(is_construct ? roots.the_hole_value().ptr()
                                    : SafeTaggedOrUndefined(isolate, receiver)),
        isolate);
    DirectHandle<Object> api_receiver(rooted_api_receiver);
    Handle<Object> rooted_api_args[kWasmMaxOutgoingArgSlots == 0
                                       ? 1
                                       : kWasmMaxOutgoingArgSlots];
    DirectHandle<Object> api_args[kWasmMaxOutgoingArgSlots == 0
                                      ? 1
                                      : kWasmMaxOutgoingArgSlots];
    int api_argc = actual_argc;
    if (api_argc > kWasmMaxOutgoingArgSlots) {
      api_argc = kWasmMaxOutgoingArgSlots;
    }
    for (int i = 0; i < api_argc; ++i) {
      rooted_api_args[i] =
          Handle<Object>(Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[i])),
                         isolate);
      api_args[i] = DirectHandle<Object>(rooted_api_args[i]);
    }
#ifdef __wasi__
    static int wasm_api_probe_always_count = 0;
    bool trace_api_probe_always =
        kTraceWasmFallbackDetails && wasm_api_probe_always_count < 64;
    if (trace_api_probe_always) {
      ++wasm_api_probe_always_count;
      PrintF("WasmJSEntry: API probe #%d builtin=%s argc=%d name=",
             wasm_api_probe_always_count, Builtins::name(builtin), api_argc);
      DumpNameForTrace(shared->Name());
      DumpRuntimeArg(" receiver", 0, (*api_receiver).ptr());
      for (int i = 0; i < api_argc && i < 4; ++i) {
        DumpRuntimeArg(" arg", i, (*api_args[i]).ptr());
      }
      PrintF(" context=0x%x native=0x%x\n",
             static_cast<unsigned>(api_context.ptr()),
             static_cast<unsigned>(api_native_context.ptr()));
    }
    static int api_fallback_args_trace_count = 0;
    bool trace_cjs_loader_api_args =
        kTraceWasmFallbackDetails &&
        (SharedDebugNameEqualsAsciiForTrace(shared,
                                            "compileFunctionForCJSLoader") ||
         (api_argc == 4 && IsString(*api_args[0]) &&
          IsString(*api_args[1]) && IsBoolean(*api_args[2])));
    bool trace_contextify_run_api_args =
        kTraceWasmFallbackDetails &&
        (SharedDebugNameEqualsAsciiForTrace(shared, "runInContext") ||
         SharedDebugNameEqualsAsciiForTrace(shared, "runInThisContext"));
    if (kTraceWasmFallbackDetails &&
        (trace_cjs_loader_api_args || trace_contextify_run_api_args ||
         api_fallback_args_trace_count < 128)) {
      PrintF("WasmJSEntry: API fallback args builtin=%s argc=%d name=",
             Builtins::name(builtin), api_argc);
      DumpNameForTrace(shared->Name());
      DumpRuntimeArg(" receiver", 0, (*api_receiver).ptr());
      for (int i = 0; i < api_argc && i < 8; ++i) {
        DumpRuntimeArg(" arg", i, (*api_args[i]).ptr());
      }
      PrintF("\n");
      if (!trace_cjs_loader_api_args && !trace_contextify_run_api_args) {
        api_fallback_args_trace_count++;
      }
    }
#endif
    Handle<HeapObject> rooted_new_target(
        Cast<HeapObject>(Tagged<Object>(
            is_construct ? new_target_address : roots.undefined_value().ptr())),
        isolate);
    DirectHandle<HeapObject> new_target(rooted_new_target);
#ifdef __wasi__
    bool trace_api_invoke =
        kTraceWasmFallbackDetails &&
        (trace_cjs_loader_api_args || trace_contextify_run_api_args ||
         api_fallback_args_trace_count < 128);
    if (trace_api_invoke) {
      PrintF("WasmJSEntry: API fallback before InvokeApiFunction builtin=%s "
             "argc=%d name=",
             Builtins::name(builtin), api_argc);
      DumpNameForTrace(shared->Name());
      DumpRuntimeArg(" receiver", 0, (*api_receiver).ptr());
      PrintF("\n");
    }
#endif
    MaybeHandle<Object> maybe_result;
    Address api_result_address = roots.exception().ptr();
    bool api_result_valid = false;
    maybe_result = Builtins::InvokeApiFunction(
        isolate, is_construct, function_template, api_receiver,
        ZoneVector<const DirectHandle<Object>>(api_args, api_argc), new_target,
        &api_result_address);
    DirectHandle<Object> api_result;
    if (!isolate->has_exception() &&
        api_result_address != roots.exception().ptr()) {
      api_result_valid = true;
    } else if (maybe_result.ToHandle(&api_result)) {
      api_result_address = (*api_result).ptr();
      api_result_valid = true;
    }
#ifdef __wasi__
    if (trace_api_probe_always) {
      PrintF("WasmJSEntry: API probe result #%d empty=%d has_exception=%d ",
             wasm_api_probe_always_count, maybe_result.is_null() ? 1 : 0,
             isolate->has_exception() ? 1 : 0);
      if (api_result_valid) {
        DumpRuntimeArg("result", 0, api_result_address);
      }
      PrintF("\n");
    }
    if (trace_api_invoke) {
      PrintF("WasmJSEntry: API fallback after InvokeApiFunction empty=%d "
             "has_exception=%d name=",
             maybe_result.is_null() ? 1 : 0, isolate->has_exception() ? 1 : 0);
      DumpNameForTrace(shared->Name());
      PrintF("\n");
    }
#endif
    if (!api_result_valid) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
#ifdef __wasi__
    if (trace_api_invoke) {
      PrintF("WasmJSEntry: API fallback result ");
      DumpRuntimeArg("result", 0, api_result_address);
      PrintF("\n");
    }
#endif
    *out_result = api_result_address;
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kObjectPrototypeValueOf) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<Object> receiver_object(
        Tagged<Object>(SafeTaggedOrUndefined(isolate, receiver)), isolate);
    DirectHandle<JSReceiver> object;
    if (!Object::ToObject(isolate, receiver_object,
                          "Object.prototype.valueOf")
             .ToHandle(&object)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    *out_result = (*object).ptr();
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kObjectPrototypeToString) {
    Address receiver_address = SafeTaggedOrUndefined(isolate, receiver);
    Tagged<Object> receiver_object(receiver_address);
    const char* tag = "[object Object]";
    if (IsUndefined(receiver_object, roots)) {
      tag = "[object Undefined]";
    } else if (IsNull(receiver_object, roots)) {
      tag = "[object Null]";
    } else if (IsJSArray(receiver_object)) {
      tag = "[object Array]";
    } else if (IsJSRegExp(receiver_object)) {
      tag = "[object RegExp]";
    } else if (IsCallable(receiver_object)) {
      tag = "[object Function]";
    } else if (IsString(receiver_object)) {
      tag = "[object String]";
    } else if (IsNumber(receiver_object)) {
      tag = "[object Number]";
    } else if (IsBoolean(receiver_object)) {
      tag = "[object Boolean]";
    } else if (IsBigInt(receiver_object)) {
      tag = "[object BigInt]";
    } else if (IsSymbol(receiver_object)) {
      tag = "[object Symbol]";
    }
    HandleScope scope(isolate);
    DirectHandle<String> result =
        isolate->factory()->NewStringFromAsciiChecked(tag);
    *out_result = (*result).ptr();
    return true;
  }

  if (builtin == Builtin::kSymbolConstructor) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<Symbol> result = isolate->factory()->NewSymbol();
    Address description_address =
        actual_argc > 0 ? SafeTaggedOrUndefined(isolate, argv[0])
                        : roots.undefined_value().ptr();
    if (!IsUndefined(Tagged<Object>(description_address), isolate)) {
      Handle<Object> description =
          handle(Tagged<Object>(description_address), isolate);
      Handle<String> description_string;
      if (!Object::ToString(isolate, description)
               .ToHandle(&description_string)) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
      result->set_description(*description_string);
    }
    *out_result = (*result).ptr();
    isolate->set_context(saved_context);
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmJSEntry: fallback SymbolConstructor result=0x%x\n",
             static_cast<unsigned>(*out_result));
    }
    return true;
  }

  if (builtin == Builtin::kSymbolPrototypeDescriptionGetter) {
    Address receiver_address = SafeTaggedOrUndefined(isolate, receiver);
    if (IsSafeTaggedHandleValue(receiver_address)) {
      Tagged<Object> receiver_object(receiver_address);
      if (IsJSPrimitiveWrapper(receiver_object)) {
        receiver_object = Cast<JSPrimitiveWrapper>(receiver_object)->value();
      }
      if (IsSymbol(receiver_object)) {
        *out_result = Cast<Symbol>(receiver_object)->description().ptr();
        if (kTraceWasmFallbackDetails) {
          PrintF("WasmJSEntry: fallback SymbolPrototypeDescriptionGetter "
                 "receiver=0x%x result=0x%x\n",
                 static_cast<unsigned>(receiver_address),
                 static_cast<unsigned>(*out_result));
        }
        return true;
      }
    }
    return false;
  }

  if (builtin == Builtin::kSymbolFor) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    Handle<Object> key_obj =
        actual_argc > 0
            ? handle(Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[0])),
                     isolate)
            : handle(roots.undefined_value(), isolate);
    Handle<String> key;
    if (!Object::ToString(isolate, key_obj).ToHandle(&key)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    *out_result =
        (*isolate->SymbolFor(RootIndex::kPublicSymbolTable, key, false)).ptr();
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kFunctionPrototypeToString) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<Object> receiver_object(
        Tagged<Object>(SafeTaggedOrUndefined(isolate, receiver)), isolate);
    DirectHandle<String> result;
    if (IsJSBoundFunction(*receiver_object)) {
      result = JSBoundFunction::ToString(
          isolate, Cast<JSBoundFunction>(receiver_object));
    } else if (IsJSFunction(*receiver_object)) {
      result =
          JSFunction::ToString(isolate, Cast<JSFunction>(receiver_object));
    } else if (IsJSReceiver(*receiver_object) &&
               Cast<JSReceiver>(*receiver_object)->map()->is_callable()) {
      result = direct_handle(roots.function_native_code_string(), isolate);
    } else {
      isolate->Throw(*isolate->factory()->NewTypeError(
          MessageTemplate::kNotGeneric,
          isolate->factory()->NewStringFromAsciiChecked(
              "Function.prototype.toString"),
          isolate->factory()->Function_string()));
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    *out_result = (*result).ptr();
    isolate->set_context(saved_context);
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmJSEntry: fallback FunctionPrototypeToString result=0x%x\n",
             static_cast<unsigned>(*out_result));
    }
    return true;
  }

  if (builtin == Builtin::kFastFunctionPrototypeBind ||
      builtin == Builtin::kFunctionPrototypeBind) {
    Address target_address = SafeTaggedOrUndefined(isolate, receiver);
    if (!IsSafeTaggedHandleValue(target_address) ||
        !IsCallable(Tagged<Object>(target_address)) ||
        !IsJSReceiver(Tagged<Object>(target_address))) {
      return false;
    }

    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<JSReceiver> target =
        direct_handle(Cast<JSReceiver>(Tagged<Object>(target_address)),
                      isolate);
    DirectHandle<JSAny> this_arg =
        actual_argc > 0
            ? direct_handle(
                  Cast<JSAny>(Tagged<Object>(
                      SafeTaggedOrUndefined(isolate, argv[0]))),
                  isolate)
            : direct_handle(Cast<JSAny>(roots.undefined_value()), isolate);

    int bound_argc = actual_argc > 1 ? actual_argc - 1 : 0;
    if (bound_argc > kWasmMaxOutgoingArgSlots) {
      bound_argc = kWasmMaxOutgoingArgSlots;
    }
    DirectHandleVector<Object> bind_roots(isolate);
    bind_roots.reserve(4 + bound_argc);
    bind_roots.push_back(target);
    bind_roots.push_back(this_arg);
    for (int i = 0; i < bound_argc; ++i) {
      bind_roots.push_back(direct_handle(
          Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[i + 1])),
          isolate));
    }

    DirectHandle<JSPrototype> prototype;
    if (!JSReceiver::GetPrototype(isolate, Cast<JSReceiver>(bind_roots[0]))
             .ToHandle(&prototype)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    bind_roots.push_back(prototype);
    const int prototype_index = 2 + bound_argc;

    DirectHandle<JSBoundFunction> bound_function;
    if (!isolate->factory()
             ->NewJSBoundFunction(
                 Cast<JSReceiver>(bind_roots[0]),
                 Cast<JSAny>(bind_roots[1]),
                 base::Vector<DirectHandle<Object>>(bind_roots.data() + 2,
                                                    bound_argc),
                 Cast<JSPrototype>(bind_roots[prototype_index]))
             .ToHandle(&bound_function)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    bind_roots.push_back(bound_function);
    const int bound_function_index = prototype_index + 1;

    Maybe<bool> copy_result =
        JSFunctionOrBoundFunctionOrWrappedFunction::CopyNameAndLength(
            isolate, Cast<JSBoundFunction>(bind_roots[bound_function_index]),
            Cast<JSReceiver>(bind_roots[0]),
            isolate->factory()->bound__string(), bound_argc);
    if (copy_result.IsNothing()) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    *out_result = (*bind_roots[bound_function_index]).ptr();
    isolate->set_context(saved_context);
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmJSEntry: fallback %s target=0x%x argc=%d bound_argc=%d "
             "result=0x%x\n",
             Builtins::name(builtin), static_cast<unsigned>(target_address),
             actual_argc, bound_argc, static_cast<unsigned>(*out_result));
    }
    return true;
  }

  if (builtin == Builtin::kArrayPrototypeJoin ||
      builtin == Builtin::kArrayPrototypeToString) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<Object> receiver_object(
        Tagged<Object>(SafeTaggedOrUndefined(isolate, receiver)), isolate);
    DirectHandle<JSReceiver> object;
    if (!Object::ToObject(isolate, receiver_object, "Array.prototype.join")
             .ToHandle(&object)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<Object> length_object;
    if (!Object::GetLengthFromArrayLike(isolate, object)
             .ToHandle(&length_object)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    const double raw_length = Object::NumberValue(*length_object);
    if (raw_length < 0 || raw_length > FixedArray::kMaxLength) {
      isolate->Throw(*isolate->factory()->NewRangeError(
          MessageTemplate::kInvalidArrayLength, length_object));
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    const uint32_t length = static_cast<uint32_t>(raw_length);

    DirectHandle<String> separator = isolate->factory()->comma_string();
    if (builtin == Builtin::kArrayPrototypeJoin && actual_argc > 0) {
      DirectHandle<Object> separator_object(
          Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[0])), isolate);
      if (!IsUndefined(*separator_object, roots)) {
        if (!Object::ToString(isolate, separator_object)
                 .ToHandle(&separator)) {
          isolate->set_context(saved_context);
          *out_result = roots.exception().ptr();
          return true;
        }
      }
    }

    if (length == 0) {
      *out_result = roots.empty_string().ptr();
      isolate->set_context(saved_context);
      return true;
    }

    IncrementalStringBuilder builder(isolate);
    for (uint32_t index = 0; index < length; ++index) {
      if (index != 0 && separator->length() != 0) {
        builder.AppendString(separator);
      }

      DirectHandle<Object> element;
      if (!JSReceiver::GetElement(isolate, object, index).ToHandle(&element)) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
      if (IsUndefined(*element, roots) || IsNull(*element, roots)) {
        continue;
      }

      DirectHandle<String> element_string;
      if (!Object::ToString(isolate, element).ToHandle(&element_string)) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
      builder.AppendString(element_string);
    }

    DirectHandle<String> result;
    if (!builder.Finish().ToHandle(&result)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    *out_result = (*result).ptr();
    isolate->set_context(saved_context);
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmJSEntry: fallback ArrayPrototypeJoin length=%u result=0x%x\n",
             length, static_cast<unsigned>(*out_result));
    }
    return true;
  }

  if (builtin == Builtin::kArrayFrom) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    Address items_address = actual_argc > 0
                                ? SafeTaggedOrUndefined(isolate, argv[0])
                                : roots.undefined_value().ptr();
    Address mapfn_address = actual_argc > 1
                                ? SafeTaggedOrUndefined(isolate, argv[1])
                                : roots.undefined_value().ptr();
    Address this_arg_address = actual_argc > 2
                                   ? SafeTaggedOrUndefined(isolate, argv[2])
                                   : roots.undefined_value().ptr();

    bool mapping = !IsUndefined(Tagged<Object>(mapfn_address), isolate);
    if (mapping && (!IsSafeTaggedHandleValue(mapfn_address) ||
                    !IsCallable(Tagged<Object>(mapfn_address)))) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<Object> mapfn =
        direct_handle(Tagged<Object>(mapfn_address), isolate);
    DirectHandle<Object> this_arg =
        direct_handle(Tagged<Object>(this_arg_address), isolate);
    DirectHandle<FixedArray> elements;
    uint32_t result_length = 0;

    auto store_value = [&](DirectHandle<Object> value, uint32_t index) -> bool {
      DirectHandle<Object> mapped = value;
      if (mapping) {
        DirectHandle<Object> callback_args[2];
        callback_args[0] = value;
        callback_args[1] = isolate->factory()->NewNumberFromUint(index);

        WasmInterpreterStateSnapshot state(isolate);
        Address callback_result = roots.exception().ptr();
        bool direct_call = TryCallJSFunctionDirect(
            isolate, mapfn, this_arg, 2, callback_args, &callback_result);
        MaybeHandle<Object> maybe_result;
        if (direct_call) {
          if (IsException(Tagged<Object>(callback_result), isolate)) {
            state.Restore();
            *out_result = callback_result;
            return false;
          }
          mapped = direct_handle(Tagged<Object>(callback_result), isolate);
        } else {
          maybe_result = Execution::Call(
              isolate, mapfn, this_arg,
              ZoneVector<const DirectHandle<Object>>(callback_args, 2));
          if (!maybe_result.ToHandle(&mapped)) {
            state.Restore();
            *out_result = roots.exception().ptr();
            return false;
          }
        }
        state.Restore();
      }
      elements->set(index, *mapped);
      return true;
    };

    Tagged<Object> items_object(items_address);
    if (IsSafeTaggedHandleValue(items_address) &&
        IsJSArrayIterator(items_object)) {
      DirectHandle<JSArrayIterator> iterator =
          direct_handle(Cast<JSArrayIterator>(items_object), isolate);
      DirectHandle<JSReceiver> iterated_object =
          direct_handle(iterator->iterated_object(), isolate);
      Tagged<Number> next_index = iterator->next_index();

      uint32_t index = 0;
      uint32_t length = 0;
      if (!Object::ToArrayLength(next_index, &index)) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
      if (IsJSArray(*iterated_object)) {
        if (!Object::ToArrayLength(Cast<JSArray>(*iterated_object)->length(),
                                   &length)) {
          isolate->set_context(saved_context);
          *out_result = roots.exception().ptr();
          return true;
        }
      } else {
        DirectHandle<Object> length_object;
        if (!Object::GetLengthFromArrayLike(isolate, iterated_object)
                 .ToHandle(&length_object) ||
            !Object::ToArrayLength(*length_object, &length)) {
          isolate->set_context(saved_context);
          *out_result = roots.exception().ptr();
          return true;
        }
      }
      if (length < index ||
          length - index > static_cast<uint32_t>(FixedArray::kMaxLength)) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
      result_length = length - index;
      elements = isolate->factory()->NewFixedArray(
          static_cast<int>(result_length));
      for (uint32_t current = index; current < length; ++current) {
        HandleScope iteration_scope(isolate);
        DirectHandle<Object> value;
        switch (iterator->kind()) {
          case IterationKind::kKeys:
            value = isolate->factory()->NewNumberFromUint(current);
            break;
          case IterationKind::kValues:
            if (!JSReceiver::GetElement(isolate, iterated_object, current)
                     .ToHandle(&value)) {
              isolate->set_context(saved_context);
              *out_result = roots.exception().ptr();
              return true;
            }
            break;
          case IterationKind::kEntries: {
            DirectHandle<Object> element;
            if (!JSReceiver::GetElement(isolate, iterated_object, current)
                     .ToHandle(&element)) {
              isolate->set_context(saved_context);
              *out_result = roots.exception().ptr();
              return true;
            }
            DirectHandle<FixedArray> entry = isolate->factory()->NewFixedArray(2);
            entry->set(0, *isolate->factory()->NewNumberFromUint(current));
            entry->set(1, *element);
            value = isolate->factory()->NewJSArrayWithElements(entry);
            break;
          }
        }
        if (!store_value(value, current - index)) {
          isolate->set_context(saved_context);
          return true;
        }
      }
      iterator->set_next_index(*isolate->factory()->NewNumberFromUint(length));
    } else {
      DirectHandle<Object> items =
          direct_handle(Tagged<Object>(items_address), isolate);
      DirectHandle<JSReceiver> object;
      if (!Object::ToObject(isolate, items, "Array.from").ToHandle(&object)) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
      DirectHandle<Object> length_object;
      if (!Object::GetLengthFromArrayLike(isolate, object)
               .ToHandle(&length_object)) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
      double raw_length = Object::NumberValue(*length_object);
      if (raw_length < 0 ||
          raw_length > static_cast<double>(FixedArray::kMaxLength)) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
      uint32_t length = static_cast<uint32_t>(raw_length);
      result_length = length;
      elements = isolate->factory()->NewFixedArray(static_cast<int>(length));
      for (uint32_t index = 0; index < length; ++index) {
        HandleScope iteration_scope(isolate);
        DirectHandle<Object> value;
        if (!JSReceiver::GetElement(isolate, object, index).ToHandle(&value)) {
          isolate->set_context(saved_context);
          *out_result = roots.exception().ptr();
          return true;
        }
        if (!store_value(value, index)) {
          isolate->set_context(saved_context);
          return true;
        }
      }
    }

    DirectHandle<JSArray> result = isolate->factory()->NewJSArrayWithElements(
        elements, PACKED_ELEMENTS, static_cast<int>(result_length));
    *out_result = (*result).ptr();
    isolate->set_context(saved_context);
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmJSEntry: fallback ArrayFrom length=%d result=0x%x\n",
             static_cast<int>(result_length),
             static_cast<unsigned>(*out_result));
    }
    return true;
  }

  if (builtin == Builtin::kTypedArrayConstructor) {
    ElementsKind elements_kind;
    if (!TryGetWasm32TypedArrayElementsKind(*function, &elements_kind)) {
      return false;
    }

    Address length_address =
        actual_argc > 0 ? SafeTaggedOrUndefined(isolate, argv[0])
                        : Smi::zero().ptr();
    if (!IsSafeTaggedHandleValue(length_address)) return false;

    size_t length = 0;
    Tagged<Object> length_object(length_address);
    if (IsUndefined(length_object, isolate)) {
      length = 0;
    } else if (!Object::ToIntegerIndex(length_object, &length)) {
      return false;
    }

    ExternalArrayType array_type;
    size_t element_size;
    Factory::TypeAndSizeForElementsKind(elements_kind, &array_type,
                                        &element_size);
    if (length > JSTypedArray::kMaxByteLength / element_size) {
      *out_result = roots.exception().ptr();
      return true;
    }
    size_t byte_length = length * element_size;

    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<JSArrayBuffer> buffer;
    if (!isolate->factory()
             ->NewJSArrayBufferAndBackingStore(
                 byte_length, InitializedFlag::kZeroInitialized)
             .ToHandle(&buffer)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    DirectHandle<JSTypedArray> typed_array =
        isolate->factory()->NewJSTypedArray(array_type, buffer, 0, length);
    *out_result = (*typed_array).ptr();
    isolate->set_context(saved_context);
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmJSEntry: fallback TypedArrayConstructor kind=%d length=%zu "
             "result=0x%x\n",
             static_cast<int>(elements_kind), length,
             static_cast<unsigned>(*out_result));
    }
    return true;
  }

  if (builtin == Builtin::kTypedArrayPrototypeBuffer) {
    Address receiver_address = SafeTaggedOrUndefined(isolate, receiver);
    if (!IsSafeTaggedHandleValue(receiver_address) ||
        !IsJSTypedArray(Tagged<Object>(receiver_address))) {
      return false;
    }
    DirectHandle<JSTypedArray> typed_array =
        direct_handle(Cast<JSTypedArray>(Tagged<Object>(receiver_address)),
                      isolate);
    *out_result = (*typed_array->GetBuffer()).ptr();
    return true;
  }

  if (builtin == Builtin::kTypedArrayPrototypeLength ||
      builtin == Builtin::kTypedArrayPrototypeByteLength ||
      builtin == Builtin::kTypedArrayPrototypeByteOffset) {
    Address receiver_address = SafeTaggedOrUndefined(isolate, receiver);
    if (!IsSafeTaggedHandleValue(receiver_address) ||
        !IsJSTypedArray(Tagged<Object>(receiver_address))) {
      return false;
    }

    Tagged<JSTypedArray> typed_array =
        Cast<JSTypedArray>(Tagged<Object>(receiver_address));
    size_t value = 0;
    if (!typed_array->IsDetachedOrOutOfBounds()) {
      if (builtin == Builtin::kTypedArrayPrototypeLength) {
        value = typed_array->GetLength();
      } else if (builtin == Builtin::kTypedArrayPrototypeByteLength) {
        value = typed_array->GetByteLength();
      } else {
        value = typed_array->byte_offset();
      }
    }

    if (value <= static_cast<size_t>(Smi::kMaxValue)) {
      *out_result = Smi::FromInt(static_cast<int>(value)).ptr();
    } else {
      HandleScope scope(isolate);
      *out_result = (*isolate->factory()->NewNumberFromSize(value)).ptr();
    }
    return true;
  }

  if (builtin == Builtin::kArrayIncludes ||
      builtin == Builtin::kArrayIndexOf) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    Address runtime_args[3] = {
        actual_argc > 1 ? SafeTaggedOrUndefined(isolate, argv[1])
                        : roots.undefined_value().ptr(),
        actual_argc > 0 ? SafeTaggedOrUndefined(isolate, argv[0])
                        : roots.undefined_value().ptr(),
        SafeTaggedOrUndefined(isolate, receiver),
    };
    Runtime::FunctionId runtime_id =
        builtin == Builtin::kArrayIncludes ? Runtime::kArrayIncludes_Slow
                                           : Runtime::kArrayIndexOf;
    const Runtime::Function* runtime = Runtime::FunctionForId(runtime_id);
    using RuntimeEntry = Address (*)(int, Address*, Isolate*);
    Address result = reinterpret_cast<RuntimeEntry>(runtime->entry)(
        3, &runtime_args[2], isolate);
    if (isolate->has_exception() || result == roots.exception().ptr()) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    *out_result = result;
    isolate->set_context(saved_context);
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmJSEntry: fallback %s result=0x%x\n",
             Builtins::name(builtin),
             static_cast<unsigned>(*out_result));
    }
    return true;
  }

  if (builtin == Builtin::kArrayPrototypePush ||
      builtin == Builtin::kArrayPush) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<Object> receiver_object(
        Tagged<Object>(SafeTaggedOrUndefined(isolate, receiver)), isolate);
    DirectHandle<JSReceiver> object;
    if (!Object::ToObject(isolate, receiver_object, "Array.prototype.push")
             .ToHandle(&object)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<Object> raw_length_number;
    if (!Object::GetLengthFromArrayLike(isolate, object)
             .ToHandle(&raw_length_number)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    double length = Object::NumberValue(*raw_length_number);
    if (actual_argc > kMaxSafeInteger - length) {
      isolate->Throw(*isolate->factory()->NewTypeError(
          MessageTemplate::kPushPastSafeLength,
          isolate->factory()->NewNumberFromInt(actual_argc),
          raw_length_number));
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<JSAny> object_any = Cast<JSAny>(object);
    for (int i = 0; i < actual_argc; ++i) {
      DirectHandle<Object> element(
          Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[i])), isolate);
      if (length <= JSObject::kMaxElementIndex) {
        DirectHandle<Object> ignored;
        if (!Object::SetElement(isolate, object_any,
                                static_cast<uint32_t>(length), element,
                                ShouldThrow::kThrowOnError)
                 .ToHandle(&ignored)) {
          isolate->set_context(saved_context);
          *out_result = roots.exception().ptr();
          return true;
        }
      } else {
        PropertyKey key(isolate, length);
        LookupIterator it(isolate, object, key);
        Maybe<bool> maybe_set = Object::SetProperty(
            &it, element, StoreOrigin::kMaybeKeyed,
            Just(ShouldThrow::kThrowOnError));
        if (maybe_set.IsNothing()) {
          isolate->set_context(saved_context);
          *out_result = roots.exception().ptr();
          return true;
        }
      }
      ++length;
    }

    DirectHandle<Object> final_length =
        isolate->factory()->NewNumber(length);
    DirectHandle<Object> ignored;
    if (!Object::SetProperty(isolate, object_any,
                             isolate->factory()->length_string(),
                             final_length, StoreOrigin::kMaybeKeyed,
                             Just(ShouldThrow::kThrowOnError))
             .ToHandle(&ignored)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    *out_result = (*final_length).ptr();
    isolate->set_context(saved_context);
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmJSEntry: fallback %s argc=%d result=0x%x\n",
             Builtins::name(builtin), actual_argc,
             static_cast<unsigned>(*out_result));
    }
    return true;
  }

  if (builtin == Builtin::kArrayPrototypeValues ||
      builtin == Builtin::kArrayPrototypeKeys ||
      builtin == Builtin::kArrayPrototypeEntries) {
    IterationKind kind = IterationKind::kValues;
    const char* method_name = "Array.prototype.values";
    if (builtin == Builtin::kArrayPrototypeKeys) {
      kind = IterationKind::kKeys;
      method_name = "Array.prototype.keys";
    } else if (builtin == Builtin::kArrayPrototypeEntries) {
      kind = IterationKind::kEntries;
      method_name = "Array.prototype.entries";
    }

    Address receiver_address = SafeTaggedOrUndefined(isolate, receiver);
    if (!IsSafeTaggedHandleValue(receiver_address)) {
      PrintF("WasmJSEntry: Array iterator receiver invalid ");
      DumpRuntimeArg("receiver", 0, receiver_address);
      PrintF("\n");
      *out_result = roots.exception().ptr();
      return true;
    }

    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<Object> receiver_object =
        direct_handle(Tagged<Object>(receiver_address), isolate);
    DirectHandle<JSReceiver> iterated_object;
    if (!Object::ToObject(isolate, receiver_object, method_name)
             .ToHandle(&iterated_object)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<Map> iterator_map;
    if (!TryFindNativeContextMapByInstanceType(
            isolate, JS_ARRAY_ITERATOR_TYPE, &iterator_map)) {
      isolate->set_context(saved_context);
      PrintF("WasmJSEntry: missing array iterator map\n");
      *out_result = roots.exception().ptr();
      return true;
    }
    DirectHandle<JSArrayIterator> iterator =
        Cast<JSArrayIterator>(isolate->factory()->NewJSObjectFromMap(
            iterator_map, AllocationType::kYoung));
    iterator->set_iterated_object(*iterated_object);
    iterator->set_next_index(Smi::zero(), SKIP_WRITE_BARRIER);
    iterator->set_kind(kind);
    *out_result = (*iterator).ptr();
    isolate->set_context(saved_context);
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmJSEntry: fallback %s result=0x%x\n", method_name,
             static_cast<unsigned>(*out_result));
    }
    return true;
  }

  if (builtin == Builtin::kObjectSetPrototypeOf) {
    Address object_address = actual_argc > 0
                                 ? SafeTaggedOrUndefined(isolate, argv[0])
                                 : roots.undefined_value().ptr();
    Address proto_address = actual_argc > 1
                                ? SafeTaggedOrUndefined(isolate, argv[1])
                                : roots.undefined_value().ptr();

    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));
    HandleScope scope(isolate);

    DirectHandle<Object> object =
        direct_handle(Tagged<Object>(object_address), isolate);
    if (IsNullOrUndefined(*object, isolate)) {
      isolate->Throw(*isolate->factory()->NewTypeError(
          MessageTemplate::kCalledOnNullOrUndefined,
          isolate->factory()->NewStringFromAsciiChecked(
              "Object.setPrototypeOf")));
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<Object> proto =
        direct_handle(Tagged<Object>(proto_address), isolate);
    if (!IsNull(*proto, isolate) && !IsJSReceiver(*proto)) {
      isolate->Throw(*isolate->factory()->NewTypeError(
          MessageTemplate::kProtoObjectOrNull, proto));
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    if (!IsJSReceiver(*object)) {
      *out_result = (*object).ptr();
      isolate->set_context(saved_context);
      if (kTraceWasmFallbackDetails) {
        PrintF("WasmJSEntry: fallback ObjectSetPrototypeOf primitive "
               "result=0x%x\n",
               static_cast<unsigned>(*out_result));
      }
      return true;
    }

    DirectHandle<JSReceiver> receiver = Cast<JSReceiver>(object);
    Maybe<bool> result = JSReceiver::SetPrototype(
        isolate, receiver, proto, true, kThrowOnError);
    if (result.IsNothing()) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    *out_result = (*object).ptr();
    isolate->set_context(saved_context);
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmJSEntry: fallback ObjectSetPrototypeOf result=0x%x\n",
             static_cast<unsigned>(*out_result));
    }
    return true;
  }

  if (builtin == Builtin::kObjectAssign) {
    Address target_address = actual_argc > 0
                                 ? SafeTaggedOrUndefined(isolate, argv[0])
                                 : roots.undefined_value().ptr();

    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));
    HandleScope scope(isolate);
    DirectHandle<Object> target_object =
        direct_handle(Tagged<Object>(target_address), isolate);
    DirectHandle<JSReceiver> target;
    if (!Object::ToObject(isolate, target_object).ToHandle(&target)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    for (int i = 1; i < actual_argc; ++i) {
      Address source_address = SafeTaggedOrUndefined(isolate, argv[i]);
      Tagged<Object> source_object(source_address);
      if (IsUndefined(source_object, isolate) ||
          IsNull(source_object, isolate)) {
        continue;
      }
      DirectHandle<Object> source = direct_handle(source_object, isolate);
      Maybe<bool> copied = JSReceiver::SetOrCopyDataProperties(
          isolate, target, source, PropertiesEnumerationMode::kEnumerationOrder);
      if (copied.IsNothing()) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
    }

    *out_result = (*target).ptr();
    isolate->set_context(saved_context);
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmJSEntry: fallback ObjectAssign result=0x%x sources=%d\n",
             static_cast<unsigned>(*out_result),
             actual_argc > 0 ? actual_argc - 1 : 0);
    }
    return true;
  }

  if (builtin == Builtin::kObjectDefineProperty ||
      builtin == Builtin::kObjectDefineProperties ||
      builtin == Builtin::kObjectFreeze ||
      builtin == Builtin::kObjectSeal ||
      builtin == Builtin::kObjectGetOwnPropertyDescriptors) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));
    HandleScope scope(isolate);

    DirectHandle<Object> target =
        actual_argc > 0
            ? direct_handle(
                  Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[0])),
                  isolate)
            : direct_handle(roots.undefined_value(), isolate);

    if (builtin == Builtin::kObjectDefineProperty) {
      DirectHandle<Object> key =
          actual_argc > 1
              ? direct_handle(
                    Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[1])),
                    isolate)
              : direct_handle(roots.undefined_value(), isolate);
      Handle<Object> attributes =
          actual_argc > 2
              ? handle(Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[2])),
                       isolate)
              : handle(roots.undefined_value(), isolate);
#ifdef __wasi__
      static int object_define_property_trace_count = 0;
      bool trace_object_define_property =
          kTraceWasmFallbackDetails &&
          (++object_define_property_trace_count <= 32 ||
           (object_define_property_trace_count % 256) == 0);
      if (trace_object_define_property) {
        PrintF("WasmJSEntry: ObjectDefineProperty fallback enter #%d argc=%d",
               object_define_property_trace_count, actual_argc);
        DumpRuntimeArg(" target", 0, (*target).ptr());
        DumpRuntimeArg(" key", 0, (*key).ptr());
        DumpRuntimeArg(" attrs", 0, (*attributes).ptr());
        PrintF("\n");
      }
#endif
      Tagged<Object> result =
          JSReceiver::DefineProperty(isolate, target, key, attributes);
      if (isolate->has_exception() || IsException(result, isolate)) {
#ifdef __wasi__
        if (trace_object_define_property) {
          PrintF("WasmJSEntry: ObjectDefineProperty fallback exception "
                 "has_exception=%d\n",
                 isolate->has_exception() ? 1 : 0);
        }
#endif
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
      *out_result = result.ptr();
#ifdef __wasi__
      if (trace_object_define_property) {
        PrintF("WasmJSEntry: ObjectDefineProperty fallback result ");
        DumpRuntimeArg("result", 0, *out_result);
        PrintF(" has_exception=%d\n", isolate->has_exception() ? 1 : 0);
      }
#endif
      isolate->set_context(saved_context);
      return true;
    }

    if (builtin == Builtin::kObjectDefineProperties) {
      DirectHandle<Object> properties =
          actual_argc > 1
              ? direct_handle(
                    Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[1])),
                    isolate)
              : direct_handle(roots.undefined_value(), isolate);
      DirectHandle<Object> result;
      if (!JSReceiver::DefineProperties(isolate, target, properties)
               .ToHandle(&result)) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
      *out_result = (*result).ptr();
      isolate->set_context(saved_context);
      return true;
    }

    if (builtin == Builtin::kObjectFreeze || builtin == Builtin::kObjectSeal) {
      if (IsJSReceiver(*target)) {
        auto level = builtin == Builtin::kObjectFreeze ? FROZEN : SEALED;
        Maybe<bool> result = JSReceiver::SetIntegrityLevel(
            isolate, Cast<JSReceiver>(target), level, kThrowOnError);
        if (result.IsNothing()) {
          isolate->set_context(saved_context);
          *out_result = roots.exception().ptr();
          return true;
        }
      }
      *out_result = (*target).ptr();
      isolate->set_context(saved_context);
      return true;
    }

    DirectHandle<JSReceiver> receiver_object;
    if (!Object::ToObject(isolate, target).ToHandle(&receiver_object)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<FixedArray> keys;
    if (!KeyAccumulator::GetKeys(isolate, receiver_object,
                                 KeyCollectionMode::kOwnOnly, ALL_PROPERTIES,
                                 GetKeysConversion::kConvertToString)
             .ToHandle(&keys)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    DirectHandle<JSObject> descriptors =
        NewWasm32PlainJSObject(isolate);
    for (int i = 0; i < keys->length(); ++i) {
      DirectHandle<Name> key(Cast<Name>(keys->get(i)), isolate);
      PropertyDescriptor desc;
      Maybe<bool> found = JSReceiver::GetOwnPropertyDescriptor(
          isolate, receiver_object, key, &desc);
      if (found.IsNothing()) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
      if (!found.FromJust()) continue;
      DirectHandle<Object> descriptor = desc.ToObject(isolate);
      Maybe<bool> success = JSReceiver::CreateDataProperty(
          isolate, descriptors, key, descriptor, Just(kDontThrow));
      if (success.IsNothing()) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
      CHECK(success.FromJust());
    }
    *out_result = (*descriptors).ptr();
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kObjectEntries) {
    Address object_address = actual_argc > 0
                                 ? SafeTaggedOrUndefined(isolate, argv[0])
                                 : roots.undefined_value().ptr();

    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));
    HandleScope scope(isolate);
    DirectHandle<Object> object =
        direct_handle(Tagged<Object>(object_address), isolate);
    DirectHandle<JSReceiver> receiver;
    if (!Object::ToObject(isolate, object).ToHandle(&receiver)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<FixedArray> keys;
    if (!KeyAccumulator::GetKeys(isolate, receiver,
                                 KeyCollectionMode::kOwnOnly,
                                 ENUMERABLE_STRINGS,
                                 GetKeysConversion::kConvertToString)
             .ToHandle(&keys)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<FixedArray> entries =
        isolate->factory()->NewFixedArray(keys->length());
    for (int i = 0; i < keys->length(); ++i) {
      DirectHandle<Object> key(keys->get(i), isolate);
      DirectHandle<Object> value;
      if (!Runtime::GetObjectProperty(isolate, receiver, key)
               .ToHandle(&value)) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
      DirectHandle<FixedArray> pair = isolate->factory()->NewFixedArray(2);
      pair->set(0, *key);
      pair->set(1, *value);
      DirectHandle<JSArray> pair_array =
          isolate->factory()->NewJSArrayWithElements(pair);
      entries->set(i, *pair_array);
    }
    DirectHandle<JSArray> result =
        isolate->factory()->NewJSArrayWithElements(entries);
    *out_result = (*result).ptr();
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kObjectKeys ||
      builtin == Builtin::kObjectGetOwnPropertyNames) {
    Address object_address = actual_argc > 0
                                 ? SafeTaggedOrUndefined(isolate, argv[0])
                                 : roots.undefined_value().ptr();

    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));
    HandleScope scope(isolate);
    DirectHandle<Object> object =
        direct_handle(Tagged<Object>(object_address), isolate);
    DirectHandle<JSReceiver> receiver;
    if (!Object::ToObject(isolate, object).ToHandle(&receiver)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<FixedArray> keys;
    PropertyFilter filter =
        builtin == Builtin::kObjectKeys ? ENUMERABLE_STRINGS : SKIP_SYMBOLS;
    if (!KeyAccumulator::GetKeys(isolate, receiver,
                                 KeyCollectionMode::kOwnOnly, filter,
                                 GetKeysConversion::kConvertToString)
             .ToHandle(&keys)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    DirectHandle<JSArray> result =
        isolate->factory()->NewJSArrayWithElements(keys);
    *out_result = (*result).ptr();
    isolate->set_context(saved_context);
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmJSEntry: fallback %s result=0x%x\n",
             Builtins::name(builtin), static_cast<unsigned>(*out_result));
    }
    return true;
  }

  if (builtin == Builtin::kObjectGetOwnPropertyDescriptor) {
    Address object_address = actual_argc > 0
                                 ? SafeTaggedOrUndefined(isolate, argv[0])
                                 : roots.undefined_value().ptr();
    Address key_address = actual_argc > 1
                              ? SafeTaggedOrUndefined(isolate, argv[1])
                              : roots.undefined_value().ptr();

    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));
    HandleScope scope(isolate);
    DirectHandle<Object> object =
        direct_handle(Tagged<Object>(object_address), isolate);
    DirectHandle<JSReceiver> receiver;
    if (!Object::ToObject(isolate, object).ToHandle(&receiver)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<Object> key =
        direct_handle(Tagged<Object>(key_address), isolate);
    DirectHandle<Name> name;
    if (!Object::ToName(isolate, key).ToHandle(&name)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    PropertyDescriptor desc;
    Maybe<bool> found =
        JSReceiver::GetOwnPropertyDescriptor(isolate, receiver, name, &desc);
    if (found.IsNothing()) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    if (!found.FromJust()) {
      *out_result = roots.undefined_value().ptr();
      isolate->set_context(saved_context);
      if (kTraceWasmFallbackDetails) {
        PrintF("WasmJSEntry: fallback ObjectGetOwnPropertyDescriptor "
               "result=undefined\n");
      }
      return true;
    }

    DirectHandle<Object> result = desc.ToObject(isolate);
    *out_result = (*result).ptr();
    isolate->set_context(saved_context);
    if (kTraceWasmFallbackDetails) {
      PrintF(
          "WasmJSEntry: fallback ObjectGetOwnPropertyDescriptor result=0x%x\n",
          static_cast<unsigned>(*out_result));
    }
    return true;
  }

  if (builtin == Builtin::kObjectValues) {
    Address object_address = actual_argc > 0
                                 ? SafeTaggedOrUndefined(isolate, argv[0])
                                 : roots.undefined_value().ptr();

    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));
    HandleScope scope(isolate);
    DirectHandle<Object> object =
        direct_handle(Tagged<Object>(object_address), isolate);
    DirectHandle<JSReceiver> receiver;
    if (!Object::ToObject(isolate, object).ToHandle(&receiver)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<FixedArray> values;
    if (!JSReceiver::GetOwnValues(isolate, receiver,
                                  PropertyFilter::ENUMERABLE_STRINGS, false)
             .ToHandle(&values)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<JSArray> result =
        isolate->factory()->NewJSArrayWithElements(values);
    *out_result = (*result).ptr();
    isolate->set_context(saved_context);
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmJSEntry: fallback ObjectValues result=0x%x\n",
             static_cast<unsigned>(*out_result));
    }
    return true;
  }

  if (builtin == Builtin::kStringPrototypeIterator) {
    Address receiver_address = SafeTaggedOrUndefined(isolate, receiver);
    if (!IsSafeTaggedHandleValue(receiver_address)) {
      PrintF("WasmJSEntry: StringPrototypeIterator receiver invalid ");
      DumpRuntimeArg("receiver", 0, receiver_address);
      PrintF("\n");
      *out_result = roots.exception().ptr();
      return true;
    }

    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    Handle<Object> receiver_object =
        handle(Tagged<Object>(receiver_address), isolate);
    Handle<String> string;
    if (IsJSPrimitiveWrapper(*receiver_object) &&
        IsString(Cast<JSPrimitiveWrapper>(*receiver_object)->value())) {
      string = handle(
          Cast<String>(Cast<JSPrimitiveWrapper>(*receiver_object)->value()),
          isolate);
    } else if (IsString(*receiver_object)) {
      string = handle(Cast<String>(*receiver_object), isolate);
    } else if (!Object::ToString(isolate, receiver_object).ToHandle(&string)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<JSStringIterator> iterator =
        isolate->factory()->NewJSStringIterator(string);
    *out_result = (*iterator).ptr();
    isolate->set_context(saved_context);
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmJSEntry: fallback StringPrototypeIterator result=0x%x\n",
             static_cast<unsigned>(*out_result));
    }
    return true;
  }

  if (builtin == Builtin::kJsonParse) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    Handle<Object> source =
        actual_argc > 0
            ? handle(Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[0])),
                     isolate)
            : handle(roots.undefined_value(), isolate);
    Handle<Object> reviver =
        actual_argc > 1
            ? handle(Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[1])),
                     isolate)
            : handle(roots.undefined_value(), isolate);
    Handle<String> string;
    if (!Object::ToString(isolate, source).ToHandle(&string)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    string = String::Flatten(isolate, string);
    DirectHandle<Object> result;
    MaybeHandle<Object> maybe_result =
        String::IsOneByteRepresentationUnderneath(*string)
            ? JsonParser<uint8_t>::Parse(isolate, string, reviver)
            : JsonParser<uint16_t>::Parse(isolate, string, reviver);
    if (!maybe_result.ToHandle(&result)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    *out_result = (*result).ptr();
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kJsonStringify) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    Handle<Object> object_raw =
        actual_argc > 0
            ? handle(Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[0])),
                     isolate)
            : handle(roots.undefined_value(), isolate);
    Handle<Object> replacer_raw =
        actual_argc > 1
            ? handle(Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[1])),
                     isolate)
            : handle(roots.undefined_value(), isolate);
    Handle<Object> indent =
        actual_argc > 2
            ? handle(Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[2])),
                     isolate)
            : handle(roots.undefined_value(), isolate);

    Handle<JSAny> object = Cast<JSAny>(object_raw);
    Handle<JSAny> replacer = Cast<JSAny>(replacer_raw);
    DirectHandle<Object> result;
    if (!JsonStringify(isolate, object, replacer, indent).ToHandle(&result)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    *out_result = (*result).ptr();
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kFunctionPrototypeCall) {
    Address callable_address = SafeTaggedOrUndefined(isolate, receiver);
    Address this_arg_address =
        actual_argc > 0 ? SafeTaggedOrUndefined(isolate, argv[0])
                        : roots.undefined_value().ptr();

    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<Object> callable =
        direct_handle(Tagged<Object>(callable_address), isolate);
    if (!IsCallable(*callable)) {
      isolate->Throw(*isolate->factory()->NewTypeError(
          MessageTemplate::kCalledNonCallable, callable));
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<Object> this_arg =
        direct_handle(Tagged<Object>(this_arg_address), isolate);
    std::vector<DirectHandle<Object>> call_args;
    if (actual_argc > 1) {
      call_args.reserve(actual_argc - 1);
      for (int i = 1; i < actual_argc; ++i) {
        Address arg_address = SafeTaggedOrUndefined(isolate, argv[i]);
        call_args.push_back(direct_handle(Tagged<Object>(arg_address), isolate));
      }
    }

    DirectHandle<Object>* call_args_data =
        call_args.empty() ? nullptr : call_args.data();
    const int call_arg_count = static_cast<int>(call_args.size());
    Tagged<Context> function_call_context = isolate->context();
    if (IsJSFunction(*callable)) {
      isolate->set_context(
          Wasm32JSFunctionContext(Cast<JSFunction>(*callable)));
    }
    if (TryRunObjectPrototypeHasOwnPropertyBuiltin(
            isolate, callable, this_arg, call_arg_count, call_args_data,
            out_result) ||
        TryRunArraySliceBuiltin(isolate, callable, this_arg, call_arg_count,
                                call_args_data, out_result) ||
        TryRunArrayForEachBuiltin(isolate, callable, this_arg, call_arg_count,
                                  call_args_data, out_result) ||
        TryRunArrayFilterBuiltin(isolate, callable, this_arg, call_arg_count,
                                 call_args_data, out_result) ||
        TryRunArrayMapBuiltin(isolate, callable, this_arg, call_arg_count,
                              call_args_data, out_result) ||
        TryRunArrayReduceBuiltin(isolate, callable, this_arg, call_arg_count,
                                 call_args_data, out_result)) {
      isolate->set_context(saved_context);
      return true;
    }
    isolate->set_context(function_call_context);

    WasmInterpreterStateSnapshot state(isolate);
    Address direct_result = roots.exception().ptr();
    bool used_direct_call = TryCallJSFunctionDirect(
        isolate, callable, this_arg, call_arg_count, call_args_data,
        &direct_result);
    MaybeHandle<Object> maybe_result;
    if (!used_direct_call) {
      maybe_result = Execution::Call(
          isolate, callable, this_arg,
          ZoneVector<const DirectHandle<Object>>(
              call_args.empty() ? nullptr : call_args.data(),
              call_args.size()));
    }
    state.Restore();
    isolate->set_context(saved_context);
    if (used_direct_call) {
      *out_result = direct_result;
      return true;
    }
    DirectHandle<Object> result;
    if (!maybe_result.ToHandle(&result)) {
      *out_result = roots.exception().ptr();
      return true;
    }
    *out_result = (*result).ptr();
    return true;
  }

#ifdef V8_INTL_SUPPORT
  if (builtin == Builtin::kStringPrototypeToLowerCaseIntl) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    Handle<Object> receiver_object(
        Tagged<Object>(SafeTaggedOrUndefined(isolate, receiver)), isolate);
    Handle<String> string;
    if (!Object::ToString(isolate, receiver_object).ToHandle(&string)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    string = String::Flatten(isolate, string);
    DirectHandle<String> direct_string = direct_handle(*string, isolate);
    Handle<String> result;
    if (!Intl::ConvertToLower(isolate, direct_string).ToHandle(&result)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    *out_result = (*result).ptr();
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kStringPrototypeToUpperCaseIntl) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    Handle<Object> receiver_object(
        Tagged<Object>(SafeTaggedOrUndefined(isolate, receiver)), isolate);
    Handle<String> string;
    if (!Object::ToString(isolate, receiver_object).ToHandle(&string)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    string = String::Flatten(isolate, string);
    DirectHandle<String> direct_string = direct_handle(*string, isolate);
    DirectHandle<String> result;
    if (!Intl::ConvertToUpper(isolate, direct_string).ToHandle(&result)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    *out_result = (*result).ptr();
    isolate->set_context(saved_context);
    return true;
  }
#endif

  if (builtin == Builtin::kStringPrototypeStartsWith) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    Handle<Object> receiver_object(
        Tagged<Object>(SafeTaggedOrUndefined(isolate, receiver)), isolate);
    Handle<String> receiver_string;
    if (!Object::ToString(isolate, receiver_object)
             .ToHandle(&receiver_string)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    Address search_address =
        actual_argc > 0 ? SafeTaggedOrUndefined(isolate, argv[0])
                        : roots.undefined_value().ptr();
    Handle<Object> search_object(Tagged<Object>(search_address), isolate);
    Handle<String> search_string;
    if (!Object::ToString(isolate, search_object).ToHandle(&search_string)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    receiver_string = String::Flatten(isolate, receiver_string);
    search_string = String::Flatten(isolate, search_string);
    const int receiver_length = receiver_string->length();
    const int search_length = search_string->length();

    int position = 0;
    if (actual_argc > 1) {
      Address position_address = SafeTaggedOrUndefined(isolate, argv[1]);
      if (!IsUndefined(Tagged<Object>(position_address), roots)) {
        Handle<Object> position_object(Tagged<Object>(position_address),
                                       isolate);
        Handle<Number> position_number;
        if (!Object::ToInteger(isolate, position_object)
                 .ToHandle(&position_number)) {
          isolate->set_context(saved_context);
          *out_result = roots.exception().ptr();
          return true;
        }
        double position_double = Object::NumberValue(*position_number);
        if (!(position_double > 0)) {
          position = 0;
        } else if (position_double >= receiver_length) {
          position = receiver_length;
        } else {
          position = static_cast<int>(position_double);
        }
      }
    }

    bool starts_with = false;
    if (search_length <= receiver_length - position) {
      starts_with = true;
      for (int i = 0; i < search_length; ++i) {
        if (receiver_string->Get(position + i) != search_string->Get(i)) {
          starts_with = false;
          break;
        }
      }
    }
    *out_result = starts_with ? roots.true_value().ptr()
                              : roots.false_value().ptr();
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kObjectGetOwnPropertySymbols) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    Address object_address = actual_argc > 0
                                 ? SafeTaggedOrUndefined(isolate, argv[0])
                                 : roots.undefined_value().ptr();
    DirectHandle<Object> target =
        direct_handle(Tagged<Object>(object_address), isolate);
    DirectHandle<JSReceiver> object;
    if (!Object::ToObject(isolate, target).ToHandle(&object)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<FixedArray> keys;
    if (!KeyAccumulator::GetKeys(isolate, object,
                                 KeyCollectionMode::kOwnOnly, SKIP_STRINGS,
                                 GetKeysConversion::kConvertToString)
             .ToHandle(&keys)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<JSArray> result =
        isolate->factory()->NewJSArrayWithElements(keys);
    *out_result = result->ptr();
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kObjectPrototypePropertyIsEnumerable) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<Object> target = direct_handle(
        Tagged<Object>(SafeTaggedOrUndefined(isolate, receiver)), isolate);
    DirectHandle<JSReceiver> object;
    if (!Object::ToObject(isolate, target).ToHandle(&object)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<Object> key_value = direct_handle(
        Tagged<Object>(actual_argc > 0
                           ? SafeTaggedOrUndefined(isolate, argv[0])
                           : roots.undefined_value().ptr()),
        isolate);
    DirectHandle<Name> key;
    if (!Object::ToName(isolate, key_value).ToHandle(&key)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    Maybe<PropertyAttributes> maybe_attributes =
        JSReceiver::GetOwnPropertyAttributes(isolate, object, key);
    if (maybe_attributes.IsNothing()) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    PropertyAttributes attributes = maybe_attributes.FromJust();
    if (attributes != ABSENT && (attributes & DONT_ENUM) == 0) {
      *out_result = roots.true_value().ptr();
    } else {
      *out_result = roots.false_value().ptr();
    }
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kArrayPrototypeReverse) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<Object> receiver_object(
        Tagged<Object>(SafeTaggedOrUndefined(isolate, receiver)), isolate);
    DirectHandle<JSReceiver> object;
    if (!Object::ToObject(isolate, receiver_object, "Array.prototype.reverse")
             .ToHandle(&object)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<Object> length_object;
    if (!Object::GetLengthFromArrayLike(isolate, object)
             .ToHandle(&length_object)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    double raw_length = Object::NumberValue(*length_object);
    if (raw_length < 0) raw_length = 0;
    if (raw_length > static_cast<double>(kMaxUInt32)) {
      raw_length = static_cast<double>(kMaxUInt32);
    }
    uint32_t length = static_cast<uint32_t>(raw_length);
    DirectHandle<JSAny> object_any = Cast<JSAny>(object);

    auto set_element = [&](uint32_t index,
                           DirectHandle<Object> value) -> bool {
      DirectHandle<Object> ignored;
      return Object::SetElement(isolate, object_any, index, value,
                                ShouldThrow::kThrowOnError)
          .ToHandle(&ignored);
    };
    auto delete_element = [&](uint32_t index) -> bool {
      DirectHandle<Object> key = isolate->factory()->NewNumberFromUint(index);
      Maybe<bool> deleted = Runtime::DeleteObjectProperty(
          isolate, object, key, LanguageMode::kStrict);
      return deleted.IsJust() && deleted.FromJust();
    };

    for (uint32_t lower = 0, upper = length == 0 ? 0 : length - 1;
         lower < upper; ++lower, --upper) {
      Maybe<bool> lower_exists = JSReceiver::HasElement(isolate, object, lower);
      Maybe<bool> upper_exists = JSReceiver::HasElement(isolate, object, upper);
      if (lower_exists.IsNothing() || upper_exists.IsNothing()) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }

      DirectHandle<Object> lower_value(roots.undefined_value(), isolate);
      DirectHandle<Object> upper_value(roots.undefined_value(), isolate);
      if (lower_exists.FromJust() &&
          !JSReceiver::GetElement(isolate, object, lower)
               .ToHandle(&lower_value)) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
      if (upper_exists.FromJust() &&
          !JSReceiver::GetElement(isolate, object, upper)
               .ToHandle(&upper_value)) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }

      bool ok = true;
      if (lower_exists.FromJust() && upper_exists.FromJust()) {
        ok = set_element(lower, upper_value) &&
             set_element(upper, lower_value);
      } else if (!lower_exists.FromJust() && upper_exists.FromJust()) {
        ok = set_element(lower, upper_value) && delete_element(upper);
      } else if (lower_exists.FromJust() && !upper_exists.FromJust()) {
        ok = delete_element(lower) && set_element(upper, lower_value);
      }
      if (!ok) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
    }

    *out_result = (*object).ptr();
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kArrayPrototypeToSorted) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<Object> comparefn =
        actual_argc > 0
            ? direct_handle(
                  Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[0])),
                  isolate)
            : direct_handle(roots.undefined_value(), isolate);
    if (!IsUndefined(*comparefn, roots) && !IsCallable(*comparefn)) {
      isolate->Throw(*isolate->factory()->NewTypeError(
          MessageTemplate::kBadSortComparisonFunction, comparefn));
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<Object> receiver_object(
        Tagged<Object>(SafeTaggedOrUndefined(isolate, receiver)), isolate);
    DirectHandle<JSReceiver> object;
    if (!Object::ToObject(isolate, receiver_object,
                          "Array.prototype.toSorted")
             .ToHandle(&object)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<Object> length_object;
    if (!Object::GetLengthFromArrayLike(isolate, object)
             .ToHandle(&length_object)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    double raw_length = Object::NumberValue(*length_object);
    if (raw_length < 0 || raw_length > FixedArray::kMaxLength) {
      isolate->Throw(*isolate->factory()->NewRangeError(
          MessageTemplate::kInvalidArrayLength, length_object));
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    int length = static_cast<int>(raw_length);
    DirectHandle<FixedArray> elements =
        isolate->factory()->NewFixedArray(length);
    for (int index = 0; index < length; ++index) {
      DirectHandle<Object> value(roots.undefined_value(), isolate);
      Maybe<bool> maybe_has_element =
          JSReceiver::HasElement(isolate, object, index);
      if (maybe_has_element.IsNothing()) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
      if (maybe_has_element.FromJust()) {
        if (!JSReceiver::GetElement(isolate, object, index).ToHandle(&value)) {
          isolate->set_context(saved_context);
          *out_result = roots.exception().ptr();
          return true;
        }
      }
      elements->set(index, *value);
    }

    for (int i = 1; i < length; ++i) {
      int j = i;
      while (j > 0) {
        DirectHandle<Object> left =
            direct_handle(elements->get(j), isolate);
        DirectHandle<Object> right =
            direct_handle(elements->get(j - 1), isolate);
        bool left_less = false;
        if (!TryCompareForArrayToSorted(isolate, comparefn, left, right,
                                        &left_less)) {
          isolate->set_context(saved_context);
          *out_result = roots.exception().ptr();
          return true;
        }
        if (!left_less) break;
        Tagged<Object> tmp = elements->get(j - 1);
        elements->set(j - 1, elements->get(j));
        elements->set(j, tmp);
        --j;
      }
    }

    DirectHandle<JSArray> result = isolate->factory()->NewJSArrayWithElements(
        elements, PACKED_ELEMENTS, length);
    *out_result = (*result).ptr();
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kArrayPrototypeFill) {
    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<Object> receiver_object(
        Tagged<Object>(SafeTaggedOrUndefined(isolate, receiver)), isolate);
    DirectHandle<JSReceiver> object;
    if (!Object::ToObject(isolate, receiver_object, "Array.prototype.fill")
             .ToHandle(&object)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<Object> length_object;
    if (!Object::GetLengthFromArrayLike(isolate, object)
             .ToHandle(&length_object)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    double length = Object::NumberValue(*length_object);
    if (length < 0) length = 0;
    if (length > static_cast<double>(kMaxUInt32)) {
      length = static_cast<double>(kMaxUInt32);
    }

    auto read_relative_index = [&](int arg_index, double default_value,
                                   double* out_index) -> bool {
      DirectHandle<Object> arg =
          arg_index < actual_argc
              ? direct_handle(
                    Tagged<Object>(
                        SafeTaggedOrUndefined(isolate, argv[arg_index])),
                    isolate)
              : direct_handle(roots.undefined_value(), isolate);
      if (IsUndefined(*arg, roots)) {
        *out_index = default_value;
        return true;
      }
      DirectHandle<Number> number;
      if (!Object::ToInteger(isolate, arg).ToHandle(&number)) return false;
      double relative = Object::NumberValue(*number);
      if (relative < 0) {
        double index = length + relative;
        *out_index = index > 0 ? index : 0;
      } else {
        *out_index = relative < length ? relative : length;
      }
      return true;
    };

    double start_index = 0;
    double end_index = length;
    if (!read_relative_index(1, 0, &start_index) ||
        !read_relative_index(2, length, &end_index)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    if (start_index < 0) start_index = 0;
    if (end_index < start_index) end_index = start_index;
    if (end_index > length) end_index = length;

    DirectHandle<Object> value =
        actual_argc > 0
            ? direct_handle(
                  Tagged<Object>(SafeTaggedOrUndefined(isolate, argv[0])),
                  isolate)
            : direct_handle(roots.undefined_value(), isolate);
    DirectHandle<JSAny> object_any = Cast<JSAny>(object);
    for (uint32_t index = static_cast<uint32_t>(start_index);
         index < static_cast<uint32_t>(end_index); ++index) {
      DirectHandle<Object> ignored;
      if (!Object::SetElement(isolate, object_any, index, value,
                              ShouldThrow::kThrowOnError)
               .ToHandle(&ignored)) {
        isolate->set_context(saved_context);
        *out_result = roots.exception().ptr();
        return true;
      }
    }

    *out_result = (*object).ptr();
    isolate->set_context(saved_context);
    return true;
  }

  if (builtin == Builtin::kReflectApply ||
      builtin == Builtin::kFunctionPrototypeApply) {
    bool is_reflect_apply = builtin == Builtin::kReflectApply;
    Address callable_address =
        is_reflect_apply
            ? (actual_argc > 0 ? SafeTaggedOrUndefined(isolate, argv[0])
                               : roots.undefined_value().ptr())
            : SafeTaggedOrUndefined(isolate, receiver);
    Address this_arg_address =
        is_reflect_apply
            ? (actual_argc > 1 ? SafeTaggedOrUndefined(isolate, argv[1])
                               : roots.undefined_value().ptr())
            : (actual_argc > 0 ? SafeTaggedOrUndefined(isolate, argv[0])
                               : roots.undefined_value().ptr());
    Address args_array_address =
        is_reflect_apply
            ? (actual_argc > 2 ? SafeTaggedOrUndefined(isolate, argv[2])
                               : roots.undefined_value().ptr())
            : (actual_argc > 1 ? SafeTaggedOrUndefined(isolate, argv[1])
                               : roots.undefined_value().ptr());

    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<Object> callable =
        direct_handle(Tagged<Object>(callable_address), isolate);
    if (!IsCallable(*callable)) {
      isolate->Throw(*isolate->factory()->NewTypeError(
          MessageTemplate::kCalledNonCallable, callable));
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<Object> this_arg =
        direct_handle(Tagged<Object>(this_arg_address), isolate);
    DirectHandle<Object> args_array =
        direct_handle(Tagged<Object>(args_array_address), isolate);

#ifdef __wasi__
    static int reflect_apply_trace_count = 0;
    const bool trace_reflect_apply =
        kTraceWasmFallbackDetails && reflect_apply_trace_count < 64;
    if (trace_reflect_apply) {
      PrintF("WasmJSEntry: %s fallback enter callable=",
             Builtins::name(builtin));
      DumpRuntimeArg("callable", 0, callable_address);
      PrintF(" this=");
      DumpRuntimeArg("this", 0, this_arg_address);
      PrintF(" argsArray=");
      DumpRuntimeArg("argsArray", 0, args_array_address);
      if (IsJSFunction(*callable)) {
        PrintF(" jsfn_name=");
        DumpNameForTrace(
            Wasm32JSFunctionShared(Cast<JSFunction>(*callable))->Name());
      }
      PrintF("\n");
    }
#endif

    DirectHandle<FixedArray> args_list;
    if (IsNullOrUndefined(*args_array, isolate)) {
      args_list = isolate->factory()->empty_fixed_array();
    } else if (!Object::CreateListFromArrayLike(isolate, args_array,
                                                ElementTypes::kAll)
                    .ToHandle(&args_list)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    std::vector<DirectHandle<Object>> call_args;
    call_args.reserve(args_list->length());
    for (int i = 0; i < args_list->length(); ++i) {
      call_args.push_back(direct_handle(args_list->get(i), isolate));
    }
#ifdef __wasi__
    if (trace_reflect_apply) {
      PrintF("WasmJSEntry: %s fallback args length=%d",
             Builtins::name(builtin), args_list->length());
      for (int i = 0; i < args_list->length() && i < 6; ++i) {
        PrintF(" ");
        DumpRuntimeArg("arg", i, (*call_args[i]).ptr());
      }
      PrintF("\n");
    }
#endif

    WasmInterpreterStateSnapshot state(isolate);
    Address direct_result = roots.exception().ptr();
    bool used_direct_call = TryCallJSFunctionDirect(
        isolate, callable, this_arg, static_cast<int>(call_args.size()),
        call_args.empty() ? nullptr : call_args.data(), &direct_result);
    MaybeHandle<Object> maybe_result;
    if (!used_direct_call) {
      maybe_result = Execution::Call(
          isolate, callable, this_arg,
          ZoneVector<const DirectHandle<Object>>(
              call_args.empty() ? nullptr : call_args.data(),
              call_args.size()));
    }
    state.Restore();
    isolate->set_context(saved_context);
#ifdef __wasi__
    if (trace_reflect_apply) {
      PrintF("WasmJSEntry: %s fallback return direct=%d maybe_empty=%d "
             "has_exception=%d ",
             Builtins::name(builtin), used_direct_call ? 1 : 0,
             (!used_direct_call && maybe_result.is_null()) ? 1 : 0,
             isolate->has_exception() ? 1 : 0);
      DumpRuntimeArg("result", 0,
                     used_direct_call ? direct_result : roots.undefined_value().ptr());
      PrintF("\n");
      reflect_apply_trace_count++;
    }
#endif
    if (used_direct_call) {
      *out_result = direct_result;
      return true;
    }
    DirectHandle<Object> result;
    if (!maybe_result.ToHandle(&result)) {
      *out_result = roots.exception().ptr();
      return true;
    }
    *out_result = (*result).ptr();
    return true;
  }

  if (builtin == Builtin::kReflectDefineProperty) {
    if (kTraceWasmJSEntry) {
      PrintF("WasmJSEntry: ReflectDefineProperty args actual_argc=%d",
             actual_argc);
      for (int i = 0; i < actual_argc && i < 4; ++i) {
        DumpRuntimeArg("argv", i, argv[i]);
      }
      PrintF("\n");
    }

    Address target_address = actual_argc > 0
                                 ? SafeTaggedOrUndefined(isolate, argv[0])
                                 : roots.undefined_value().ptr();
    Address key_address = actual_argc > 1
                              ? SafeTaggedOrUndefined(isolate, argv[1])
                              : roots.undefined_value().ptr();
    Address attributes_address = actual_argc > 2
                                     ? SafeTaggedOrUndefined(isolate, argv[2])
                                     : roots.undefined_value().ptr();
    if (!IsSafeTaggedHandleValue(target_address) ||
        !IsJSReceiver(Tagged<Object>(target_address))) {
      PrintF("WasmJSEntry: ReflectDefineProperty target invalid ");
      DumpRuntimeArg("target", 0, target_address);
      PrintF("\n");
      *out_result = roots.exception().ptr();
      return true;
    }

    Tagged<Context> saved_context = isolate->context();
    isolate->set_context(Wasm32JSFunctionContext(*function));

    HandleScope scope(isolate);
    DirectHandle<Object> key = direct_handle(Tagged<Object>(key_address),
                                             isolate);
    Handle<JSAny> attributes =
        handle(Cast<JSAny>(Tagged<Object>(attributes_address)), isolate);
    DirectHandle<Name> name;
    if (!Object::ToName(isolate, key).ToHandle(&name)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }
    bool trace_reflect_own_keys_name =
        kTraceWasmFallbackDetails &&
        IsReflectOwnKeysTraceName(isolate, *name);
    if (trace_reflect_own_keys_name) {
      PrintF("WasmJSEntry: ReflectDefineProperty trace before ");
      DumpRuntimeArg("target", 0, target_address);
      DumpRuntimeArg("key", 0, (*name).ptr());
      DumpRuntimeArg("attributes", 0, attributes_address);
      PrintF("\n");
    }

    PropertyDescriptor desc;
    if (!PropertyDescriptor::ToPropertyDescriptor(isolate, attributes, &desc)) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    DirectHandle<JSReceiver> target =
        direct_handle(Cast<JSReceiver>(Tagged<Object>(target_address)),
                      isolate);
    Maybe<bool> result = JSReceiver::DefineOwnProperty(
        isolate, target, name, &desc, Just(kDontThrow));
    if (result.IsNothing()) {
      isolate->set_context(saved_context);
      *out_result = roots.exception().ptr();
      return true;
    }

    *out_result = (*isolate->factory()->ToBoolean(result.FromJust())).ptr();
    isolate->set_context(saved_context);
    if (trace_reflect_own_keys_name) {
      PrintF("WasmJSEntry: ReflectDefineProperty trace after ");
      DumpRuntimeArg("result", 0, *out_result);
      if (desc.has_value()) {
        DumpRuntimeArg("desc_value", 0, (*desc.value()).ptr());
      }
      PrintF("\n");
    }
    if (kTraceWasmJSEntry) {
      PrintF("WasmJSEntry: fallback ReflectDefineProperty result=0x%x\n",
             static_cast<unsigned>(*out_result));
    }
    return true;
  }

  if (builtin != Builtin::kReflectOwnKeys) return false;

  if (kTraceWasmJSEntry) {
    PrintF("WasmJSEntry: ReflectOwnKeys args actual_argc=%d", actual_argc);
    for (int i = 0; i < actual_argc && i < 4; ++i) {
      DumpRuntimeArg("argv", i, argv[i]);
    }
    PrintF("\n");
  }
  Address target_address = actual_argc > 0
                               ? SafeTaggedOrUndefined(isolate, argv[0])
                               : roots.undefined_value().ptr();
  if (!IsSafeTaggedHandleValue(target_address) ||
      !IsJSReceiver(Tagged<Object>(target_address))) {
    PrintF("WasmJSEntry: ReflectOwnKeys target invalid ");
    DumpRuntimeArg("target", 0, target_address);
    PrintF("\n");
    *out_result = roots.exception().ptr();
    return true;
  }

  Tagged<Context> saved_context = isolate->context();
  isolate->set_context(Wasm32JSFunctionContext(*function));

  HandleScope scope(isolate);
  DirectHandle<JSReceiver> target =
      direct_handle(Cast<JSReceiver>(Tagged<Object>(target_address)), isolate);
  DirectHandle<FixedArray> keys;
  if (!KeyAccumulator::GetKeys(isolate, target, KeyCollectionMode::kOwnOnly,
                               ALL_PROPERTIES,
                               GetKeysConversion::kConvertToString)
           .ToHandle(&keys)) {
    isolate->set_context(saved_context);
    *out_result = roots.exception().ptr();
    return true;
  }
  DirectHandle<JSArray> result =
      isolate->factory()->NewJSArrayWithElements(keys);
  *out_result = (*result).ptr();
  isolate->set_context(saved_context);
  if (kTraceWasmJSEntry) {
    PrintF("WasmJSEntry: fallback ReflectOwnKeys result=0x%x\n",
           static_cast<unsigned>(*out_result));
  }
  return true;
}

}  // namespace

extern "C" Address WasmRuntimeCallFromGenerated(Address runtime_entry,
                                                int32_t argc) {
  constexpr int kMaxWasmRuntimeArgs = 64;
  Address root = g_wasm_regs[kWasmRegRoot];
  if (root == kNullAddress) root = g_wasm_regs[SlotFor(kRootRegister)];
  Isolate* isolate = GetWasm32IsolateFromRoot(&root);
  if (isolate == nullptr) return Smi::zero().ptr();
  SetCurrentIsolateScope current_isolate_scope(isolate);
  ClearEntrypointStackWindow();
  g_wasm_regs[kWasmRegRoot] = root;
  g_wasm_regs[SlotFor(kRootRegister)] = root;
  if (argc < 0 || argc > kMaxWasmRuntimeArgs) {
    PrintF("WasmRuntimeCallFromGenerated: bad entry=0x%x argc=%d\n",
           static_cast<unsigned>(runtime_entry), argc);
    return Smi::zero().ptr();
  }

  const Runtime::Function* function = nullptr;
  int32_t encoded_runtime_entry = static_cast<int32_t>(runtime_entry);
  if (encoded_runtime_entry < 0) {
    int runtime_id = -encoded_runtime_entry - 1;
    if (runtime_id >= 0 && runtime_id < Runtime::kNumFunctions) {
      function =
          Runtime::FunctionForId(static_cast<Runtime::FunctionId>(runtime_id));
    }
  } else {
    function = Runtime::FunctionForEntry(runtime_entry);
  }
  if (function == nullptr) {
    PrintF("WasmRuntimeCallFromGenerated: unknown entry=0x%x argc=%d\n",
           static_cast<unsigned>(runtime_entry), argc);
    return Smi::zero().ptr();
  }
  bool allow_wasm32_runtime_abi =
      function->function_id == Runtime::kGetDerivedMap && argc == 3;
  if (!allow_wasm32_runtime_abi &&
      (function->result_size != 1 ||
       (function->nargs >= 0 && function->nargs != argc))) {
    PrintF("WasmRuntimeCallFromGenerated: unsupported %s id=%d argc=%d "
           "expected=%d results=%d\n",
           function->name, static_cast<int>(function->function_id), argc,
           function->nargs,
           function->result_size);
    DumpCurrentRuntimeCallStateForTrace("unsupported-state");
    PrintF("WasmRuntimeCallFromGenerated: unsupported target detail");
    DumpFunctionSourceForTrace(
        g_wasm_regs[SlotFor(kJavaScriptCallTargetRegister)]);
    PrintF("\n");
    PrintF("WasmRuntimeCallFromGenerated: unsupported code_start detail");
    DumpFunctionSourceForTrace(
        g_wasm_regs[SlotFor(kJavaScriptCallCodeStartRegister)]);
    PrintF("\n");
    return Smi::zero().ptr();
  }

  Address argv[kMaxWasmRuntimeArgs == 0 ? 1 : kMaxWasmRuntimeArgs];
  for (int i = 0; i < argc; ++i) {
    int slot = kWasmOutgoingArgSlotBase + i;
    argv[argc - 1 - i] = g_wasm_regs[slot];
  }
  if (function->function_id == Runtime::kNewObject && argc == 2) {
    argv[1] = SafeRuntimeArgOrUndefined(
        isolate, g_wasm_regs[SlotFor(kJavaScriptCallTargetRegister)]);
    argv[0] = SafeRuntimeArgOrUndefined(
        isolate, g_wasm_regs[SlotFor(kJavaScriptCallNewTargetRegister)]);
  }

  WasmGCStateScope gc_state(isolate);

  if (IsBytecodeBudgetInterruptRuntime(function->function_id)) {
    Address result = ReadOnlyRoots(isolate).undefined_value().ptr();
    g_wasm_regs[SlotFor(kReturnRegister0)] = result;
    return result;
  }

  if (kTraceWasmFallbackDetails &&
      (function->function_id == Runtime::kLoadIC_Miss ||
       function->function_id == Runtime::kKeyedLoadIC_Miss ||
       function->function_id == Runtime::kJSReceiverGetPrototypeOf ||
       function->function_id == Runtime::kNewObject)) {
    PrintF("WasmRuntimeCallFromGenerated: entering %s id=%d argc=%d "
           "entry=0x%x context=0x%x\n",
           function->name, static_cast<int>(function->function_id), argc,
           static_cast<unsigned>(runtime_entry),
           static_cast<unsigned>(g_wasm_regs[SlotFor(kContextRegister)]));
    for (int i = 0; i < argc; ++i) {
      DumpRuntimeArg("arg", i, argv[argc - 1 - i]);
    }
    PrintF("\n");
    for (int i = 0; i < argc; ++i) {
      DumpRuntimeArg("slot", kWasmOutgoingArgSlotBase + i,
                     g_wasm_regs[kWasmOutgoingArgSlotBase + i]);
    }
    PrintF("\n");
    for (int i = -8; i < 16; ++i) {
      int slot = kWasmOutgoingArgSlotBase + i;
      if (slot >= 0 && slot < kWasmRegFileSize) {
        DumpRuntimeArg("win", slot, g_wasm_regs[slot]);
      }
    }
    PrintF("\n");
  }

  static uint32_t to_numeric_count = 0;
  uint32_t to_numeric_sample = 0;
  if (kTraceWasmFallbackDetails &&
      function->function_id == Runtime::kToNumeric) {
    to_numeric_sample = ++to_numeric_count;
    if (to_numeric_sample <= 16 || (to_numeric_sample % 10000) == 0) {
      PrintF("WasmRuntimeCallFromGenerated: ToNumeric call count=%u argc=%d "
             "context=0x%x arg0=0x%x\n",
             to_numeric_sample, argc,
             static_cast<unsigned>(g_wasm_regs[SlotFor(kContextRegister)]),
             argc > 0 ? static_cast<unsigned>(argv[argc - 1]) : 0);
      if (argc > 0) DumpRuntimeArg("ToNumeric.arg", 0, argv[argc - 1]);
      DumpRuntimeArg("ToNumeric.acc", 0,
                     g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
      PrintF("\n");
      DumpCurrentInterpreterBytecodeForTrace("ToNumeric.pc");
    }
  }

  Tagged<Context> saved_context = isolate->context();
  Address context = g_wasm_regs[SlotFor(kContextRegister)];
  bool switched_context = false;
  if (context != kNullAddress && IsContext(Tagged<Object>(context))) {
    isolate->set_context(Cast<Context>(Tagged<Object>(context)));
    switched_context = true;
  }

  Address fallback_result = kNullAddress;
  if (TryFallbackICMiss(isolate, function->function_id, argv, argc,
                        &fallback_result)) {
    if (switched_context) isolate->set_context(saved_context);
    g_wasm_regs[SlotFor(kReturnRegister0)] = fallback_result;
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmRuntimeCallFromGenerated: %s id=%d argc=%d fallback=0x%x\n",
             function->name, static_cast<int>(function->function_id), argc,
             static_cast<unsigned>(fallback_result));
    }
    return fallback_result;
  }
  if (TryFallbackGeneratedRuntime(isolate, function->function_id, argv, argc,
                                  &fallback_result)) {
    if (switched_context) isolate->set_context(saved_context);
    g_wasm_regs[SlotFor(kReturnRegister0)] = fallback_result;
#ifdef __wasi__
    if (kTraceWasmFallbackDetails &&
        function->function_id == Runtime::kJSReceiverGetPrototypeOf) {
      PrintF("WasmRuntimeCallFromGenerated: generated runtime return %s id=%d "
             "argc=%d ",
             function->name, static_cast<int>(function->function_id), argc);
      DumpRuntimeArg("result", 0, fallback_result);
      PrintF("\n");
    }
#endif
    if (kTraceWasmFallbackDetails) {
      PrintF("WasmRuntimeCallFromGenerated: %s id=%d argc=%d fallback=0x%x\n",
             function->name, static_cast<int>(function->function_id), argc,
             static_cast<unsigned>(fallback_result));
    }
    return fallback_result;
  }

  using RuntimeEntry = Address (*)(int, Address*, Isolate*);
  Address* args_object = argc == 0 ? argv : &argv[argc - 1];
  Address result =
      reinterpret_cast<RuntimeEntry>(function->entry)(argc, args_object,
                                                      isolate);
  if (switched_context) isolate->set_context(saved_context);

  if (function->function_id == Runtime::kNewObject &&
      IsHeapObject(Tagged<Object>(result))) {
    HandleScope init_scope(isolate);
    Tagged<Object> object(result);
    if (IsJSSet(object)) {
      DirectHandle<JSSet> set(Cast<JSSet>(object), isolate);
      JSSet::Initialize(set, isolate);
      if (kTraceWasmFallbackDetails) {
        PrintF("WasmRuntimeCallFromGenerated: initialized JSSet table "
               "result=0x%x table=0x%x\n",
               static_cast<unsigned>(result),
               static_cast<unsigned>(set->table().ptr()));
      }
    } else if (IsJSMap(object)) {
      DirectHandle<JSMap> map(Cast<JSMap>(object), isolate);
      JSMap::Initialize(map, isolate);
      if (kTraceWasmFallbackDetails) {
        PrintF("WasmRuntimeCallFromGenerated: initialized JSMap table "
               "result=0x%x table=0x%x\n",
               static_cast<unsigned>(result),
               static_cast<unsigned>(map->table().ptr()));
      }
    } else if (IsJSWeakCollection(object)) {
      DirectHandle<JSWeakCollection> weak_collection(
          Cast<JSWeakCollection>(object), isolate);
      JSWeakCollection::Initialize(weak_collection, isolate);
      if (kTraceWasmFallbackDetails) {
        PrintF("WasmRuntimeCallFromGenerated: initialized JSWeakCollection "
               "table result=0x%x table=0x%x\n",
               static_cast<unsigned>(result),
               static_cast<unsigned>(weak_collection->table().ptr()));
      }
    }
  }

  g_wasm_regs[SlotFor(kReturnRegister0)] = result;
  if (to_numeric_sample != 0 &&
      (to_numeric_sample <= 16 || (to_numeric_sample % 10000) == 0)) {
    PrintF("WasmRuntimeCallFromGenerated: ToNumeric return count=%u "
           "result=0x%x\n",
           to_numeric_sample, static_cast<unsigned>(result));
    DumpRuntimeArg("ToNumeric.result", 0, result);
    PrintF("\n");
  }
  if (kTraceWasmFallbackDetails) {
    PrintF("WasmRuntimeCallFromGenerated: %s id=%d argc=%d result=0x%x\n",
           function->name, static_cast<int>(function->function_id), argc,
           static_cast<unsigned>(result));
  }
  return result;
}

extern "C" void WasmInterpreterEntryTrampoline() {
  Address root = g_wasm_regs[kWasmRegRoot];
  Isolate* isolate = GetWasm32IsolateFromRoot(&root);
  if (isolate == nullptr) return;
  g_wasm_regs[kWasmRegRoot] = root;
  g_wasm_regs[SlotFor(kRootRegister)] = root;
  Address target = g_wasm_regs[SlotFor(kJavaScriptCallTargetRegister)];
  Address new_target = g_wasm_regs[SlotFor(kJavaScriptCallNewTargetRegister)];
  int argc =
      static_cast<int>(g_wasm_regs[SlotFor(kJavaScriptCallArgCountRegister)]);
  if (kTraceWasmInterpreterSteps) {
    PrintF("WasmInterpreterEntryTrampoline: target=0x%x new_target=0x%x "
           "argc=%d\n",
           static_cast<unsigned>(target), static_cast<unsigned>(new_target),
           argc);
  }

  Tagged<Object> target_object(target);
  if (!IsJSFunction(target_object)) {
    PrintF("WasmInterpreterEntryTrampoline: target is not JSFunction\n");
    g_wasm_regs[SlotFor(kReturnRegister0)] = Smi::zero().ptr();
    return;
  }

  Tagged<JSFunction> function = Cast<JSFunction>(target_object);
  Tagged<SharedFunctionInfo> shared = Wasm32JSFunctionShared(function);
#ifdef __wasi__
  if (kTraceWasmFallbackDetails &&
      FunctionMatchesWasmEvalTraceNeedle(shared)) {
    PrintF("WasmInterpreterEntryTrampoline: eval probe target=0x%x "
           "new_target=0x%x argc=%d has_bytecode=%d ",
           static_cast<unsigned>(target), static_cast<unsigned>(new_target),
           argc, shared->HasBytecodeArray() ? 1 : 0);
    DumpFunctionSourceForTrace(target);
    PrintF("\n");
  }
#endif
  if (!shared->HasBytecodeArray()) {
    PrintF("WasmInterpreterEntryTrampoline: missing bytecode array "
           "target=0x%x sfi=0x%x builtin=%d kind=%d name=",
           static_cast<unsigned>(target), static_cast<unsigned>(shared.ptr()),
           function->code(isolate)->is_builtin()
               ? static_cast<int>(function->code(isolate)->builtin_id())
               : -1,
           static_cast<int>(shared->kind()));
    DumpNameForTrace(shared->Name());
    PrintF("\n");
    DumpFunctionSourceForTrace(target);
    g_wasm_regs[SlotFor(kReturnRegister0)] = Smi::zero().ptr();
    return;
  }

  Address fast_result = kNullAddress;
  if (TryRunPerContextPrimordialsGetNewKey(isolate, shared, argc,
                                           &fast_result)) {
    g_wasm_regs[SlotFor(kReturnRegister0)] = fast_result;
    return;
  }

  Tagged<Context> function_context = Wasm32JSFunctionContext(function);
  Tagged<BytecodeArray> bytecode = shared->GetBytecodeArray(isolate);
  Address bytecode_offset = BytecodeArray::kHeaderSize - kHeapObjectTag;
  Address dispatch_table = isolate->interpreter()->dispatch_table_address();
  Address undefined = ReadOnlyRoots(isolate).undefined_value().ptr();
  Address feedback_vector =
      Wasm32JSFunctionFeedbackVectorOrUndefined(isolate, function).ptr();
  static int wasm_interpreter_entry_trace_count = 0;
  bool trace_entry_steps = false;
  int trace_entry_index = -1;
#ifdef __wasi__
  bool trace_buffer_probe =
      shared->StartPosition() == 38 || shared->StartPosition() == 95 ||
      shared->StartPosition() == 147;
  bool trace_per_context_primordials =
      kTraceWasmFallbackDetails &&
      FunctionMatchesPerContextPrimordialsTraceNeedle(shared);
  bool trace_eval_source =
      kTraceWasmFallbackDetails && FunctionMatchesWasmEvalTraceNeedle(shared);
  bool trace_domexception =
      kTraceWasmFallbackDetails &&
      FunctionScriptNameContainsAsciiForTrace(
          shared, "node:internal/per_context/domexception");
  bool trace_bootstrap_realm_source =
      kTraceWasmFallbackDetails &&
      FunctionScriptNameContainsAsciiForTrace(shared,
                                             "node:internal/bootstrap/realm");
  bool trace_bootstrap_entry =
      kTraceWasmFallbackDetails &&
      (trace_domexception || wasm_interpreter_entry_trace_count < 2);
  if (trace_buffer_probe || trace_bootstrap_entry ||
      (trace_per_context_primordials &&
       wasm_interpreter_entry_trace_count < 32) ||
      trace_eval_source ||
      trace_bootstrap_realm_source ||
      (kTraceWasmFallbackDetails && wasm_interpreter_entry_trace_count < 12)) {
    trace_entry_index = wasm_interpreter_entry_trace_count++;
    trace_entry_steps = true;
    PrintF("WasmInterpreterEntryTrace: enter #%d eval_match=%d "
           "primordials_match=%d argc=%d actual=%d bytecode_len=%d regs=%d "
           "params=%d ",
           trace_entry_index, trace_eval_source ? 1 : 0,
           trace_per_context_primordials ? 1 : 0, argc,
           argc - kJSArgcReceiverSlots,
           bytecode->length(), bytecode->register_count(),
           bytecode->parameter_count());
    DumpFunctionSourceForTrace(target);
    PrintF("\n");
  }
#endif
  if (kTraceWasmInterpreterSteps) {
    PrintF("WasmInterpreterEntryTrampoline: feedback has=%d value=",
           IsFeedbackVector(Tagged<Object>(feedback_vector)));
    DumpRuntimeArg("fbv", 0, feedback_vector);
    PrintF(" fbv_operand=%d fbv_offset=%d\n",
           interpreter::Register::feedback_vector().ToOperand(),
           InterpreterFrameConstants::kFeedbackVectorFromFp);
  }

  ClearInterpreterFrame();
  StoreInterpreterFrameOffset(CommonFrameConstants::kCallerFPOffset, 0);
  StoreInterpreterFrameOffset(CommonFrameConstants::kCallerPCOffset, 0);
  StoreInterpreterFrameOffset(StandardFrameConstants::kContextOffset,
                              function_context.ptr());
  StoreInterpreterFrameOffset(StandardFrameConstants::kFunctionOffset, target);
  StoreInterpreterFrameOffset(StandardFrameConstants::kArgCOffset,
                              static_cast<Address>(argc));
  StoreInterpreterFrameOffset(InterpreterFrameConstants::kBytecodeArrayFromFp,
                              bytecode.ptr());
  StoreInterpreterFrameOffset(InterpreterFrameConstants::kBytecodeOffsetFromFp,
                              Smi::FromInt(static_cast<int>(bytecode_offset))
                                  .ptr());
  StoreInterpreterFrameOffset(InterpreterFrameConstants::kFeedbackVectorFromFp,
                              feedback_vector);
  Address current_context_from_frame =
      g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
          interpreter::Register::current_context().ToOperand() *
          kSystemPointerSize)];
  if (kTraceWasmInterpreterSteps) {
    PrintF("WasmInterpreterEntryTrampoline: context=0x%x current_operand=%d "
           "frame_current=0x%x ctx_slot=0x%x fp=0x%x\n",
           static_cast<unsigned>(function_context.ptr()),
           interpreter::Register::current_context().ToOperand(),
           static_cast<unsigned>(current_context_from_frame),
           static_cast<unsigned>(g_wasm_interpreter_frame[
               InterpreterFrameSlotForOffset(
                   StandardFrameConstants::kContextOffset)]),
           static_cast<unsigned>(g_wasm_current_frame_pointer));
    PrintF("WasmInterpreterEntryTrampoline:");
    DumpRuntimeArg("ctx", 0, function_context.ptr());
    PrintF(" mem_pages=%zu mem_bytes=0x%zx\n",
           static_cast<size_t>(__builtin_wasm_memory_size(0)),
           static_cast<size_t>(__builtin_wasm_memory_size(0)) * 65536u);
  }

  int register_count = bytecode->register_count();
  for (int i = 0; i < register_count; ++i) {
    StoreInterpreterRegister(interpreter::Register(i), undefined);
  }

  int actual_argc = argc - kJSArgcReceiverSlots;
  if (actual_argc < 0) actual_argc = 0;
  int parameter_count = bytecode->parameter_count();
  int frame_parameter_count = parameter_count;
  if (frame_parameter_count < actual_argc + 1) {
    frame_parameter_count = actual_argc + 1;
  }
  for (int i = 0; i < frame_parameter_count; ++i) {
    Address value = undefined;
    if (i == 0) {
      value = IsDerivedConstructor(shared->kind())
                  ? ReadOnlyRoots(isolate).the_hole_value().ptr()
                  : g_wasm_regs[kWasmJSEntryArgSlotBase];
    } else if (i - 1 < actual_argc) {
      value = g_wasm_regs[kWasmJSEntryArgSlotBase + i];
    }
    StoreInterpreterRegister(interpreter::Register::FromParameterIndex(i), value);
  }

  interpreter::Register incoming =
      bytecode->incoming_new_target_or_generator_register();
  if (incoming.is_valid()) {
    StoreInterpreterRegister(incoming, new_target);
  }

  g_wasm_regs[SlotFor(kRootRegister)] = root;
  g_wasm_regs[SlotFor(kContextRegister)] = function_context.ptr();
  g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)] = undefined;
  g_wasm_regs[SlotFor(Register::r4())] = bytecode_offset;
  g_wasm_regs[SlotFor(Register::r5())] = bytecode.ptr();
  g_wasm_regs[SlotFor(Register::r6())] = dispatch_table;
  g_wasm_regs[SlotFor(kInterpreterBytecodeOffsetRegister)] = bytecode_offset;
  g_wasm_regs[SlotFor(kInterpreterBytecodeArrayRegister)] = bytecode.ptr();
  g_wasm_regs[kWasmStackSlotBase] = bytecode_offset;
  g_wasm_regs[kWasmStackSlotBase + 1] = bytecode.ptr();
  g_wasm_regs[kWasmStackSlotBase + 2] = dispatch_table;
  g_wasm_regs[kWasmStackSlotBase + 3] = g_wasm_current_frame_pointer;

  using WasmRegFileFn = void (*)();
  constexpr int kMaxInterpreterSteps = 1000000000;
  constexpr int kMaxInterpreterTailTrace = 32;
  int tail_step[kMaxInterpreterTailTrace] = {};
  int tail_index[kMaxInterpreterTailTrace] = {};
  uint8_t tail_opcode[kMaxInterpreterTailTrace] = {};
  interpreter::Bytecode tail_bytecode[kMaxInterpreterTailTrace] = {};
  int tail_operand0[kMaxInterpreterTailTrace] = {};
  int tail_operand1[kMaxInterpreterTailTrace] = {};
  int tail_operand2[kMaxInterpreterTailTrace] = {};
  Address tail_accumulator[kMaxInterpreterTailTrace] = {};
  Address current_offset = bytecode_offset;
  interpreter::OperandScale operand_scale = interpreter::OperandScale::kSingle;
  PendingWasmJSCall deferred_call;
  Address deferred_call_next_offset = bytecode_offset;

  for (int step = 0; step < kMaxInterpreterSteps; ++step) {
#ifdef __wasi__
    if (g_wasm_request_duplex_getter_returned && step > 0 &&
        (step % 10000) == 0 && g_wasm_post_getter_heartbeat_count < 128) {
      ++g_wasm_post_getter_heartbeat_count;
      std::unique_ptr<char[]> heartbeat_name = shared->DebugNameCStr();
      std::fprintf(stderr,
                   "WASM32_POST_GETTER_PC #%d start=%d name=%s step=%d pc=%u len=%d\n",
                   g_wasm_post_getter_heartbeat_count,
                   shared->StartPosition(), heartbeat_name.get(), step,
                   static_cast<unsigned>(current_offset - bytecode_offset),
                   bytecode->length());
      std::fflush(stderr);
    }
#endif
    if (deferred_call.pending) {
      if (deferred_call.diagnostic) {
        PrintF("WASM32_CALL_DEFERRED_RUN source=%d callable=0x%x\n",
               deferred_call.source_position,
               static_cast<unsigned>(deferred_call.callable));
      }
      Address deferred_result = RunPendingWasmJSCall(isolate, deferred_call);
      if (deferred_call.diagnostic) {
        PrintF("WASM32_CALL_DEFERRED_RETURN source=%d result=0x%x exception=%d\n",
               deferred_call.source_position,
               static_cast<unsigned>(deferred_result),
               isolate->has_exception() ? 1 : 0);
      }
      deferred_call.pending = false;
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &deferred_result);
      current_offset = deferred_call_next_offset;
    }

    // Bound temporary handles to one bytecode. Recursive WasmJSEntry calls
    // run at the top of the next iteration, after this scope is destroyed.
    // Interpreter register values remain protected by the registered wasm32
    // root storage.
    HandleScope iteration_scope(isolate);
    Address rooted_function =
        g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
            StandardFrameConstants::kFunctionOffset)];
    Address rooted_bytecode =
        g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
            InterpreterFrameConstants::kBytecodeArrayFromFp)];
    if (!IsSafeTaggedHandleValue(rooted_function) ||
        !IsJSFunction(Tagged<Object>(rooted_function)) ||
        !IsSafeTaggedHandleValue(rooted_bytecode) ||
        !IsBytecodeArray(Tagged<Object>(rooted_bytecode))) {
      g_wasm_regs[SlotFor(kReturnRegister0)] =
          ReadOnlyRoots(isolate).exception().ptr();
      return;
    }
    function = Cast<JSFunction>(Tagged<Object>(rooted_function));
    bytecode = Cast<BytecodeArray>(Tagged<Object>(rooted_bytecode));
    shared = Wasm32JSFunctionShared(function);

    int bytecode_index = static_cast<int>(current_offset - bytecode_offset);
    if (bytecode_index < 0 || bytecode_index >= bytecode->length()) {
      PrintF("WasmInterpreterEntryTrampoline: bytecode offset OOB index=%d "
             "length=%d raw_offset=0x%x\n",
             bytecode_index, bytecode->length(),
             static_cast<unsigned>(current_offset));
      g_wasm_regs[SlotFor(kReturnRegister0)] =
          ReadOnlyRoots(isolate).exception().ptr();
      return;
    }

    uint8_t opcode = bytecode->get(bytecode_index);
    if (opcode >
        interpreter::Bytecodes::ToByte(interpreter::Bytecode::kLast)) {
      g_wasm_regs[SlotFor(kReturnRegister0)] =
          ReadOnlyRoots(isolate).exception().ptr();
      return;
    }
    interpreter::Bytecode bytecode_enum =
        interpreter::Bytecodes::FromByte(opcode);
    int decoded_source_position = bytecode->SourcePosition(bytecode_index);
    if (decoded_source_position >= 7531400 &&
        decoded_source_position <= 7531800) {
      PrintF("WASM32_SOURCE_TRACE source=%d index=%d opcode=%s scale=%d\n",
             decoded_source_position, bytecode_index,
             interpreter::Bytecodes::ToString(bytecode_enum),
             static_cast<int>(operand_scale));
    }
    if (bytecode_enum == interpreter::Bytecode::kCallUndefinedReceiver0) {
      static int decoded_call0_trace_count = 0;
      if (decoded_call0_trace_count < 128) {
        ++decoded_call0_trace_count;
        PrintF("WASM32_DECODE_CALL0 #%d index=%d scale=%d\n",
               decoded_call0_trace_count, bytecode_index,
               static_cast<int>(operand_scale));
      }
    }
    int bytecode_size =
        interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
    if (bytecode_size <= 0 || bytecode_size > bytecode->length() - bytecode_index) {
      g_wasm_regs[SlotFor(kReturnRegister0)] =
          ReadOnlyRoots(isolate).exception().ptr();
      return;
    }
    int tail_slot = step % kMaxInterpreterTailTrace;
    tail_step[tail_slot] = step;
    tail_index[tail_slot] = bytecode_index;
    tail_opcode[tail_slot] = opcode;
    tail_bytecode[tail_slot] = bytecode_enum;
    tail_accumulator[tail_slot] =
        g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
    int operand_count_for_tail =
        interpreter::Bytecodes::NumberOfOperands(bytecode_enum);
    tail_operand0[tail_slot] =
        operand_count_for_tail > 0
            ? static_cast<int>(ReadBytecodeUnsignedOperand(
                  bytecode, bytecode_index, bytecode_enum, 0, operand_scale))
            : 0;
    tail_operand1[tail_slot] =
        operand_count_for_tail > 1
            ? static_cast<int>(ReadBytecodeUnsignedOperand(
                  bytecode, bytecode_index, bytecode_enum, 1, operand_scale))
            : 0;
    tail_operand2[tail_slot] =
        operand_count_for_tail > 2
            ? static_cast<int>(ReadBytecodeUnsignedOperand(
                  bytecode, bytecode_index, bytecode_enum, 2, operand_scale))
            : 0;
#ifdef __wasi__
    if (shared->StartPosition() == 0 && bytecode->length() > 200000 &&
        bytecode_index >= 135430 && bytecode_index <= 135550 &&
        g_wasm_main_await_window_count < 160) {
      ++g_wasm_main_await_window_count;
      std::fprintf(stderr,
                   "WASM32_MAIN_AWAIT_WINDOW #%d pc=%d opcode=%s "
                   "op0=%d op1=%d op2=%d acc=0x%x source=%d\n",
                   g_wasm_main_await_window_count, bytecode_index,
                   interpreter::Bytecodes::ToString(bytecode_enum),
                   tail_operand0[tail_slot], tail_operand1[tail_slot],
                   tail_operand2[tail_slot],
                   static_cast<unsigned>(tail_accumulator[tail_slot]),
                   decoded_source_position);
      std::fflush(stderr);
    }
    if (g_wasm_request_duplex_getter_returned &&
        shared->StartPosition() == 483118 &&
        bytecode_index >= 2100 && bytecode_index <= 2280 &&
        g_wasm_loop_window_count < 192) {
      ++g_wasm_loop_window_count;
      std::fprintf(stderr,
                   "WASM32_LOOP_WINDOW #%d step=%d pc=%d opcode=%s "
                   "op0=%d op1=%d op2=%d acc=0x%x\n",
                   g_wasm_loop_window_count, step, bytecode_index,
                   interpreter::Bytecodes::ToString(bytecode_enum),
                   tail_operand0[tail_slot], tail_operand1[tail_slot],
                   tail_operand2[tail_slot],
                   static_cast<unsigned>(tail_accumulator[tail_slot]));
      std::fflush(stderr);
    }
#endif
    if (interpreter::Bytecodes::IsPrefixScalingBytecode(bytecode_enum)) {
      operand_scale =
          interpreter::Bytecodes::PrefixBytecodeToOperandScale(bytecode_enum);
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum,
                                      interpreter::OperandScale::kSingle);
      continue;
    }
    if (bytecode_enum == interpreter::Bytecode::kDebugger) {
      PrintF("WASM32_DEBUGGER_TRACE index=%d\n", bytecode_index);
      g_trace_after_collection_fallback_steps = 64;
    }
    if (bytecode_enum == interpreter::Bytecode::kLdaSmi &&
        ReadBytecodeSignedOperand(bytecode, bytecode_index, bytecode_enum, 0,
                                  operand_scale) == 123456789) {
      PrintF("WASM32_SMI_TRACE index=%d\n", bytecode_index);
      g_trace_after_collection_fallback_steps = 64;
    }

    Builtin handler_builtin = Builtin::kIllegal;
    Address entry =
        WasmBytecodeHandlerEntry(isolate, opcode, operand_scale,
                                 &handler_builtin);
    bool trace_collection_followup =
        g_trace_after_collection_fallback_steps > 0;
    if (trace_collection_followup) {
      --g_trace_after_collection_fallback_steps;
    }
    bool trace_fs_utils_ownkeys =
        kTraceWasmFallbackDetails && bytecode->length() == 2762 &&
        shared->StartPosition() == 0 && shared->EndPosition() == 27082 &&
        bytecode_index >= 1230 && bytecode_index <= 1305;
    bool trace_copy_prototype_loop =
        kTraceWasmFallbackDetails &&
        FunctionMatchesPerContextPrimordialsCopyPrototype(shared) &&
        bytecode_index >= 0 && bytecode_index <= 65 && step < 260;
    bool trace_claude_call_pc = bytecode_index >= 136200 &&
                                bytecode_index <= 136270;
    bool should_log_step =
        trace_collection_followup || trace_fs_utils_ownkeys ||
        trace_copy_prototype_loop || trace_claude_call_pc ||
        (trace_entry_steps && (step < 260 || (step % 100000) == 0)) ||
#ifdef __wasi__
        (trace_per_context_primordials &&
         (step < 260 || (step % 100000) == 0)) ||
#endif
        (kTraceWasmInterpreterSteps &&
         (step < 16 || bytecode_enum == interpreter::Bytecode::kCreateClosure ||
        bytecode_enum == interpreter::Bytecode::kLdaGlobal ||
        bytecode_enum == interpreter::Bytecode::kLdaGlobalInsideTypeof ||
        bytecode_enum == interpreter::Bytecode::kGetNamedProperty ||
        bytecode_enum == interpreter::Bytecode::kGetKeyedProperty ||
        bytecode_enum == interpreter::Bytecode::kToName ||
        bytecode_enum == interpreter::Bytecode::kAdd ||
        bytecode_enum ==
            interpreter::Bytecode::kAdd_LhsIsStringConstant_Internalize ||
        bytecode_enum == interpreter::Bytecode::kAddSmi ||
        bytecode_enum == interpreter::Bytecode::kBitwiseOrSmi ||
        bytecode_enum == interpreter::Bytecode::kBitwiseXorSmi ||
        bytecode_enum == interpreter::Bytecode::kBitwiseAndSmi ||
        bytecode_enum == interpreter::Bytecode::kShiftLeftSmi ||
        bytecode_enum == interpreter::Bytecode::kShiftRightSmi ||
        bytecode_enum == interpreter::Bytecode::kShiftRightLogicalSmi ||
        bytecode_enum == interpreter::Bytecode::kSetNamedProperty ||
        bytecode_enum == interpreter::Bytecode::kDefineNamedOwnProperty ||
        bytecode_enum == interpreter::Bytecode::kDefineKeyedOwnProperty ||
        bytecode_enum == interpreter::Bytecode::kCreateRegExpLiteral ||
        bytecode_enum == interpreter::Bytecode::kCreateArrayLiteral ||
        bytecode_enum == interpreter::Bytecode::kStaInArrayLiteral ||
        bytecode_enum == interpreter::Bytecode::kCreateObjectLiteral ||
        bytecode_enum ==
            interpreter::Bytecode::kDefineKeyedOwnPropertyInLiteral ||
        bytecode_enum == interpreter::Bytecode::kGetIterator ||
        interpreter::Bytecodes::IsJump(bytecode_enum) ||
        interpreter::Bytecodes::IsSwitch(bytecode_enum) ||
        bytecode_enum == interpreter::Bytecode::kCallAnyReceiver ||
        bytecode_enum == interpreter::Bytecode::kCallProperty ||
        bytecode_enum == interpreter::Bytecode::kCallProperty0 ||
        bytecode_enum == interpreter::Bytecode::kCallProperty1 ||
        bytecode_enum == interpreter::Bytecode::kCallProperty2 ||
        bytecode_enum == interpreter::Bytecode::kCallUndefinedReceiver ||
        bytecode_enum == interpreter::Bytecode::kCallUndefinedReceiver0 ||
        bytecode_enum == interpreter::Bytecode::kCallUndefinedReceiver1 ||
        bytecode_enum == interpreter::Bytecode::kCallUndefinedReceiver2 ||
        bytecode_enum == interpreter::Bytecode::kTestEqual ||
        bytecode_enum == interpreter::Bytecode::kTestEqualStrict ||
        bytecode_enum == interpreter::Bytecode::kTestLessThan ||
        bytecode_enum == interpreter::Bytecode::kTestGreaterThan ||
        bytecode_enum == interpreter::Bytecode::kTestLessThanOrEqual ||
        bytecode_enum == interpreter::Bytecode::kTestGreaterThanOrEqual ||
        bytecode_enum == interpreter::Bytecode::kTestReferenceEqual ||
        bytecode_enum == interpreter::Bytecode::kTestNull ||
        bytecode_enum == interpreter::Bytecode::kTestUndefined ||
        bytecode_enum == interpreter::Bytecode::kTestInstanceOf ||
         bytecode_enum == interpreter::Bytecode::kTestTypeOf ||
         (step >= 420 && step <= 440)));
    if (should_log_step) {
      PrintF("WasmInterpreterEntryTrampoline: step=%d bytecode=0x%x(%s) "
             "offset=0x%x index=%d size=%d scale=%d handler_builtin=%d "
             "handler=0x%x frame_fp=0x%x regs=%d params=%d\n",
             step,
             static_cast<unsigned>(opcode),
             interpreter::Bytecodes::ToString(bytecode_enum),
             static_cast<unsigned>(current_offset),
             bytecode_index,
             interpreter::Bytecodes::Size(bytecode_enum, operand_scale),
             static_cast<int>(operand_scale),
             static_cast<int>(handler_builtin), static_cast<unsigned>(entry),
             static_cast<unsigned>(g_wasm_current_frame_pointer),
             register_count, parameter_count);
      int operand_count = interpreter::Bytecodes::NumberOfOperands(bytecode_enum);
      PrintF("  operands:");
      for (int operand_index = 0; operand_index < operand_count; ++operand_index) {
        PrintF(" #%d=0x%x", operand_index,
               ReadBytecodeUnsignedOperand(bytecode, bytecode_index,
                                           bytecode_enum, operand_index,
                                           operand_scale));
      }
      PrintF(" raw:");
      for (int raw_index = 0;
           raw_index < interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
           ++raw_index) {
        PrintF(" %02x", bytecode->get(bytecode_index + raw_index));
      }
      PrintF("\n");
      PrintF("  frame:");
      DumpRuntimeArg("function", 0,
                     g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
                         StandardFrameConstants::kFunctionOffset)]);
      DumpRuntimeArg("context", 0,
                     g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
                         StandardFrameConstants::kContextOffset)]);
      DumpRuntimeArg("current_context", 0,
                     g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
                         interpreter::Register::current_context().ToOperand() *
                         kSystemPointerSize)]);
      DumpRuntimeArg("bytecode_array", 0,
                     g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
                         InterpreterFrameConstants::kBytecodeArrayFromFp)]);
      DumpRuntimeArg("bytecode_offset", 0,
                     g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
                         InterpreterFrameConstants::kBytecodeOffsetFromFp)]);
      DumpRuntimeArg("feedback", 0,
                     g_wasm_interpreter_frame[InterpreterFrameSlotForOffset(
                         InterpreterFrameConstants::kFeedbackVectorFromFp)]);
      int logged_parameter_count = parameter_count < 5 ? parameter_count : 5;
      for (int parameter_index = 0; parameter_index < logged_parameter_count;
           ++parameter_index) {
        DumpRuntimeArg(
            "p", parameter_index,
            ReadInterpreterRegister(
                interpreter::Register::FromParameterIndex(parameter_index)));
      }
      int logged_register_count =
          trace_fs_utils_ownkeys || trace_copy_prototype_loop ||
                  (step >= 420 && step <= 440)
              ? register_count
              : (register_count < 8 ? register_count : 8);
#ifdef __wasi__
      if (trace_per_context_primordials) {
        logged_register_count = register_count;
      }
#endif
      for (int register_index = 0; register_index < logged_register_count;
           ++register_index) {
        DumpRuntimeArg("l", register_index,
                       ReadInterpreterRegister(
                           interpreter::Register(register_index)));
      }
      PrintF(" r0=0x%x r10=0x%x r11=0x%x\n",
             static_cast<unsigned>(
                 g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]),
             static_cast<unsigned>(
                 g_wasm_regs[SlotFor(kInterpreterBytecodeOffsetRegister)]),
             static_cast<unsigned>(
                 g_wasm_regs[SlotFor(kInterpreterBytecodeArrayRegister)]));
    }
    Address fallback_result = kNullAddress;
    Address fallback_offset = kNullAddress;
    WasmGCStateScope step_gc_state(isolate);
    if (TryRunStarBytecode(bytecode, bytecode_index, bytecode_enum,
                           operand_scale, &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback %s result=0x%x\n",
               interpreter::Bytecodes::ToString(bytecode_enum),
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunMovBytecode(bytecode, bytecode_index, bytecode_enum,
                          operand_scale, &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback Mov result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunContextStackBytecode(bytecode, bytecode_index, bytecode_enum,
                                   operand_scale, &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback %s result=0x%x\n",
               interpreter::Bytecodes::ToString(bytecode_enum),
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunModuleVariableBytecode(isolate, bytecode, bytecode_index,
                                     bytecode_enum, operand_scale,
                                     &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunCatchContextBytecode(isolate, bytecode, bytecode_index,
                                   bytecode_enum, operand_scale,
                                   &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunLdarBytecode(bytecode, bytecode_index, bytecode_enum,
                           operand_scale, &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback Ldar result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunLdaConstantBytecode(isolate, bytecode, bytecode_index,
                                  bytecode_enum, operand_scale,
                                  &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback %s result=0x%x\n",
               interpreter::Bytecodes::ToString(bytecode_enum),
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunThrowReferenceErrorIfHoleBytecode(
            isolate, bytecode, bytecode_index, bytecode_enum, operand_scale,
            &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback ThrowReferenceErrorIfHole result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunThrowSuperBytecode(isolate, bytecode_enum, &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback %s result=0x%x\n",
               interpreter::Bytecodes::ToString(bytecode_enum),
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunThrowIfNotSuperConstructorBytecode(
            isolate, bytecode, bytecode_index, bytecode_enum, operand_scale,
            &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback ThrowIfNotSuperConstructor result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    Address thrown_value =
        g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
    if (TryRunThrowControlBytecode(isolate, bytecode, bytecode_index,
                                   bytecode_enum, operand_scale,
                                   &fallback_result)) {
      if (fallback_result == ReadOnlyRoots(isolate).exception().ptr()) {
        HandlerTable handler_table(bytecode);
        int handler_index =
            handler_table.LookupHandlerIndexForRange(bytecode_index);
        if (handler_index != HandlerTable::kNoHandlerFound) {
          int context_register = handler_table.GetRangeData(handler_index);
          Address handler_context = ReadInterpreterRegister(
              interpreter::Register(context_register));
          if (IsSafeTaggedHandleValue(handler_context) &&
              IsContext(Tagged<Object>(handler_context))) {
            PublishCurrentInterpreterContext(handler_context);
            isolate->clear_exception();
            int accumulator_slot = SlotFor(kInterpreterAccumulatorRegister);
            g_wasm_regs[accumulator_slot] = thrown_value;
            MirrorWasmGCRegSlotForWrite(accumulator_slot, thrown_value);
            current_offset =
                bytecode_offset + handler_table.GetRangeHandler(handler_index);
            operand_scale = interpreter::OperandScale::kSingle;
            continue;
          }
        }
      }
      int accumulator_slot = SlotFor(kInterpreterAccumulatorRegister);
      g_wasm_regs[accumulator_slot] = fallback_result;
      MirrorWasmGCRegSlotForWrite(accumulator_slot, fallback_result);
      int return_slot = SlotFor(kReturnRegister0);
      g_wasm_regs[return_slot] = fallback_result;
      MirrorWasmGCRegSlotForWrite(return_slot, fallback_result);
      if (should_log_step) {
        PrintF("  fallback %s result=0x%x\n",
               interpreter::Bytecodes::ToString(bytecode_enum),
               static_cast<unsigned>(fallback_result));
      }
      return;
    }
    if (TryRunCompareBytecode(isolate, bytecode, bytecode_index,
                              bytecode_enum, operand_scale,
                              &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback %s result=0x%x\n",
               interpreter::Bytecodes::ToString(bytecode_enum),
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunReferenceTestBytecode(isolate, bytecode, bytecode_index,
                                    bytecode_enum, operand_scale,
                                    &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback %s result=0x%x\n",
               interpreter::Bytecodes::ToString(bytecode_enum),
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunTestTypeOfBytecode(isolate, bytecode, bytecode_index,
                                 bytecode_enum, operand_scale,
                                 &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback TestTypeOf result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunTypeOfBytecode(isolate, bytecode_enum, &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunBooleanConversionBytecode(isolate, bytecode_enum,
                                        &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback %s result=0x%x\n",
               interpreter::Bytecodes::ToString(bytecode_enum),
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (bytecode_enum == interpreter::Bytecode::kSwitchOnGeneratorState) {
      int32_t generator_operand = ReadBytecodeSignedOperand(
          bytecode, bytecode_index, bytecode_enum, 0, operand_scale);
      Address generator_address = SafeTaggedOrUndefined(
          isolate, ReadInterpreterRegister(
                       interpreter::Register::FromOperand(generator_operand)));
      if (generator_address == ReadOnlyRoots(isolate).undefined_value().ptr()) {
        current_offset +=
            interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      } else if (IsJSGeneratorObject(Tagged<Object>(generator_address)) ||
                 IsJSAsyncFunctionObject(Tagged<Object>(generator_address)) ||
                 IsJSAsyncGeneratorObject(Tagged<Object>(generator_address))) {
        Tagged<JSGeneratorObject> generator =
            Cast<JSGeneratorObject>(Tagged<Object>(generator_address));
        int state = generator->continuation();
        uint32_t table_start = ReadBytecodeUnsignedOperand(
            bytecode, bytecode_index, bytecode_enum, 1, operand_scale);
        uint32_t table_length = ReadBytecodeUnsignedOperand(
            bytecode, bytecode_index, bytecode_enum, 2, operand_scale);
        if (state < 0 || static_cast<uint32_t>(state) >= table_length) {
          g_wasm_regs[SlotFor(kReturnRegister0)] =
              ReadOnlyRoots(isolate).exception().ptr();
          return;
        }
        Tagged<Object> jump_entry =
            bytecode->constant_pool()->get(table_start + state);
        if (!IsSmi(jump_entry)) {
          g_wasm_regs[SlotFor(kReturnRegister0)] =
              ReadOnlyRoots(isolate).exception().ptr();
          return;
        }
        generator->set_continuation(JSGeneratorObject::kGeneratorExecuting);
        isolate->set_context(generator->context());
        StoreInterpreterFrameOffset(StandardFrameConstants::kContextOffset,
                                    generator->context().ptr());
        g_wasm_regs[SlotFor(kContextRegister)] = generator->context().ptr();
        current_offset += Smi::ToInt(jump_entry);
      } else {
        g_wasm_regs[SlotFor(kReturnRegister0)] =
            ReadOnlyRoots(isolate).exception().ptr();
        return;
      }
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (bytecode_enum == interpreter::Bytecode::kSwitchOnSmiNoFeedback) {
      Address accumulator =
          g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
      uint32_t table_start = ReadBytecodeUnsignedOperand(
          bytecode, bytecode_index, bytecode_enum, 0, operand_scale);
      uint32_t table_length = ReadBytecodeUnsignedOperand(
          bytecode, bytecode_index, bytecode_enum, 1, operand_scale);
      int32_t case_value_base = ReadBytecodeSignedOperand(
          bytecode, bytecode_index, bytecode_enum, 2, operand_scale);
      bool jumped = false;
      if (IsSmi(Tagged<Object>(accumulator))) {
        int64_t case_index =
            static_cast<int64_t>(Smi::ToInt(Tagged<Smi>(accumulator))) -
            case_value_base;
        if (case_index >= 0 &&
            static_cast<uint64_t>(case_index) < table_length &&
            table_start + static_cast<uint32_t>(case_index) <
                static_cast<uint32_t>(bytecode->constant_pool()->length())) {
          Tagged<Object> jump_entry = bytecode->constant_pool()->get(
              table_start + static_cast<uint32_t>(case_index));
          if (IsSmi(jump_entry)) {
            current_offset += Smi::ToInt(jump_entry);
            jumped = true;
          }
        }
      }
      if (!jumped) {
        current_offset +=
            interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      }
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (bytecode_enum == interpreter::Bytecode::kSuspendGenerator) {
      int32_t generator_operand = ReadBytecodeSignedOperand(
          bytecode, bytecode_index, bytecode_enum, 0, operand_scale);
      int32_t first_register_operand = ReadBytecodeSignedOperand(
          bytecode, bytecode_index, bytecode_enum, 1, operand_scale);
      uint32_t register_count_to_store = ReadBytecodeUnsignedOperand(
          bytecode, bytecode_index, bytecode_enum, 2, operand_scale);
      uint32_t suspend_id = ReadBytecodeUnsignedOperand(
          bytecode, bytecode_index, bytecode_enum, 3, operand_scale);
#ifdef __wasi__
      if (shared->StartPosition() == 0 && bytecode->length() > 200000) {
        Address suspend_accumulator = g_wasm_regs[
            SlotFor(kInterpreterAccumulatorRegister)];
        int promise_state = -1;
        if (IsJSPromise(Tagged<Object>(suspend_accumulator))) {
          promise_state = static_cast<int>(
              Cast<JSPromise>(Tagged<Object>(suspend_accumulator))->status());
        }
        std::fprintf(stderr,
                     "WASM32_MAIN_SUSPEND pc=%d source=%d suspend_id=%u "
                     "acc=0x%x promise_state=%d\n",
                     bytecode_index, bytecode->SourcePosition(bytecode_index),
                     suspend_id,
                     static_cast<unsigned>(suspend_accumulator), promise_state);
        std::fflush(stderr);
      }
#endif
      Address generator_address = SafeTaggedOrUndefined(
          isolate, ReadInterpreterRegister(
                       interpreter::Register::FromOperand(generator_operand)));
      if (!IsJSGeneratorObject(Tagged<Object>(generator_address)) &&
          !IsJSAsyncFunctionObject(Tagged<Object>(generator_address)) &&
          !IsJSAsyncGeneratorObject(Tagged<Object>(generator_address))) {
        g_wasm_regs[SlotFor(kReturnRegister0)] =
            ReadOnlyRoots(isolate).exception().ptr();
        return;
      }
      Tagged<JSGeneratorObject> generator =
          Cast<JSGeneratorObject>(Tagged<Object>(generator_address));
      Tagged<FixedArray> stored = generator->parameters_and_registers();
      int stored_index = 0;
      int formal_count = bytecode->parameter_count_without_receiver();
      for (int i = 0; i < formal_count && stored_index < stored->length(); ++i) {
        Address value = SafeTaggedOrUndefined(
            isolate, ReadInterpreterRegister(
                         interpreter::Register::FromParameterIndex(i + 1)));
        stored->set(stored_index++, Tagged<Object>(value));
      }
      for (uint32_t i = 0;
           i < register_count_to_store && stored_index < stored->length(); ++i) {
        Address value = SafeTaggedOrUndefined(
            isolate, ReadInterpreterRegister(
                         RegisterFromListOperand(first_register_operand, i)));
        stored->set(stored_index++, Tagged<Object>(value));
      }
      Address suspend_context_address = CurrentInterpreterContext();
      if (!IsSafeTaggedHandleValue(suspend_context_address) ||
          !IsContext(Tagged<Object>(suspend_context_address))) {
        g_wasm_regs[SlotFor(kReturnRegister0)] =
            ReadOnlyRoots(isolate).exception().ptr();
        return;
      }
      Tagged<Context> suspend_context =
          Cast<Context>(Tagged<Object>(suspend_context_address));
      generator->set_context(suspend_context);
      generator->set_continuation(static_cast<int>(suspend_id));
      generator->set_input_or_debug_pos(Smi::FromInt(bytecode_index));

      Address result =
          g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)];
      g_wasm_regs[SlotFor(kReturnRegister0)] = result;
      return;
    }
    if (bytecode_enum == interpreter::Bytecode::kResumeGenerator) {
      int32_t generator_operand = ReadBytecodeSignedOperand(
          bytecode, bytecode_index, bytecode_enum, 0, operand_scale);
      int32_t first_register_operand = ReadBytecodeSignedOperand(
          bytecode, bytecode_index, bytecode_enum, 1, operand_scale);
      uint32_t register_count_to_restore = ReadBytecodeUnsignedOperand(
          bytecode, bytecode_index, bytecode_enum, 2, operand_scale);
      Address generator_address = SafeTaggedOrUndefined(
          isolate, ReadInterpreterRegister(
                       interpreter::Register::FromOperand(generator_operand)));
      if (!IsJSGeneratorObject(Tagged<Object>(generator_address)) &&
          !IsJSAsyncFunctionObject(Tagged<Object>(generator_address)) &&
          !IsJSAsyncGeneratorObject(Tagged<Object>(generator_address))) {
        g_wasm_regs[SlotFor(kReturnRegister0)] =
            ReadOnlyRoots(isolate).exception().ptr();
        return;
      }
      Tagged<JSGeneratorObject> generator =
          Cast<JSGeneratorObject>(Tagged<Object>(generator_address));
      Tagged<FixedArray> stored = generator->parameters_and_registers();
      StoreInterpreterRegister(interpreter::Register::FromParameterIndex(0),
                               generator->receiver().ptr());
      int formal_count = bytecode->parameter_count_without_receiver();
      int stored_index = 0;
      for (int i = 0; i < formal_count && stored_index < stored->length(); ++i) {
        Address value =
            SafeTaggedOrUndefined(isolate, stored->get(stored_index).ptr());
        StoreInterpreterRegister(
            interpreter::Register::FromParameterIndex(i + 1), value);
        stored->set(stored_index++, ReadOnlyRoots(isolate).stale_register());
      }
      for (uint32_t i = 0;
           i < register_count_to_restore && stored_index < stored->length(); ++i) {
        Address value =
            SafeTaggedOrUndefined(isolate, stored->get(stored_index).ptr());
        StoreInterpreterRegister(
            RegisterFromListOperand(first_register_operand, i), value);
        stored->set(stored_index++, ReadOnlyRoots(isolate).stale_register());
      }
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)] =
          generator->input_or_debug_pos().ptr();
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunJumpBytecode(isolate, bytecode, bytecode_index, bytecode_enum,
                           operand_scale, current_offset, &fallback_offset)) {
      if (should_log_step) {
        PrintF("  fallback %s next=0x%x\n",
               interpreter::Bytecodes::ToString(bytecode_enum),
               static_cast<unsigned>(fallback_offset));
      }
      current_offset = fallback_offset;
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunForInBytecode(isolate, bytecode, bytecode_index, bytecode_enum,
                            operand_scale, &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunRuntimeCallBytecode(isolate, bytecode, bytecode_index,
                                  bytecode_enum, operand_scale,
                                  &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback %s result=0x%x\n",
               interpreter::Bytecodes::ToString(bytecode_enum),
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    PendingWasmJSCall pending_call;
    if (TryRunCallBytecode(isolate, bytecode, bytecode_index, bytecode_enum,
                           operand_scale, function, &fallback_result,
                           &pending_call)) {
      if (pending_call.pending) {
        deferred_call = pending_call;
        deferred_call_next_offset = current_offset + bytecode_size;
        operand_scale = interpreter::OperandScale::kSingle;
        continue;
      }
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback %s result=0x%x\n",
               interpreter::Bytecodes::ToString(bytecode_enum),
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunConstructBytecode(isolate, bytecode, bytecode_index,
                                bytecode_enum, operand_scale,
                                &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback Construct result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunFindNonDefaultConstructorOrConstructBytecode(
            isolate, bytecode, bytecode_index, bytecode_enum, operand_scale,
            &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback FindNonDefaultConstructorOrConstruct result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunCreateFunctionContextBytecode(isolate, bytecode, bytecode_index,
                                            bytecode_enum, operand_scale,
                                            &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback CreateFunctionContext result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunCreateBlockContextBytecode(isolate, bytecode, bytecode_index,
                                         bytecode_enum, operand_scale,
                                         &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback CreateBlockContext result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunCreateClosureBytecode(isolate, bytecode, bytecode_index,
                                    bytecode_enum, operand_scale,
                                    &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback CreateClosure result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunLdaContextSlotBytecode(isolate, bytecode, bytecode_index,
                                     bytecode_enum, operand_scale,
                                     &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback %s result=0x%x\n",
               interpreter::Bytecodes::ToString(bytecode_enum),
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunStaContextSlotBytecode(isolate, bytecode, bytecode_index,
                                     bytecode_enum, operand_scale,
                                     &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback %s result=0x%x\n",
               interpreter::Bytecodes::ToString(bytecode_enum),
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunLdaGlobalBytecode(isolate, bytecode, bytecode_index,
                                bytecode_enum, operand_scale,
                                &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback LdaGlobal result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunGetNamedPropertyBytecode(isolate, bytecode, bytecode_index,
                                       bytecode_enum, operand_scale,
                                       &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback GetNamedProperty result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunGetKeyedPropertyBytecode(isolate, bytecode, bytecode_index,
                                       bytecode_enum, operand_scale,
                                       &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback GetKeyedProperty result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunToStringBytecode(isolate, bytecode_enum, &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback ToString result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunToNumberOrNumericBytecode(isolate, bytecode_enum,
                                        &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback %s result=0x%x\n",
               interpreter::Bytecodes::ToString(bytecode_enum),
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunToNameBytecode(isolate, bytecode_enum, &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback ToName result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunTestInBytecode(isolate, bytecode, bytecode_index, bytecode_enum,
                             operand_scale, &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback TestIn result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunTestInstanceOfBytecode(isolate, bytecode, bytecode_index,
                                     bytecode_enum, operand_scale,
                                     &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback TestInstanceOf result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunAddBytecode(isolate, bytecode, bytecode_index, bytecode_enum,
                          operand_scale, &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback %s result=0x%x\n",
               interpreter::Bytecodes::ToString(bytecode_enum),
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunIncDecBytecode(isolate, bytecode_enum, &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback %s result=0x%x\n",
               interpreter::Bytecodes::ToString(bytecode_enum),
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunBitwiseSmiBytecode(isolate, bytecode, bytecode_index,
                                 bytecode_enum, operand_scale,
                                 &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback %s result=0x%x\n",
               interpreter::Bytecodes::ToString(bytecode_enum),
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunBitwiseNotBytecode(isolate, bytecode_enum, &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunSetNamedPropertyBytecode(isolate, bytecode, bytecode_index,
                                       bytecode_enum, operand_scale,
                                       &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback %s result=0x%x\n",
               interpreter::Bytecodes::ToString(bytecode_enum),
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunSetKeyedPropertyBytecode(isolate, bytecode, bytecode_index,
                                       bytecode_enum, operand_scale,
                                       &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback %s result=0x%x\n",
               interpreter::Bytecodes::ToString(bytecode_enum),
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunDefineKeyedOwnPropertyBytecode(isolate, bytecode, bytecode_index,
                                             bytecode_enum, operand_scale,
                                             &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback %s result=0x%x\n",
               interpreter::Bytecodes::ToString(bytecode_enum),
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunDeletePropertyBytecode(isolate, bytecode, bytecode_index,
                                     bytecode_enum, operand_scale,
                                     &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback %s result=0x%x\n",
               interpreter::Bytecodes::ToString(bytecode_enum),
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunCloneObjectBytecode(isolate, bytecode, bytecode_index,
                                  bytecode_enum, operand_scale,
                                  &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback CloneObject result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunCreateRegExpLiteralBytecode(isolate, bytecode, bytecode_index,
                                          bytecode_enum, operand_scale,
                                          &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback CreateRegExpLiteral result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunCreateArrayLiteralBytecode(isolate, bytecode, bytecode_index,
                                         bytecode_enum, operand_scale,
                                         &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback CreateArrayLiteral result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunCreateRestParameterBytecode(isolate, bytecode_enum,
                                          &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback CreateRestParameter result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunCreateEmptyArrayLiteralBytecode(isolate, bytecode_enum,
                                              &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback CreateEmptyArrayLiteral result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunCreateArrayFromIterableBytecode(isolate, bytecode_enum,
                                              &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunStaInArrayLiteralBytecode(isolate, bytecode, bytecode_index,
                                        bytecode_enum, operand_scale,
                                        &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback StaInArrayLiteral result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunCreateObjectLiteralBytecode(isolate, bytecode, bytecode_index,
                                          bytecode_enum, operand_scale,
                                          &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback CreateObjectLiteral result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunCreateEmptyObjectLiteralBytecode(isolate, bytecode_enum,
                                               &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback CreateEmptyObjectLiteral result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunDefineKeyedOwnPropertyInLiteralBytecode(
            isolate, bytecode, bytecode_index, bytecode_enum, operand_scale,
            &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback DefineKeyedOwnPropertyInLiteral result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }
    if (TryRunGetIteratorBytecode(isolate, bytecode, bytecode_index,
                                  bytecode_enum, operand_scale,
                                  &fallback_result)) {
      PublishWasmInterpreterFallbackResult(isolate, "bytecode fallback",
                                           &fallback_result);
      if (should_log_step) {
        PrintF("  fallback GetIterator result=0x%x\n",
               static_cast<unsigned>(fallback_result));
      }
      current_offset +=
          interpreter::Bytecodes::Size(bytecode_enum, operand_scale);
      operand_scale = interpreter::OperandScale::kSingle;
      continue;
    }

    if (interpreter::Bytecodes::Returns(bytecode_enum)) {
      Address result = NormalizeWasmInterpreterResult(
          isolate, "return bytecode",
          g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]);
#ifdef __wasi__
      if (shared->StartPosition() == 0 && bytecode->length() > 200000) {
        int async_promise_state = -1;
        Address async_object_address = new_target;
        if (IsJSAsyncFunctionObject(Tagged<Object>(async_object_address))) {
          async_promise_state = static_cast<int>(
              Cast<JSAsyncFunctionObject>(Tagged<Object>(async_object_address))
                  ->promise()
                  ->status());
        }
        std::fprintf(stderr,
                     "WASM32_MAIN_RETURN pc=%d opcode=%s result=0x%x "
                     "async_object=0x%x promise_state=%d\n",
                     bytecode_index,
                     interpreter::Bytecodes::ToString(bytecode_enum),
                     static_cast<unsigned>(result),
                     static_cast<unsigned>(async_object_address),
                     async_promise_state);
        if (IsJSAsyncFunctionObject(Tagged<Object>(async_object_address))) {
          Tagged<JSPromise> async_promise =
              Cast<JSAsyncFunctionObject>(Tagged<Object>(async_object_address))
                  ->promise();
          if (async_promise_state != 0) {
            Tagged<Object> promise_result = async_promise->result();
            std::fprintf(stderr,
                         "WASM32_MAIN_PROMISE_RAW promise=0x%x result=0x%x "
                         "undefined=0x%x "
                         "exception=0x%x hole=0x%x null=0x%x true=0x%x "
                         "false=0x%x\n",
                         static_cast<unsigned>(async_promise.ptr()),
                         static_cast<unsigned>(promise_result.ptr()),
                         static_cast<unsigned>(ReadOnlyRoots(isolate)
                                                   .undefined_value()
                                                   .ptr()),
                         static_cast<unsigned>(
                             ReadOnlyRoots(isolate).exception().ptr()),
                         static_cast<unsigned>(
                             ReadOnlyRoots(isolate).the_hole_value().ptr()),
                         static_cast<unsigned>(
                             ReadOnlyRoots(isolate).null_value().ptr()),
                         static_cast<unsigned>(
                             ReadOnlyRoots(isolate).true_value().ptr()),
                         static_cast<unsigned>(
                             ReadOnlyRoots(isolate).false_value().ptr()));
            PrintF("WASM32_MAIN_PROMISE_RESULT");
            DumpRuntimeArg("result", 0, promise_result.ptr());
            PrintStringPreviewForTrace("result_string", promise_result, 0,
                                       240);
            if (IsJSReceiver(promise_result)) {
              DumpNamedDataPropertyForTrace(isolate, promise_result.ptr(),
                                            "name");
              DumpNamedDataPropertyForTrace(isolate, promise_result.ptr(),
                                            "message");
              DumpNamedDataPropertyForTrace(isolate, promise_result.ptr(),
                                            "code");
            }
            PrintF("\n");
            std::fflush(stdout);
          }
        }
        std::fflush(stderr);
      }
#endif
      g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)] = result;
      g_wasm_regs[SlotFor(kReturnRegister0)] = result;
#ifdef __wasi__
      if (trace_entry_steps) {
        PrintF("WasmInterpreterEntryTrace: return #%d step=%d bytecode=%s ",
               trace_entry_index, step,
               interpreter::Bytecodes::ToString(bytecode_enum));
        DumpRuntimeArg("result", 0, result);
        PrintF("\n");
      }
#endif
      if (should_log_step) {
        PrintF("WasmInterpreterEntryTrampoline: fallback return bytecode=0x%x "
               "steps=%d result=0x%x\n",
               static_cast<unsigned>(opcode), step + 1,
               static_cast<unsigned>(result));
      }
      return;
    }

    if (entry == kNullAddress) {
#ifdef __wasi__
      if (shared->StartPosition() == 0 && bytecode->length() > 200000) {
        std::fprintf(stderr,
                     "WASM32_MAIN_MISSING_HANDLER pc=%d opcode=%s "
                     "handler_builtin=%d\n",
                     bytecode_index,
                     interpreter::Bytecodes::ToString(bytecode_enum),
                     static_cast<int>(handler_builtin));
        std::fflush(stderr);
      }
#endif
      if (kTraceWasmFallbackDetails) {
        PrintF("WasmInterpreterEntryTrampoline: missing handler bytecode=0x%x "
               "scale=%d\n",
               static_cast<unsigned>(opcode), static_cast<int>(operand_scale));
      }
      g_wasm_regs[SlotFor(kReturnRegister0)] = Smi::zero().ptr();
      return;
    }

    g_wasm_regs[SlotFor(kInterpreterBytecodeOffsetRegister)] = current_offset;
    g_wasm_regs[SlotFor(Register::r4())] = current_offset;
    g_wasm_regs[SlotFor(Register::r5())] = bytecode.ptr();
    g_wasm_regs[SlotFor(Register::r6())] = dispatch_table;
    g_wasm_regs[SlotFor(kInterpreterBytecodeArrayRegister)] = bytecode.ptr();
    StoreInterpreterFrameOffset(InterpreterFrameConstants::kBytecodeOffsetFromFp,
                                Smi::FromInt(static_cast<int>(current_offset))
                                    .ptr());

    if (trace_collection_followup) {
      PrintF("WasmInterpreterEntryTrampoline: generated handler enter "
             "step=%d bytecode=%s offset=0x%x handler_builtin=%d "
             "handler=0x%x acc=0x%x\n",
             step, interpreter::Bytecodes::ToString(bytecode_enum),
             static_cast<unsigned>(current_offset),
             static_cast<int>(handler_builtin), static_cast<unsigned>(entry),
             static_cast<unsigned>(
                 g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]));
    }
    reinterpret_cast<WasmRegFileFn>(entry)();
    if (trace_collection_followup || (trace_entry_steps && step < 96)) {
      PrintF("WasmInterpreterEntryTrampoline: generated handler exit "
             "step=%d bytecode=%s offset_reg=0x%x acc=0x%x ret=0x%x\n",
             step, interpreter::Bytecodes::ToString(bytecode_enum),
             static_cast<unsigned>(
                 g_wasm_regs[SlotFor(kInterpreterBytecodeOffsetRegister)]),
             static_cast<unsigned>(
                 g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)]),
             static_cast<unsigned>(g_wasm_regs[SlotFor(kReturnRegister0)]));
    }

    Address next_offset = current_offset +
                          interpreter::Bytecodes::Size(bytecode_enum,
                                                       operand_scale);
    if (interpreter::Bytecodes::IsJump(bytecode_enum) ||
        interpreter::Bytecodes::IsSwitch(bytecode_enum)) {
      Address handler_offset =
          g_wasm_regs[SlotFor(kInterpreterBytecodeOffsetRegister)];
      Address bytecode_start = bytecode_offset;
      Address bytecode_limit =
          bytecode_offset + static_cast<Address>(bytecode->length());
      if (handler_offset != current_offset) {
        if (handler_offset >= bytecode_start && handler_offset < bytecode_limit) {
          next_offset = handler_offset;
        } else {
          PrintF("WasmInterpreterEntryTrampoline: ignoring invalid branch "
                 "offset=0x%x bytecode=%s current=0x%x range=[0x%x,0x%x)\n",
                 static_cast<unsigned>(handler_offset),
                 interpreter::Bytecodes::ToString(bytecode_enum),
                 static_cast<unsigned>(current_offset),
                 static_cast<unsigned>(bytecode_start),
                 static_cast<unsigned>(bytecode_limit));
        }
      }
    }
    current_offset = next_offset;
    operand_scale = interpreter::OperandScale::kSingle;
  }

  PrintF("WasmInterpreterEntryTrampoline: exceeded max steps length=%d\n",
         bytecode->length());
  PrintF("WasmInterpreterEntryTrampoline: function target=0x%x sfi=0x%x "
         "bytecode=0x%x regs=%d params=%d start=%d end=%d literal_id=%d\n",
         static_cast<unsigned>(target), static_cast<unsigned>(shared.ptr()),
         static_cast<unsigned>(bytecode.ptr()), register_count, parameter_count,
         shared->StartPosition(), shared->EndPosition(),
         shared->function_literal_id());
  DumpFunctionSourceForTrace(target);
  PrintF("\n");
  for (int i = 0; i < parameter_count; ++i) {
    DumpRuntimeArg(
        "  overflow param", i,
        ReadInterpreterRegister(interpreter::Register::FromParameterIndex(i)));
  }
  PrintF("\n");
  for (int i = 0; i < kMaxInterpreterTailTrace; ++i) {
    int slot = (kMaxInterpreterSteps + i) % kMaxInterpreterTailTrace;
    PrintF("  tail step=%d index=%d opcode=0x%x(%s) operands=%d,%d,%d "
           "acc=0x%x\n",
           tail_step[slot], tail_index[slot],
           static_cast<unsigned>(tail_opcode[slot]),
           interpreter::Bytecodes::ToString(tail_bytecode[slot]),
           tail_operand0[slot], tail_operand1[slot], tail_operand2[slot],
           static_cast<unsigned>(tail_accumulator[slot]));
  }
  for (int i = 0; i < register_count; ++i) {
    DumpRuntimeArg("  overflow local", i,
                   ReadInterpreterRegister(interpreter::Register(i)));
  }
  PrintF("\n");
  Address result = ReadOnlyRoots(isolate).undefined_value().ptr();
  if (isolate->has_exception()) isolate->clear_exception();
  isolate->clear_pending_message();
  g_wasm_regs[SlotFor(kInterpreterAccumulatorRegister)] = result;
  g_wasm_regs[SlotFor(kReturnRegister0)] = result;
}

// Hand-written JSEntry. Signature mirrors execution.cc's JSEntryFunction.
// This is the narrow C++ -> generated-wasm bridge: seed the emulated wasm32
// register/stack slots, then call the JSFunction's current code entry.
extern "C" Address WasmJSEntry(Address root, Address new_target, Address target,
                               Address receiver, intptr_t argc,
                               Address** argv) {
  Isolate* isolate = GetWasm32IsolateFromRoot(&root);
  if (isolate == nullptr) return Smi::zero().ptr();
  WasmJSEntryDepthScope entry_depth_scope;
  int actual_argc = static_cast<int>(argc) - kJSArgcReceiverSlots;
  if (actual_argc < 0) actual_argc = 0;
  if (actual_argc + 1 > kWasmMaxOutgoingArgSlots) {
    return Smi::zero().ptr();
  }
  SetCurrentIsolateScope current_isolate_scope(isolate);
  WasmInterpreterStateSnapshot entry_state(isolate);
  ClearEntrypointRegisterFile();
  g_wasm_regs[kWasmRegRoot] = root;
  Address undefined = ReadOnlyRoots(isolate).undefined_value().ptr();
  g_wasm_regs[kWasmJSEntryArgSlotBase] = receiver;
  for (int i = 0; i < actual_argc; ++i) {
    Address value = undefined;
    if (argv != nullptr && argv[i] != nullptr) {
      value = *argv[i];
    }
    g_wasm_regs[kWasmJSEntryArgSlotBase + 1 + i] = value;
  }
  if (kTraceWasmJSEntry) {
    PrintF("WasmJSEntry: enter root=0x%x new_target=0x%x target=0x%x "
           "receiver=0x%x argc=%d\n",
           static_cast<unsigned>(root), static_cast<unsigned>(new_target),
           static_cast<unsigned>(target), static_cast<unsigned>(receiver),
           static_cast<int>(argc));
  }

  Tagged<Object> target_object(target);
  if (IsJSBoundFunction(target_object)) {
    Address result;
    {
      HandleScope scope(isolate);
      DirectHandle<JSBoundFunction> bound_function(
          Cast<JSBoundFunction>(target_object), isolate);
      DirectHandle<FixedArray> bound_arguments(
          bound_function->bound_arguments(), isolate);
      const int bound_argc = bound_arguments->length();
      const int total_argc = bound_argc + actual_argc;
      if (total_argc + 1 > kWasmMaxOutgoingArgSlots) {
        result = Smi::zero().ptr();
      } else {
        Address merged_args[kWasmMaxOutgoingArgSlots == 0
                                ? 1
                                : kWasmMaxOutgoingArgSlots];
        Address* merged_argv[kWasmMaxOutgoingArgSlots == 0
                                 ? 1
                                 : kWasmMaxOutgoingArgSlots];
        for (int i = 0; i < bound_argc; ++i) {
          merged_args[i] = bound_arguments->get(i).ptr();
          merged_argv[i] = &merged_args[i];
        }
        for (int i = 0; i < actual_argc; ++i) {
          merged_args[bound_argc + i] =
              g_wasm_regs[kWasmJSEntryArgSlotBase + 1 + i];
          merged_argv[bound_argc + i] = &merged_args[bound_argc + i];
        }

        Address bound_target = bound_function->bound_target_function().ptr();
        Address bound_receiver = bound_function->bound_this().ptr();
        if (kTraceWasmJSEntry) {
          PrintF(
              "WasmJSEntry: expand bound function target=0x%x bound_argc=%d "
              "actual_argc=%d receiver=0x%x\n",
              static_cast<unsigned>(bound_target), bound_argc, actual_argc,
              static_cast<unsigned>(bound_receiver));
        }
        result = WasmJSEntry(root, new_target, bound_target, bound_receiver,
                             total_argc + kJSArgcReceiverSlots,
                             total_argc == 0 ? nullptr : merged_argv);
      }
    }
    entry_state.Restore();
    return result;
  }
  if (!IsJSFunction(target_object)) {
    PrintF("WasmJSEntry: target is not JSFunction\n");
    entry_state.Restore();
    return Smi::zero().ptr();
  }

  Tagged<JSFunction> function = Cast<JSFunction>(target_object);
#ifdef __wasi__
  if (!function->is_compiled(isolate)) {
    static int semantic_compile_count = 0;
    int compile_index = ++semantic_compile_count;
    HandleScope compile_scope(isolate);
    DirectHandle<JSFunction> function_handle(function, isolate);
    IsCompiledScope is_compiled_scope(
        function_handle->shared()->is_compiled_scope(isolate));
    if (compile_index <= 100) {
      std::fprintf(stderr,
                   "WASM32_SEMANTIC_COMPILE_BEGIN #%d start=%d end=%d\n",
                   compile_index, function_handle->shared()->StartPosition(),
                   function_handle->shared()->EndPosition());
      std::fflush(stderr);
    }
    if (!Compiler::Compile(isolate, function_handle,
                           Compiler::KEEP_EXCEPTION,
                           &is_compiled_scope)) {
      entry_state.Restore();
      return ReadOnlyRoots(isolate).exception().ptr();
    }
    if (compile_index <= 100) {
      Tagged<Code> compiled_code = function_handle->code(isolate);
      std::fprintf(stderr,
                   "WASM32_SEMANTIC_COMPILE_END #%d builtin=%d id=%d name=%s\n",
                   compile_index, compiled_code->is_builtin() ? 1 : 0,
                   compiled_code->is_builtin()
                       ? static_cast<int>(compiled_code->builtin_id())
                       : -1,
                   compiled_code->is_builtin()
                       ? Builtins::name(compiled_code->builtin_id())
                       : "<none>");
      std::fflush(stderr);
    }
    function = *function_handle;
  }
#endif
  Tagged<Code> code = function->code(isolate);
  Tagged<SharedFunctionInfo> shared = Wasm32JSFunctionShared(function);
  Tagged<Context> function_context = Wasm32JSFunctionContext(function);
  Address entry = code->instruction_start();
#ifdef __wasi__
  // Native/generated code entries bypass the wasm32 C++ bytecode fallbacks
  // and their recursive-call handle boundaries. Keep every function that has
  // bytecode on the hand-written interpreter path, even if tiering has changed
  // the JSFunction's current Code object.
  if (shared->HasBytecodeArray()) {
    entry = reinterpret_cast<Address>(&WasmInterpreterEntryTrampoline);
  }
#endif
#ifdef __wasi__
  int trace_entry_start = shared->StartPosition();
  bool trace_mcq_entry = SharedDebugNameEqualsAsciiForTrace(shared, "McQ") ||
                         trace_entry_start == 7530000 ||
                         trace_entry_start == 455011 ||
                         (trace_entry_start == 0 &&
                          shared->EndPosition() > 9000000) ||
                         (trace_entry_start >= 7242924 &&
                          trace_entry_start <= 7243075);
  if (trace_mcq_entry) {
    std::fprintf(stderr,
                 "WASM32_TARGET_ENTRY_REAL start=%d builtin=%d builtin_id=%d name=%s "
                 "entry=0x%x has_bytecode=%d\n",
                 trace_entry_start,
                 code->is_builtin() ? 1 : 0,
                 code->is_builtin() ? static_cast<int>(code->builtin_id()) : -1,
                 code->is_builtin() ? Builtins::name(code->builtin_id())
                                    : "<none>",
                 static_cast<unsigned>(entry),
                 shared->HasBytecodeArray() ? 1 : 0);
    std::fflush(stderr);
    PrintF("WASM32_MCQ_ENTRY builtin=%d builtin_id=%d name=%s entry=0x%x\n",
           code->is_builtin() ? 1 : 0,
           code->is_builtin() ? static_cast<int>(code->builtin_id()) : -1,
           code->is_builtin() ? Builtins::name(code->builtin_id()) : "<none>",
           static_cast<unsigned>(entry));
  }
#endif
#ifdef __wasi__
  bool trace_eval_js_entry =
      kTraceWasmJSEntry && FunctionMatchesWasmEvalTraceNeedle(shared);
  if (kTraceWasmJSEntry && (shared->is_script() || trace_eval_js_entry)) {
    static int wasm_js_entry_script_trace_count = 0;
    if (wasm_js_entry_script_trace_count < 64) {
      ++wasm_js_entry_script_trace_count;
      PrintF("WasmJSEntry: script entry #%d receiver=0x%x actual_argc=%d "
             "eval_match=%d code_builtin=%d builtin_id=%d builtin_name=%s "
             "entry=0x%x builtin_funcref=0x%x ",
             wasm_js_entry_script_trace_count,
             static_cast<unsigned>(receiver), actual_argc,
             trace_eval_js_entry ? 1 : 0,
             code->is_builtin() ? 1 : 0,
             code->is_builtin() ? static_cast<int>(code->builtin_id()) : -1,
             code->is_builtin() ? Builtins::name(code->builtin_id()) : "<none>",
             static_cast<unsigned>(entry),
             code->is_builtin()
                 ? static_cast<unsigned>(reinterpret_cast<Address>(
                       WasmBuiltinFuncref(code->builtin_id())))
                 : 0);
      DumpFunctionSourceForTrace(target);
      for (int i = 0; i < actual_argc && i < 4; ++i) {
        DumpRuntimeArg(" script_arg", i,
                       g_wasm_regs[kWasmJSEntryArgSlotBase + 1 + i]);
      }
      PrintF("\n");
    }
  }
#endif
  bool uses_interpreter_entry = false;
  bool trace_internal_async_hooks_require = false;
  if (actual_argc > 0) {
    Address first_arg = SafeTaggedOrUndefined(
        isolate, g_wasm_regs[kWasmJSEntryArgSlotBase + 1]);
    if (IsString(Tagged<Object>(first_arg))) {
      HandleScope trace_scope(isolate);
      Handle<String> first_arg_string =
          handle(Cast<String>(Tagged<Object>(first_arg)), isolate);
      trace_internal_async_hooks_require = String::Equals(
          isolate, first_arg_string,
          isolate->factory()->InternalizeUtf8String("internal/async_hooks"));
    }
  }
  if (kTraceWasmJSEntry && trace_internal_async_hooks_require) {
    PrintF("WasmJSEntry: internal/async_hooks target_name=");
    DumpNameForTrace(shared->Name());
    PrintF(" target=0x%x receiver=0x%x\n", static_cast<unsigned>(target),
           static_cast<unsigned>(receiver));
  }
  if (kTraceWasmJSEntry) {
    PrintF("WasmJSEntry: argv actual_argc=%d", actual_argc);
    for (int i = 0; i < actual_argc && i < 6; ++i) {
      DumpRuntimeArg("argv", i, g_wasm_regs[kWasmJSEntryArgSlotBase + 1 + i]);
    }
    PrintF("\n");
  }
  if (code->is_builtin()) {
    Builtin builtin = code->builtin_id();
#ifdef __wasi__
    static int wasm_js_entry_api_dispatch_trace_count = 0;
    if ((builtin == Builtin::kHandleApiCallOrConstruct ||
         builtin == Builtin::kCallApiCallbackGeneric ||
         builtin == Builtin::kCallApiCallbackOptimizedNoProfiling ||
         builtin == Builtin::kCallApiCallbackOptimized ||
         std::strstr(Builtins::name(builtin), "Api") != nullptr) &&
        kTraceWasmJSEntry &&
        wasm_js_entry_api_dispatch_trace_count < 64) {
      ++wasm_js_entry_api_dispatch_trace_count;
      PrintF("WasmJSEntry: api dispatch #%d builtin_id=%d name=%s "
             "target=0x%x actual_argc=%d shared=0x%x is_api=%d ",
             wasm_js_entry_api_dispatch_trace_count, static_cast<int>(builtin),
             Builtins::name(builtin), static_cast<unsigned>(target),
             actual_argc, static_cast<unsigned>(shared.ptr()),
             shared->IsApiFunction() ? 1 : 0);
      DumpNameForTrace(shared->Name());
      PrintF("\n");
    }
    if (kTraceWasmJSEntry && builtin == Builtin::kObjectDefineProperty) {
      static int object_define_entry_trace_count = 0;
      bool trace_object_define_entry =
          ++object_define_entry_trace_count <= 32 ||
          (object_define_entry_trace_count % 256) == 0;
      if (trace_object_define_entry) {
        PrintF("WasmJSEntry: builtin ObjectDefineProperty #%d target=0x%x "
               "receiver=0x%x actual_argc=%d",
               object_define_entry_trace_count, static_cast<unsigned>(target),
               static_cast<unsigned>(receiver), actual_argc);
        for (int i = 0; i < actual_argc && i < 4; ++i) {
          DumpRuntimeArg("arg", i,
                         g_wasm_regs[kWasmJSEntryArgSlotBase + 1 + i]);
        }
        PrintF("\n");
      }
    }
#endif
    if (kTraceWasmJSEntry &&
        std::strstr(Builtins::name(builtin), "RegExp") != nullptr) {
      PrintF("WasmJSEntry: regexp builtin target=%d name=%s receiver=0x%x "
             "actual_argc=%d",
             static_cast<int>(builtin), Builtins::name(builtin),
             static_cast<unsigned>(receiver), actual_argc);
      if (actual_argc > 0) {
        DumpRuntimeArg("arg", 0, g_wasm_regs[kWasmJSEntryArgSlotBase + 1]);
      }
      PrintF("\n");
    }
    uses_interpreter_entry =
        builtin == Builtin::kInterpreterEntryTrampoline ||
        builtin == Builtin::kInterpreterEntryTrampolineForProfiling;
    Address fallback_result = kNullAddress;
    bool used_fallback = false;
    {
      WasmGCStateScope gc_state(isolate);
      used_fallback = TryFallbackJSEntryBuiltin(
          isolate, builtin, function, receiver, new_target, actual_argc,
          &g_wasm_regs[kWasmJSEntryArgSlotBase + 1], &fallback_result);
    }
    if (used_fallback) {
#ifdef __wasi__
      if (trace_mcq_entry) {
        std::fprintf(stderr,
                     "WASM32_TARGET_FALLBACK_REAL start=%d result=0x%x exception=%d\n",
                     trace_entry_start,
                     static_cast<unsigned>(fallback_result),
                     isolate->has_exception() ? 1 : 0);
        std::fflush(stderr);
        PrintF("WASM32_MCQ_FALLBACK result=0x%x exception=%d\n",
               static_cast<unsigned>(fallback_result),
               isolate->has_exception() ? 1 : 0);
      }
#endif
      entry_state.Restore();
      return fallback_result;
    }
    void* fn = WasmBuiltinFuncref(builtin);
    if (fn == nullptr) {
      if (kTraceWasmJSEntry) {
        PrintF("WasmJSEntry: unregistered builtin target=%d name=%s "
               "entry=0x%x\n",
               static_cast<int>(builtin), Builtins::name(builtin),
               static_cast<unsigned>(entry));
      }
      entry_state.Restore();
      return Smi::zero().ptr();
    }
    entry = reinterpret_cast<Address>(fn);
  }
  if (kTraceWasmJSEntry) {
    PrintF("WasmJSEntry: code builtin=%d name=%s is_builtin=%d entry=0x%x\n",
           static_cast<int>(code->builtin_id()),
           code->is_builtin() ? Builtins::name(code->builtin_id()) : "<none>",
           code->is_builtin(),
           static_cast<unsigned>(entry));
  }
  if (entry == kNullAddress) {
    PrintF("WasmJSEntry: null code entry\n");
    entry_state.Restore();
    return Smi::zero().ptr();
  }

  constexpr int kWasmFixedFrameSlotLimit = kWasmStackSlotBase - 1;
  int last_argument_offset =
      CommonFrameConstants::kFixedFrameSizeAboveFp +
      actual_argc * kSystemPointerSize;
  if (kWasmFixedFrameSlotBase + StandardFrameConstants::kFixedSlotCountFromFp >=
          kWasmFixedFrameSlotLimit ||
      GeneratedFrameSlotForOffset(last_argument_offset) >=
          kWasmOutgoingArgSlotBase ||
      actual_argc + 1 > kWasmMaxOutgoingArgSlots) {
    entry_state.Restore();
    return Smi::zero().ptr();
  }

  ClearInterpreterFrame();
  for (int i = kWasmFixedFrameSlotBase; i < kWasmFixedFrameSlotLimit; ++i) {
    g_wasm_regs[i] = undefined;
  }
  StoreInterpreterFrameOffset(CommonFrameConstants::kCallerFPOffset, 0);
  StoreInterpreterFrameOffset(CommonFrameConstants::kCallerPCOffset, 0);
  StoreInterpreterFrameOffset(StandardFrameConstants::kContextOffset,
                              function_context.ptr());
  StoreInterpreterFrameOffset(StandardFrameConstants::kFunctionOffset, target);
  StoreInterpreterFrameOffset(StandardFrameConstants::kArgCOffset,
                              static_cast<Address>(argc));
  StoreGeneratedFrameOffset(CommonFrameConstants::kCallerFPOffset, 0);
  StoreGeneratedFrameOffset(CommonFrameConstants::kCallerPCOffset, 0);
  StoreGeneratedFrameOffset(StandardFrameConstants::kContextOffset,
                            function_context.ptr());
  StoreGeneratedFrameOffset(StandardFrameConstants::kFunctionOffset, target);
  StoreGeneratedFrameOffset(StandardFrameConstants::kArgCOffset,
                            static_cast<Address>(argc));

  StoreInterpreterFrameOffset(CommonFrameConstants::kFixedFrameSizeAboveFp,
                              receiver);
  StoreGeneratedFrameOffset(CommonFrameConstants::kFixedFrameSizeAboveFp,
                            receiver);
  for (int i = 0; i < actual_argc; ++i) {
    Address value = g_wasm_regs[kWasmJSEntryArgSlotBase + 1 + i];
    int offset = CommonFrameConstants::kFixedFrameSizeAboveFp +
                 (i + 1) * kSystemPointerSize;
    StoreInterpreterFrameOffset(offset, value);
    StoreGeneratedFrameOffset(offset, value);
  }

  g_wasm_regs[SlotFor(kRootRegister)] = root;
  g_wasm_regs[SlotFor(kContextRegister)] = function_context.ptr();
  g_wasm_regs[SlotFor(kJavaScriptCallTargetRegister)] = target;
  g_wasm_regs[SlotFor(kJavaScriptCallNewTargetRegister)] = new_target;
  g_wasm_regs[SlotFor(kJavaScriptCallArgCountRegister)] =
      static_cast<Address>(argc);
  g_wasm_regs[SlotFor(kJavaScriptCallCodeStartRegister)] = entry;
  g_wasm_regs[SlotFor(kJavaScriptCallDispatchHandleRegister)] = 0;
  if (kTraceWasmJSEntry) {
    PrintF("WasmJSEntry: frame slots");
    DumpRuntimeArg("ctx_slot", 0,
                   g_wasm_regs[GeneratedFrameSlotForOffset(
                       StandardFrameConstants::kContextOffset)]);
    DumpRuntimeArg("receiver_slot", 0,
                   g_wasm_regs[GeneratedFrameSlotForOffset(
                       CommonFrameConstants::kFixedFrameSizeAboveFp)]);
    if (actual_argc > 0) {
      DumpRuntimeArg("arg0_slot", 0,
                     g_wasm_regs[GeneratedFrameSlotForOffset(
                         CommonFrameConstants::kFixedFrameSizeAboveFp +
                         kSystemPointerSize)]);
    }
    PrintF("\n");
  }
  USE(uses_interpreter_entry);

  using WasmRegFileFn = void (*)();
  reinterpret_cast<WasmRegFileFn>(entry)();
  Address result = g_wasm_regs[SlotFor(kReturnRegister0)];
#ifdef __wasi__
  if (trace_entry_start == 7243075) {
    g_wasm_request_duplex_getter_returned = true;
  }
  if (trace_mcq_entry) {
    std::fprintf(stderr,
                 "WASM32_TARGET_RETURN_REAL start=%d result=0x%x exception=%d\n",
                 trace_entry_start,
                 static_cast<unsigned>(result),
                 isolate->has_exception() ? 1 : 0);
    std::fflush(stderr);
    PrintF("WASM32_MCQ_RETURN result=0x%x exception=%d\n",
           static_cast<unsigned>(result), isolate->has_exception() ? 1 : 0);
  }
#endif
  if (entry_depth_scope.outermost() && result == Smi::zero().ptr() &&
      !isolate->has_exception()) {
    HandleScope result_scope(isolate);
    result = (*isolate->factory()->NewHeapNumber(0.0)).ptr();
  }
  entry_state.Restore();
  if (kTraceWasmJSEntry && trace_internal_async_hooks_require) {
    PrintF("WasmJSEntry: internal/async_hooks return ");
    DumpRuntimeArg("result", 0, result);
    PrintF(" has_exception=%d\n", isolate->has_exception());
  }
  if (kTraceWasmJSEntry) {
    PrintF("WasmJSEntry: return=0x%x\n",
           static_cast<unsigned>(result));
  }
  return result;
}

extern "C" Address WasmRunScriptEntryForApi(Address root, Address target,
                                            Address receiver,
                                            Address host_options) {
  Isolate* isolate = GetWasm32IsolateFromRoot(&root);
  if (isolate == nullptr) return Smi::zero().ptr();
  Address arg0 = host_options;
  Address* argv[] = {&arg0};
#ifdef __wasi__
  static int wasm_api_script_trace_count = 0;
  if (kTraceWasmJSEntry && wasm_api_script_trace_count < 16) {
    ++wasm_api_script_trace_count;
    PrintF("WasmRunScriptEntryForApi: #%d target=0x%x receiver=0x%x "
           "host_options=0x%x\n",
           wasm_api_script_trace_count, static_cast<unsigned>(target),
           static_cast<unsigned>(receiver),
           static_cast<unsigned>(host_options));
  }
#endif
  Address undefined = ReadOnlyRoots(isolate).undefined_value().ptr();
  return WasmJSEntry(root, undefined, target, receiver, JSParameterCount(1),
                     argv);
}

void RegisterGeneratedWasmBuiltins() __attribute__((weak));

// Registers all hand-written wasm builtins. Called once during builtin setup.
void RegisterAllWasmBuiltins() {
  RegisterWasmBuiltin(Builtin::kIllegal,
                      reinterpret_cast<void*>(&WasmProbeBuiltin));
  RegisterWasmBuiltin(Builtin::kJSEntry, reinterpret_cast<void*>(&WasmJSEntry));
  RegisterWasmBuiltin(Builtin::kInterpreterEntryTrampoline,
                      reinterpret_cast<void*>(&WasmInterpreterEntryTrampoline));
  RegisterWasmBuiltin(Builtin::kInterpreterEntryTrampolineForProfiling,
                      reinterpret_cast<void*>(&WasmInterpreterEntryTrampoline));
  if (RegisterGeneratedWasmBuiltins != nullptr) {
    RegisterGeneratedWasmBuiltins();
  }

  // Generated Ignition handlers tail-dispatch directly to the next handler.
  // Calls need to return to the C++ interpreter loop so TryRunCallBytecode can
  // bound its HandleScope and defer recursive WasmJSEntry until that scope has
  // been destroyed. Replace call-handler dispatch targets with a no-op barrier;
  // the outer loop then advances to and executes the call through the fallback.
#define REGISTER_WASM32_CALL_BARRIER(Name)                                \
  RegisterWasmBuiltin(Builtin::k##Name##Handler,                           \
                      reinterpret_cast<void*>(&WasmProbeBuiltin));         \
  RegisterWasmBuiltin(Builtin::k##Name##WideHandler,                       \
                      reinterpret_cast<void*>(&WasmProbeBuiltin));         \
  RegisterWasmBuiltin(Builtin::k##Name##ExtraWideHandler,                  \
                      reinterpret_cast<void*>(&WasmProbeBuiltin))
  REGISTER_WASM32_CALL_BARRIER(CallAnyReceiver);
  REGISTER_WASM32_CALL_BARRIER(CallProperty);
  REGISTER_WASM32_CALL_BARRIER(CallProperty0);
  REGISTER_WASM32_CALL_BARRIER(CallProperty1);
  REGISTER_WASM32_CALL_BARRIER(CallProperty2);
  REGISTER_WASM32_CALL_BARRIER(CallUndefinedReceiver);
  REGISTER_WASM32_CALL_BARRIER(CallUndefinedReceiver0);
  REGISTER_WASM32_CALL_BARRIER(CallUndefinedReceiver1);
  REGISTER_WASM32_CALL_BARRIER(CallUndefinedReceiver2);
  REGISTER_WASM32_CALL_BARRIER(CallWithSpread);
#undef REGISTER_WASM32_CALL_BARRIER

}

}  // namespace internal

void Isolate::SetHostImportModuleDynamicallyCallback(
    HostImportModuleDynamicallyCallback callback) {
  internal::Isolate* isolate = reinterpret_cast<internal::Isolate*>(this);
  isolate->SetHostImportModuleDynamicallyCallback(callback);
}

void Isolate::SetHostImportModuleWithPhaseDynamicallyCallback(
    HostImportModuleWithPhaseDynamicallyCallback callback) {
  internal::Isolate* isolate = reinterpret_cast<internal::Isolate*>(this);
  isolate->SetHostImportModuleWithPhaseDynamicallyCallback(callback);
}

void Isolate::SetHostInitializeImportMetaObjectCallback(
    HostInitializeImportMetaObjectCallback callback) {
  internal::Isolate* isolate = reinterpret_cast<internal::Isolate*>(this);
  isolate->SetHostInitializeImportMetaObjectCallback(callback);
}

}  // namespace v8

#endif  // V8_TARGET_ARCH_WASM32
