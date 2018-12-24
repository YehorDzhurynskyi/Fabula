#include "pch.h"
#include "SpriteAtlas.h"
#include "application.h"
#include "Camera.h"

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

void SpriteAtlas::draw(SpriteURI uri, const Transform& transform) const
{
    const Sprite& sprite = m_sprites[AS(u8, uri)];
    SDL_Rect srcRect;
    srcRect.x = sprite.offset.x;
    srcRect.y = sprite.offset.y;
    srcRect.w = sprite.size.x;
    srcRect.h = sprite.size.y;

    const Transform screenTransform = Camera::get().toScreenSpace(transform);

    SDL_Rect destRect;
    destRect.w = screenTransform.size.x;
    destRect.h = screenTransform.size.y;
    destRect.x = screenTransform.position.x - destRect.w / 2;
    destRect.y = screenTransform.position.y - destRect.h / 2;

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
