#include "pch.h"
#include "Game/Game.h"
#include "Camera.h"
#include "Input.h"

const float Game::g_MapWidth = 8.0f;

Game::Game()
    : m_atlas("Assets/atlas.bmp")
{}

void Game::update()
{
    m_player.update();

    const float cameraOffset = Camera::get().getVisibleWorldBounds().y / 4.0f;
    Camera::get().Position.y = m_player.m_transform.position.y + cameraOffset;
}

void Game::render()
{
    m_atlas.draw(SpriteURI::Player, m_player.m_transform);
}

void Player::update()
{
    const float speed = 5.0f;
    const float dx = speed * g_DeltaTime;

    m_transform.position.x -= dx * Input::MovedLeft;
    m_transform.position.x += dx * Input::MovedRight;
}

void Player::render() const
{

}
