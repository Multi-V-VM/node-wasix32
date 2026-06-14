# wasm32 Builtin Dispatch Spine + Hand-written JSEntry — Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Make V8 builtin `Code` objects on the wasm32/WASI port dispatch through the wasm `__indirect_function_table` to real C++ functions, and hand-write `JSEntry` so `Invoke` gets past the `undefined element: out of bounds table access` trap.

**Architecture:** Implements Plan 1 (milestones 1–2) of `docs/superpowers/specs/2026-06-14-wasm32-turbofan-backend-design.md`. A global `Builtin → C++ function pointer` registry is populated by hand-written builtins; after each builtin `Code` object is built, its `instruction_start` is set to the registered function pointer — which on wasm *is* an `__indirect_function_table` index — so the existing `reinterpret_cast<Signature*>(instruction_start)(args...)` dispatch in `GeneratedCode::Call` performs a valid `call_indirect`. The entry builtins (`JSEntry` etc.) are written with the exact C++ signature `GeneratedCode::Call` expects, so this milestone needs no `g_regs` marshalling yet (that arrives with builtin↔builtin calls in a later plan).

**Tech Stack:** C++ (wasi-sdk clang targeting wasm32-wasi), V8 internals, wasmer for smoke runs. Build per memory `node-wasix-build-process`.

---

## Testing reality (read first)

There is **no unit-test framework that runs in this embedding** — `cctest` is itself a wasm binary. The test loop for every task is:

1. Rebuild `node.wasm` (incremental; see the build command below).
2. Run: `wasmer run node.wasm -- -e "1"` from the repo root.
3. Assert on the **trap signature / stderr** (filter debug spam):
   `... 2>&1 | grep -vE '^HashSeed|^Eternalize|^MakeAccessorInfo|^NewAccessorInfo' | tail -30`

