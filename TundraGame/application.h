#pragma once

struct SDL_Window;
struct SDL_Renderer;

extern SDL_Window* g_SDLWindow;
extern SDL_Renderer* g_SDLRenderer;

void run();
