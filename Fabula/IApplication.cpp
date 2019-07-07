#include "Fabula/pch.h"
#include "IApplication.h"

namespace fbl
{

IApplication* g_Application = nullptr;

bool IApplication::IsRunning() const
{
    return m_IsRunning;
}

void IApplication::Start()
{
    m_IsRunning = true;
    Run();
}

void IApplication::Terminate()
{
    m_IsRunning = false;
}

}
