// WASI-specific stubdata symbols to satisfy linker requirements
#ifdef __wasi__
extern "C" {
  // Provide minimal stub symbols for WASI builds
  const unsigned char icudt77_dat[1] = {0};
  const unsigned int icudt77_dat_size = 0;
}
#endif