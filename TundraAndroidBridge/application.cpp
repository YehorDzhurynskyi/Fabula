#include "pch.h"
#include "application.h"

#ifdef WIN32
const u32 WinFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
#else
const u32 WinFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS;
#endif

SDL_Window* g_SDLWindow;
SDL_Renderer* g_SDLRenderer;

void run()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        REVEAL_SDL_ERROR("SDL Initialization failed")
    }

    g_SDLWindow = SDL_CreateWindow("Tundra",
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   640,
                                   480,
                                   WinFlags);
    if (g_SDLWindow == nullptr)
    {
        REVEAL_SDL_ERROR("SDL window creation failed")
    }

    g_SDLRenderer = SDL_CreateRenderer(g_SDLWindow, -1, SDL_RENDERER_ACCELERATED);
    if (g_SDLRenderer == nullptr)
    {
        REVEAL_SDL_ERROR("SDL renderer creation failed")
    }

    u64 lastPerfCounter = SDL_GetPerformanceCounter();
    u64 frequency = SDL_GetPerformanceFrequency();
    i32 fps = 0;
    float elapsedTime = 0.0f;
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
                
            } break;

            default: break;
            }
        }

        SDL_SetRenderDrawColor(g_SDLRenderer, 0xff, 0x00, 0xff, 0x00);
        SDL_RenderClear(g_SDLRenderer);
        SDL_RenderPresent(g_SDLRenderer);

        ++fps;
        const u64 now = SDL_GetPerformanceCounter();
        const float SPF = (now - lastPerfCounter) / (float)frequency;
        lastPerfCounter = now;

        elapsedTime += SPF;
        if (elapsedTime > 1.0f)
        {
            elapsedTime -= 1.0f;
            SDL_Log("FPS: %i, %fms", fps, 1000.f / fps);
            fps = 0;
        }
    }

    SDL_DestroyRenderer(g_SDLRenderer);
    SDL_DestroyWindow(g_SDLWindow);
    SDL_Quit();
}
