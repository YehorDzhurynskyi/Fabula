#pragma once

#include "SpriteAtlas.h"

class Game;
class Player
{
    friend Game;
public:
    void update();
    void render() const;

private:
    Transform m_transform;
};

class Game
{
public:
    static const float g_MapWidth;

public:
    Game();

    void update();
    void render();

private:
    Player m_player;
    SpriteAtlas m_atlas;
};
