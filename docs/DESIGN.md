---
tags: [design, hub]
status: draft
---

# CraftingTable Engine: Design Document

Status: Draft v0.1 (2026-10-07)
Owner: project lead (senior engine developer)
Audience: human contributors and the LLM agents that will do most of the engine development

This is the hub note. Each subsystem has its own note under `design/`, listed in the [Map of notes](#map-of-notes).

---

## Vision

A general-purpose, cross-platform 3D game engine written in C++ that is **AI-forward**: designed from the ground up to be driven headless by an LLM. Every editor action is scriptable, every asset that can be text is text, and every subsystem can report its state in structured form so an agent can verify its own work.

The ImGui editor is a convenience for humans. It is a client of the same command API that agents use, never a privileged path.

### Pillars

1. **Headless first.** Everything works without a window. A human editor, an MCP agent, a Python script, and a CI job are all just clients. See [Agent Interface](design/agent-interface.md).
2. **Text is the source of truth.** Scenes, prefabs, materials, and config are USDA. Binary data is derived, cached, and never authored by hand. See [Assets and USD](design/assets-and-usd.md).
3. **One definition, many projections.** Components are defined once in an IDL and generated into C++, USD schema, serializers, inspector UI, Luau bindings, and JSON Schema. See [ECS and Reflection](design/ecs-and-reflection.md).
4. **Verifiable by construction.** Deterministic fixed-tick simulation, offscreen rendering, golden images, and scenario tests give agents a tight loop: change, run, observe, assert. See [Testing](design/testing.md).
5. **YAGNI with escape hatches.** Pick proven libraries and simple techniques first. Keep module boundaries clean so any one of them can be replaced later. See [Architecture: Hard rules](design/architecture.md#hard-rules).

### Non-goals (for now)

- Competitive multiplayer (anti-cheat, lag-compensated hit registration, full client prediction everywhere). See [Networking](design/networking.md).
- A custom RHI. NVRHI is used directly. See [Rendering](design/rendering.md).
- Shipping USD in the game runtime.
- Polished in-game UI framework. See [Gameplay and Middleware: Game UI](design/gameplay-and-middleware.md#game-ui).

---

## Platforms and Minimum Spec

| Platform | Graphics path | Notes |
|---|---|---|
| Windows 10/11 | NVRHI Vulkan (primary), D3D12 | Primary dev platform |
| Linux / Steam Deck | NVRHI Vulkan | Deck is a first-class perf target |
| macOS (Apple Silicon) | NVRHI Vulkan via MoltenVK | No hardware RT, no mesh shaders. RT is an optional tier |
| Consoles | Future | Xbox can likely reuse the D3D12 path; PlayStation needs a new backend. Keep the renderer boundary clean |

Minimum GPU target: bindless-capable hardware (roughly Turing / RDNA2 / Apple M1 class). Hardware ray tracing is an optional quality tier, never required.

---

## Tech Stack Summary

| Area | Choice | Note |
|---|---|---|
| Language | C++20 (adopt C++23 features where all three compilers support them) | [Core](design/core.md) |
| Build | CMake, all third-party deps vendored in-tree (pinned sources) | [Core](design/core.md) |
| Graphics abstraction | NVRHI, used directly inside the renderer module | [Rendering](design/rendering.md) |
| Shaders | HLSL compiled with DXC (DXIL + SPIR-V), offline via ShaderMake | [Rendering](design/rendering.md) |
| Frame structure | Simple ordered pass list, deferred G-buffer | [Rendering](design/rendering.md) |
| Platform layer | SDL3 (windowing, input, gamepad only; not SDL_GPU) | [Gameplay and Middleware](design/gameplay-and-middleware.md) |
| World model | Hand-rolled archetype ECS | [ECS and Reflection](design/ecs-and-reflection.md) |
| Reflection | Custom IDL + Python code generator | [ECS and Reflection](design/ecs-and-reflection.md) |
| Scene / asset authoring | OpenUSD (USDA), linked by tools only | [Assets and USD](design/assets-and-usd.md) |
| Runtime asset format | Cooked binary (BCn textures, packed meshes) | [Assets and USD](design/assets-and-usd.md) |
| Editor UI | Dear ImGui (docking branch), optional client of the command API | [Agent Interface](design/agent-interface.md) |
| Agent interface | JSON-RPC engine API, MCP server + Python client package | [Agent Interface](design/agent-interface.md) |
| Game scripting | Luau | [Gameplay and Middleware](design/gameplay-and-middleware.md) |
| Tool scripting | Python (with OpenUSD `pxr` bindings) | [Gameplay and Middleware](design/gameplay-and-middleware.md) |
| Native gameplay | C++ game module DLL with hot reload | [Gameplay and Middleware](design/gameplay-and-middleware.md) |
| Physics | Jolt | [Gameplay and Middleware](design/gameplay-and-middleware.md) |
| Audio | miniaudio | [Gameplay and Middleware](design/gameplay-and-middleware.md) |
| Networking transport | GameNetworkingSockets (Valve) | [Networking](design/networking.md) |
| Containers / memory | STL + custom allocators (frame/linear/pool, PMR where useful) | [Core](design/core.md) |
| Math | GLM | [Core](design/core.md) |
| Testing | doctest, golden-image tests, headless scenario tests | [Testing](design/testing.md) |
| CI | GitHub Actions (Windows + Linux, macOS later), lavapipe / WARP for GPU-less runners | [Testing](design/testing.md) |
| JSON | nlohmann/json (swap for a faster lib only if profiling says so) | [Agent Interface](design/agent-interface.md) |
| Asset import / processing | cgltf, stb_image, tinyexr, meshoptimizer, a BC7 encoder (e.g. bc7enc) | [Assets and USD](design/assets-and-usd.md) |

---

## Map of notes

**Architecture**
- [Architecture](design/architecture.md): system overview, module layout, hard rules
- [Core](design/core.md): build, memory, math, threading, frame loop, determinism

**Subsystems**
- [ECS and Reflection](design/ecs-and-reflection.md): archetype ECS, component IDL, codegen
- [Assets and USD](design/assets-and-usd.md): authoring formats, edit-time source of truth, cooking
- [Agent Interface](design/agent-interface.md): command layer, JSON-RPC, MCP tools, human editor
- [Rendering](design/rendering.md): NVRHI, passes, roadmap
- [Gameplay and Middleware](design/gameplay-and-middleware.md): C++ hot reload, Luau, Python, SDL3, Jolt, miniaudio, game UI
- [Networking](design/networking.md): co-op replication model, transport, testability

**Process**
- [Style Guide](style-guide.md): code, shader, data, and docs conventions
- [Testing](design/testing.md): verification layers and CI
- [Milestones](design/milestones.md): M0 through M5
- [Open Questions](design/open-questions.md)
- [Decision Log](design/decision-log.md)
