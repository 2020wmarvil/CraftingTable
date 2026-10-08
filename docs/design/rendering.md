---
tags: [design, rendering]
---

# Rendering

Back to [DESIGN](../DESIGN.md)

## Decisions
- **NVRHI used directly**, no wrapper on top. Contained entirely in `render/` (see [Architecture: Hard rules](architecture.md#hard-rules)).
- **Bindless from day one** inside NVRHI (`BindlessLayout` + `DescriptorTable`) for textures and materials, so the data model is already GPU-driven friendly.
- **NVRHI automatic state tracking stays on** until profiling shows it matters.
- **Simple ordered pass list**, not a render graph. Upgrade when pass count or transient memory makes it hurt.
- **Deferred G-buffer** shading.
- **HLSL + DXC** via ShaderMake. Shared C++/HLSL layout headers (see [Core: Math](core.md#math)).
- **Offscreen rendering is a first-class mode** (no window, no swapchain) for headless capture and [golden-image tests](testing.md#golden-image-tests).
- The renderer consumes a render snapshot produced by the extract step, never ECS storage (see [Core: Threading and frame loop](core.md#threading-and-frame-loop)).

## Initial passes
1. Depth prepass (optional)
2. G-buffer
3. Directional light + shadows (cascaded shadow maps)
4. Deferred lighting (punctual lights)
5. Sky / environment
6. Tonemap + output
7. ImGui overlay (editor only)

## Roadmap
YAGNI: in rough order, each only when a game needs it.
- Render graph (pass culling, transient aliasing, automatic barriers)
- GPU culling and indirect draws
- TAA and upscalers (DLSS / FSR / XeSS)
- Advanced PBR and post (area lights, volumetrics, auto-exposure)
- Large open worlds (streaming, origin rebasing, see [Open Questions](open-questions.md))
- Hardware ray tracing tier (shadows, reflections, GI)
- Virtualized geometry (meshlet clusters, LOD hierarchy, likely with a move to a visibility buffer), with a compute + indirect path for MoltenVK and small triangles
- Custom RHI if NVRHI becomes the bottleneck (Metal backend is the most likely trigger)
