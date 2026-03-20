#!/usr/bin/env python3
"""Wrapper around genccode that reflows generated assembly.

Clang's wasm32 assembler in LLVM 20 crashes when parsing exceptionally long
comma-separated literal lists.  ICU's genccode emits 32 values per line, which
is enough to trigger that bug once the data file grows large.  This wrapper
executes the real genccode binary and then rewrites any `.S` output so that
each `.long` directive contains a smaller, safer number of values per line.
"""

from __future__ import annotations

import subprocess
import sys
from pathlib import Path
from typing import Iterable

_MAX_VALUES_PER_LINE = 8


def _reflow_long_lines(path: Path) -> None:
    """Rewrite `.long` lines to keep the number of literals manageable."""
    lines = path.read_text().splitlines()
    rewritten = []
    changed = False

    for line in lines:
        stripped = line.lstrip()
        prefix_len = len(line) - len(stripped)
        indent = line[:prefix_len]

        if stripped.startswith('.section .note.GNU-stack'):
            changed = True
            continue

        if stripped.startswith('.section .rodata'):
            rewritten.append(f"{indent}.section .rodata,\"\",@")
            changed = True
            continue

        if stripped.startswith('.type ') and '%object' in stripped:
            rewritten.append(line.replace('%object', '@object'))
            changed = True
            continue

        if stripped.startswith('#'):
            changed = True
            continue

        if stripped.startswith('.long '):
            payload = stripped[len('.long '):]
            # Remove trailing comments or whitespace and split literals.
            literals = [item.strip() for item in payload.split(',')]
            literals = [item for item in literals if item]
            if not literals:
                rewritten.append(line)
                continue

            for start in range(0, len(literals), _MAX_VALUES_PER_LINE):
                chunk = literals[start:start + _MAX_VALUES_PER_LINE]
                rewritten.append(f"{indent}.long {','.join(chunk)}")
            changed = True
        else:
            rewritten.append(line)

    if changed:
        path.write_text('\n'.join(rewritten) + '\n')


def _derive_output_path(args: Iterable[str]) -> Path:
    destdir = Path('.')
    output_base = None
    inputs = []
    iterator = iter(args)
    for arg in iterator:
        if arg in ('-d', '--destdir'):
            destdir = Path(next(iterator, ''))
        elif arg in ('-f', '--filename'):
            output_base = next(iterator, None)
        elif arg.startswith('-'):
            # Skip option parameter if this flag expects one.
            # We do not need exhaustive handling because we only have to deal
            # with the small set used by Node's build.
            continue
        else:
            inputs.append(arg)

    if output_base is None:
        if not inputs:
            raise ValueError('genccode wrapper could not determine output file')
        candidate = Path(inputs[-1]).name
        output_base = Path(candidate).stem + '_dat'

    # Determine extension based on whether -a option is present
    # If no -a option, genccode produces .c output; with -a it produces .S
    has_asm_option = any(arg in ('-a', '--assembly') for arg in args)
    ext = '.S' if has_asm_option else '.c'
    output = destdir / f"{output_base}{ext}"
    return output


def main(argv: list[str]) -> int:
    if len(argv) < 2:
        print('Usage: run_genccode.py <genccode> [args...]', file=sys.stderr)
        return 1

    tool = Path(argv[1])
    tool_args = argv[2:]

    try:
        output_path = _derive_output_path(tool_args)
    except ValueError as err:
        print(f'run_genccode.py: {err}', file=sys.stderr)
        return 1

    # Execute the real genccode binary.
    result = subprocess.run([str(tool), *tool_args], check=False)
    if result.returncode != 0:
        return result.returncode

    # Only reflow assembly files - C files don't need it
    if output_path.suffix == '.S':
        _reflow_long_lines(output_path)
    return 0


if __name__ == '__main__':
    sys.exit(main(sys.argv))
