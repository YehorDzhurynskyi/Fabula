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
    glyph_1.Size = vec2f(64, 128);

    Sprite glyph_2;
    glyph_2.Offset = vec2f(152, 256);
    glyph_2.Size = vec2f(80, 128);

    Sprite glyph_3;
    glyph_3.Offset = vec2f(232, 256);
    glyph_3.Size = vec2f(80, 128);

    Sprite glyph_4;
    glyph_4.Offset = vec2f(312, 256);
    glyph_4.Size = vec2f(84, 128);

    Sprite glyph_5;
    glyph_5.Offset = vec2f(396, 256);
    glyph_5.Size = vec2f(80, 128);

    Sprite glyph_6;
    glyph_6.Offset = vec2f(476, 256);
    glyph_6.Size = vec2f(80, 128);

    Sprite glyph_7;
    glyph_7.Offset = vec2f(556, 256);
    glyph_7.Size = vec2f(80, 128);

    Sprite glyph_8;
    glyph_8.Offset = vec2f(636, 256);
    glyph_8.Size = vec2f(80, 128);

    Sprite glyph_9;
    glyph_9.Offset = vec2f(716, 256);
    glyph_9.Size = vec2f(80, 128);

    Sprite glyph_M;
    glyph_M.Offset = vec2f(796, 256);
    glyph_M.Size = vec2f(86, 128);

    Sprite plane;
    plane.Offset = vec2f(0, 1536);
    plane.Size = vec2f(512, 512);

    Sprite circle;
    circle.Offset = vec2f(512, 1536);
    circle.Size = vec2f(512, 512);

    Sprite tree;
    tree.Offset = vec2f(0, 512);
    tree.Size = vec2f(512, 512);

    Sprite rock;
    rock.Offset = vec2f(512, 512);
    rock.Size = vec2f(512, 512);

    Sprite iconPause;
    iconPause.Offset = vec2f(0, 384);
    iconPause.Size = vec2f(128, 128);

    SpriteAtlas::SpriteArray result {
        glyph_0, glyph_1, glyph_2, glyph_3, glyph_4,
        glyph_5, glyph_6, glyph_7, glyph_8, glyph_9,
        glyph_M,
        plane, circle, tree, rock, iconPause
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
    player.Pitch = 12;
    player.NOfFrames = 12;
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

const Sprite& SpriteAtlas::at(const SpriteURI uri)
{
    return g_Sprites[AS(u8, uri)];
}
const AnimatedSprite& SpriteAtlas::at(const AnimatedSpriteURI uri)
{
    return g_AnimatedSprites[AS(u8, uri)];
}
