#!/usr/bin/env python3
# Copyright 2024 the V8 project authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

"""
Wrapper script to invoke the host-compiled torque executable when
cross-compiling V8 for WASI/wasm32 targets.

This script is invoked by the GYP build system when v8_target_arch=="wasm32".
It locates and executes the host torque binary, which must be built before
this script runs (ensured by the 'torque#host' dependency in v8.gyp).

Usage:
    python3 torque_host_wrapper.py -o <output_dir> -v8-root <root> <files...>

Environment variables:
    TORQUE_HOST_PATH    - Override path to host torque executable
    TORQUE_WRAPPER_DEBUG - Enable debug output when set to any value
"""

import os
import subprocess
import sys
from pathlib import Path


def find_host_torque():
    """
    Locate the host-compiled torque executable.

    Search order:
    1. TORQUE_HOST_PATH environment variable (if set)
    2. out/Release/torque (standard location for release builds)
    3. out/Debug/torque (fallback for debug builds)

    Returns:
        Path to torque executable, or None if not found.
    """
    script_dir = Path(__file__).resolve().parent
    # Navigate from deps/v8/tools/v8_gypfiles to repo root
    repo_root = script_dir.parents[3]

    # Check environment override first
    env_path = os.environ.get('TORQUE_HOST_PATH')
    if env_path:
        torque_path = Path(env_path)
        if torque_path.exists() and os.access(torque_path, os.X_OK):
            return torque_path
        print(f"Warning: TORQUE_HOST_PATH={env_path} not found or not executable",
              file=sys.stderr)

    # Standard search locations
    search_paths = [
        repo_root / 'out' / 'Release' / 'torque',
        repo_root / 'out' / 'Debug' / 'torque',
    ]

    for path in search_paths:
        if path.exists() and os.access(path, os.X_OK):
            return path

    return None


def main():
    torque_path = find_host_torque()

    if not torque_path:
        print("Error: Host torque executable not found.", file=sys.stderr)
        print("", file=sys.stderr)
        print("Expected locations:", file=sys.stderr)
        print("  - out/Release/torque", file=sys.stderr)
        print("  - out/Debug/torque", file=sys.stderr)
        print("", file=sys.stderr)
        print("The host torque must be built before running this wrapper.", file=sys.stderr)
        print("This is normally handled by the 'torque#host' GYP dependency.", file=sys.stderr)
        print("", file=sys.stderr)
        print("If building manually, first build torque for the host:", file=sys.stderr)
        print("  make -C out BUILDTYPE=Release torque", file=sys.stderr)
        print("", file=sys.stderr)
        print("Or set TORQUE_HOST_PATH to point to a pre-built torque:", file=sys.stderr)
        print("  export TORQUE_HOST_PATH=/path/to/torque", file=sys.stderr)
        sys.exit(1)

    # Build command: torque + all arguments passed to this script
    cmd = [str(torque_path)] + sys.argv[1:]

    # Log the command for debugging
    if os.environ.get('TORQUE_WRAPPER_DEBUG'):
        print(f"torque_host_wrapper: executing {' '.join(cmd)}", file=sys.stderr)
        print(f"torque_host_wrapper: torque path = {torque_path}", file=sys.stderr)

    # Execute torque and propagate exit code
    try:
        result = subprocess.run(cmd, check=False)
        sys.exit(result.returncode)
    except FileNotFoundError as e:
        print(f"Error: Failed to execute torque: {e}", file=sys.stderr)
        sys.exit(1)
    except Exception as e:
        print(f"Error: Unexpected error running torque: {e}", file=sys.stderr)
        sys.exit(1)


if __name__ == '__main__':
    main()
