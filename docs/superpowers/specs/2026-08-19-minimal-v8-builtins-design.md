# Minimal V8 Builtins for wasm32 Node.js

**Date:** 2026-08-19  
**Status:** Design Draft  
**Scope:** Implement minimal V8 builtins to enable basic JavaScript execution on wasm32 Node.js

## Problem Statement

The wasm32 Node.js port crashes during bootstrap with `RuntimeError: out of bounds memory access` in `Map::SetPrototype`. Root cause: V8 builtins are empty stubs on wasm32, preventing any JavaScript execution. This blocks running even simple CLI tools with basic JavaScript logic.

## Goals

Enable wasm32 Node.js to execute simple JavaScript CLI tools with:
- Basic JavaScript logic (variables, functions, arithmetic)
- No external dependencies
- Minimal feature set to reduce implementation scope

## Non-Goals

- Full V8 compatibility with all JavaScript features
- Advanced features (generators, async/await, modules)
- Production-ready Node.js on wasm32
- Performance optimization

## Architecture Overview

The system consists of three layers:

### Layer 1: Builtin Dispatch Layer
- Maps builtin IDs to actual wasm function implementations  
- Uses `__indirect_function_table` for function calls
- Replaces broken `fn_ptr_ = reinterpret_cast<Signature*>(code->instruction_start())` pattern

### Layer 2: Core Builtins (5 functions)
- `JSEntry` - Entry point for JavaScript execution
- `CEntry` - Entry point for C++ calls from JavaScript  
- `InterpreterEntryTrampoline` - Entry point for Ignition interpreter
- `RecordWrite` - Minimal write barrier
- `EnterContext` - Context setup

### Layer 3: Minimal Ignition Interpreter
- Basic bytecode execution (bytecodes 0-50)
- Supports: variables, function calls, basic math, object literals
- Excludes: generators, try-catch, async/await

## Component Breakdown

### Component 1: Builtin Dispatch Table
**File:** `deps/v8/src/builtins/wasm32/builtin-dispatch-wasm32.cc`

**Purpose:** Maps builtin IDs to wasm function pointers in `__indirect_function_table`

**Key Classes:**
```cpp
class BuiltinDispatchTable {
 public:
  static int GetFunctionIndex(Builtin builtin);
  static void RegisterBuiltin(Builtin builtin, int function_index);
 private:
  static std::array<int, kNumBuiltins> builtin_indices_;
};
```

### Component 2: Minimal Core Builtins
**File:** `deps/v8/src/builtins/wasm32/builtins-wasm32.cc`

**Implementation Requirements:**
- Each builtin compiles to a proper wasm function
- Registers itself in the dispatch table during initialization
- Provides minimal functionality for basic JS execution

**Core Builtins:**
```cpp
void Generate_JSEntry(MacroAssembler* masm);
void Generate_CEntry(MacroAssembler* masm);
void Generate_InterpreterEntryTrampoline(MacroAssembler* masm);
void Generate_RecordWrite(MacroAssembler* masm);
void Generate_EnterContext(MacroAssembler* masm);
```

### Component 3: Minimal Ignition Interpreter
**Files:** `deps/v8/src/interpreter/interpreter.cc` (wasm32 additions)

**Scope:**
- Implement first 50 bytecode handlers
- Focus on: LdaGlobal, Star, Add, Sub, Mul, Return, Call
- Skip advanced bytecodes (debugger, generators, async)

### Component 4: Bootstrap Fix
**File:** `src/node.cc` (InitializePrimordials fix)

**Approach:**
- Simplify primordial object creation
- Avoid complex prototype chains during bootstrap
- Use minimal object setup that works with stub builtins

### Component 5: Build Integration
**Files:** 
- `out/tools/v8_gypfiles/v8_base_without_compiler.target.mk`
- `out/tools/v8_gypfiles/v8_initializers.target.mk`

**Changes:**
- Add `builtin-dispatch-wasm32.o` to object list
- Ensure proper wasm function registration
- Update link flags for function table access

## Implementation Strategy

### Phase 1: Foundation (Week 1)
**Goal:** Create builtin dispatch infrastructure

**Tasks:**
1. Implement `BuiltinDispatchTable` class
2. Add `GetBuiltinFunction(Builtin)` helper
3. Hook into existing `Builtins::CallBuiltin()` machinery
4. Replace `code->instruction_start()` with dispatch table lookup
5. Update `GeneratedCode::Call()` to use function table indices

