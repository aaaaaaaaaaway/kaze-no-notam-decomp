#!/usr/bin/env python3
"""Wrapper so ./diff.py works from the repository root."""
import runpy
import sys
from pathlib import Path

differ = Path(__file__).resolve().parent / "tools/asm-differ/diff.py"
if not differ.is_file():
    sys.exit("tools/asm-differ is empty. Run: git submodule update --init")
sys.argv[0] = str(differ)
runpy.run_path(str(differ), run_name="__main__")
