#!/bin/bash

set -xe

if [[ -z "${WASI_SDK_PATH}" ]]; then
    echo "WASI_SDK_PATH environment variable is not set."
    exit 1
fi

if [[ -z "${WASIX_SYSROOT}" ]]; then
    echo "WASIX_SYSROOT environment variable is not set. Using the default"
    exit 1
fi

export RANLIB="$WASI_SDK_PATH/bin/ranlib"
export AR="$WASI_SDK_PATH/bin/ar"
export NM="$WASI_SDK_PATH/bin/nm"
export CC="$WASI_SDK_PATH/bin/clang"
export CXX="$WASI_SDK_PATH/bin/clang"
export CFLAGS="\
--sysroot=$WASIX_SYSROOT \
--target=wasm32-wasi \
-matomics \
-mbulk-memory \
-mmutable-globals \
-pthread \
-mthread-model posix \
-ftls-model=local-exec \
-fno-trapping-math \
-D_WASI_EMULATED_MMAN \
-D_WASI_EMULATED_SIGNAL \
-D_WASI_EMULATED_PROCESS_CLOCKS \
-DUSE_TIMEGM \
-DOPENSSL_NO_SECURE_MEMORY \
-DOPENSSL_NO_DGRAM \
-DOPENSSL_THREADS \
-O3 \
-g \
-flto"
LDFLAGS="\
-Wl,--shared-memory \
-Wl,--max-memory=4294967296 \
-Wl,--import-memory \
-Wl,--export-dynamic \
-Wl,--export=__heap_base \
-Wl,--export=__stack_pointer \
-Wl,--export=__data_end \
-Wl,--export=__wasm_init_tls \
-Wl,--export=__wasm_signal \
-Wl,--export=__tls_size \
-Wl,--export=__tls_align \
-Wl,--export=__tls_base"

./Configure no-shared no-pic threads no-asm no-tests no-apps no-afalgeng -DUSE_TIMEGM -DOPENSSL_NO_SECURE_MEMORY -DOPENSSL_NO_DGRAM -DOPENSSL_THREADS

make -j4

$RANLIB libcrypto.a
$RANLIB libssl.a