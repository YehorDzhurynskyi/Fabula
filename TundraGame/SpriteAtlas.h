#pragma once

#include "SDL_render.h"
#include <array>

enum class SpriteURI : u8
{
    Rock = 0,
    Bush,
    Stump,
    Tree1,
    Tree2,
    Tree3,
    Player,
    Snowball,
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

    void draw(SpriteURI uri, const Transform& transform) const;

private:
    SDL_Texture* m_sdlTexture;

    std::array<Sprite, AS(u8, SpriteURI::COUNT)> m_sprites {
        Sprite{ vec2i(0, 0), vec2i(128, 128) }, // Rock
        Sprite{ vec2i(128, 0), vec2i(128, 128) }, // Bush
        Sprite{ vec2i(64, 128), vec2i(64, 64) }, // Stump
        Sprite{ vec2i(256, 0), vec2i(128, 128) }, // Tree1
        Sprite{ vec2i(384, 0), vec2i(128, 128) }, // Tree2
        Sprite{ vec2i(0, 128), vec2i(64, 64) }, // Tree3
        Sprite{ vec2i(0, 256), vec2i(128, 128) }, // Player
        Sprite{ vec2i(0, 384), vec2i(128, 128) } //Snowball
    };
};
