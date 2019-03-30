#include "SDL.h"
#include <assert.h>
#include "glad.h"

SDL_Window* g_SDLWindow = nullptr;
bool g_Running = true;
float g_DeltaTime = 0.0f;

const unsigned int WinFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

#undef main

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        assert(!"SDL Initialization failed");
    }

    const int winWidth = 450;
    const int winHeight = 800;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

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
        assert(!"SDL window creation failed");
    }

    const SDL_GLContext glContext = SDL_GL_CreateContext(g_SDLWindow);

    const bool gladStatus = gladLoadGL();
    assert(gladStatus);

    SDL_GL_SetSwapInterval(1);
#ifdef _DEBUG
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_VERBOSE);
#else
    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_WARN);
#endif

    unsigned long long lastPerfCounter = SDL_GetPerformanceCounter();
    const unsigned long long frequency = SDL_GetPerformanceFrequency();
    int fps = 0;
    float elapsedTime = 0.0f;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.98f, 0.18f, 0.98f, 1.0f);

	while (g_Running)
	{
		SDL_GL_SwapWindow(g_SDLWindow);

		++fps;
		const unsigned long long now = SDL_GetPerformanceCounter();
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

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(g_SDLWindow);
    SDL_Quit();
	
	return 0;
}
