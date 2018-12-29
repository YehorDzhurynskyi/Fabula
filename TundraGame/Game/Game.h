#pragma once

#include "SpriteAtlas.h"
#include "Camera.h"
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
    static const float g_ChunkGenerationOffset;

public:
    Game();

    void update();
    void render();

private:
    void generateNextChunk();

    template<typename T>
    void generateChunk(const T& chunk)
    {
        const float globalYPos =
            m_player.Transform.Position.y +
            Camera::g_MinimumVisibleWorldHeight +
            g_ChunkGenerationOffset;

        for (const auto& localPosition : chunk)
        {
            Obstacle obstacle;
            obstacle.Transform.Position.x = Game::g_MapWidth * 0.5f * localPosition.x;
            obstacle.Transform.Position.y = Camera::g_MinimumVisibleWorldHeight * 0.5f * localPosition.y + globalYPos;
            obstacle.SpriteURI = AS(SpriteURI, rand() % (AS(u8, SpriteURI::COUNT) - 3));

            switch (obstacle.SpriteURI)
            {
            case SpriteURI::Rock:
            {
                obstacle.Transform.Size = vec2f(0.75f, 0.75f) + rand01() * vec2f(0.25f, 0.25f);
            } break;
            case SpriteURI::Tree1:
            case SpriteURI::Tree2:
            case SpriteURI::Tree3:
            {
                obstacle.Transform.Size = vec2f(1.75f, 1.75f) + rand01() * vec2f(0.5f, 0.5f);
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

    std::vector<Obstacle> m_Debug; // TODO delete
};
