#pragma once

#include "Fabula/types.h"

namespace fbl
{

using ProgramID = fblU32;
using ShaderID = fblU32;
using TextureID = fblU32;
using VertexBufferID = fblU32;
using IndexBufferID = fblU32;
using FrameBufferID = fblU32;
using ShaderLocationID = fblU32;

struct Transform
{
    fblV2f Position = { 0.0f, 0.0f };
    fblV2f Size = { 1.0f, 1.0f };
};

}
