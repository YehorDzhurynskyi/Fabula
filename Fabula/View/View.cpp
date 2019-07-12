#include "Fabula/pch.h"
#include "View.h"

namespace fbl
{

void View::Run()
{
    m_IsRunning = true;
    while (m_IsRunning)
    {
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
