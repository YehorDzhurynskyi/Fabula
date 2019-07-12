#pragma once

#include "View.h"
#include <SDL.h>

namespace fbl
{

class ViewSDL : public View
{
public:
    fblBool Init() override;
    void Shutdown() override;

protected:
    void SwapBuffers() override;
    void PollEvents() override final;
    virtual void ProcessEvent(const SDL_Event& event) = 0;

protected:
    SDL_Window* m_SDLWindow = nullptr;
    SDL_GLContext m_SDL_GLContext = nullptr;
};

}