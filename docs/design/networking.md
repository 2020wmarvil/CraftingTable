---
tags: [design, networking]
---

# Networking

Back to [DESIGN](../DESIGN.md)

Target: **networked co-op** games. Not competitive games.

## Model
- **Host-authoritative snapshot replication.** One player hosts (listen server), or a headless dedicated server runs the same engine with no renderer.
- Server owns world state. Clients receive delta-compressed snapshots of replicated components (marked in the IDL, see [ECS and Reflection: Component IDL and codegen](ecs-and-reflection.md#component-idl-and-codegen)) and interpolate remote entities.
- **Client-authoritative local pawn movement** with server sanity checks as the starting point. This avoids prediction and reconciliation work up front and is what many co-op games ship. Full prediction can be added per system later.
- Player actions (interact, fire, use item) are sent as reliable or unreliable messages and resolved on the server.
- Interest management (relevancy) is added when world size requires it.
- Runs inside the fixed tick (see [Core: Threading and frame loop](core.md#threading-and-frame-loop)).

## Transport
- **GameNetworkingSockets**: reliable and unreliable messages, encryption, connection quality stats, and NAT traversal / relay options.
- Same API as the Steamworks networking interface, so Steam builds can switch to Steam Datagram Relay for friend-to-friend co-op without changing game code.

## Testability
- Server and clients can run as multiple `World` instances in one process (see [Architecture: Hard rules](architecture.md#hard-rules)), connected through an in-memory or loopback transport with configurable latency, jitter, and loss.
- Scenario tests assert convergence (for example: after N ticks, all clients agree with the server on positions within tolerance). See [Testing: Scenario tests](testing.md#scenario-tests).
