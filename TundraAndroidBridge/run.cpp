#include "pch.h"
#include <SDL.h>
#include <stdlib.h>
#include <math.h>

#define REVEAL_SDL_ERROR(_msg)                                          \
    SDL_Log("SDL ERROR: %s, SDL_GetError(): %s", _msg, SDL_GetError()); \
    SDL_ClearError();                                                   \
    SDL_TriggerBreakpoint();                                            \
                                                                        \

#define MIN(_x1, _x2) ((_x1) < (_x2) ? (_x1) : (_x2))
#define MAX(_x1, _x2) ((_x1) > (_x2) ? (_x1) : (_x2))

#define FOR(_n) for (int index = 0; index < _n; ++index)

const int MaxNumberOfLayers = 1000;
int g_nOfLayers = 1;

template<typename T>
T clamp(const T& val, const T& min, const T& max)
{
    return MAX(min, MIN(val, max));
}

void process_key_event(const SDL_Event& event)
{
    switch (event.key.keysym.sym)
    {
    case SDLK_PAGEUP:
    {
        ++g_nOfLayers;
    } break;

    case SDLK_PAGEDOWN:
    {
        --g_nOfLayers;
    } break;
    }
    g_nOfLayers = clamp<int>(g_nOfLayers, 1, MaxNumberOfLayers);
    SDL_Log("NUMBER OF LAYERS: %i\n", g_nOfLayers);
}

SDL_Texture* g_sdlTextures[MaxNumberOfLayers];

void run()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        REVEAL_SDL_ERROR("SDL Initialization failed")
    }

#ifdef WIN32
    u32 winFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
#else
    u32 winFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS;
#endif

    SDL_Window* sdlWindow = SDL_CreateWindow("Tundra",
                                             SDL_WINDOWPOS_UNDEFINED,
                                             SDL_WINDOWPOS_UNDEFINED,
                                             640,
                                             480,
                                             winFlags);
    if (sdlWindow == nullptr)
    {
        REVEAL_SDL_ERROR("SDL window creation failed")
    }

    SDL_Renderer* sdlRenderer = SDL_CreateRenderer(sdlWindow, -1, SDL_RENDERER_ACCELERATED);
    if (sdlRenderer == nullptr)
    {
        REVEAL_SDL_ERROR("SDL renderer creation failed")
    }

    if (SDL_RenderSetLogicalSize(sdlRenderer, 640, 480) < 0)
    {
        REVEAL_SDL_ERROR("SDL setting logical scale failed")
    }
    if (SDL_RenderSetIntegerScale(sdlRenderer, SDL_TRUE) < 0)
    {
        REVEAL_SDL_ERROR("SDL setting integer scale failed")
    }

    FOR(MaxNumberOfLayers)
    {
        SDL_Surface* sdlSurface = SDL_CreateRGBSurfaceWithFormat(0, 128, 128, 32, SDL_PIXELFORMAT_RGBA32);
        if (sdlSurface == nullptr)
        {
            REVEAL_SDL_ERROR("SDL surface creation failed")
        }

        if (SDL_LockSurface(sdlSurface) < 0)
        {
            REVEAL_SDL_ERROR("SDL surface creation failed")
        }

        FOR(sdlSurface->w * sdlSurface->h)
        {
            i32* dest = (i32*)sdlSurface->pixels + index;
            *dest = ((float)rand() / RAND_MAX) * INT_MAX;
            *dest |= 0xff000000;
        }

        SDL_UnlockSurface(sdlSurface);

        g_sdlTextures[index] = SDL_CreateTextureFromSurface(sdlRenderer, sdlSurface);
        if (g_sdlTextures[index] == nullptr)
        {
            REVEAL_SDL_ERROR("SDL texture creation failed")
        }

        u32 format;
        i32 w;
        i32 h;
        i32 access;
        if (SDL_QueryTexture(g_sdlTextures[index], &format, &access, &w, &h) < 0)
        {
            REVEAL_SDL_ERROR("SDL querying creation failed")
        }

        i32 bpp;
        u32 r;
        u32 g;
        u32 b;
        u32 a;
        if (!SDL_PixelFormatEnumToMasks(format, &bpp, &r, &g, &b, &a))
        {
            REVEAL_SDL_ERROR("SDL pixel format convertion failed")
        }

        SDL_FreeSurface(sdlSurface);
    }

    u64 lastPerfCounter = SDL_GetPerformanceCounter();
    u64 frequency = SDL_GetPerformanceFrequency();
    i32 fps = 0;
    float elapsedTime = 0.0f;
    float totalElapsedTime = 0.0f;
    bool quit = false;

    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
            {
                quit = true;
            } break;

            case SDL_KEYDOWN:
            case SDL_KEYUP:
            {
                process_key_event(event);
            } break;

            default: break;
            }
        }

        SDL_SetRenderDrawColor(sdlRenderer, 0xff, 0x00, 0xff, 0x00);
        SDL_RenderClear(sdlRenderer);

        FOR(MaxNumberOfLayers)
        {
            const float k = (index * 10.0f) / MaxNumberOfLayers;

            SDL_Rect destRect;
            destRect.x = 320 - 50 + 300 * sinf(2.0f * M_PI * (totalElapsedTime + k) / 2.0f);
            destRect.y = 240 - 50 + 120 * cosf(2.0f * M_PI * (totalElapsedTime + k) / 2.0f);
            destRect.w = 100;
            destRect.h = 100;

            SDL_RenderCopy(sdlRenderer, g_sdlTextures[index], nullptr, &destRect);
        }

        SDL_RenderPresent(sdlRenderer);

        ++fps;
        const u64 now = SDL_GetPerformanceCounter();
        const float SPF = (now - lastPerfCounter) / (float)frequency;
        lastPerfCounter = now;

        totalElapsedTime += SPF;

        elapsedTime += SPF;
        if (elapsedTime > 1.0f)
        {
            elapsedTime -= 1.0f;
            SDL_Log("FPS: %i, %fms", fps, 1000.f / fps);
            fps = 0;
        }
    }

    SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(sdlWindow);
    SDL_Quit();
}
