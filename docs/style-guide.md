---
tags: [process, style]
status: accepted
---

# Style Guide

Back to [DESIGN](DESIGN.md)

Most code in this repo will be written by LLM agents, so this guide optimizes for three things:

1. **Mechanical enforcement.** Anything a tool can check (format, naming, includes) is checked by a tool in CI, not by review.
2. **Greppability and explicitness.** Names say what things are. Little hidden magic. An agent reading one file should understand it without opening five others.
3. **One name per concept across languages.** C++, the IDL, USD, Luau, and JSON Schema all use the same field names, so codegen is a 1:1 mapping.

When this guide is silent, match the surrounding code.

---

## C++

### Naming

| Thing | Style | Example |
|---|---|---|
| Namespaces | lowercase, root `ct` | `ct::render`, `ct::ecs` |
| Types, concepts, enums | PascalCase | `RenderDevice`, `MeshHandle`, `Allocator` |
| Enum values (`enum class` only) | PascalCase | `MotionType::Dynamic` |
| Functions, methods | camelCase | `createBuffer()`, `world.spawn()` |
| Local variables, parameters | camelCase | `frameIndex`, `deltaTime` |
| Data members of classes | `m_` + camelCase | `m_device`, `m_frameAllocator` |
| Fields of plain structs (incl. generated components) | camelCase, no prefix | `transform.position`, `rigidBody.mass` |
| Static members / file statics | `s_` + camelCase | `s_instanceCount` |
| Globals (rare, see [Architecture: Hard rules](design/architecture.md#hard-rules)) | `g_` + camelCase | `g_logSink` |
| Constants (`constexpr`, `static const`) | `k` + PascalCase | `kMaxFramesInFlight` |
| Macros | `CT_` + UPPER_SNAKE | `CT_ASSERT`, `CT_LOG_INFO` |
| Template parameters | PascalCase | `template <typename T, typename Alloc>` |
| Booleans | `is`/`has`/`should`/`can` prefix | `isVisible`, `hasParent` |
| Files | snake_case, `.h` / `.cpp` | `render_device.h` |

Rationale for camelCase fields: USD attributes are camelCase (`extentsHint`, `doubleSided`), so IDL fields, C++ struct fields, USD attributes, Luau properties, and JSON keys are all spelled identically.

STL-style names (`begin`, `end`, `size`, `push_back`) are allowed on container-like types so range-for and algorithms work.

### Formatting (clang-format)
- 4-space indent, no tabs, 120 column limit.
- Braces on their own line (Allman) for namespaces, types, and functions; attached for control flow and lambdas.
- Always use braces for `if`/`for`/`while`, even single statements.
- Pointer and reference bind to the type: `int* ptr`, `const Mesh& mesh`.
- `#pragma once` in every header.
- Include order (enforced): own header, then `ct/` engine headers, then third-party, then standard library. Blank line between groups.
- Engine includes use the module path: `#include "render/render_device.h"`.

### Language rules
- **C++20.** C++23 library features allowed where MSVC, Clang, and GCC all ship them (`std::expected`, `std::print` are fine). No C++20 modules for now.
- **Errors:**
  - Programmer errors: `CT_ASSERT` (debug only) or `CT_VERIFY` (always on, crashes with a structured log).
  - Recoverable failures: return `std::expected<T, ct::Error>`. Mark fallible functions `[[nodiscard]]`.
  - Engine code does not throw. Tools may catch third-party exceptions (OpenUSD, JSON) at the boundary and convert them to `ct::Error`.
- **Ownership:**
  - `std::unique_ptr` for single ownership. Raw pointers and references are always non-owning.
  - No `std::shared_ptr` in engine code unless a library API requires it (NVRHI's `RefCountPtr` is fine inside `render/`).
  - Cross-system references use generational handles (`EntityId`, `MeshHandle`), not pointers.
- **No RTTI in engine code** (`dynamic_cast`, `typeid`). Type information comes from the IDL reflection.
- **`auto`** only when the type is obvious from the right-hand side or unutterable (iterators, lambdas).
- **`const`** by default for locals and methods. `override` on every override. `explicit` on single-argument constructors.
- **Templates** sparingly, with concepts instead of SFINAE.
- **Hot paths** (per-tick, per-frame) do not hit the general heap. Use the frame allocator or pools.
- **No `using namespace`** in headers. Allowed in `.cpp` files for `ct` sub-namespaces only.
- **Logging** through `CT_LOG_*(category, fmt, args...)` (std::format syntax), never `printf`/`std::cout`.

### Comments
- `///` doc comments on every public type and function in headers. Agents read these, so say what it does, the units, and what is invalid input.
- Inline comments explain *why*, not *what*.
- No commented-out code.
- `TODO:` with a short reason. Link an issue when one exists.

### Example
```cpp
#pragma once

#include "core/expected.h"
#include "render/handles.h"

#include <span>

namespace ct::render
{

/// Owns GPU buffers for static meshes. Not thread-safe; call from the render thread only.
class MeshCache
{
public:
    static constexpr uint32_t kMaxMeshes = 65536;

    /// Uploads vertex and index data. Returns an error if the cache is full.
    [[nodiscard]] Expected<MeshHandle> upload(std::span<const Vertex> vertices, std::span<const uint32_t> indices);

    /// Returns true if the handle refers to a live mesh.
    bool isValid(MeshHandle handle) const;

private:
    uint32_t m_liveCount = 0;
};

} // namespace ct::render
```

---

## HLSL
- Same naming as C++ (PascalCase types, camelCase functions and variables, `k` constants).
- Shared C++/HLSL layout headers live in `shaders/shared/` with the `.hlsli` extension and compile in both languages.
- Entry points are named by stage: `vsMain`, `psMain`, `csMain`.

## IDL
- Components PascalCase, fields camelCase, a `doc` string on every component and field (see [ECS and Reflection](design/ecs-and-reflection.md)).
- Units in `units(...)` rather than in the name (`mass units(kg)`, not `massKg`).

## USD
- Prim names PascalCase (`/World/Player/Camera`).
- Engine attributes are namespaced by component: `rigidBody:mass`, `light:intensity`.
- Agent edits go in their own sublayer (see [Assets and USD](design/assets-and-usd.md)).

## Python (tools)
- PEP 8 naming (snake_case functions and variables, PascalCase classes). This is the one deliberate exception to the cross-language rule, because every Python library and the `pxr` API follow it.
- Type hints required on public functions.
- Format and lint with ruff.

## Luau
- `--!strict` at the top of every script.
- camelCase locals and functions, PascalCase modules and types (Roblox community convention).
- Format with StyLua.

## Markdown docs
- kebab-case filenames, standard relative Markdown links, frontmatter `tags`.
- No em dashes. Use commas, periods, parentheses, or restructure the sentence.

## Git
- Commit subject in imperative mood, at most 72 characters ("Add mesh cache", not "Added mesh cache"). Body explains why.
- No AI attribution lines (`Co-Authored-By` etc.) in commit messages.
- LF line endings everywhere, enforced by `.gitattributes` (`* text=auto eol=lf`).

---

## Enforcement

| File | Purpose |
|---|---|
| `.clang-format` | All C++/HLSL formatting rules above |
| `.clang-tidy` | `readability-identifier-naming` for the naming table, plus a curated set of `bugprone-*`, `modernize-*`, `performance-*` checks |
| `.editorconfig` | Indent, charset, final newline for every file type |
| `.gitattributes` | LF line endings, binary file markers |
| `ruff.toml`, `stylua.toml` | Python and Luau |

CI fails on any formatting diff or naming violation. Agents run the formatters before committing.

---

## Local tooling

Visual Studio 2026 ships clang-format and clang-tidy under `VC/Tools/Llvm/x64/bin`. HLSL is formatted with the C++ rules via `clang-format --assume-filename=x.cpp`. ruff and StyLua are installed separately.
