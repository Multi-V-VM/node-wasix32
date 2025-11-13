#!/usr/bin/env bash
# Helper script run by the workflow to configure and build Node.js for wasm32-wasi / wasix.
# IMPORTANT:
# - This script is intentionally generic: compiling Node.js into a single usable wasm binary
#   typically requires repository-specific patches (V8 build changes, disabling JIT, snapshotting,
#   changes to the Node build system, and sometimes a custom embedder).
# - Adjust configure/build steps below to match your repository's build system and any patches.
#
# This script:
# - ensures an output folder
# - sets up a sane environment for a wasi-sdk-based cross-compile
# - runs configure + build commands used by Node's upstream build when possible
# - collects artifacts into out/

set -euo pipefail
shopt -s globstar nullglob

ROOT="$(pwd)"
OUT_DIR="${OUT_DIR:-${ROOT}/out}"
WASI_SDK_PATH="${WASI_SDK_PATH:-${ROOT}/wasi-sdk-${WASI_SDK_VERSION:-25.0}}"

mkdir -p "$OUT_DIR"
echo "Build root: $ROOT"
echo "Out dir: $OUT_DIR"
echo "WASI_SDK_PATH: $WASI_SDK_PATH"

# Validate expected tools
if [ ! -x "${WASI_SDK_PATH}/bin/clang" ]; then
  echo "ERROR: wasi-sdk clang not found at ${WASI_SDK_PATH}/bin/clang"
  echo "Set WASI_SDK_PATH or include wasi-sdk in your PATH."
  exit 1
fi

# Recommended compiler wrappers (you may prefer to set CC/CXX exactly in Actions step)
export CC="${WASI_SDK_PATH}/bin/clang --target=wasm32-wasi --sysroot=${WASI_SDK_PATH}/share/wasi-sysroot"
export CXX="${WASI_SDK_PATH}/bin/clang++ --target=wasm32-wasi --sysroot=${WASI_SDK_PATH}/share/wasi-sysroot"
export AR="${WASI_SDK_PATH}/bin/llvm-ar"
export RANLIB="${WASI_SDK_PATH}/bin/llvm-ranlib"

echo "Using CC: $CC"
echo "Using CXX: $CXX"
echo "Using AR: $AR"

# Optional: tweak LDFLAGS to produce a standalone wasm module.
# Many builds require -Wl,--no-entry and to export specific symbols used by your embedder.
# Adjust these flags to match your port's requirements.
export LDFLAGS="${LDFLAGS:-'-Wl,--no-entry -Wl,--export-all -Wl,--allow-undefined'}"

# If your repo contains a separate build/ directory or a patched Node source tree, point to it.
# This script assumes Node's top-level configure exists. If your port uses GN/Ninja for V8, you may need
# to invoke those steps directly.
if [ -f "./configure" ]; then
  echo "Found configure script; attempting to run configure for a wasm build (may require patches)."
  # Example configure flags - adjust or remove as needed.
  # Common useful flags for experimental wasm builds:
  #  --without-inspector  : remove inspector support
  #  --without-intl       : skip ICU to simplify build
  #  --without-ssl        : skip openssl if not needed (note: many Node APIs depend on TLS)
  #  --dest-cpu=wasm32    : if supported by the configure script
  #  --prefix              : where 'make install' will place files
  #
  # Note: Upstream Node's configure may not support wasm32; many ports require a custom configure wrapper.
  ./configure \
    --prefix="$OUT_DIR/install" \
    --without-inspector \
    --without-intl \
    --without-ssl || {
      echo "Configure failed — configure may not support wasm32 target without port-specific changes."
      echo "If configure fails, edit this script to call your port's configure/build steps (GN/Ninja for V8, etc.)."
    }

  echo "Starting make build (this may take a long time)"
  # Use the number of available processors
  MAKEFLAGS="-j$(nproc)" make ${MAKEFLAGS:-} || {
    echo "make failed — building Node for wasm usually needs patches; check logs for compiler errors."
    # continue so we can collect logs/artifacts for debugging
  }

  echo "Attempting 'make install' into $OUT_DIR/install (may not produce wasm binary directly)"
  make install || true
else
  echo "No top-level configure found. Your port may use a different build pipeline (GN/Ninja, custom scripts)."
  echo "Look for README or build instructions in your repo and adapt this script to call them."
fi

# If GN/Ninja is used for V8 or a custom build, here is an example of building with ninja:
# (uncomment and adapt if your port has a out/wasm/ directory with build.ninja)
#
# if [ -d "out/wasm" ]; then
#   echo "Found out/wasm — building with ninja"
#   ninja -C out/wasm || true
# fi

# Link the final executable
echo "Linking final node.wasm executable..."
"$CXX" -o "$OUT_DIR/node.wasm" "out/Release/obj.target/node/src/node_main.o" "out/Release/libnode.a" -Wl,--start-group -Wl,--whole-archive "out/Release/libnode.a" -Wl,--no-whole-archive -Wl,--end-group -ldl -lrt -lm -lpthread

# Collect likely wasm artifacts (wildcard search) into out/
echo "Collecting wasm artifacts and build logs..."
for f in **/*.wasm; do
  mkdir -p "$OUT_DIR/wasm"
  cp "$f" "$OUT_DIR/wasm/" || true
done

# Also copy build directories (useful for debugging)
if [ -d "build" ]; then
  rsync -a build/ "$OUT_DIR/build/" || true
fi
if [ -d "out" ]; then
  rsync -a out/ "$OUT_DIR/build-out/" || true
fi

echo "Build script finished. Inspect $OUT_DIR for artifacts. If build failed, examine the build logs above."
echo "Notes / next steps:"
echo " - Most Node->WASM ports require V8 changes (disable JIT or use an interpreter build), snapshotting, and a custom embedder."
echo " - If you have patch files or a README describing custom build steps, incorporate those into this script or call them from the workflow."
