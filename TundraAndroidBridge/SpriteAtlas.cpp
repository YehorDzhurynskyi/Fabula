#include "pch.h"
#include "SpriteAtlas.h"
#include "application.h"
#include "SDL_image.h"
#include "Camera.h"

namespace
{

SpriteAtlas::SpriteArray&& initSprites()
{
    Sprite rock;
    rock.Offset = vec2i(1024, 512);
    rock.Size = vec2i(512, 512);

    Sprite tree1;
    tree1.Offset = vec2i(0, 512);
    tree1.Size = vec2i(512, 512);

    Sprite tree2;
    tree2.Offset = vec2i(512, 512);
    tree2.Size = vec2i(512, 512);

    Sprite tree3;
    tree3.Offset = vec2i(1536, 512);
    tree3.Size = vec2i(512, 512);

    Sprite debug1;
    debug1.Offset = vec2i(0, 1536);
    debug1.Size = vec2i(512, 512);

    Sprite debug2;
    debug2.Offset = vec2i(512, 1536);
    debug2.Size = vec2i(512, 512);

    Sprite colliderDebug;
    colliderDebug.Offset = vec2i(1024, 1536);
    colliderDebug.Size = vec2i(512, 512);

    return SpriteAtlas::SpriteArray {
        rock,
        tree1, tree2, tree3,
        debug1, debug2, colliderDebug
    };
}

SpriteAtlas::AnimatedSpriteArray&& initAnimatedSprites()
{
    AnimatedSprite player;
    player.Offset = vec2i(0, 0);
    player.Pitch = 9;
    player.NOfFrames = 9;
    player.Size = vec2i(player.Pitch, (player.NOfFrames / player.Pitch));
    player.Size *= 128;

    AnimatedSprite snowball;
    snowball.Offset = vec2i(0, 128);
    snowball.Pitch = 8;
    snowball.NOfFrames = 8;
    snowball.Size = vec2i(snowball.Pitch, (snowball.NOfFrames / snowball.Pitch));
    snowball.Size *= 128;

    return SpriteAtlas::AnimatedSpriteArray{ player, snowball };
}

}

const SpriteAtlas::SpriteArray SpriteAtlas::g_Sprites = initSprites();
const SpriteAtlas::AnimatedSpriteArray SpriteAtlas::g_AnimatedSprites = initAnimatedSprites();

SpriteAtlas::SpriteAtlas(const char* filename)
{
    SDL_Surface* sdlSurface = IMG_Load(filename);
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

void SpriteAtlas::draw(const SDL_Rect& srcRect, const Transform& transform) const
{
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

void SpriteAtlas::draw(SpriteURI uri, const Transform& transform) const
{
    const Sprite& sprite = g_Sprites[AS(u8, uri)];
    SDL_Rect srcRect;
    srcRect.x = sprite.Offset.x;
    srcRect.y = sprite.Offset.y;
    srcRect.w = sprite.Size.x;
    srcRect.h = sprite.Size.y;

    draw(srcRect, transform);
}

void SpriteAtlas::draw(AnimatedSpriteURI uri, const Transform& transform, const float time) const
{
    const AnimatedSprite& sprite = g_AnimatedSprites[AS(u8, uri)];
    const i32 sizeOfFrame = sprite.Size.x / sprite.Pitch;

    const i32 frame = (i32)time % sprite.NOfFrames;
    assert(frame == clamp<i32>(frame, 0, sprite.NOfFrames - 1));

    SDL_Rect srcRect;
    srcRect.x = sprite.Offset.x + sizeOfFrame * (frame % sprite.Pitch);
    srcRect.y = sprite.Offset.y + sizeOfFrame * (frame / sprite.Pitch);
    srcRect.w = sizeOfFrame;
    srcRect.h = sizeOfFrame;

    draw(srcRect, transform);
}