**Build command (from repo root, run as root):**
```bash
make -C out BUILDTYPE=Release -j$(nproc) \
  -o /home/victoryang00/node-wasix32/out/Release/mksnapshot \
  -o /home/victoryang00/node-wasix32/out/Release/torque \
  -o /home/victoryang00/node-wasix32/out/Release/node_mksnapshot \
  -o /home/victoryang00/node-wasix32/out/196eef7d411008fac8b82aa9f38714e3740138fd.intermediate \
  -o /home/victoryang00/node-wasix32/out/5b313717fc52d5d50e9fb22a8eb50443dee74d02.intermediate \
  -o /home/victoryang00/node-wasix32/out/79d8d70785ed943ff8ccc1e4f23e8039ca0fcaf1.intermediate \
  -o /home/victoryang00/node-wasix32/out/d4386a59c12620d5dbfef3ccb4004f3356088fdf.intermediate \
  -o /home/victoryang00/node-wasix32/out/f7d7f6d9211480c5f0062c4176e954ee2fc77758.intermediate \
  node \
  && cp out/Release/node node.wasm
```
If make deleted an `*.intermediate`, `touch` it first. New `.cc` files added to V8 libraries must be appended to the corresponding `out/tools/v8_gypfiles/*.target.mk` object list (gyp won't regenerate it) — this is called out in the tasks that add files.

---

## File structure

- **Create** `deps/v8/src/builtins/wasm32/builtins-wasm32-abi.h` — register-file slot indices and the `Builtin → funcref` registry API (declarations only; header-only constants).
- **Create** `deps/v8/src/builtins/wasm32/builtins-wasm32-registry.cc` — registry storage, `RegisterWasmBuiltin`, `WasmBuiltinFuncref`.
- **Modify** `deps/v8/src/builtins/wasm32/builtins-wasm32.cc` — implement `JSEntry`/`JSConstructEntry`/`JSRunMicrotasksEntry` as real C++ functions; register them and a trivial probe builtin.
- **Modify** `deps/v8/src/builtins/setup-builtins-internal.cc` — after installing each builtin, override its `Code::instruction_start` with the registered funcref when one exists.
- **Modify** `out/tools/v8_gypfiles/v8_base_without_compiler.target.mk` — add the new `.o` for the registry file.

---

## Task 1: Register-file slot constants + registry API header

**Files:**
- Create: `deps/v8/src/builtins/wasm32/builtins-wasm32-abi.h`

- [ ] **Step 1: Create the ABI header**

```cpp
// Copyright 2026 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#ifndef V8_BUILTINS_WASM32_BUILTINS_WASM32_ABI_H_
#define V8_BUILTINS_WASM32_BUILTINS_WASM32_ABI_H_

#if V8_TARGET_ARCH_WASM32

#include "src/builtins/builtins.h"
#include "src/common/globals.h"

namespace v8 {
namespace internal {

// Emulated register file in linear memory. This is the ABI between C++ and
// generated/hand-written wasm builtins. Fixed slots hold the root register,
// context, argument registers, and the return slot. Only used by builtins that
// take the uniform g_regs ABI; the entry builtins (JSEntry etc.) use direct
// typed C++ signatures and do NOT read g_regs in this milestone.
// Sized generously; tighten later against interface-descriptors-wasm32.
constexpr int kWasmRegFileSize = 64;
extern Address g_wasm_regs[kWasmRegFileSize];

// Well-known slot indices (must agree with interface-descriptors-wasm32-inl.h
// when generated builtins arrive).
enum WasmRegSlot : int {
  kWasmRegRoot = 0,      // isolate root (kRootRegister)
  kWasmRegContext = 1,   // current context (kContextRegister)
  kWasmRegReturn = 2,    // return value slot
  kWasmRegArg0 = 8,      // first argument slot; args are kWasmRegArg0 + i
};

// Builtin -> wasm function pointer registry. A registered function pointer's
// integer value is its __indirect_function_table index; instruction_start for
// the builtin is set to these bits so GeneratedCode::Call's call_indirect
// dispatches correctly.
using WasmBuiltinFn = void (*)();
void RegisterWasmBuiltin(Builtin builtin, void* fnptr);
// Returns nullptr if no wasm implementation is registered for `builtin`.
void* WasmBuiltinFuncref(Builtin builtin);

}  // namespace internal
}  // namespace v8

#endif  // V8_TARGET_ARCH_WASM32
#endif  // V8_BUILTINS_WASM32_BUILTINS_WASM32_ABI_H_
```

- [ ] **Step 2: Sanity-compile the header alone**

Run:
```bash
/opt/wasi-sdk/bin/clang++ --target=wasm32-wasi -DV8_TARGET_ARCH_WASM32=1 \
  -I deps/v8 -I deps/v8/include -fsyntax-only \
  -x c++ deps/v8/src/builtins/wasm32/builtins-wasm32-abi.h 2>&1 | head
```
Expected: may emit unrelated missing-include diagnostics for `globals.h`, but **no syntax error in this header's own declarations**. (A clean `-fsyntax-only` is not required here because the header depends on V8 internals; the real check is Task 2's build.)

- [ ] **Step 3: Commit**

```bash
git add deps/v8/src/builtins/wasm32/builtins-wasm32-abi.h
git commit -m "wasm32: add builtin dispatch ABI header (g_regs + funcref registry)"
```

---

## Task 2: Registry implementation + wire into the build

**Files:**
- Create: `deps/v8/src/builtins/wasm32/builtins-wasm32-registry.cc`
- Modify: `out/tools/v8_gypfiles/v8_base_without_compiler.target.mk`

- [ ] **Step 1: Create the registry implementation**

```cpp
// Copyright 2026 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#if V8_TARGET_ARCH_WASM32

#include "src/builtins/wasm32/builtins-wasm32-abi.h"

#include <array>

namespace v8 {
namespace internal {

Address g_wasm_regs[kWasmRegFileSize] = {0};

namespace {
std::array<void*, Builtins::kBuiltinCount>& Table() {
  static std::array<void*, Builtins::kBuiltinCount> table = {};
  return table;
}
}  // namespace

void RegisterWasmBuiltin(Builtin builtin, void* fnptr) {
  Table()[Builtins::ToInt(builtin)] = fnptr;
}

void* WasmBuiltinFuncref(Builtin builtin) {
  return Table()[Builtins::ToInt(builtin)];
}

}  // namespace internal
}  // namespace v8

#endif  // V8_TARGET_ARCH_WASM32
```

