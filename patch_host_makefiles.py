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

    # Remove the GLIBCXX ABI flag since macOS uses libc++, not libstdc++
    content = content.replace("'-D_GLIBCXX_USE_CXX11_ABI=1' \\\n", "")

    # Use C++2b for spaceship operator and concepts support
    content = content.replace("-std=c++20", "-std=c++2b -fconcepts")

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

    if content != original_content:
        with open(makefile_path, 'w') as f:
            f.write(content)
        return True

    return False

def patch_torque_makefile():
    """Patch torque.host.mk specifically to add necessary libraries and frameworks."""
    torque_mk = 'out/tools/v8_gypfiles/torque.host.mk'

    if not os.path.exists(torque_mk):
        return False

    with open(torque_mk, 'r') as f:
        content = f.read()

    original_content = content

    # Add C++ library to LIBS if not present
    if 'LIBS := ' in content and 'LIBS := -lc++' not in content:
        content = re.sub(
            r'^LIBS\s*:=\s*$',
            'LIBS := -lc++',
            content,
            flags=re.MULTILINE
        )

    # Add CoreFoundation framework for macOS time zone support
    if 'LDFLAGS_Release :=' in content and '-framework CoreFoundation' not in content:
        content = re.sub(
            r'(LDFLAGS_Release\s*:=\s*.*?)(?=\\?$)',
            r'\1 \\\n\t-framework CoreFoundation',
            content,
            flags=re.MULTILINE
        )

    # Also add CoreFoundation to Debug builds if they exist
    if 'LDFLAGS_Debug :=' in content and '-framework CoreFoundation' not in content:
        content = re.sub(
            r'(LDFLAGS_Debug\s*:=\s*.*?)(?=\\?$)',
            r'\1 \\\n\t-framework CoreFoundation',
            content,
            flags=re.MULTILINE
        )

    # Add system libraries for proper symbol resolution
    if 'LIBS := -lc++' in content and '-framework CoreFoundation' not in content:
        content = content.replace('LIBS := -lc++', 'LIBS := -lc++ -framework CoreFoundation')

    # Add system libraries for V8 and C++ standard library symbols
    if 'LIBS := -lc++ -framework CoreFoundation' in content:
        # Add system libraries that V8 and C++ standard library might need
        if '-framework SystemConfiguration' not in content:
            content = content.replace('LIBS := -lc++ -framework CoreFoundation',
                                    'LIBS := -lc++ -framework CoreFoundation -framework SystemConfiguration')

        # Add essential system libraries for threading and memory management
        if '-lpthread' not in content and '-lSystem' not in content:
            content = content.replace('LIBS := -lc++ -framework CoreFoundation -framework SystemConfiguration',
                                    'LIBS := -lc++ -framework CoreFoundation -framework SystemConfiguration -lpthread -lSystem')

    # Add system libraries to LDFLAGS as well
    if 'LDFLAGS_Release :=' in content:
        # Add SystemConfiguration framework for networking and system calls
        if '-framework SystemConfiguration' not in content:
            content = re.sub(
                r'(LDFLAGS_Release\s*:=\s*.*?)(?=\\?$)',
                r'\1 \\\n\t-framework SystemConfiguration \\\n\t-lpthread \\\n\t-lSystem',
                content,
                flags=re.MULTILINE
            )

    if 'LDFLAGS_Debug :=' in content:
        if '-framework SystemConfiguration' not in content:
            content = re.sub(
                r'(LDFLAGS_Debug\s*:=\s*.*?)(?=\\?$)',
                r'\1 \\\n\t-framework SystemConfiguration \\\n\t-lpthread \\\n\t-lSystem',
                content,
                flags=re.MULTILINE
            )

    if content != original_content:
        with open(torque_mk, 'w') as f:
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
        print("✅ Patched root Makefile alink command")

    # Patch torque.host.mk specifically to add C++ library and CoreFoundation
    if patch_torque_makefile():
        print("✅ Configured torque.host.mk with C++ library and CoreFoundation")

if __name__ == '__main__':
    main()
