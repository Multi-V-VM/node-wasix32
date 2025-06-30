{
  'targets': [
    {
      'target_name': 'simdutf',
      'type': 'static_library',
      'sources': [
        'simdutf_part1.cpp',
        'simdutf_part2.cpp',
        'simdutf_part3.cpp',
        'simdutf_part4.cpp',
      ],
      'conditions': [
        ['target_arch=="wasm32"', {
          'cflags': [
            '-Os',  # Optimize for size
            '-ffunction-sections',
            '-fdata-sections',
          ],
          'ldflags': [
            '-Wl,--gc-sections',
          ],
        }],
      ],
    }
  ]
}
