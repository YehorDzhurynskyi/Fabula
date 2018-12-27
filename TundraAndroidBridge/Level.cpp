#include "pch.h"
#include "Game/Level.h"

namespace Level
{

const std::array<S_DensityChunk, 1> S_ChunkSelection
{
    S_DensityChunk { vec2f(-0.5f, -0.5f), vec2f(0.7f, 0.35f), vec2f(-0.15f, 0.9f) }
};

const std::array<M_DensityChunk, 1> M_ChunkSelection
{
    {
        vec2f(-0.8f, -0.7f), vec2f(0.1f, -0.2f), vec2f(-0.1f, 0.25f),
        vec2f(0.7f, 0.5f), vec2f(-0.5f, 0.95f)
    }
};

const std::array<L_DensityChunk, 1> L_ChunkSelection
{
    {
        vec2f(-0.85f, -0.95f), vec2f(0.8f, -0.8f), vec2f(0.55f, -0.0f),
        vec2f(-0.1f, -0.65f), vec2f(-0.8f, 0.1f), vec2f(-0.25f, 0.7f),
        vec2f(0.6f, 0.9f)
    }
};

}
