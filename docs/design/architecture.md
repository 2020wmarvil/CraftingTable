---
tags: [design, architecture]
---

# Architecture

Back to [DESIGN](../DESIGN.md)

## Overview

```
                 +--------------------+   +-----------------+   +-------------+
                 |  ImGui Editor      |   |  MCP Server     |   | Python      |
                 |  (human client)    |   |  (agent client) |   | scripts/CI  |
                 +---------+----------+   +--------+--------+   +------+------+
                           |                       |                   |
                           +-----------+-----------+-------------------+
                                       |  JSON-RPC (localhost) / in-process
                              +--------v---------+
                              |  Command Layer   |  typed, serializable, undoable,
                              |                  |  transactional
                              +--------+---------+
                                       |
                 +---------------------v----------------------+
   Tools only    |  USD Stage (authoritative at edit time)    |
                 +---------------------+----------------------+
                                       | ObjectsChanged notices -> incremental per-prim cook
                 +---------------------v----------------------+
   Runtime       |  World (ECS)  <-  cooked runtime data       |
                 |  Sim: scripts, gameplay, physics, net       |
                 +---------------------+----------------------+
                                       | extract (end of tick)
                              +--------v---------+
                              | Render snapshot  | -> Render thread -> NVRHI
                              +------------------+
```

The layers in this diagram are described in detail in [Agent Interface](agent-interface.md) (clients and command layer), [Assets and USD](assets-and-usd.md) (stage and cooking), [ECS and Reflection](ecs-and-reflection.md) (world), [Core: Threading and frame loop](core.md#threading-and-frame-loop) (extract and render thread), and [Rendering](rendering.md).

## Module layout

Modules marked `*` exist today; the rest are planned. Each module is one CMake target with public headers in `<module>/include/<module>/` (so includes read `"core/log.h"`) and sources in `<module>/src/`.

```
engine/
  core/      * logging (structured), asserts, errors; later allocators, containers, jobs, file system, time
  math/        GLM config + engine-specific helpers, shared GPU layout types
  reflect/     generated reflection runtime, type registry
  ecs/         archetype ECS, queries, world
  platform/    SDL3 wrapper: window, input, gamepad
  render/      NVRHI device, passes, materials, render snapshot (only module that sees nvrhi::)
  physics/     Jolt integration
  audio/       miniaudio integration
  net/         transport (GNS), replication, sessions
  script/      Luau VM, bindings
  asset/       runtime loaders for cooked data, asset handles
  sim/         fixed-tick loop, game module host (hot reload)
tools/
  idl/         component IDL files + Python codegen
  cooker/      USD -> runtime data (links OpenUSD)
  editor/    * the editor executable (links OpenUSD): headless for agents, ImGui window for humans
  server/      JSON-RPC command server library, linked into the editor
  mcp/         MCP server + Python client package
shaders/       HLSL, shared C++/HLSL layout headers
game/          sample game module(s)
tests/       * unit, smoke, golden-image, scenario
third_party/ * git submodules pinned to exact commits
scripts/     * build environment and check scripts
cmake/       * shared CMake helpers
```

### Executables

There are only two programs, and "headless" is a mode, not a program:

- **Editor** (`tools/editor`, `ct_editor`). One process serves both humans and agents: agents drive it with `--headless` over JSON-RPC/MCP, and humans run it with the ImGui window. An agent can attach to a human's live session. The UI target may link only the command layer, so CMake enforces "ImGui is just a client" at link time.
- **Game runtime** (planned for M5). Cooked data only, no USD or Python. Also runs headless as a dedicated server.

Process conventions: stdout carries command output (data), stderr carries logs. Exit code 0 is success, 2 is a usage error.

## Hard rules

- **No global world state.** `World` is an instance. Multiple worlds per process must work (server + N clients in one test, editor world + play world). Required by [Networking: Testability](networking.md#testability).
- **Only `render/` includes NVRHI headers.** Everything else uses renderer-level handles (mesh, material, texture IDs). This is the escape hatch for a future custom RHI.
- **Only `tools/` links OpenUSD.** The game runtime never depends on USD.
- **Every mutation from outside the simulation goes through the command layer.** See [Agent Interface: Command layer](agent-interface.md#command-layer).
- **Every log, error, and stat is available as structured data** (JSON), not just text.
