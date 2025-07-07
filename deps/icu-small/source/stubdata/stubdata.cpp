// Force empty compilation to avoid section size issues
#if 0
extern "C" {
  const unsigned char icudt77_dat[] = {
    0x27, 0x00, 0xDA, 0x27,  // ICU data header magic
    0x00, 0x00, 0x00, 0x00,  // Header size
    0x00, 0x00, 0x00, 0x02,  // Data format
    0x00, 0x00, 0x00, 0x00   // Minimal data
  };
  const unsigned int icudt77_dat_size = sizeof(icudt77_dat);
}
#endif
