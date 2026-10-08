---
tags: [design, testing]
---

# Testing

Back to [DESIGN](../DESIGN.md)

Engine development is primarily LLM-driven, so verification is a core feature, not an afterthought.

## Unit tests
doctest. Covers core libs, ECS, allocators, codegen output, serialization round-trips.

## Golden-image tests
Headless offscreen render of test scenes (see [Rendering](rendering.md)), compared to references with a perceptual tolerance (e.g. NVIDIA FLIP). Catches renderer regressions.

Golden references are rendered on the software rasterizer used by CI so they are stable. GPU hardware runs use a looser tolerance.

## Scenario tests
Load a USDA scene, run N deterministic ticks (see [Core: Determinism](core.md#determinism)), assert on world state. Catches gameplay, physics, scripting, and [networking](networking.md#testability) regressions.

## CI
GitHub Actions on Windows and Linux (macOS later). lavapipe (Vulkan) / WARP (D3D12) on GPU-less runners. Runs everything above per push.

## Structured results
All test results are emitted as structured JSON so the `test.run` tool can return them to agents directly (see [Agent Interface: Initial agent tool surface (M1)](agent-interface.md#initial-agent-tool-surface-m1)).
