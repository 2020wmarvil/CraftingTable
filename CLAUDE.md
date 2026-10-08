# CraftingTable

A cross-platform C++ 3D game engine designed to be driven headless by LLM agents.

## Docs

- [docs/DESIGN.md](docs/DESIGN.md): design hub. Read the relevant note in `docs/design/` before working on a subsystem.
- [docs/design/architecture.md](docs/design/architecture.md#hard-rules): hard rules that must not be broken without the user's approval.
- [docs/style-guide.md](docs/style-guide.md): code, docs, and git conventions.

## Status

M0 (foundations): build, core logging, headless editor skeleton, tests, CI. See [docs/design/milestones.md](docs/design/milestones.md).

## Build and verify

Clone with `--recursive` (deps are submodules). On Windows, prefix commands with `scripts\msvc.cmd` to get the MSVC environment; on Linux use the `linux-clang` presets.

```
scripts\msvc.cmd cmake --preset windows-msvc
scripts\msvc.cmd cmake --build --preset windows-msvc-debug
scripts\msvc.cmd ctest --preset windows-msvc-debug
scripts\msvc.cmd python scripts/check.py format --fix
scripts\msvc.cmd python scripts/check.py tidy --build-dir build/windows-msvc
```

Executables land in `build/<preset>/bin/<Config>/`. CI runs all of the above on Windows and Linux.

## Workflow

- Build, test, format, and tidy before claiming a change works.
- If a change conflicts with a design note, ask first. If approved, update the note and [docs/design/decision-log.md](docs/design/decision-log.md).
