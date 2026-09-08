#!/usr/bin/env bash
set -euo pipefail

root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
cache_dir=${WASMER_CACHE_DIR:-"${XDG_CACHE_HOME:-$HOME/.cache}/node-wasix32/wasmer"}
output=$(mktemp)
trap 'rm -f "$output"' EXIT

cd "$root"
WASM32_AOT_STATS=1 WASMER_CACHE_DIR="$cache_dir" \
  tools/wasmer-aot-run.sh \
  /workspace/test/wasm32/test-ignition-loop-aot.js >"$output" 2>&1

cat "$output"
rg -q '^WASM32_AOT_STATS plans=[1-9][0-9]* completed=[1-9][0-9]* bailouts=[0-9]+$' "$output"
