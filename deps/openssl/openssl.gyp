{
  'variables': {
    'gas_version%': '0.0',
    'llvm_version%': '0.0',
    'nasm_version%': '0.0',
    'conditions': [
      ['GENERATOR == "ninja"', {
        'modules_dir': '<(PRODUCT_DIR_ABS_CSTR)/obj/lib/openssl-modules',
      }, {
        'modules_dir': '<(PRODUCT_DIR_ABS_CSTR)/obj.target/deps/openssl/lib/openssl-modules',
      }],
      ['OS=="mac"', {
        'modules_dir': '<(PRODUCT_DIR_ABS_CSTR)/obj.target/deps/openssl/lib/openssl-modules',
      }],
    ],
  },
  'targets': [
    {
      'target_name': 'openssl',
      'type': '<(library)',
      'includes': ['./openssl.gypi', './openssl_common.gypi'],
      'sources': [ '<@(openssl_sources)' ],
      'defines': [
        # Compile out hardware engines.  Most are stubs that dynamically load
        # the real driver but that poses a security liability when an attacker
        # is able to create a malicious DLL in one of the default search paths.
        'OPENSSL_NO_HW',
        'OPENSSL_API_COMPAT=0x10100001L',
        'STATIC_LEGACY',
        #'OPENSSL_NO_DEPRECATED',
      ],
      'conditions': [
        [ 'openssl_no_asm==1 or OS=="wasi"', {
          'includes': ['./openssl_no_asm.gypi'],
          'sources+': [ '<@(openssl_sources_no_asm)' ],
        }, 'target_arch=="arm64" and OS=="win"', {
          # VC-WIN64-ARM inherits from VC-noCE-common that has no asms.
          'includes': ['./openssl_no_asm.gypi'],
          'sources+': [ '<@(openssl_sources_no_asm)' ],
        }, 'gas_version and v(gas_version) >= v("2.26") or '
           'nasm_version and v(nasm_version) >= v("2.11.8")', {
           # Require AVX512IFMA supported. See
           # https://www.openssl.org/docs/man1.1.1/man3/OPENSSL_ia32cap.html
           # Currently crypto/poly1305/asm/poly1305-x86_64.pl requires AVX512IFMA.
        #   'includes': ['./openssl_asm.gypi'],
        }, {
        #   'includes': ['./openssl_asm_avx2.gypi'],
        }],
        ['node_shared_openssl=="false"', {
          'defines': [
            'MODULESDIR="<(modules_dir)"',
          ]
        }],
      ],
      'direct_dependent_settings': {
        'include_dirs': [ 'openssl/include', 'openssl/crypto/include']
      }
    }, {
      # openssl-fipsmodule target
      'target_name': 'openssl-fipsmodule',
      'type': 'shared_library',
      'includes': ['./openssl_common.gypi'],
      'include_dirs+': ['openssl/apps/include'],
      'cflags': [ '-fPIC' ],
      #'ldflags': [ '-o', 'fips.so' ],
      #'ldflags': [ '-Wl,--version-script=providers/fips.ld',],
      'conditions': [
     ],
     'direct_dependent_settings': {
       'include_dirs': [ 'openssl/include', 'openssl/crypto/include']
     }
   },
  ]
}