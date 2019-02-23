#include "pch.h"
#include "common.h"

float g_DeltaTime = 0.0166666f;

float rand01()
{
    return (float)rand() / RAND_MAX;
}

float to_radians(const float angle)
{
    return angle * (M_PI / 180.0f);
}
