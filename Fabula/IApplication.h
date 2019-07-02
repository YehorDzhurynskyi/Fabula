#pragma once

namespace fbl
{
    class IApplication
    {
    public:
        IApplication() = default;
        virtual ~IApplication() = default;

        virtual bool Init() = 0;
        virtual void Shutdown() = 0;

        void Start();
        void Terminate();
        bool IsRunning() const;

    protected:
        virtual void Run() = 0;

    private:
        bool m_IsRunning = false;
    };

    std::unique_ptr<IApplication> create_application();
    extern IApplication* g_Application;
}
