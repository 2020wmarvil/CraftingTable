---
tags: [design, roadmap]
---

# Milestones

Back to [DESIGN](../DESIGN.md)

## M0: Foundations
- Repo, CMake presets, deps as submodules, CI on Windows and Linux (build, test, format, tidy).
- doctest, structured JSON logging, editor executable skeleton (headless only).

## M1: Headless verification loop (the agent baseline)
- IDL + codegen v0 (Transform, Hierarchy, MeshRenderer, Camera, Light). See [ECS and Reflection](ecs-and-reflection.md).
- ECS v0.
- OpenUSD stage load into ECS projection (tools side). See [Assets and USD](assets-and-usd.md).
- NVRHI Vulkan offscreen renderer: G-buffer + directional light, PNG capture. See [Rendering](rendering.md).
- First golden-image test and first scenario test. See [Testing](testing.md).
- JSON-RPC server + MCP server with the M1 tool surface. See [Agent Interface](agent-interface.md).
- Exit criterion: an agent can open a USDA scene, modify it through MCP, render it, see the image, and run the tests, all without a window.

## M2: Human editor
- SDL3 window, ImGui docking editor as a client of the command layer.
- Generated inspector, outliner, viewport, undo/redo, live USD layer reload.

## M3: Simulation
- Fixed tick with interpolated rendering.
- Jolt, Luau, C++ game module hot reload, miniaudio, play mode. See [Gameplay and Middleware](gameplay-and-middleware.md).

## M4: Networked co-op baseline
- GameNetworkingSockets, replication from IDL metadata. See [Networking](networking.md).
- In-process server + 2 clients with simulated latency/loss; replicated physics props and player pawns.
- Convergence scenario tests in CI.

## M5: Cooking and standalone runtime
- Cooker: BCn textures, optimized meshes, flattened scenes.
- Standalone game executable with no USD or Python dependency.

## Later
D3D12 backend enablement, macOS via MoltenVK, [rendering roadmap](rendering.md#roadmap) items, game UI.
