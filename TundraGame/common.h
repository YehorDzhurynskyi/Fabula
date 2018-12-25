#pragma once

#include <assert.h>
#include "SDL_log.h"
#include "SDL_assert.h"
#include <algorithm>

extern float g_DeltaTime;

#define FOR(_n) for (int index = 0; index < _n; ++index)

#define REVEAL_SDL_ERROR(_msg)                                          \
    SDL_Log("SDL ERROR: %s, SDL_GetError(): %s", _msg, SDL_GetError()); \
    SDL_ClearError();                                                   \
    SDL_TriggerBreakpoint();                                            \
    exit(-1);                                                           \
                                                                        \

#define AS(_T, _V) static_cast<_T>(_V)
#define ARRLEN(_V) (sizeof(_V) / sizeof(_V[0]))

template<typename T>
T clamp(const T& val, const T& min, const T& max)
{
    return std::min<T>(max, std::max<T>(val, min));
}
