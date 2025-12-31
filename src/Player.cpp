#include "Common.h"

Player::Player()
{
    x = 100.0f;
    y = 0.0f;
    vx = 0.0f;
    vy = 0.0f;
}

void Player::Update()
{
    // ‰¼Fd—Í‚¾‚¯
    const float gravity = 0.5f;
    vy += gravity;

    x += vx;
    y += vy;

    // ‰¼F°
    if (y > 150.0f)
    {
        y = 150.0f;
        vy = 0.0f;
    }
}

void Player::Draw() const
{
    DrawBox(
        static_cast<int>(x),
        static_cast<int>(y),
        static_cast<int>(x + 16),
        static_cast<int>(y + 16),
        GetColor(255, 255, 255),
        TRUE
    );
}