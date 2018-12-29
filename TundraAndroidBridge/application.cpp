#include "pch.h"

#ifdef FBL_ANDROID
#include "SDL_opengles2.h"
#include "SDL_opengles2_gl2.h"
#else
#include "glew.h"
#endif

#include "SDL.h"
#include "SDL_image.h"
#include "Renderer.h"

#include "application.h"
#include "Input.h"
#include "Game/Game.h"

#ifdef FBL_WIN32
const u32 WinFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
#else
const u32 WinFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS;
#endif

SDL_Window* g_SDLWindow = nullptr;
bool g_Running = true;

void run()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        REVEAL_SDL_ERROR("SDL Initialization failed")
    }

    IMG_Init(IMG_INIT_PNG);

    const i32 winWidth = 450;
    const i32 winHeight = 800;

#ifdef FBL_WIN32
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
#else
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#endif

    //SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 1);
    //SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 1);
    //SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 1);
    //SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 1);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

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

    SDL_GLContext glContext = SDL_GL_CreateContext(g_SDLWindow);

#ifdef FBL_WIN32
    GLenum glewStatus = glewInit();
    assert(glewStatus == GLEW_OK);
#endif

    SDL_GL_SetSwapInterval(1);

    u64 lastPerfCounter = SDL_GetPerformanceCounter();
    u64 frequency = SDL_GetPerformanceFrequency();
    i32 fps = 0;
    float elapsedTime = 0.0f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.95f, 0.95f, 0.95f, 1.0f);

    {
        Game game;
        g_Running = Renderer::get().init();

        assert(g_Running);

        while (g_Running)
        {
            Input::handle_input();
            game.update();

            glClear(GL_COLOR_BUFFER_BIT);
            game.render();
            Renderer::get().present();
            SDL_GL_SwapWindow(g_SDLWindow);

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

        Renderer::get().shutdown();
    }

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(g_SDLWindow);
    IMG_Quit();
    SDL_Quit();
}
