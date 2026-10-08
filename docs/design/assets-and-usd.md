---
tags: [design, assets, usd]
---

# Assets and USD

Back to [DESIGN](../DESIGN.md)

## Authoring
- **USDA** for scenes, prefabs, materials, and config.
- **Heavy payloads** (meshes, textures, audio) are separate files referenced from USDA: `.usdc` or glTF for meshes, PNG/EXR for textures, WAV/OGG for audio.
- Agents edit composition and attributes in USDA. Python scripts using `pxr` may generate geometry and write `.usdc`.
- Agent edits go into their own sublayer by default so they can be reviewed, diffed, and discarded like a branch.

## Edit-time source of truth
- The USD stage is authoritative while editing.
- Commands (see [Agent Interface: Command layer](agent-interface.md#command-layer)) write to the stage. `UsdNotice::ObjectsChanged` drives incremental per-prim cooking into the ECS projection.
- Play mode copies the projection into a separate transient world. Nothing writes back to USD unless explicitly requested.
- Because the editor cooks per prim and feeds the same runtime loaders, the editor exercises the game's load path every time something is edited.

## Runtime
- The cooker flattens stages into runtime formats: BCn textures, packed/optimized meshes (meshoptimizer), flattened entity data.
- Cooked output is a cache keyed by source content hash plus cooker version. It is never committed.
- Shipping builds load only cooked data. OpenUSD is linked by tools only (see [Architecture: Hard rules](architecture.md#hard-rules)).

## Import / processing libraries
cgltf, stb_image, tinyexr, meshoptimizer, a BC7 encoder (e.g. bc7enc).
