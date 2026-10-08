---
tags: [design, ecs, reflection]
---

# ECS and Reflection

Back to [DESIGN](../DESIGN.md)

## ECS
- Hand-rolled archetype ECS with chunked storage.
- Entities are generational IDs. Hierarchy (parent/child transforms) is a component plus a system, not a separate scene graph.
- Queries over component sets, with change tracking (needed for [replication](networking.md) and editor sync).

## Component IDL and codegen
Components are defined in a small IDL. Every component and every field **must** have a doc string (agents read them).

Example (syntax illustrative, to be finalized in M1, see [Open Questions](open-questions.md)):
```
component RigidBody {
  doc "Dynamic or static physics body simulated by Jolt."
  replicated
  field motion : enum(Static, Dynamic, Kinematic) = Dynamic  doc "How the body is simulated."
  field mass   : float = 1.0  range(0, inf) units(kg)       doc "Mass of the body."
}
```

The Python generator (run as a CMake custom command) emits:
1. C++ component structs
2. USD read/write (cooker, editor)
3. Cooked binary serialization (runtime)
4. ImGui inspector metadata
5. Luau bindings
6. JSON Schema ([MCP tools](agent-interface.md), Python client)
7. A codeless USD `schema.usda`, so usdview and DCC tools understand engine types
8. Replication metadata for fields marked `replicated`

A layout hash per component lets [hot reload](gameplay-and-middleware.md#gameplay-code-and-scripting) detect layout changes and migrate or reject.
