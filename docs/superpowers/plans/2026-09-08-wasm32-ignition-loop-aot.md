# WASM32 Ignition Integer-Loop AOT Tier Implementation Plan

> **For agentic workers:** REQUIRED SUB-SKILL: Use superpowers:subagent-driven-development (recommended) or superpowers:executing-plans to implement this plan task-by-task. Steps use checkbox (`- [ ]`) syntax for tracking.

**Goal:** Add an ahead-of-time compiled, guarded executor for a narrow Smi/i32 Ignition loop subset, with exact fallback to the existing WASM32 semantic interpreter.

**Architecture:** The implementation remains private to `builtins-wasm32.cc`. A pure recognizer converts one verified bytecode loop into a compact plan; a no-allocation executor runs that plan using `int32_t` semantics and commits register/PC state only after success. Unsupported shapes, non-Smi values, interrupts, or arithmetic representations that cannot safely be committed leave state untouched and use the existing bytecode interpreter.

**Tech Stack:** V8 C++ internals, Ignition bytecodes, WASM32/WASI Clang build, Wasmer Cranelift cache, Node JavaScript test scripts.

---

## File structure

- Modify: `deps/v8/src/builtins/wasm32/builtins-wasm32.cc`
  - Owns the private plan data structure, recognizer, executor, guarded hook in
    the bytecode loop, and disabled-by-default aggregate statistics.
- Create: `test/wasm32/test-ignition-loop-aot.js`
  - Runs eligible and rejected JavaScript loop cases and verifies their public
    results; it prints one machine-readable benchmark line only when invoked
    with `WASM32_AOT_BENCHMARK=1`.
- Create: `tools/run-wasm32-ignition-loop-aot-test.sh`
  - Runs the focused test against `out/Release/node` using a persistent
    Cranelift cache and fails if the result or required statistics are absent.

### Task 1: Add an observable, disabled-by-default focused test harness

**Files:**
- Create: `test/wasm32/test-ignition-loop-aot.js`
- Create: `tools/run-wasm32-ignition-loop-aot-test.sh`

- [ ] **Step 1: Write the failing JavaScript test**

```js
'use strict';

const assert = require('node:assert/strict');

function sumTo(limit) {
  let sum = 0;
  for (let i = 0; i < limit; i++) sum = (sum + i) | 0;
  return sum;
}

function rejectedByCall(limit) {
  let sum = 0;
  for (let i = 0; i < limit; i++) sum = (sum + Number(i)) | 0;
  return sum;
}

assert.equal(sumTo(1_000_000), 1_783_293_664);
assert.equal(sumTo(-1), 0);
assert.equal(sumTo(65_537), -2_147_450_880);
assert.equal(rejectedByCall(10), 45);

if (process.env.WASM32_AOT_BENCHMARK === '1') {
  const start = process.hrtime.bigint();
  const result = sumTo(1_000_000);
  const elapsedNs = process.hrtime.bigint() - start;
  console.log(`WASM32_AOT_BENCH result=${result} elapsed_ns=${elapsedNs}`);
}
```

- [ ] **Step 2: Write the runner with a required AOT-statistics contract**

```bash
#!/usr/bin/env bash
set -euo pipefail

root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
cache_dir=${WASMER_CACHE_DIR:-"${XDG_CACHE_HOME:-$HOME/.cache}/node-wasix32/wasmer"}
output=$(mktemp)
trap 'rm -f "$output"' EXIT

WASM32_AOT_STATS=1 WASMER_CACHE_DIR="$cache_dir" \
  "$root/tools/wasmer-aot-run.sh" \
  "$root/test/wasm32/test-ignition-loop-aot.js" >"$output" 2>&1

cat "$output"
rg -q '^WASM32_AOT_STATS plans=[1-9][0-9]* completed=[1-9][0-9]* bailouts=[0-9]+$' "$output"
```

- [ ] **Step 3: Run the test before implementation**

Run:

```bash
chmod +x tools/run-wasm32-ignition-loop-aot-test.sh
tools/run-wasm32-ignition-loop-aot-test.sh
```

Expected: non-zero exit because `WASM32_AOT_STATS` does not yet exist.

### Task 2: Define the private loop plan and pure recognizer

**Files:**
- Modify: `deps/v8/src/builtins/wasm32/builtins-wasm32.cc`

- [ ] **Step 1: Add the smallest private data model near the WASM32 interpreter helpers**

```cpp
enum class Wasm32LoopAotStatus : uint8_t { kNotEligible, kBailedOut, kCompleted };

struct Wasm32I32LoopPlan {
  int loop_header_pc;
  int loop_exit_pc;
  interpreter::Register index;
  interpreter::Register limit;
  interpreter::Register accumulator;
  int32_t increment;
};

struct Wasm32LoopAotStats {
  uint32_t plans = 0;
  uint32_t completed = 0;
  uint32_t bailouts = 0;
};
```

- [ ] **Step 2: Implement a bounded recognizer**

```cpp
bool TryBuildWasm32I32LoopPlan(
    Tagged<BytecodeArray> bytecodes, int loop_header_pc,
    Wasm32I32LoopPlan* out_plan) {
  // Decode only the exact forward body, conditional exit, and JumpLoop shape.
  // Return false for every opcode that is not explicitly accepted.
}
```

Required recognizer checks:

- The header, every decoded instruction, the conditional exit, and the
  `JumpLoop` target are within `bytecodes->length()`.
