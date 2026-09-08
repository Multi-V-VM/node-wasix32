#!/usr/bin/env bash
set -euo pipefail

root=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")/.." && pwd)
cache_dir=${WASMER_CACHE_DIR:-"${XDG_CACHE_HOME:-$HOME/.cache}/node-wasix32/wasmer"}
compiler=${WASMER_COMPILER:-cranelift}

case "$compiler" in
  cranelift) compiler_flag=--cranelift ;;
  llvm) compiler_flag=--llvm ;;
  singlepass) compiler_flag=--singlepass ;;
  v8) compiler_flag=--v8 ;;
  *)
    printf 'Unsupported WASMER_COMPILER: %s\n' "$compiler" >&2
    exit 2
    ;;
esac

mkdir -p -- "$cache_dir"
runtime_env=()
for name in WASM32_AOT_STATS WASM32_AOT_BENCHMARK WASM32_DISABLE_LOOP_AOT; do
  if [[ -v $name ]]; then
    runtime_env+=(--env "$name=${!name}")
  fi
done

exec wasmer run \
  --cache-dir "$cache_dir" \
  "$compiler_flag" \
  --mapdir "/workspace:$PWD" \
  "${runtime_env[@]}" \
  "$root/out/Release/node" \
  -- "$@"
