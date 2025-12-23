#include "Common.h"

Player::Player()
{
    x = 100.0f;
    y = 100.0f;
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
    if (y > 400.0f)
    {
        y = 400.0f;
        vy = 0.0f;
    }
}

void Player::Draw() const
{
    DrawBox(
        static_cast<int>(x),
        static_cast<int>(y),
        static_cast<int>(x + 32),
        static_cast<int>(y + 32),
        GetColor(255, 255, 255),
        TRUE
    );
}