- [ ] **Step 2: Add the new object to the v8_base target makefile**

Open `out/tools/v8_gypfiles/v8_base_without_compiler.target.mk`. Find the line:
```
	$(obj).target/$(TARGET)/deps/v8/src/builtins/wasm32/builtins-wasm32.o \
```
Add immediately after it:
```
	$(obj).target/$(TARGET)/deps/v8/src/builtins/wasm32/builtins-wasm32-registry.o \
```
(If the `builtins-wasm32.o` line is absent, add both under the other `deps/v8/src/builtins/` entries.)

- [ ] **Step 3: Build to verify the registry compiles and links**

Run the build command from the "Testing reality" section.
Expected: link succeeds; `node.wasm` is produced. (Behavior unchanged — nothing calls the registry yet.)

- [ ] **Step 4: Commit**

```bash
git add deps/v8/src/builtins/wasm32/builtins-wasm32-registry.cc out/tools/v8_gypfiles/v8_base_without_compiler.target.mk
git commit -m "wasm32: implement builtin funcref registry and add to build"
```

---

## Task 3: Override builtin instruction_start with the registered funcref

**Files:**
- Modify: `deps/v8/src/builtins/setup-builtins-internal.cc`

**Context:** `SetupBuiltinsInternal` installs each builtin via `AddBuiltin`. We add a pass that, for any builtin with a registered wasm function, overrides its `Code::instruction_start` with the funcref bits. Use the existing `Code::SetInstructionStartForOffHeapBuiltin(IsolateForSandbox, Address)` (code-inl.h:861). **Verification needed during impl:** if the built `Code` still reports `has_instruction_stream() == true` (from `BuildWithMacroAssembler`'s empty stream), `InstructionStart()` may ignore the override — in that case also clear the stream or build these via `BuildPlaceholder`-style codes without a stream. Confirm with a debug print of `code->instruction_start()` after the override.

- [ ] **Step 1: Add the include**

At the top includes of `deps/v8/src/builtins/setup-builtins-internal.cc`, after the existing `#ifdef __wasi__` block near the top, add:
```cpp
#ifdef __wasi__
#include "src/builtins/wasm32/builtins-wasm32-abi.h"
#endif
```

- [ ] **Step 2: Add the override pass**

In `SetupBuiltinsInternal`, locate the loop that ends with `AddBuiltin(builtins, builtin, ...)` (the `for (Builtin builtin = Builtins::kFirst; ...)` loop near the end). Immediately **after** that loop and before `builtins->MarkInitialized();`, insert:
```cpp
#ifdef __wasi__
  // Override instruction_start for builtins backed by a hand-written/generated
  // wasm function so GeneratedCode::Call's call_indirect dispatches to it.
  for (Builtin builtin = Builtins::kFirst; builtin <= Builtins::kLast;
       ++builtin) {
    void* fnptr = WasmBuiltinFuncref(builtin);
    if (fnptr == nullptr) continue;
    Tagged<Code> code = builtins->code(builtin);
    code->SetInstructionStartForOffHeapBuiltin(
        isolate, reinterpret_cast<Address>(fnptr));
    fprintf(stderr, "wasm builtin %d instruction_start=%p\n",
            Builtins::ToInt(builtin), (void*)code->instruction_start());
    fflush(stderr);
  }
#endif
```

- [ ] **Step 3: Build**

Run the build command. Expected: builds and links. No behavior change yet (registry is still empty — no builtin registered), so the override loop is a no-op and the smoke run still traps at the same place.

- [ ] **Step 4: Smoke run to confirm no regression**

Run: `wasmer run node.wasm -- -e "1" 2>&1 | grep -vE '^HashSeed|^Eternalize|^MakeAccessorInfo|^NewAccessorInfo' | tail -8`
Expected: still `undefined element: out of bounds table access` at `Invoke` (unchanged — proves the override pass is inert when the registry is empty).

- [ ] **Step 5: Commit**

```bash
git add deps/v8/src/builtins/setup-builtins-internal.cc
git commit -m "wasm32: override builtin instruction_start with registered funcref"
```

---

## Task 4: Probe builtin — prove call_indirect dispatch end-to-end

**Files:**
- Modify: `deps/v8/src/builtins/wasm32/builtins-wasm32.cc`

**Context:** Before touching JSEntry, prove the whole mechanism with the simplest possible case: register a C++ function as the implementation of an unused leaf builtin, then confirm at startup that its `Code::instruction_start()` equals the function pointer (the override pass ran) and that calling through it works. We use `Builtin::kIllegal` as the probe target (it is referenced during bootstrap by a sanity check: `CHECK(IsCode(i_isolate->builtins()->code(Builtin::kIllegal)))`, so wiring it is observable and safe to dispatch as a no-op trap-free function).

- [ ] **Step 1: Add the probe function and registrar**

In `deps/v8/src/builtins/wasm32/builtins-wasm32.cc`, add the include near the top (after the existing includes):
```cpp
#include "src/builtins/wasm32/builtins-wasm32-abi.h"
```
At the end of the `namespace v8 { namespace internal {` block (before the closing braces), add:
```cpp
// Probe builtin: a trap-free no-op used to validate the dispatch spine.
// Signature is irrelevant for this milestone — it is never actually called
// from generated code, only its funcref/instruction_start wiring is checked.
extern "C" void WasmProbeBuiltin() { /* no-op */ }

// Registers all hand-written wasm builtins. Called once during builtin setup.
void RegisterAllWasmBuiltins() {
  RegisterWasmBuiltin(Builtin::kIllegal,
                      reinterpret_cast<void*>(&WasmProbeBuiltin));
}
```
Also declare `void RegisterAllWasmBuiltins();` in `builtins-wasm32-abi.h` (add next to the registry API) so it can be called from setup.

- [ ] **Step 2: Call the registrar before the override pass**

In `deps/v8/src/builtins/setup-builtins-internal.cc`, inside `SetupBuiltinsInternal`, at the very start of the function body (after `Builtins* builtins = isolate->builtins();`), add:
```cpp
#ifdef __wasi__
  RegisterAllWasmBuiltins();
#endif
```

- [ ] **Step 3: Build**

Run the build command. Expected: builds and links.

- [ ] **Step 4: Smoke run — confirm the probe wired up**

Run: `wasmer run node.wasm -- -e "1" 2>&1 | grep "wasm builtin" | head`
Expected: a line `wasm builtin <N> instruction_start=<non-zero ptr>` where `<N>` is `Builtins::ToInt(Builtin::kIllegal)` and the pointer is non-zero and small (a table index, not a huge data-blob address). This proves the override pass set `instruction_start` to the registered funcref.

- [ ] **Step 5: Commit**

```bash
git add deps/v8/src/builtins/wasm32/builtins-wasm32.cc deps/v8/src/builtins/wasm32/builtins-wasm32-abi.h deps/v8/src/builtins/setup-builtins-internal.cc
git commit -m "wasm32: probe builtin proves funcref dispatch wiring"
```

---

## Task 5: Hand-write JSEntry to move the trap past dispatch

**Files:**
- Modify: `deps/v8/src/builtins/wasm32/builtins-wasm32.cc`

**Context:** `Invoke` (execution.cc:411) does
`JSEntryFunction stub_entry = JSEntryFunction::FromAddress(isolate, code->instruction_start());`
then `stub_entry.Call(root, new_target, target, receiver, argc, argv)` with
`JSEntryFunction = GeneratedCode<Address(Address,Address,Address,Address,intptr_t,Address**)>`.
Because we write the C++ `JSEntry` with **exactly this signature**, `reinterpret_cast<Signature*>(&JSEntry_wasm)` type-checks at the wasm level and the `call_indirect` succeeds — no `g_regs` needed at the C++→entry boundary in this milestone. Inside JSEntry, calling the *target's* code is the next milestone; for now JSEntry sets up minimal state and returns a value so we can confirm the trap **moves past the JSEntry dispatch** (the current failure point).

**Verification needed during impl:** confirm the exact `JSEntryFunction` signature in `deps/v8/src/execution/execution.cc` (search `using JSEntryFunction`) and mirror it precisely, including the `Address**` argv and `intptr_t` argc.

- [ ] **Step 1: Implement JSEntry and register it**

In `builtins-wasm32.cc`, add (inside `namespace v8::internal`):
```cpp
// Hand-written JSEntry. Signature mirrors execution.cc's JSEntryFunction.
// Milestone scope: prove dispatch succeeds. Calling into the target's code is
// the next plan; here we return Smi::zero() so the trap, if any, moves to the
// caller's handling of the result rather than the JSEntry call_indirect.
extern "C" Address WasmJSEntry(Address root, Address new_target, Address target,
                               Address receiver, intptr_t argc,
                               Address** argv) {
  g_wasm_regs[kWasmRegRoot] = root;
  // TODO(next plan): set up entry frame and call target's Code via g_regs ABI.
  return Smi::zero().ptr();
}
```
Add to `RegisterAllWasmBuiltins()`:
```cpp
  RegisterWasmBuiltin(Builtin::kJSEntry,
                      reinterpret_cast<void*>(&WasmJSEntry));
```

- [ ] **Step 2: Build**

Run the build command. Expected: builds and links.

- [ ] **Step 3: Smoke run — confirm the trap moved**

Run: `wasmer run node.wasm -- -e "1" 2>&1 | grep -vE '^HashSeed|^Eternalize|^MakeAccessorInfo|^NewAccessorInfo' | tail -20`
Expected: the **`undefined element: out of bounds table access` at `v8::internal::Invoke` is gone.** The run now fails *differently* — deeper (e.g. inside primordials interpretation, a different builtin's dispatch, or a result-handling check). Record the new top-of-stack failure: it is the entry point for the next plan. (If the same JSEntry-dispatch trap persists, the signature did not match — revisit Step 1 against execution.cc.)

- [ ] **Step 4: Commit**

```bash
git add deps/v8/src/builtins/wasm32/builtins-wasm32.cc
git commit -m "wasm32: hand-written JSEntry — Invoke dispatches without table trap"
```

---

## Task 6: Record outcome and hand off to Plan 2

**Files:**
- Modify: `docs/superpowers/plans/2026-06-14-wasm32-dispatch-spine.md` (this file)

- [ ] **Step 1: Append an Outcome section**

At the bottom of this plan file, add a short `## Outcome` section recording: the new failure signature after Task 5, whether `has_instruction_stream()` required clearing (Task 3 verification result), and the confirmed `JSEntryFunction` signature. This is the input to Plan 2 (the `g_regs` builtin↔builtin call path + the next entry point).

- [ ] **Step 2: Commit**

```bash
git add docs/superpowers/plans/2026-06-14-wasm32-dispatch-spine.md
git commit -m "wasm32: record dispatch-spine outcome for Plan 2 handoff"
```

---

## Self-review notes

- **Spec coverage:** implements spec §2 (dispatch model: funcref registry + instruction_start override + uniform-dispatch foundation) and §5's first entry builtin (JSEntry); validates spec §7 milestones 1–2. `g_regs` is declared here (§2/§2.1) but only the root slot is exercised — full marshalling is deferred to the next plan, as the spec sequences it.
- **Out of scope (correctly deferred):** the code-generator (§3), build pipeline (§4), remaining bootstrap builtins (CEntry/InterpreterEntryTrampoline), and the `g_regs` call marshalling — each is a later plan.
- **Known verification points (flagged inline, not placeholders):** Task 3 `has_instruction_stream()` interaction; Task 5 exact `JSEntryFunction` signature. Both have explicit confirmation steps with observable expected output.
- **Type consistency:** `RegisterWasmBuiltin`/`WasmBuiltinFuncref`/`RegisterAllWasmBuiltins`/`g_wasm_regs`/`kWasmReg*` are used consistently across the ABI header, registry, builtins file, and setup hook.
