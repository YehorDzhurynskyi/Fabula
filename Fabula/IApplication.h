#pragma once

struct SDL_Window;

namespace fbl
{
    class FBL_API IApplication
    {
    public:
        IApplication() = default;
        virtual ~IApplication() = default;

        virtual void Init() = 0;
        virtual void Shutdown() = 0;

        virtual void Run() = 0;

        void Terminate();
        bool IsRunning() const;

        SDL_Window* GetSDLWindow();

    protected:
        bool m_IsRunning = false;
        SDL_Window* m_SDLWindow = nullptr;
    };

    std::unique_ptr<IApplication> create_application();
    extern IApplication* g_Application;
}
