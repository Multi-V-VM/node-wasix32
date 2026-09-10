#!/usr/bin/env node

import assert from 'node:assert/strict';
import fs from 'node:fs';

const [baselinePath, candidatePath, benchmarkName = 'text', minimum = '1.05'] =
  process.argv.slice(2);
assert.ok(baselinePath && candidatePath, 'usage: compare <baseline> <candidate> [name] [minimum]');

function median(values) {
  const sorted = [...values].sort((a, b) => a - b);
  return sorted[Math.floor(sorted.length / 2)];
}

function readResult(path) {
  const results = fs.readFileSync(path, 'utf8')
    .split('\n')
    .filter((line) => line.startsWith('{'))
    .map((line) => JSON.parse(line));
  const result = results.find(({ name }) => name === benchmarkName);
  assert.ok(result, `missing benchmark ${benchmarkName} in ${path}`);
  return median(result.elapsed_ns);
}

const baseline = readResult(baselinePath);
const candidate = readResult(candidatePath);
const speedup = baseline / candidate;
console.log(JSON.stringify({ benchmarkName, baseline, candidate, speedup }));
assert.ok(speedup >= Number(minimum),
          `expected ${benchmarkName} speedup >= ${minimum}x, got ${speedup.toFixed(3)}x`);
