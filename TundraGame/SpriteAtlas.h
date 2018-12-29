#pragma once

#include "SDL_render.h"
#include <array>

enum class SpriteURI : u8
{
    Glyph_0 = 0,
    Glyph_1,
    Glyph_2,
    Glyph_3,
    Glyph_4,
    Glyph_5,
    Glyph_6,
    Glyph_7,
    Glyph_8,
    Glyph_9,
    Glyph_M,
    Plane,
    Circle,
    Tree,
    Rock,
    COUNT
};

enum class AnimatedSpriteURI : u8
{
    Player = 0,
    Snowball,
    COUNT
};

struct Sprite
{
    vec2f Offset;
    vec2f Size;
};

struct AnimatedSprite : public Sprite
{
    i32 NOfFrames = 0;
    i32 Pitch = 0;
};

class SpriteAtlas
{
public:
    using SpriteArray = std::array<Sprite, AS(u8, SpriteURI::COUNT)>;
    using AnimatedSpriteArray = std::array<AnimatedSprite, AS(u8, AnimatedSpriteURI::COUNT)>;

    static vec2f uvOffsetOf(const SpriteURI uri);
    static vec2f uvSizeOf(const SpriteURI uri);

public:
    explicit SpriteAtlas(const char* filename);
    ~SpriteAtlas();

    SDL_Texture* getSDLTexture();

    void draw(SpriteURI uri, const Transform& transform) const;
    void draw(AnimatedSpriteURI uri, const Transform& transform, const float time) const;

private:
    void draw(const SDL_Rect& srcRect, const Transform& transform) const;

private:
    SDL_Texture* m_sdlTexture;

    static const SpriteArray g_Sprites;
    static const AnimatedSpriteArray g_AnimatedSprites;
};
