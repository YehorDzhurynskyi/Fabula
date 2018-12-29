#include "pch.h"
#include "SpriteAtlas.h"
#include "application.h"
#include "SDL_image.h"
#include "Camera.h"

namespace
{

SpriteAtlas::SpriteArray&& initSprites()
{
    Sprite glyph_0;
    glyph_0.Offset = vec2f(0, 256);
    glyph_0.Size = vec2f(88, 128);

    Sprite glyph_1;
    glyph_1.Offset = vec2f(88, 256);
    glyph_1.Size = vec2f(32, 128);

    Sprite glyph_2;
    glyph_2.Offset = vec2f(120, 256);
    glyph_2.Size = vec2f(80, 128);

    Sprite glyph_3;
    glyph_3.Offset = vec2f(200, 256);
    glyph_3.Size = vec2f(80, 128);

    Sprite glyph_4;
    glyph_4.Offset = vec2f(280, 256);
    glyph_4.Size = vec2f(84, 128);

    Sprite glyph_5;
    glyph_5.Offset = vec2f(364, 256);
    glyph_5.Size = vec2f(80, 128);

    Sprite glyph_6;
    glyph_6.Offset = vec2f(444, 256);
    glyph_6.Size = vec2f(80, 128);

    Sprite glyph_7;
    glyph_7.Offset = vec2f(524, 256);
    glyph_7.Size = vec2f(80, 128);

    Sprite glyph_8;
    glyph_8.Offset = vec2f(604, 256);
    glyph_8.Size = vec2f(80, 128);

    Sprite glyph_9;
    glyph_9.Offset = vec2f(684, 256);
    glyph_9.Size = vec2f(80, 128);

    Sprite glyph_M;
    glyph_M.Offset = vec2f(764, 256);
    glyph_M.Size = vec2f(86, 128);

    Sprite plane;
    plane.Offset = vec2f(0, 1536);
    plane.Size = vec2f(512, 512);

    Sprite circle;
    circle.Offset = vec2f(1024, 1536);
    circle.Size = vec2f(512, 512);

    Sprite tree;
    tree.Offset = vec2f(0, 512);
    tree.Size = vec2f(512, 512);

    Sprite rock;
    rock.Offset = vec2f(512, 512);
    rock.Size = vec2f(512, 512);

    SpriteAtlas::SpriteArray result {
        glyph_0, glyph_1, glyph_2, glyph_3, glyph_4,
        glyph_5, glyph_6, glyph_7, glyph_8, glyph_9,
        glyph_M,
        plane, circle, tree, rock
    };

    for (auto& el : result)
    {
        el.Offset /= 2048.0f;
        el.Size /= 2048.0f;
    }

    return std::move(result);
}

SpriteAtlas::AnimatedSpriteArray&& initAnimatedSprites()
{
    AnimatedSprite player;
    player.Offset = vec2f(0, 0) / 2048.0f;
    player.Pitch = 9;
    player.NOfFrames = 9;
    player.Size = vec2f(player.Pitch, (player.NOfFrames / player.Pitch)) * (128.0f / 2048.0f);

    AnimatedSprite snowball;
    snowball.Offset = vec2f(0, 128) / 2048.0f;
    snowball.Pitch = 8;
    snowball.NOfFrames = 8;
    snowball.Size = vec2f(snowball.Pitch, (snowball.NOfFrames / snowball.Pitch)) * (128.0f / 2048.0f);

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

#if 0
    m_sdlTexture = SDL_CreateTextureFromSurface(g_SDLRenderer, sdlSurface);
    if (m_sdlTexture == nullptr)
    {
        REVEAL_SDL_ERROR("SDL creating texture from surface failed")
    }
#endif

    SDL_FreeSurface(sdlSurface);
}

vec2f SpriteAtlas::uvOffsetOf(const SpriteURI uri)
{
    return g_Sprites[AS(u8, uri)].Offset;
}

vec2f SpriteAtlas::uvSizeOf(const SpriteURI uri)
{
    return g_Sprites[AS(u8, uri)].Size;
}

SpriteAtlas::~SpriteAtlas()
{
    SDL_DestroyTexture(m_sdlTexture);
}

SDL_Texture* SpriteAtlas::getSDLTexture()
{
    return m_sdlTexture;
}

void SpriteAtlas::draw(const SDL_Rect& srcRect, const Transform& transform) const
{
    const Transform screenTransform = Camera::get().toScreenSpace(transform);

    SDL_Rect destRect;
    destRect.w = screenTransform.Size.x;
    destRect.h = screenTransform.Size.y;
    destRect.x = screenTransform.Position.x - destRect.w / 2;
    destRect.y = screenTransform.Position.y - destRect.h / 2;

#if 0
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
#endif
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
