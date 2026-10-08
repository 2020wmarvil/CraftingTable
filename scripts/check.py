"""Format and lint checks for CraftingTable. Used by CI and locally.

Usage:
    python scripts/check.py format [--fix]
    python scripts/check.py tidy --build-dir build/windows-msvc

On Windows, run through scripts\\msvc.cmd so clang-tidy can find the MSVC headers and the
Visual Studio copies of clang-format and clang-tidy are on PATH.
"""

import argparse
import shutil
import subprocess
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent
CXX_SUFFIXES = (".h", ".cpp", ".hlsl", ".hlsli")
EXCLUDED_DIRS = ("third_party/", "build/")


def tracked_files(suffixes: tuple[str, ...]) -> list[str]:
    """Returns tracked and untracked-but-not-ignored files with the given suffixes, excluding vendored code."""
    output = subprocess.run(
        ["git", "ls-files", "--cached", "--others", "--exclude-standard"],
        cwd=REPO_ROOT,
        check=True,
        capture_output=True,
        text=True,
    ).stdout
    return sorted(
        path
        for path in set(output.splitlines())
        if path.endswith(suffixes) and not path.startswith(EXCLUDED_DIRS) and (REPO_ROOT / path).exists()
    )


def require_tool(name: str) -> str:
    path = shutil.which(name)
    if path is None:
        sys.exit(f"error: {name} not found on PATH")
    return path


def run_format(fix: bool) -> int:
    clang_format = require_tool("clang-format")
    files = tracked_files(CXX_SUFFIXES)
    hlsl = [f for f in files if f.endswith((".hlsl", ".hlsli"))]
    cxx = [f for f in files if f not in hlsl]
    mode = ["-i"] if fix else ["--dry-run", "--Werror"]
    result = 0
    if cxx:
        result |= subprocess.run([clang_format, *mode, *cxx], cwd=REPO_ROOT).returncode
    for path in hlsl:
        # clang-format has no HLSL mode; format with the C++ rules.
        result |= subprocess.run([clang_format, *mode, "--assume-filename=x.cpp", path], cwd=REPO_ROOT).returncode
    print(f"format: {len(files)} files, {'fixed' if fix else 'ok' if result == 0 else 'FAILED'}")
    return result


def run_tidy(build_dir: Path) -> int:
    clang_tidy = require_tool("clang-tidy")
    if not (build_dir / "compile_commands.json").exists():
        sys.exit(f"error: {build_dir / 'compile_commands.json'} not found; configure a preset first")
    files = tracked_files((".cpp",))
    result = subprocess.run(
        [
            clang_tidy,
            "-p",
            str(build_dir),
            "--quiet",
            "--warnings-as-errors=*",
            # MSVC-only flags in the compile database are harmless under clang.
            "--extra-arg=-Wno-unused-command-line-argument",
            *files,
        ],
        cwd=REPO_ROOT,
    ).returncode
    print(f"tidy: {len(files)} files, {'ok' if result == 0 else 'FAILED'}")
    return result


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    commands = parser.add_subparsers(dest="command", required=True)
    format_parser = commands.add_parser("format", help="check (or fix) clang-format")
    format_parser.add_argument("--fix", action="store_true", help="rewrite files instead of checking")
    tidy_parser = commands.add_parser("tidy", help="run clang-tidy using a configured build directory")
    tidy_parser.add_argument("--build-dir", type=Path, required=True)
    args = parser.parse_args()

    if args.command == "format":
        return run_format(args.fix)
    return run_tidy(REPO_ROOT / args.build_dir)


if __name__ == "__main__":
    sys.exit(main())
