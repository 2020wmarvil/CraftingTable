# CraftingTable

A cross-platform C++ 3D game engine designed to be driven headless by LLM agents.

## Docs

- [docs/DESIGN.md](docs/DESIGN.md): design hub. Read the relevant note in `docs/design/` before working on a subsystem.
- [docs/design/architecture.md](docs/design/architecture.md#hard-rules): hard rules that must not be broken without the user's approval.
- [docs/style-guide.md](docs/style-guide.md): code, docs, and git conventions.

## Status

Pre-M0: design docs and style configs only, no code yet. See [docs/design/milestones.md](docs/design/milestones.md).

## Workflow

- Format and lint with the repo configs before finishing.
- Verify changes by building and running tests before claiming they work.
- If a change conflicts with a design note, ask first. If approved, update the note and [docs/design/decision-log.md](docs/design/decision-log.md).
