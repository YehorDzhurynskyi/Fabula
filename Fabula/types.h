#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

using fblChar = char;
using fblBool = bool;

using fblFloat = float;
using fblDouble = float;

using fblSize_t = size_t;

using fblU8 = uint8_t;
using fblU16 = uint16_t;
using fblU32 = uint32_t;
using fblU64 = uint64_t;

using fblS8 = int8_t;
using fblS16 = int16_t;
using fblS32 = int32_t;
using fblS64 = int64_t;

using fblV2F = glm::vec2;
using fblV2S = glm::ivec2;
using fblV3F = glm::vec3;

using fblMat3x3F = glm::mat3x3;
using fblMat4x4F = glm::mat4x4;

struct fblRect
{
    fblV2F LeftTop = { 0.0f, 0.0f };
    fblV2F RightBottom = { 0.0f, 0.0f };
};
