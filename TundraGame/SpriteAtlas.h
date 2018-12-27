#pragma once

#include "SDL_render.h"
#include <array>

enum class SpriteURI : u8
{
    Rock = 0,
    Tree1,
    Tree2,
    Tree3,
    Debug1,
    Debug2,
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
    vec2i Offset;
    vec2i Size;
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

public:
    explicit SpriteAtlas(const char* filename);
    ~SpriteAtlas();

    void draw(SpriteURI uri, const Transform& transform) const;
    void draw(AnimatedSpriteURI uri, const Transform& transform, const float time) const;

private:
    void draw(const SDL_Rect& srcRect, const Transform& transform) const;

private:
    SDL_Texture* m_sdlTexture;

    static const SpriteArray g_Sprites;
    static const AnimatedSpriteArray g_AnimatedSprites;
};
