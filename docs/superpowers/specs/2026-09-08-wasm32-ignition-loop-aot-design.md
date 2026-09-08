# WASM32 Ignition Integer-Loop AOT Tier

## Goal

Accelerate the hot numeric-loop subset of JavaScript executed by the WASM32
Ignition fallback without changing JavaScript-visible behavior. The initial
success target is a 10x throughput improvement for the established one-million
iteration `(sum + i) | 0` benchmark relative to the current semantic bytecode
interpreter. This target does not apply to arbitrary Node or Claude Code
workloads.

## Non-goals

- Dynamic generation or nested execution of WebAssembly modules.
- General JavaScript compilation, object-property optimization, calls,
  allocations, exceptions, async code, BigInt, floating point, or typed-array
  optimization.
- Changing Ignition bytecode definitions, generated handlers, GC layout, or
  the existing WasmJSEntry recursion boundaries.

## Chosen architecture

The new tier is compiled ahead of time as part of `node.wasm`. It is a small
C++ loop executor invoked only after a bytecode-pattern recognizer establishes
that a function is an eligible closed integer loop. This avoids a nested guest
Wasm compilation path, which would itself be interpreted by DrumBrake and
would add re-entrancy and handle-lifetime risk.

The tier lives beside the existing WASM32 fallback interpreter in
`deps/v8/src/builtins/wasm32/builtins-wasm32.cc`. Its public boundary is
internal to that file: a recognizer receives a `BytecodeArray` and the current
interpreter register state; an executor receives a validated immutable loop
plan plus raw tagged register values; and a result reports either completed
state or a bail-out condition.

## Eligibility and plan format

Version one accepts one natural loop with all of the following properties:

- One backedge using `JumpLoop`; no other backward branch.
- Smi-only loop counter, bound, and accumulator registers.
- Constant Smi initialization and increments.
- Integer arithmetic represented by supported arithmetic bytecodes followed by
  an explicit ToInt32-equivalent coercion pattern used by the benchmark.
- Comparison and conditional branch determining loop termination.
- Register loads/stores and an eventual return of a live loop value.
- No bytecodes for calls, property access, context/module accesses, literal or
  object allocation, feedback-vector mutation, throw, await, debugger, or
  interrupt-sensitive operations inside the loop.

The recognized plan stores bytecode offsets, loop registers, signed constants,
the comparison relation, and the expected exit PC. It never stores raw heap
object pointers beyond the synchronous invocation.

## Execution and semantics

At the bytecode dispatch boundary, the recognizer is consulted before entering
the loop body. Failed recognition is invisible and immediately follows the
existing semantic interpreter path.

For a recognized plan, the executor first validates that every input is a Smi
and that the plan remains within the bytecode-array bounds. It performs the
loop using signed `int32_t` operations with explicit JavaScript `ToInt32`
wrapping. On normal completion it writes the final tagged Smi values to the
existing interpreter-frame storage, sets the bytecode PC to the validated exit
PC, and returns to the ordinary dispatch loop.

If a value cannot be represented as a Smi, an unsupported control-flow shape
is encountered, an arithmetic result is not representable under the plan's
required tagged result, or an interrupt/exception state is observed, the tier
does not commit partial register state. It returns `bail-out`, and execution
continues at the original loop-entry PC in the existing interpreter.

## Safety invariants

- Recognition is pure: it neither allocates nor changes bytecode, feedback, or
  interpreter state.
- Execution does not allocate, enter JavaScript, create handles, invoke GC, or
  run nested Wasm.
- Register writes occur only after successful complete execution.
- Bytecode offsets are bounds-checked before use and the exit PC is validated
  to be forward of the backedge.
- Unsupported bytecode always remains on the existing fallback path.

## Observability

A disabled-by-default counter records recognition attempts, successful plans,
completed executions, and bail-outs. No per-iteration trace is emitted. A
temporary test-only flag may expose these counters for the focused benchmark;
production default output remains silent.

## Validation

The implementation will add focused tests for:

- Eligible counter-and-accumulator loops returning correct results, including
  negative increments and `int32` wrapping.
- Near-match loops rejected because of a call, property access, unsupported
  arithmetic, invalid branch target, or non-Smi input.
- Bail-out preserving the original interpreter result and exception behavior.
- A benchmark harness reporting the baseline and AOT elapsed time under the
  same `wasmer` compiler/cache settings.

The tier is accepted only if correctness tests pass and the target benchmark
achieves at least 10x throughput. If it cannot, the tier remains guarded and
the measured shortfall is reported rather than widening its opcode surface.

## Measured result

Measured on 2026-09-08 with the same Wasmer Cranelift cache and the focused
one-million-iteration `(sum + i) | 0` script:

| Mode | Result | elapsed_ns | Relative speed |
| --- | ---: | ---: | ---: |
| Interpreter | 1783293664 | 17620629966 | 1.00x |
| Loop AOT | 1783293664 | 1481681 | 11892.3x |

Object access, calls, I/O, async execution, and Claude Code remain on the
semantic interpreter path.
