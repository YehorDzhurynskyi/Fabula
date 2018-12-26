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
    SpriteURI SpriteURI;
    Transform Transform;
};

class Snowball
{
public:
    void update();

    Transform Transform;
    vec2f Velocity;
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
    void generateNextChunk();

    template<typename T>
    void generateChunk(const T& chunk)
    {
        const float globalYPos = m_player.Transform.position.y + Camera::g_MinimumVisibleWorldHeight;

        for (const auto& localPosition : chunk)
        {
            Obstacle obstacle;
            obstacle.Transform.position.x = Game::g_MapWidth * 0.5f * localPosition.x;
            obstacle.Transform.position.y = Camera::g_MinimumVisibleWorldHeight * 0.5f * localPosition.y + globalYPos;
            obstacle.SpriteURI = AS(SpriteURI, rand() % AS(u8, SpriteURI::COUNT));

            switch (obstacle.SpriteURI)
            {
            case SpriteURI::Rock:
            {
                obstacle.Transform.size = vec2f(0.75f, 0.75f);
            } break;
            case SpriteURI::Tree1:
            case SpriteURI::Tree2:
            {
                obstacle.Transform.size = vec2f(2.0f, 2.0f); break;
            } break;
            default:
            {
                assert(!"Unrecognized obstacle");
            } break;
            }

            m_obstacles.push_back(obstacle);
        }
    }

private:
    Player m_player;
    SpriteAtlas m_atlas;
    std::vector<Obstacle> m_obstacles;
    std::vector<Snowball> m_snowballs;
};
