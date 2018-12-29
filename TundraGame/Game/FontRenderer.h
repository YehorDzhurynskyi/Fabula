#pragma once

#include "SpriteAtlas.h"

class FontRenderer
{
private:
    FontRenderer();

public:
    static FontRenderer& get()
    {
        static FontRenderer instance;
        return instance;
    }

public:
    void renderText(const char* text, const vec2f position, const float rHeight);
    void renderTextLeft(const char* text, const vec2f position, const float rHeight);
    void renderTextRight(const char* text, const vec2f position, const float rHeight);
    void renderTextCenter(const char* text, const vec2f position, const float rHeight);

private:
    float calculateTextRWidth(const char* text, const float rHeight);

private:
    SpriteAtlas m_atlas;
};
