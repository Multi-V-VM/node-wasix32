# Design: Full Turbofan/Turboshaft wasm32 backend first

**Date:** 2026-06-14
**Status:** Approved design
**Related specs:**
- `doc/superpowers/specs/2026-06-14-wasm32-turbofan-backend-design.md`
- `doc/superpowers/specs/2026-06-14-wasm32-dispatch-spine.md`

## Goal

Implement the full V8 wasm32 compiler backend before treating interpreter
execution as the primary milestone. The first real acceptance target is not a
smoke-only `node.wasm -e "1"` exit. It is generated wasm backend code executing
through V8 builtin dispatch, then expanding that backend until Node can execute
JavaScript without the current startup bypasses.

The existing dispatch spine remains the foundation. The current hand-written
`WasmJSEntry` stub is not sufficient: it returns `Smi::zero()` and does not call
generated or interpreted JavaScript. This design replaces that gap with a real
Turbofan/Turboshaft-to-wasm backend.

## Architecture

This is a compiler backend project, not a runtime-only shim. The backend adds
these units:

- `deps/v8/src/compiler/backend/wasm32/instruction-selector-wasm32.cc`
  lowers Turbofan machine graph operations into wasm32-specific instruction
  codes.
- `deps/v8/src/compiler/backend/wasm32/code-generator-wasm32.cc`
  emits wasm bytecode for selected instructions, control flow, calls, constants,
  memory operations, and register movement.
- `deps/v8/src/codegen/wasm32/wasm32-encoder.{h,cc}` owns LEB128 encoding,
  wasm function bodies, locals, blocks, branches, memory accesses, and
  `call_indirect` emission.
- `deps/v8/src/codegen/wasm32/register-wasm32.h` defines the virtual register
  model backed by the existing `g_wasm_regs` ABI.
- `deps/v8/src/builtins/wasm32/builtins-wasm32-registry.cc` evolves from a
  hand-written builtin registry into the generated builtin table.
- Build and snapshot wiring ensures generated builtin functions become valid
  wasm table entries and that wasm32 builtin `Code::instruction_start()` values
  resolve to executable generated code.

The implementation order is intentionally big-backend-first: build the encoder,
selector, code generator, metadata path, and build integration before removing
bootstrap bypasses. Small generated builtins can be used as proof points, but the
design target is the complete backend surface needed by V8 builtins and bytecode
handlers.

## Data Flow

The generated-code path is:

```text
Turbofan or Turboshaft graph
  -> wasm32 instruction selection
  -> wasm32 instruction stream
  -> wasm32 code generator
  -> wasm encoder
  -> wasm function body
  -> builtin registry
  -> V8 Code object entrypoint
  -> dispatch spine call_indirect
```

The important invariant is that `Code::instruction_start()` must no longer be a
fake native pointer for generated wasm32 builtins. For wasm32 builtins it must
resolve to a stable wasm function-table entry or encoded entry value that the
dispatch spine can call.

The `g_wasm_regs` ABI remains the bridge between generated code, hand-written
entry builtins, and runtime calls. Arguments, return values, root register, frame
pointer, context, and scratch values move through the emulated register file
until later optimization work proves a more direct representation safe.

C++ runtime calls stay imported host calls. Generated wasm code may call C++
helpers through narrow trampolines, but builtin-to-builtin calls should go
through the wasm table so the generated backend exercises its own dispatch path.

## Backend Coverage

The full-backend pass covers these compiler surfaces before the design is
considered complete:

- Integer constants, tagged pointer values, root loads, frame register movement,
  context register movement, and root register movement.
- `Word32`, pointer-sized `WordPtr`, comparisons, branches, merges, and returns.
- Linear-memory loads and stores while preserving V8 tagged-pointer assumptions.
- Direct builtin calls, indirect builtin calls through the wasm table, and
  imported C++ runtime calls through narrow trampolines.
- Frame-state, safepoint, and deopt metadata sufficient to compile snapshot and
  builtin code without corrupting GC expectations.
- A Turboshaft-facing emission boundary. Initial Turboshaft enablement can reuse
  the same wasm emitter after Turbofan support works.

Unsupported operations must fail during compilation with the opcode name and
the builtin or function being compiled. The backend must not silently emit trap
stubs for missing coverage, because that hides compiler gaps until Node startup
fails much later.

## Build And Snapshot Integration

The wasm32 backend files are added to V8's existing generated make flow with
target guards so non-wasm targets are unaffected. Snapshot generation is the main
integration risk: generated builtins must be registered before any
`Code::instruction_start()` consumer expects executable code.

The build path has these responsibilities:

- Build V8/Node with the wasm32 backend source files included.
- Ensure mksnapshot-under-Wasmer can compile wasm32 builtins without executing
  JavaScript during generation.
- Emit generated builtin functions into the final `node.wasm` or into linked
  wasm object code whose function table entries are stable at runtime.
- Serialize builtin identity in the snapshot in a way that can be resolved to
  the runtime wasm table entry after final linking.
- Resolve builtin `Code` objects to generated wasm entries before Node bootstrap
  executes any builtin code.

## Error Handling

Failures are diagnosed at the layer that owns the invariant:

- Graph lowering errors identify unsupported Turbofan/Turboshaft operations.
- Encoder errors identify invalid wasm structure or unsupported immediates.
- Registry errors identify missing builtin table entries.
- `Code` metadata errors identify stale or fake `instruction_start()` values.
- Runtime import errors identify broken C++ trampoline signatures or argument
  marshalling.
- Bootstrap errors identify remaining Node/V8 startup dependencies after the
  backend has proven generated-code execution.

## Validation Gates

Validation proceeds in this order:

1. Build V8/Node with the wasm32 backend files included.
2. Compile a generated builtin through the wasm32 backend and register it as a
   wasm function.
3. Verify `Code::instruction_start()` resolves to that generated wasm entry.
4. Execute builtin-to-builtin and builtin-to-runtime calls through the dispatch
   spine.
5. Expand backend opcode coverage until bootstrap-critical generated builtins
   compile without fallback trap bodies for missing opcodes.
6. Remove the current smoke bypasses only after generated code can carry the
   startup path.
7. Prove real JavaScript execution with `node.wasm -e "1+1"`.
8. Prove observable JavaScript execution with `node.wasm -e "console.log(1)"`.

The final success criterion is real JavaScript execution through generated wasm
backend code. A process exit produced by short-circuiting Node bootstrap or by a
stubbed `JSEntry` does not satisfy this design.

## Scope Boundary

This design does not require optimizing generated wasm output, natural wasm CFG
reconstruction, Maglev support, SIMD support, threads, or a non-WASI embedding.
Those are follow-on improvements after the backend executes JavaScript
correctly.
