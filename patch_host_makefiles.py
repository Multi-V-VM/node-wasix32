#!/usr/bin/env python3
"""
Patch GYP-generated host makefiles to use correct architecture (arm64 instead of wasm32).

This script is necessary because when Node.js is configured with --dest-cpu=wasm32,
GYP applies the V8_TARGET_ARCH_WASM32 flag to both target and host builds.
Host builds (torque, mksnapshot, etc.) should use the native architecture (arm64)
to avoid including WASI-specific stubs that are incomplete for the host.
"""

import os
import re
import sys

def patch_makefile(filepath):
    """Replace V8_TARGET_ARCH_WASM32 with V8_TARGET_ARCH_ARM64 in a makefile."""
    with open(filepath, 'r') as f:
        content = f.read()

    original_content = content
    # Replace the WASM32 arch flag with ARM64
    content = content.replace("'-DV8_TARGET_ARCH_WASM32'", "'-DV8_TARGET_ARCH_ARM64'")

    # Add the 64_BIT flag if ARM64 is defined but 64_BIT is not
    # We need to handle both DEFS_Debug and DEFS_Release sections
    if "'-DV8_TARGET_ARCH_ARM64'" in content:
        # Replace all occurrences of ARM64 with ARM64 + 64_BIT, but avoid double-adding
        lines = content.split('\n')
        new_lines = []
        for i, line in enumerate(lines):
            new_lines.append(line)
            # If this line has ARM64 and the next line doesn't have 64_BIT, add it
            if "'-DV8_TARGET_ARCH_ARM64'" in line and (i+1 >= len(lines) or "'-DV8_TARGET_ARCH_64_BIT'" not in lines[i+1]):
                # Add the 64_BIT flag on the next line
                indent = '\t' if line.startswith('\t') else ''
                new_lines.append(f"{indent}'-DV8_TARGET_ARCH_64_BIT' \\")
        content = '\n'.join(new_lines)

    if content != original_content:
        with open(filepath, 'w') as f:
            f.write(content)
        return True
    return False

def patch_root_makefile():
    """Patch the root Makefile to use native ar for host builds and wasm-ld for target."""
    makefile_path = 'out/Makefile'

    if not os.path.exists(makefile_path):
        return False

    with open(makefile_path, 'r') as f:
        content = f.read()

    original_content = content

    # Find and replace cmd_alink to conditionally use ar or wasm-ld based on TOOLSET
    old_alink = (
        "cmd_alink = rm -f $@ && /opt/wasi-sdk/bin/wasm-ld -static -o $@ $(filter %.o,$^)"
    )
    new_alink = (
        "cmd_alink = rm -f $@; "
        "if [ '$(TOOLSET)' = 'host' ]; then "
        "$(AR.host) rcs $@ $(filter %.o,$^); "
        "else "
        "/opt/wasi-sdk/bin/wasm-ld -static -o $@ $(filter %.o,$^); "
        "fi"
    )

    if old_alink in content:
        content = content.replace(old_alink, new_alink)
        with open(makefile_path, 'w') as f:
            f.write(content)
        return True

    return False

def main():
    makefile_dir = 'out/tools/v8_gypfiles'

    if not os.path.isdir(makefile_dir):
        print(f"Error: {makefile_dir} directory not found")
        sys.exit(1)

    patched_count = 0
    for filename in os.listdir(makefile_dir):
        if filename.endswith('.host.mk'):
            filepath = os.path.join(makefile_dir, filename)
            if patch_makefile(filepath):
                patched_count += 1
                print(f"✅ Patched {filename}")

    print(f"\n✅ Successfully patched {patched_count} host makefile(s)")

    # Also patch the root Makefile for alink command
    if patch_root_makefile():
        print(f"✅ Patched root Makefile alink command")

    # Also patch torque.host.mk specifically to add C++ library linking
    torque_mk = os.path.join(makefile_dir, 'torque.host.mk')
    if os.path.exists(torque_mk):
        with open(torque_mk, 'r') as f:
            content = f.read()

        original_torque_content = content

        # Add C++ library to LIBS - be careful to only match the standalone LIBS := assignment
        if 'LIBS := ' in content and 'LIBS := -lc++' not in content:
            # Find the LIBS := assignment that's at the end of a line before other targets/variables
            content = re.sub(
                r'^LIBS\s*:=\s*$',
                'LIBS := -lc++',
                content,
                flags=re.MULTILINE
            )

        if content != original_torque_content:
            with open(torque_mk, 'w') as f:
                f.write(content)
            print(f"✅ Configured torque.host.mk with C++ library")

if __name__ == '__main__':
    main()
