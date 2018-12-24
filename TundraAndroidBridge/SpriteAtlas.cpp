#include "pch.h"
#include "SpriteAtlas.h"
#include "application.h"
#include "SDL_filesystem.h"
#include "SDL_system.h"

SpriteAtlas::SpriteAtlas(const char* filename)
{
    SDL_Surface* sdlSurface = SDL_LoadBMP(filename);
    if (sdlSurface == nullptr)
    {
        REVEAL_SDL_ERROR("SDL failed to load sprite atlas")
    }

    m_sdlTexture = SDL_CreateTextureFromSurface(g_SDLRenderer, sdlSurface);
    if (m_sdlTexture == nullptr)
    {
        REVEAL_SDL_ERROR("SDL creating texture from surface failed")
    }

    SDL_FreeSurface(sdlSurface);
}

SpriteAtlas::~SpriteAtlas()
{
    SDL_DestroyTexture(m_sdlTexture);
}

void SpriteAtlas::draw(SpriteURI uri, const vec2f& position) const
{
    const Sprite& sprite = m_sprites[AS(u8, uri)];
    SDL_Rect srcRect;
    srcRect.x = sprite.offset.x;
    srcRect.y = sprite.offset.y;
    srcRect.w = sprite.size.x;
    srcRect.h = sprite.size.y;

    SDL_Rect destRect;
    destRect.x = position.x;
    destRect.y = position.y;
    destRect.w = sprite.size.x;
    destRect.h = sprite.size.y;

    const i32 res = SDL_RenderCopyEx(g_SDLRenderer,
                                     m_sdlTexture,
                                     &srcRect,
                                     &destRect,
                                     0.0,
                                     nullptr,
                                     SDL_FLIP_NONE);
    if (res < 0)
    {
        REVEAL_SDL_ERROR("SDL creating texture from surface failed")
    }
}
