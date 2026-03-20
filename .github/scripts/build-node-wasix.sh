#!/bin/sh

# Node.js WebAssembly (WASI/Wasix) Cross-Compilation Build Script
#
# Builds Node.js targeting wasm32-wasi using the WASI SDK and Wasix sysroot.
# Expects the following environment variables to be set (e.g. by CI):
#   WASI_SDK_PATH  - path to the installed WASI SDK directory
#   WASIX_SYSROOT  - path to the Wasix sysroot
#   CC, CXX, AR, RANLIB - compiler/tool overrides (optional)
#
# Usage: ./build-node-wasix.sh
# Output: WebAssembly binary at out/Release/node

set -eu

echo "Building Node.js for WebAssembly (WASI/Wasix) - Cross-compilation"

# Check if we're in the right directory
if [ ! -f "configure.py" ]; then
    echo "Error: Must run this from the Node.js source directory"
    exit 1
fi

# Resolve WASI SDK path
if [ -z "${WASI_SDK_PATH:-}" ]; then
    echo "Error: WASI_SDK_PATH is not set. Please set it to the WASI SDK directory."
    exit 1
fi

if [ -z "${WASIX_SYSROOT:-}" ]; then
    echo "Error: WASIX_SYSROOT is not set. Please set it to the Wasix sysroot directory."
    exit 1
fi

# Set compiler defaults if not already provided via environment
: "${CC:=${WASI_SDK_PATH}/bin/clang --target=wasm32-wasi --sysroot=${WASIX_SYSROOT}}"
: "${CXX:=${WASI_SDK_PATH}/bin/clang++ --target=wasm32-wasi --sysroot=${WASIX_SYSROOT}}"
: "${AR:=${WASI_SDK_PATH}/bin/llvm-ar}"
: "${RANLIB:=${WASI_SDK_PATH}/bin/llvm-ranlib}"

export CC CXX AR RANLIB

echo "Using WASI_SDK_PATH=${WASI_SDK_PATH}"
echo "Using WASIX_SYSROOT=${WASIX_SYSROOT}"
echo "All required tools found!"

echo "Configuring Node.js for wasm32 cross-compilation..."
./configure \
    --dest-cpu=wasm32 \
    --dest-host=wasm32 \
    --cross-compiling \
    --without-inspector \
    --without-intl \
    --without-ssl

echo "Building Node.js as WebAssembly..."
NPROC=$(nproc)
make -j"${NPROC}"

# Check if build succeeded
if [ -f "out/Release/node" ]; then
    echo "Build successful!"
    echo "WebAssembly binary created: out/Release/node"
    file out/Release/node
else
    echo "Build failed - no output binary found"
    ls -la out/Release/ || true
    exit 1
fi

echo "Done!"
