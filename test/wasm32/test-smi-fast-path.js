'use strict';

const assert = require('node:assert/strict');

function add(a, b) { return a + b; }
function addImmediate(a) { return a + 7; }
function sub(a, b) { return a - b; }
function subImmediate(a) { return a - 7; }
function mul(a, b) { return a * b; }
function mulImmediate(a) { return a * 7; }
function bitwise(a, b) {
  return [a | b, a ^ b, a & b, a << b, a >> b, a >>> b];
}
function bitwiseImmediate(a) {
  return [a | 3, a ^ 3, a & 3, a << 3, a >> 3, a >>> 3];
}

assert.equal(add(20, 22), 42);
assert.equal(addImmediate(35), 42);
assert.equal(sub(50, 8), 42);
assert.equal(subImmediate(49), 42);
assert.equal(mul(6, 7), 42);
assert.equal(mulImmediate(6), 42);
assert.deepEqual(bitwise(42, 2), [42, 40, 2, 168, 10, 10]);
assert.deepEqual(bitwiseImmediate(42), [43, 41, 2, 336, 5, 5]);

assert.equal(add('node', 32), 'node32');
assert.equal(add(1_073_741_823, 1), 1_073_741_824);
assert.equal(sub(-1_073_741_824, 1), -1_073_741_825);
assert.equal(mul(1_073_741_823, 2), 2_147_483_646);
assert.ok(Object.is(mul(-1, 0), -0));

console.log('WASM32_SMI_FAST_PATH_TEST_PASS');