**Success Criteria:** No crashes during builtin registration

### Phase 2: Core Builtins (Week 2)
**Goal:** Implement 5 critical builtins

**Tasks:**
1. Implement `JSEntry` - minimal JS→C++ boundary
2. Implement `CEntry` - C++→JS boundary with return handling
3. Implement `InterpreterEntryTrampoline` - bridge to bytecode executor
4. Implement `RecordWrite` - stubbed write barrier (no-op for basic JS)
5. Implement `EnterContext` - minimal context setup
6. Replace empty stubs in `builtins-wasm32.cc`
7. Ensure each builtin compiles to wasm function

**Success Criteria:** Builtins execute without traps

### Phase 3: Minimal Interpreter (Week 3-4)
**Goal:** Basic bytecode execution

**Tasks:**
1. Implement first 50 bytecode handlers
2. Focus on essential operations (variables, math, function calls)
3. Skip advanced features (generators, try-catch, async)
4. Use existing Ignition infrastructure with disabled paths
5. Test basic JS expressions (`1+1`, variable assignment)

**Success Criteria:** Simple JS expressions work

### Phase 4: Integration & Testing (Week 5)
**Goal:** Full system validation

**Tasks:**
1. Update build system (`v8_base_without_compiler.target.mk`)
2. Ensure proper wasm function registration
3. Test incremental rebuild process
4. Validate with `node.wasm --version`
5. Progress to `node.wasm -e "console.log('test')"`
6. Test user's actual CLI tool

**Success Criteria:** User's CLI tool runs successfully

## Testing Strategy

### Level 1: Incremental Verification
- Test each component immediately after implementation
- Use existing V8 test infrastructure where possible
- Create wasm32-specific tests for new functionality

### Level 2: Builtin Testing
```javascript
// tests/wasm32/builtins_test.js
assert(builtinExists('JSEntry'), 'JSEntry builtin exists');
assert(builtinExists('CEntry'), 'CEntry builtin exists');
assert(builtinExists('InterpreterEntryTrampoline'), 'InterpreterEntryTrampoline exists');
```

### Level 3: Integration Testing
1. `node.wasm --version` (no JS execution)
2. `node.wasm -e "console.log('test')"` (basic JS)
3. `node.wasm -e "1 + 1 === 2"` (arithmetic)
4. User's actual CLI tool

### Level 4: Memory Safety
- Run with wasmer's strict memory checking
- Add debug logging to builtin dispatch
- Verify no out-of-bounds table accesses

### Level 5: Regression Prevention
- Add tests to `test/wasi/wasm32-builtins.test.js`
- Document hand-edited build files
- Ensure rebuild process works

## Success Criteria

1. ✅ Builtin dispatch table works (no registration crashes)
2. ✅ Core 5 builtins execute (can call without traps)
3. ✅ Simple JS expressions work (`1+1`, basic math)
4. ✅ Bootstrap completes (no Map::SetPrototype crashes)
5. ✅ User's CLI tool runs successfully

## Fallback Strategy

If blockers occur:
- Simplify approach further (fewer builtins, more stubs)
- Skip certain features and work around them
- Focus on exact JS operations user's CLI tool needs
- Consider hybrid approach (some stubs, some implementations)

## Risks & Mitigations

**Risk 1:** V8 architecture may not support minimal builtin approach  
**Mitigation:** Start with simplest possible implementation, iterate

**Risk 2:** Build system complexity increases significantly  
**Mitigation:** Document all changes, use incremental build testing

**Risk 3:** Unknown dependencies in bootstrap code  
**Mitigation:** Test bootstrap incrementally, simplify as needed

**Risk 4:** Memory access issues in wasm32 environment  
**Mitigation:** Use wasmer strict checking, add bounds checking

## Timeline Estimate

- **Week 1:** Foundation (builtin dispatch)
- **Week 2:** Core builtins implementation  
- **Week 3-4:** Minimal interpreter
- **Week 5:** Integration & testing

**Total:** 5 weeks to basic JavaScript execution

## Next Steps

1. Get user approval of this design
2. Create detailed implementation plan
3. Begin Phase 1 implementation
4. Iterate based on testing feedback