#pragma once

#include <EpiLib/types.h>

using fblChar = epiChar;
using fblBool = epiBool;

using fblFloat = epiFloat;
using fblDouble = epiDouble;

using fblSize_t = size_t;

using fblU8 = epiU8;
using fblU16 = epiU16;
using fblU32 = epiU32;
using fblU64 = epiU64;

using fblS8 = epiS8;
using fblS16 = epiS16;
using fblS32 = epiS32;
using fblS64 = epiS64;

using fblV2F = epiV2F;
using fblV2S = epiV2S;
using fblV3F = epiV3F;

using fblMat3x3F = epiMat3x3F;
using fblMat4x4F = epiMat4x4F;

struct fblRect
{
    fblV2F LeftTop = { 0.0f, 0.0f };
    fblV2F RightBottom = { 0.0f, 0.0f };
};
