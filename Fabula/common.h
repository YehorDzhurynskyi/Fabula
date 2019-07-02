#pragma once

#include "SDL_log.h"

extern fblFloat g_DeltaTime;

#define fblFor(_n) for (int index = 0; index < _n; ++index)

#define fblLog(...) SDL_Log(#__VA_ARGS__)

#define fblAssert(_assertion, ...)                                                      \
    do { if (!(_assertion)) { fblLog(#__VA_ARGS__); assert(_assertion); } } while (0)   \

#define fblTriggerBreakpoint() __debugbreak()

#define fblSDLError(_msg)                                               \
    fblLog("SDL ERROR: %s, SDL_GetError(): %s", _msg, SDL_GetError());  \
    SDL_ClearError();                                                   \
    fblTriggerBreakpoint();                                             \

#define fblAs(_T, _V) static_cast<_T>(_V)
#define fblArrLen(_V) (sizeof(_V) / sizeof(_V[0]))

#define fblRand01() ((fblFloat)rand() / RAND_MAX)
#define fblToRadians(_degree) ((fblFloat)_degree * (M_PI / 180.0f))

namespace fbl
{

template<typename T>
T clamp(const T& val, const T& min, const T& max)
{
    return std::min<T>(max, std::max<T>(val, min));
}

template <typename T> int sign(T val)
{
    return (T(0) < val) - (val < T(0));
}

}
