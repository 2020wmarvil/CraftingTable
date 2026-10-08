---
tags: [design, open-questions]
---

# Open Questions

Back to [DESIGN](../DESIGN.md)

1. **Large world coordinates.** Double-precision positions vs. origin rebasing. Affects ECS Transform, physics (Jolt supports double precision as a compile option), and [Networking](networking.md). Decide before M4.
2. **Prediction scope.** Which systems, if any, need client prediction beyond the local pawn. See [Networking: Model](networking.md#model).
3. **IDL syntax.** Finalize during M1. Keep it simple enough for agents to write correctly. See [ECS and Reflection](ecs-and-reflection.md).
4. **Agent safety rails.** Which commands require human confirmation (e.g. deleting layers, overwriting assets) when driven via MCP. See [Agent Interface](agent-interface.md).
5. **Console strategy.** When to validate the renderer boundary against a second backend (D3D12 is the cheap first check).
6. ~~**Agent instructions.**~~ Resolved: see [CLAUDE.md](../../CLAUDE.md) at the repo root.
