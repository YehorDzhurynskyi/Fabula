#include "Fabula/pch.h"
#include "View.h"

namespace fbl
{

void View::Run()
{
    m_Clock.Start();
    m_IsRunning = true;
    while (m_IsRunning)
    {
        m_Clock.Tick();
        Tick();

        SwapBuffers();
        PollEvents();
    }
}

void View::Terminate()
{
    m_IsRunning = false;
}

}
