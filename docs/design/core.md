---
tags: [design, core]
---

# Core

Back to [DESIGN](../DESIGN.md)

## Build and dependencies
- CMake, all deps vendored under `third_party/` at pinned versions.
- OpenUSD is built once per platform by a script into a prebuilt directory (it is too heavy to rebuild on every configure). Its source version and the Python version it targets are pinned together. See [Assets and USD](assets-and-usd.md).
- Tools and runtime are separate CMake targets so the runtime build never touches USD or Python.

## Memory and containers
- STL containers by default.
- Custom allocators where it matters: per-frame linear allocator for transient data, pools for fixed-size objects, PMR for STL containers on hot paths.
- ECS chunk storage uses its own allocator. See [ECS and Reflection](ecs-and-reflection.md).

## Math
- GLM, configured once in `math/` (left/right-handedness, depth range 0..1, SIMD flags).
- GPU-visible structs are defined in shared headers usable from both C++ and HLSL, with explicit alignment so there is exactly one definition of each layout.

## Threading and frame loop
- **Main thread:** input, fixed-tick simulation, extract.
- **Render thread:** consumes the render snapshot from the previous tick and records/submits GPU work. Runs one frame behind.
- **Worker pool:** parallel-for for heavy systems (culling, animation, cooking).
- **Fixed simulation tick** (default 60 Hz) with render interpolation between the last two sim states.
- The renderer never reads ECS storage directly. The extract step copies render-relevant state into a render-owned snapshot. This keeps the door open to a task graph later without touching gameplay code.

Per-frame order on the main thread:
1. Pump platform events, sample input.
2. While accumulator >= tick: net receive, scripts (Luau), native gameplay systems, physics step, net send.
3. Extract render snapshot with interpolation alpha, hand off to render thread.

## Determinism
- Fixed tick, per-world seeded RNG, Jolt in deterministic mode.
- Goal: same inputs on the same build and platform produce the same world state. Cross-platform bit determinism is not a goal.
- This is what makes [Testing: Scenario tests](testing.md#scenario-tests) reproducible.
