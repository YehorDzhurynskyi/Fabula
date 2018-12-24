#pragma once

#define FOR(_n) for (int index = 0; index < _n; ++index)

#define REVEAL_SDL_ERROR(_msg)                                          \
    SDL_Log("SDL ERROR: %s, SDL_GetError(): %s", _msg, SDL_GetError()); \
    SDL_ClearError();                                                   \
    SDL_TriggerBreakpoint();                                            \
                                                                        \