- The loop has one backedge and one forward exit.
- The index and accumulator writes target distinct interpreter registers.
- The loop body contains no call, property, context, allocation, exception,
  await, debugger, feedback-update, or interrupt bytecode.
- The arithmetic/coercion sequence matches `(accumulator + index) | 0` and the
  index update matches `index + constant_smi`.

- [ ] **Step 3: Add a no-output recognition counter guarded by the environment**

```cpp
bool Wasm32AotStatsEnabled() {
  return std::getenv("WASM32_AOT_STATS") != nullptr;
}
```

Only print once at process teardown or a deterministic final script boundary;
the line must match:

```text
WASM32_AOT_STATS plans=<n> completed=<n> bailouts=<n>
```

- [ ] **Step 4: Rebuild the Node target**

Run:

```bash
CC_host=/usr/bin/gcc CXX_host=/usr/bin/g++ \
CC=/opt/wasi-sdk/bin/clang CXX=/opt/wasi-sdk/bin/clang++ \
make -C out BUILDTYPE=Release -j4 node
```

Expected: successful `out/Release/node` build with no new generated-source
changes.

### Task 3: Implement transactional AOT execution and guarded dispatch hook

**Files:**
- Modify: `deps/v8/src/builtins/wasm32/builtins-wasm32.cc`

- [ ] **Step 1: Implement the executor without heap allocation or handle creation**

```cpp
Wasm32LoopAotStatus TryExecuteWasm32I32LoopPlan(
    Isolate* isolate, const Wasm32I32LoopPlan& plan,
    Tagged<BytecodeArray> bytecodes, Address* accumulator_out,
    int* next_pc_out) {
  // Read values into local Address/int32_t temporaries.
  // Do not write interpreter registers until the terminating condition is met.
  // Return kBailedOut with no writes for non-Smi, interrupt, or unsafe result.
}
```

The executor must use `static_cast<int32_t>` for each add and construct the
final Smi only after `Smi::IsValid`. It must not run a nested `WasmJSEntry`,
allocate, call into JavaScript, or execute another bytecode handler.

- [ ] **Step 2: Insert the hook only at a loop-header dispatch point**

```cpp
Wasm32I32LoopPlan plan;
if (TryBuildWasm32I32LoopPlan(bytecodes, pc, &plan)) {
  ++g_wasm32_loop_aot_stats.plans;
  switch (TryExecuteWasm32I32LoopPlan(isolate, plan, bytecodes,
                                      &result, &next_pc)) {
    case Wasm32LoopAotStatus::kCompleted:
      ++g_wasm32_loop_aot_stats.completed;
      pc = next_pc;
      continue;
    case Wasm32LoopAotStatus::kBailedOut:
      ++g_wasm32_loop_aot_stats.bailouts;
      break;
    case Wasm32LoopAotStatus::kNotEligible:
      break;
  }
}
```

The hook must occur before executing the recognized header and must leave the
current PC/register state unchanged on any non-completed status.

- [ ] **Step 3: Run the focused correctness test**

Run:

```bash
tools/run-wasm32-ignition-loop-aot-test.sh
```

Expected: JavaScript assertions pass and the final line contains non-zero
`plans` and `completed`; the rejected function still returns `45`.

### Task 4: Validate performance, reject regressions, and document the bound

**Files:**
- Modify: `test/wasm32/test-ignition-loop-aot.js`
- Modify: `docs/superpowers/specs/2026-09-08-wasm32-ignition-loop-aot-design.md`

- [ ] **Step 1: Add an interpreter-disabled control in the benchmark harness**

```js
const benchmark = process.env.WASM32_AOT_BENCHMARK === '1';
const forceInterpreter = process.env.WASM32_DISABLE_LOOP_AOT === '1';
if (benchmark) console.log(`WASM32_AOT_MODE interpreter=${forceInterpreter ? 1 : 0}`);
```

The C++ hook must honor `WASM32_DISABLE_LOOP_AOT=1` before recognition so the
same binary provides a baseline.

- [ ] **Step 2: Measure baseline and accelerated runs under identical cache settings**

Run:

```bash
cache=/tmp/wasm32-aot-bench-cache
rm -rf "$cache"
WASMER_CACHE_DIR="$cache" WASM32_DISABLE_LOOP_AOT=1 WASM32_AOT_BENCHMARK=1 \
  tools/wasmer-aot-run.sh test/wasm32/test-ignition-loop-aot.js
WASMER_CACHE_DIR="$cache" WASM32_AOT_BENCHMARK=1 \
  tools/wasmer-aot-run.sh test/wasm32/test-ignition-loop-aot.js
```

Expected: both print the same result; the accelerated `elapsed_ns` is at most
one tenth of the disabled-tier result. Record raw values rather than claiming
the target if it is not reached.

- [ ] **Step 3: Update the design document with measured results and scope**

Append one table:

```markdown
| Mode | Result | elapsed_ns | Relative speed |
| --- | ---: | ---: | ---: |
| Interpreter | `<measured>` | `<measured>` | 1.00x |
| Loop AOT | `<measured>` | `<measured>` | `<measured>` |
```

State explicitly that object access, calls, I/O, async execution, and Claude
Code remain on the semantic interpreter path.

- [ ] **Step 4: Run focused validation after the measurement update**

Run:

```bash
tools/run-wasm32-ignition-loop-aot-test.sh
```

Expected: functional assertions and required AOT statistics still pass.
