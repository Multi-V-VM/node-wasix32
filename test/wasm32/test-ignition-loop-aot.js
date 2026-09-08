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

const benchmark = process.env.WASM32_AOT_BENCHMARK === '1';

if (!benchmark) assert.equal(sumTo(1_000_000), 1_783_293_664);
assert.equal(sumTo(-1), 0);
assert.equal(sumTo(65_537), -2_147_450_880);
assert.equal(rejectedByCall(10), 45);

if (benchmark) {
  const start = process.hrtime.bigint();
  const result = sumTo(1_000_000);
  const elapsedNs = process.hrtime.bigint() - start;
  console.log(`WASM32_AOT_BENCH result=${result} elapsed_ns=${elapsedNs}`);
}
