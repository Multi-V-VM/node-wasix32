Notes for WASIX platforms
=============================

This guide has been tested on Linux (on a Debian distrib but should work on other distrib too). It should work on macOS with no modification. If you are on Windows, you probably want to take a look at WSL to have a linux environnement.

Building a static version of openssl in WASIX/Wasm32 is possible
using the wasix sysroot and recent version of LLVM/Clang that can 
output Wasm32 code.

Once you installed a working version of clang (for example v14), you need 
to build [wasix-libc](https://github.com/wasix-org/wasix-libc) sysroot, and have the "sysroot32" folder available

You can configure the project with this command line:
```
LD=llvm-ld-15 AR=llvm-ar-15 NM=llvm-nm-15 CC="clang-15 --target=wasm32-wasi" CFLAGS="--sysroot=/PATH/TO/sysroot32 -matomics -mbulk-memory -mmutable-globals -pthread -mthread-model posix -ftls-model=local-exec -fno-trapping-math -D_WASI_EMULATED_MMAN -D_WASI_EMULATED_SIGNAL -D_WASI_EMULATED_PROCESS_CLOCKS " LDFLAGS="-Wl,--shared-memory -Wl,--max-memory=4294967296 -Wl,--import-memory -Wl,--export-dynamic -Wl,--export=__heap_base -Wl,--export=__stack_pointer -Wl,--export=__data_end -Wl,--export=__wasm_init_tls -Wl,--export=__wasm_signal -Wl,--export=__tls_size -Wl,--export=__tls_align -Wl,--export=__tls_base" ./Configure -static no-asm no-tests no-apps no-afalgeng -DUSE_TIMEGM -DOPENSSL_NO_SECURE_MEMORY -DOPENSSL_NO_DGRAM -DOPENSSL_THREADS
```

That's a quite huge command line. You need to adapt the `--sysroot` parameter to the actual path where your wasix sysroot32 is.
Using the `-Wl` argument in the `CC` variable will trigger some anoying warning, but it should be fine.

The `Configure` should run successfully. So a simple `make` (or `make -j4` if you are impaciant) will build the `libssl.a` and `libcrypto.a` in the current folder.

Those library can then be used to link with other WASIX/wasm32 program, like CURL..
But before they are usable, you need to manualy run ranlib on them (not sure why it's not done automaticaly):
```
llvm-ranlib-15 libcrypto.a
llvm-ranlib-15 libsll.a
```
Libs are now ready to be used on other WASIX projects
