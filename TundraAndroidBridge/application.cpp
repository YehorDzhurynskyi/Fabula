#include "pch.h"
#include "SDL.h"
#include "application.h"
#include "Input.h"
#include "Game/Game.h"

#ifdef WIN32
const u32 WinFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
#else
const u32 WinFlags = SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS;
#endif

SDL_Window* g_SDLWindow = nullptr;
SDL_Renderer* g_SDLRenderer = nullptr;
bool g_Running = true;

float g_DeltaTime = 0.0f;

void run()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        REVEAL_SDL_ERROR("SDL Initialization failed")
    }

    const i32 winWidth = 450;
    const i32 winHeight = 800;

    g_SDLWindow = SDL_CreateWindow("Tundra",
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   winWidth,
                                   winHeight,
                                   WinFlags);
    if (g_SDLWindow == nullptr)
    {
        REVEAL_SDL_ERROR("SDL window creation failed")
    }

    g_SDLRenderer = SDL_CreateRenderer(g_SDLWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (g_SDLRenderer == nullptr)
    {
        REVEAL_SDL_ERROR("SDL renderer creation failed")
    }

    u64 lastPerfCounter = SDL_GetPerformanceCounter();
    u64 frequency = SDL_GetPerformanceFrequency();
    i32 fps = 0;
    float elapsedTime = 0.0f;

    SDL_SetRenderDrawColor(g_SDLRenderer, 0xff, 0x00, 0xff, 0x00);

    {
        Game game;

        while (g_Running)
        {
            Input::handle_input();
            game.update();

            SDL_RenderClear(g_SDLRenderer);
            game.render();
            SDL_RenderPresent(g_SDLRenderer);

            ++fps;
            const u64 now = SDL_GetPerformanceCounter();
            g_DeltaTime = (now - lastPerfCounter) / (float)frequency;
            lastPerfCounter = now;

            elapsedTime += g_DeltaTime;
            if (elapsedTime > 1.0f)
            {
                elapsedTime -= 1.0f;
                SDL_Log("FPS: %i, %fms", fps, 1000.f / fps);
                fps = 0;
            }
        }
    }

    SDL_DestroyRenderer(g_SDLRenderer);
    SDL_DestroyWindow(g_SDLWindow);
    SDL_Quit();
}
