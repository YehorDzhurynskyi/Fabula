#include "pch.h"
#include "application.h"
#include "SDL_main.h"

#ifdef FBL_PLATFORM_WIN32
# undef main
#endif

int main(int argc, char* argv[])
{
    run();
    return 0;
}
