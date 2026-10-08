---
tags: [design, gameplay, middleware]
---

# Gameplay and Middleware

Back to [DESIGN](../DESIGN.md)

## Gameplay code and scripting
- **C++ game module** built as a DLL, hot-reloaded by the host. The ECS storage lives in the host so state survives reloads. Component layout hashes guard against unsafe reloads (see [ECS and Reflection](ecs-and-reflection.md)).
- **Luau** for high-level game logic. Sandboxed, typed, fast, embeddable on every target. Scripts hot reload trivially. Bindings come from the IDL.
- **Python** for tooling, pipeline, and editor automation only. Never in the shipped runtime.

## Platform
SDL3 for windows, input, gamepads, and platform glue. SDL_GPU is not used.

## Physics
Jolt, deterministic configuration, stepped inside the fixed tick (see [Core: Determinism](core.md#determinism)). Character movement via Jolt's virtual character controller.

## Audio
miniaudio for device I/O, decoding, mixing, and basic spatialization. Spatial audio upgrades (e.g. Steam Audio) later if needed.

## Game UI
Deferred. ImGui for prototype HUDs. RmlUi is the leading candidate when a game needs real UI (its documents are text, which suits agents).
