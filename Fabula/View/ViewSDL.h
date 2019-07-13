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

    fblV2F GetResolution() const override;

protected:
    void SwapBuffers() override;
    void PollEvents() override final;
    virtual void ProcessEvent(const SDL_Event& event) = 0;

private:
    SDL_Window* m_SDLWindow = nullptr;
    SDL_GLContext m_SDL_GLContext = nullptr;
    fblV2S m_CachedResolution = fblV2S(0);
};

}
