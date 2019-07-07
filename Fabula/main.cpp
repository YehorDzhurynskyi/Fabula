#include "Fabula/pch.h"
#include "SDL_main.h"
#include "IApplication.h"

#ifdef FBL_PLATFORM_WIN32
# undef main
#endif

int main(int argc, char* argv[])
{
    std::unique_ptr<fbl::IApplication> app = fbl::create_application();
    fbl::g_Application = app.get();

    if (app->Init())
    {
        app->Start();
    }
    app->Shutdown();

    return 0;
}
