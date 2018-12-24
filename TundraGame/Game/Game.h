#pragma once

#include "SpriteAtlas.h"
#include <vector>

class Game;
class Player
{
    friend Game;
public:
    void update();

    Transform Transform;
};

class Obstacle
{
public:
    Transform Transform;
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
    std::vector<Obstacle> m_obstacles;
};
