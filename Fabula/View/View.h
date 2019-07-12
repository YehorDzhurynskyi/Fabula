#pragma once

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

protected:
    virtual void Tick() = 0;
    virtual void SwapBuffers() = 0;
    virtual void PollEvents() = 0;

private:
    fblBool m_IsRunning;
};

}
