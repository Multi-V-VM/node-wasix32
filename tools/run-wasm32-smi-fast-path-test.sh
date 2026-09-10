#!/usr/bin/env bash
set -euo pipefail

root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
output=$(mktemp)
trap 'rm -f "$output"' EXIT

cd "$root"
WASM32_SMI_FAST_PATH_STATS=1 WASM32_DISABLE_LOOP_AOT=1 \
  tools/wasmer-aot-run.sh \
  /workspace/test/wasm32/test-smi-fast-path.js >"$output" 2>&1

cat "$output"
rg -q '^WASM32_SMI_FAST_PATH_TEST_PASS$' "$output"
for operation in add sub mul bitwise; do
  rg -q "^WASM32_SMI_FAST_PATH operation=$operation$" "$output"
done

