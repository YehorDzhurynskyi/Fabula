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
#include "Event/EventBus.h"
#include "Game/Game.h"

#ifdef FBL_WIN32
const u32 WinFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
#else
const u32 WinFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_BORDERLESS;
#endif

SDL_Window* g_SDLWindow = nullptr;
bool g_Running = true;

namespace
{

SDL_Event g_EventBuffer[4];
void process_events()
{
    SDL_PumpEvents();
    while (true)
    {
        const i32 eventsCount = SDL_PeepEvents(g_EventBuffer,
                                               ARRLEN(g_EventBuffer),
                                               SDL_GETEVENT,
                                               SDL_FIRSTEVENT,
                                               SDL_LASTEVENT);
        if (eventsCount == 0)
        {
            break;
        }

        if (eventsCount < 0)
        {
            REVEAL_SDL_ERROR("SDL Failed on peeping events")
        }

        FOR(eventsCount)
        {
            SDL_Event& event = g_EventBuffer[index];
            switch (event.type)
            {
            case SDL_QUIT:
            {
                g_Running = false;
            } break;
            case SDL_KEYDOWN:
                if (event.key.keysym.scancode != SDL_SCANCODE_SPACE) break;
            case SDL_FINGERDOWN:
            {
                EventBus::get().enqueue<ClickEvent>();
            } break;
            case SDL_WINDOWEVENT:
            {
                switch (event.window.event)
                {
                case SDL_WINDOWEVENT_SHOWN:
                case SDL_WINDOWEVENT_EXPOSED:
                case SDL_WINDOWEVENT_MAXIMIZED:
                case SDL_WINDOWEVENT_RESTORED:
                case SDL_WINDOWEVENT_FOCUS_GAINED:
                {
                    WindowFocusEvent* focusEvent = EventBus::get().enqueue<WindowFocusEvent>();
                    focusEvent->Focused = true;
                } break;
                case SDL_WINDOWEVENT_HIDDEN:
                case SDL_WINDOWEVENT_MINIMIZED:
                case SDL_WINDOWEVENT_FOCUS_LOST:
                {
                    WindowFocusEvent* focusEvent = EventBus::get().enqueue<WindowFocusEvent>();
                    focusEvent->Focused = false;
                } break;
                case SDL_WINDOWEVENT_SIZE_CHANGED:
                {
                    WindowResizedEvent* resizedEvent = EventBus::get().enqueue<WindowResizedEvent>();
                    resizedEvent->Width = event.window.data1;
                    resizedEvent->Height = event.window.data2;
                } break;
                case SDL_WINDOWEVENT_CLOSE:
                {
                    event.type = SDL_QUIT;
                    SDL_PushEvent(&event);
                } break;
                }
            } break;
            }
        }
    }
}
}

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
#ifdef _DEBUG
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
#else
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN);
#endif

    u64 lastPerfCounter = SDL_GetPerformanceCounter();
    u64 frequency = SDL_GetPerformanceFrequency();
    i32 fps = 0;
    float elapsedTime = 0.0f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.9f, 0.9f, 0.9f, 1.0f);

    {
        Game game;
        g_Running = Renderer::get().init();

        assert(g_Running);

        while (g_Running)
        {
            process_events();
            game.update();

            game.render();
            SDL_GL_SwapWindow(g_SDLWindow);

            ++fps;
            const u64 now = SDL_GetPerformanceCounter();
            g_DeltaTime = (now - lastPerfCounter) / (float)frequency;
            lastPerfCounter = now;

            elapsedTime += g_DeltaTime;
            if (elapsedTime > 1.0f)
            {
                elapsedTime -= 1.0f;
                SDL_LogDebug(SDL_LOG_CATEGORY_APPLICATION, "FPS: %i, %fms", fps, 1000.f / fps);
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
