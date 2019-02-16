#pragma once

#include <cstdint>
#include <glm/vec2.hpp>

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

using vec2f = glm::vec2;
using vec2i = glm::ivec2;

using ProgramID = u32;
using ShaderID = u32;
using TextureID = u32;
using VertexBufferID = u32;
using IndexBufferID = u32;
using FrameBufferID = u32;
using ShaderLocationID = i32;

struct Transform
{
    vec2f Position = { 0.0f, 0.0f };
    vec2f Size = { 1.0f, 1.0f };
};
