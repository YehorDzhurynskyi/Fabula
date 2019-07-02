#include "pch.h"
#include "IApplication.h"

namespace fbl
{
IApplication* g_Application = nullptr;

bool IApplication::IsRunning() const
{
    return m_IsRunning;
}

SDL_Window* IApplication::GetSDLWindow()
{
    assert(m_SDLWindow != nullptr);
    return m_SDLWindow;
}

void IApplication::Terminate()
{
    m_IsRunning = false;s
}
}
