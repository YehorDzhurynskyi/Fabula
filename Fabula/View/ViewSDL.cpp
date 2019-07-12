#include "Fabula/pch.h"
#include "ViewSDL.h"

#include "SDL_image.h"

#include "Fabula/Graphics/API/opengl.h"

namespace fbl
{

fblBool ViewSDL::Init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        fblSDLError("SDL Initialization failed")
            return false;
    }

    IMG_Init(IMG_INIT_PNG);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 0);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    m_SDLWindow = SDL_CreateWindow("EpiSys",
                                   SDL_WINDOWPOS_UNDEFINED,
                                   SDL_WINDOWPOS_UNDEFINED,
                                   600,
                                   600,
                                   SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (m_SDLWindow == nullptr)
    {
        fblSDLError("SDL window creation failed")
        return false;
    }

    m_SDL_GLContext = SDL_GL_CreateContext(m_SDLWindow);

    fbl_init_opengl();

    SDL_GL_SetSwapInterval(0);
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);

    return true;
}

void ViewSDL::Shutdown()
{
    SDL_GL_DeleteContext(m_SDL_GLContext);
    SDL_DestroyWindow(m_SDLWindow);
    IMG_Quit();
    SDL_Quit();
}

void ViewSDL::SwapBuffers()
{
    SDL_GL_SwapWindow(m_SDLWindow);
}

void ViewSDL::PollEvents()
{
    SDL_Event eventBuffer[4];
    SDL_PumpEvents();

    while (true)
    {
        const fblS32 eventsCount = SDL_PeepEvents(eventBuffer,
                                                  fblArrLen(eventBuffer),
                                                  SDL_GETEVENT,
                                                  SDL_FIRSTEVENT,
                                                  SDL_LASTEVENT);
        if (eventsCount == 0)
        {
            break;
        }

        if (eventsCount < 0)
        {
            fblSDLError("SDL Failed on peeping events")
        }

        fblFor(eventsCount)
        {
            SDL_Event& event = eventBuffer[index];
            if (event.type == SDL_QUIT)
            {
                Terminate();
                continue;
            }
            ProcessEvent(event);
        }
    }
}

}
