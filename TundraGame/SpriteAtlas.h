#pragma once

#include "SDL_render.h"
#include <array>

enum class SpriteURI : u8
{
    Test = 0,
    COUNT
};

class SpriteAtlas
{
private:
    struct Sprite
    {
        vec2i offset;
        vec2i size;
    };

public:
    explicit SpriteAtlas(const char* filename);
    ~SpriteAtlas();

    void draw(SpriteURI uri, const vec2f& position) const;

private:
    SDL_Texture* m_sdlTexture;

    std::array<Sprite, AS(u8, SpriteURI::COUNT)> m_sprites {
        { vec2i(0, 0), vec2i(128, 128) }
    };
};
