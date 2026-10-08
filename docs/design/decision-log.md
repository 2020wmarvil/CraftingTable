---
tags: [design, decisions]
---

# Decision Log

Back to [DESIGN](../DESIGN.md)

| # | Decision | Rationale | Details |
|---|---|---|---|
| D1 | C++20/23 | Ecosystem, experience, console viability | [Core](core.md) |
| D2 | NVRHI directly, no wrapper | YAGNI. Fast path to RT and bindless on D3D12/Vulkan. Renderer boundary keeps a custom RHI possible later | [Rendering](rendering.md) |
| D3 | HLSL + DXC | Matches NVRHI/ShaderMake. Revisit Slang if permutations get painful | [Rendering](rendering.md) |
| D4 | Simple pass list + G-buffer | YAGNI. Render graph and visibility buffer are roadmap items | [Rendering](rendering.md) |
| D5 | Own archetype ECS | Core to performance and a deliberate "fun" investment | [ECS and Reflection](ecs-and-reflection.md) |
| D6 | Main + render thread | Simple. Extract step keeps a task graph possible later | [Core](core.md) |
| D7 | AI-forward, headless-first | Core identity of the engine | [DESIGN](../DESIGN.md) |
| D8 | USDA as authoring format, OpenUSD in tools only | Full composition and pxr Python for agents, no runtime weight | [Assets and USD](assets-and-usd.md) |
| D9 | USD stage authoritative at edit time | Undo, diffs, and agent sublayers all live at the USD layer | [Assets and USD](assets-and-usd.md) |
| D10 | IDL + Python codegen for reflection | One definition, eight projections, typed MCP tools | [ECS and Reflection](ecs-and-reflection.md) |
| D11 | MCP server + Python client over JSON-RPC | Agents and scripts share one API | [Agent Interface](agent-interface.md) |
| D12 | Luau runtime + Python tools + C++ hot reload | Agent-friendly languages, sandboxed runtime, fast native iteration | [Gameplay and Middleware](gameplay-and-middleware.md) |
| D13 | CMake + vendored deps | Full control, reproducible builds | [Core](core.md) |
| D14 | SDL3, Jolt, miniaudio, defer game UI | Proven, permissive, easy to vendor | [Gameplay and Middleware](gameplay-and-middleware.md) |
| D15 | STL + custom allocators, GLM | YAGNI. GLM is well known to agents | [Core](core.md) |
| D16 | Host-authoritative co-op networking over GameNetworkingSockets | Co-op focus, NAT traversal, Steam-compatible API | [Networking](networking.md) |
| D17 | Fixed tick + interpolated render | Required for clean replication and determinism | [Core](core.md) |
| D18 | Headless verification loop is M1 | Engine dev is LLM-driven; agents need to see and test their work first | [Milestones](milestones.md) |
