#!/usr/bin/env python3
"""
Convert V8's embedded.S (assembly format) to embedded.cc (C format) for WASM compilation.
"""

import sys


def parse_octa_value(octa_str):
    """Parse a single .octa value (128-bit hex) and return 16 bytes."""
    # Remove leading/trailing whitespace and 0x prefix if present
    octa_str = octa_str.strip()
    if octa_str.startswith('0x'):
        octa_str = octa_str[2:]

    # Pad to 32 hex digits (128 bits)
    octa_str = octa_str.zfill(32)

    # Convert to bytes (little-endian for x86/wasm32)
    bytes_list = []
    for i in range(0, 32, 2):
        byte_val = int(octa_str[-(i+2):len(octa_str)-i] if i > 0 else octa_str[-2:], 16)
        bytes_list.append(byte_val)

    return bytes_list[:16]  # Return first 16 bytes


def parse_embedded_s(input_file):
    """Parse embedded.S and extract data sections."""

    current_section = None
    symbols = {}  # name -> (section, offset)

    data_bytes = []
    code_bytes = []

    with open(input_file, 'r') as f:
        for line in f:
            line = line.strip()

            # Skip comments and empty lines
            if not line or line.startswith('//'):
                continue

            # Handle section directives
            if line == '.section .rodata':
                current_section = 'data'
                continue
            elif line == '.section .text' or '.section .text.' in line:
                current_section = 'code'
                continue

            # Handle alignment
            if line.startswith('.balign'):
                align = int(line.split()[1])
                if current_section == 'data':
                    while len(data_bytes) % align != 0:
                        data_bytes.append(0)
                elif current_section == 'code':
                    while len(code_bytes) % align != 0:
                        code_bytes.append(0)
                continue

            # Handle global symbols
            if line.startswith('.global '):
                continue
            if line.startswith('.hidden '):
                continue

            # Handle labels
            if line.endswith(':'):
                label = line[:-1].strip()
                if current_section == 'data':
                    symbols[label] = ('data', len(data_bytes))
                elif current_section == 'code':
                    symbols[label] = ('code', len(code_bytes))
                continue

            # Handle .octa directives (128-bit values)
            if line.startswith('.octa '):
                values_str = line[6:]
                # Split by comma, handling multiple values per line
                for val in values_str.split(','):
                    val = val.strip()
                    if val:
                        bytes_val = parse_octa_value(val)
                        if current_section == 'data':
                            data_bytes.extend(bytes_val)
                        elif current_section == 'code':
                            code_bytes.extend(bytes_val)
                continue

            # Handle .byte directives
            if line.startswith('.byte '):
                values = line[6:].split(',')
                for v in values:
                    v = v.strip()
                    if v:
                        if v.startswith('0x'):
                            byte_val = int(v, 16)
                        else:
                            byte_val = int(v)
                        if current_section == 'data':
                            data_bytes.append(byte_val)
                        elif current_section == 'code':
                            code_bytes.append(byte_val)
                continue

            # Handle .long directives (32-bit values)
            if line.startswith('.long '):
                values = line[6:].split(',')
                for v in values:
                    v = v.strip()
                    if v:
                        if v.startswith('0x'):
                            val = int(v, 16)
                        else:
                            val = int(v)
                        # Little-endian
                        for i in range(4):
                            if current_section == 'data':
                                data_bytes.append((val >> (i * 8)) & 0xFF)
                            elif current_section == 'code':
                                code_bytes.append((val >> (i * 8)) & 0xFF)
                continue

            # Handle .quad directives (64-bit values)
            if line.startswith('.quad '):
                values = line[6:].split(',')
                for v in values:
                    v = v.strip()
                    if v:
                        if v.startswith('0x'):
                            val = int(v, 16)
                        else:
                            val = int(v)
                        # Little-endian
                        for i in range(8):
                            if current_section == 'data':
                                data_bytes.append((val >> (i * 8)) & 0xFF)
                            elif current_section == 'code':
                                code_bytes.append((val >> (i * 8)) & 0xFF)
                continue

            # Skip other directives
            if line.startswith('.'):
                continue

    return data_bytes, code_bytes, symbols


def write_cc_file(output_file, data_bytes, code_bytes, symbols):
    """Write the C++ output file."""

    with open(output_file, 'w') as f:
        f.write('// Autogenerated file. Do not edit.\n')
        f.write('// Converted from embedded.S for WASM compilation.\n\n')
        f.write('#include <cstdint>\n\n')
        f.write('extern "C" {\n\n')

        # Write data section
        if data_bytes:
            f.write('// Embedded blob data section\n')
            f.write('__attribute__((used, visibility("default"))) alignas(8) extern const uint8_t v8_Default_embedded_blob_data_[] = {\n')
            for i, byte in enumerate(data_bytes):
                if i % 16 == 0:
                    f.write('  ')
                f.write(f'0x{byte:02x},')
                if i % 16 == 15 or i == len(data_bytes) - 1:
                    f.write('\n')
            f.write('};\n')
            f.write(f'__attribute__((used, visibility("default"))) uint32_t v8_Default_embedded_blob_data_size_ = {len(data_bytes)};\n\n')
        else:
            f.write('__attribute__((used, visibility("default"))) const uint8_t v8_Default_embedded_blob_data_[1] = {{0}};\n')
            f.write('__attribute__((used, visibility("default"))) uint32_t v8_Default_embedded_blob_data_size_ = 0;\n\n')

        # Write code section
        if code_bytes:
            f.write('// Embedded blob code section\n')
            f.write('__attribute__((used, visibility("default"))) alignas(32) extern const uint8_t v8_Default_embedded_blob_code_[] = {\n')
            for i, byte in enumerate(code_bytes):
                if i % 16 == 0:
                    f.write('  ')
                f.write(f'0x{byte:02x},')
                if i % 16 == 15 or i == len(code_bytes) - 1:
                    f.write('\n')
            f.write('};\n')
            f.write(f'__attribute__((used, visibility("default"))) uint32_t v8_Default_embedded_blob_code_size_ = {len(code_bytes)};\n\n')
        else:
            f.write('__attribute__((used, visibility("default"))) const uint8_t v8_Default_embedded_blob_code_[1] = {{0}};\n')
            f.write('__attribute__((used, visibility("default"))) uint32_t v8_Default_embedded_blob_code_size_ = 0;\n\n')

        f.write('}  // extern "C"\n')


def main():
    if len(sys.argv) < 3:
        print(f"Usage: {sys.argv[0]} <input.S> <output.cc>", file=sys.stderr)
        sys.exit(1)

    input_file = sys.argv[1]
    output_file = sys.argv[2]

    print(f"Converting {input_file} to {output_file}...")

    data_bytes, code_bytes, symbols = parse_embedded_s(input_file)

    print(f"  Data section: {len(data_bytes)} bytes")
    print(f"  Code section: {len(code_bytes)} bytes")
    print(f"  Symbols: {len(symbols)}")

    write_cc_file(output_file, data_bytes, code_bytes, symbols)

    print(f"Done. Output written to {output_file}")


if __name__ == '__main__':
    main()
