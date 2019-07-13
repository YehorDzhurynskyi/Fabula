#pragma once

#include "EpiLib/Clock.h"

namespace fbl
{

class View
{
public:
    View() = default;
    View(const View& view) = delete;
    View& operator=(const View& view) = delete;
    View(View&& view) = delete;
    View& operator=(View&& view) = delete;
    virtual ~View() = default;

    virtual fblBool Init() = 0;
    virtual void Shutdown() = 0;

    void Run();
    void Terminate();

    virtual fblV2F GetResolution() const = 0;

protected:
    virtual void Tick() = 0;
    virtual void SwapBuffers() = 0;
    virtual void PollEvents() = 0;

protected:
    epi::Clock m_Clock;

private:
    fblBool m_IsRunning;
};

}
