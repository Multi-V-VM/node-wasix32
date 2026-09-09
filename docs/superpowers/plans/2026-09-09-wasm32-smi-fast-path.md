# WASM32 Smi Fast Path Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Reduce browser Node startup and integer-heavy JavaScript execution time by avoiding per-bytecode GC root snapshots for non-allocating Smi arithmetic.

**Architecture:** Add a guarded Smi binary-operation helper to the existing non-allocating bytecode fast path. It returns `false` for coercion, overflow, negative zero, BigInt, strings, and non-Smi results so the existing semantic implementation remains authoritative.

**Tech Stack:** V8 Ignition bytecodes, C++17, WASI SDK, Wasmer, JavaScript regression tests, Chrome browser SDK benchmark.

---

### Task 1: Add semantic and dispatch coverage

**Files:**
- Create: `test/wasm32/test-smi-fast-path.js`
- Create: `tools/run-wasm32-smi-fast-path-test.sh`
- Modify: `tools/wasmer-aot-run.sh`

- [x] Add arithmetic and bitwise assertions covering Smi results and currently working fallback-only edge cases. BigInt and out-of-range unsigned shifts remain implementation guards because their existing WASM32 fallbacks are independently broken.
- [x] Run the test against the existing binary and require `WASM32_SMI_FAST_PATH` markers for `add`, `sub`, `mul`, and `bitwise`.
- [x] Confirm RED: JavaScript semantics pass but the marker assertion fails because the fast path is absent.

### Task 2: Implement guarded Smi binary operations

**Files:**
- Modify: `deps/v8/src/builtins/wasm32/builtins-wasm32.cc`

- [x] Add `TryRunSmiBinaryBytecodeFastPath()` before `WasmGCStateScope` construction.
- [x] Handle generic and immediate `Add`, `Sub`, `Mul`, bitwise, and shift bytecodes only when inputs and results are representable without allocation.
- [x] Return `false` for overflow, negative zero, unsigned results outside the Smi range, or non-Smi operands.
- [x] Emit one diagnostic marker per operation family only when `WASM32_SMI_FAST_PATH_STATS` is enabled.

### Task 3: Build and verify

**Files:**
- Build output: `out/Release/node`
- Browser artifact: `node.wasm`

- [x] Rebuild the Release WASM32 Node binary.
- [x] Run the Smi semantics/dispatch test and the existing loop-AOT regression.
- [x] Compare `benchmark-interpreter-dispatch.js` medians with the recorded baseline.
- [x] Run the local real-Chrome Wasmer SDK benchmark and report compilation, first-output, and execution timings separately.
- [x] Publish a new browser WEBC only if semantic tests pass and the measured browser result improves.
