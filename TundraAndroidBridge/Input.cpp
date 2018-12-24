#include "pch.h"
#include "Input.h"
#include "Camera.h"
#include "SDL_events.h"
#include "application.h"

SDL_Event EventBuffer[4];

void Input::handleInput()
{
#ifdef WIN32
    const u8 *keyboardState = SDL_GetKeyboardState(nullptr);

    MoveLeft = keyboardState[SDL_SCANCODE_LEFT];
    MoveRight = keyboardState[SDL_SCANCODE_RIGHT];
#endif

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
            case SDL_FINGERDOWN:
            {
                if (event.tfinger.x < 0.35f)
                {
                    MoveLeft = true;
                }
                else if (event.tfinger.x > 0.65f)
                {
                    MoveRight = true;
                }
            } break;
            case SDL_FINGERUP:
            {
                if (event.tfinger.x < 0.35f)
                {
                    MoveLeft = false;
                }
                else if (event.tfinger.x > 0.65f)
                {
                    MoveRight = false;
                }
            } break;
            }
        }
    }
}
