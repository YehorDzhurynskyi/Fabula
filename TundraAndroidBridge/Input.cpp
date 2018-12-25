#include "pch.h"
#include "Input.h"
#include "Camera.h"
#include "SDL_events.h"
#include "application.h"

namespace
{

SDL_Event EventBuffer[4];

}

namespace Input
{

bool DirectionSwitched = false;

void handle_input()
{
    DirectionSwitched = false;

    SDL_PumpEvents();
    while (true)
    {
        const i32 eventsCount = SDL_PeepEvents(EventBuffer,
                                               ARRLEN(EventBuffer),
                                               SDL_GETEVENT,
                                               SDL_FIRSTEVENT,
                                               SDL_LASTEVENT);
        if (eventsCount == 0)
        {
            break;
        }

        if (eventsCount < 0)
        {
            REVEAL_SDL_ERROR("SDL Failed on peeping events")
        }

        FOR(eventsCount)
        {
            SDL_Event& event = EventBuffer[index];
            switch (event.type)
            {
            case SDL_QUIT:
            {
                g_Running = false;
            } break;
            case SDL_KEYDOWN:
            {
                DirectionSwitched = event.key.keysym.scancode == SDL_SCANCODE_SPACE;
            } break;
            case SDL_FINGERDOWN:
            {
                DirectionSwitched = true;
            } break;
            }
        }
    }
}

}
