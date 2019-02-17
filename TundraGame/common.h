#pragma once

#include <assert.h>
#include "SDL_log.h"
#include "SDL_assert.h"
#include <cmath>
#include <algorithm>

#ifdef WIN32
# define FBL_WIN32
#else
# define FBL_ANDROID
#endif

extern float g_DeltaTime;

#define FOR(_n) for (int index = 0; index < _n; ++index)

#define REVEAL_SDL_ERROR(_msg)                                                                                  \
    SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL ERROR: %s, SDL_GetError(): %s", _msg, SDL_GetError());   \
    SDL_ClearError();                                                                                           \
    SDL_TriggerBreakpoint();                                                                                    \
    exit(-1);                                                                                                   \
                                                                                                                \

#define AS(_T, _V) static_cast<_T>(_V)
#define ARRLEN(_V) (sizeof(_V) / sizeof(_V[0]))

template<typename T>
T clamp(const T& val, const T& min, const T& max)
{
    return std::min<T>(max, std::max<T>(val, min));
}

template <typename T> int sign(T val)
{
    return (T(0) < val) - (val < T(0));
}

float rand01();
float to_radians(const float angle);

#define FBL_COLOR(_r, _g, _b, _a) ((u32)(_a << 24 | _b << 16 | _g << 8 | _r))
#define FBL_WHITE_COLOR FBL_COLOR(0xff, 0xff, 0xff, 0xff)
#define FBL_BLACK_COLOR FBL_COLOR(0x0, 0x0, 0x0, 0xff)
