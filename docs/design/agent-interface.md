---
tags: [design, agents, editor]
---

# Agent Interface

Back to [DESIGN](../DESIGN.md)

## Command layer
- Commands are typed, serializable (JSON), undoable, and can be grouped into transactions.
- All clients (ImGui, MCP, Python, tests) use the same commands.
- Command history is itself inspectable, so an agent can see what a human did and vice versa.
- Commands write to the USD stage at edit time (see [Assets and USD: Edit-time source of truth](assets-and-usd.md#edit-time-source-of-truth)).

## Engine server
- The editor exposes a JSON-RPC API on localhost (nlohmann/json), whether it runs headless or windowed.
- The server is a library inside the editor process, not a separate program (see [Architecture: Executables](architecture.md#executables)). The ImGui UI calls the same command layer in-process.

## MCP server and Python client
- One Python package provides both the Python client library and the MCP server (official MCP Python SDK), as thin wrappers over JSON-RPC.
- Tool schemas are generated from the IDL JSON Schema so they never drift from the engine (see [ECS and Reflection: Component IDL and codegen](ecs-and-reflection.md#component-idl-and-codegen)).

## Initial agent tool surface (M1)

| Tool | Purpose |
|---|---|
| `scene.open` / `scene.save` | Open or save a USD stage / layer |
| `scene.query` | Structured query of prims/entities and components |
| `prim.get` / `prim.set` | Read and write attributes through commands |
| `command.execute` / `command.undo` | Arbitrary typed commands, with undo |
| `sim.step` | Advance N fixed ticks (play mode) |
| `render.capture` | Render offscreen from a named or ad-hoc camera, return PNG |
| `log.read` | Structured log entries since a cursor |
| `stats.get` | Frame timings, GPU timings, memory, entity counts |
| `test.run` | Run a unit, golden-image, or scenario test and return structured results (see [Testing](testing.md)) |

## Human editor
- Dear ImGui (docking): viewport, outliner, inspector (generated from reflection), asset browser, undo/redo, play-in-editor.
- Optional. Nothing in the engine may depend on it.
