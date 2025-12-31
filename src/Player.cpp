#include "Common.h"

Player::Player()
{
    x = 100.0f;
    y = 0.0f;
    vx = 0.0f;
    vy = 0.0f;
    state = PlayerState::Idle;
}

void Player::Update()
{
    // ‰¼Fd—Í‚¾‚¯

    switch (state)
    {
    case PlayerState::Idle:
        UpdateIdle();
        break;
    case PlayerState::Run:
        UpdateRun();
        break;
    case PlayerState::Jump:
        UpdateJump();
        break;
    case PlayerState::Fall:
        UpdateFall();
        break;
    }

    x += vx;
    y += vy;


}

void Player::UpdateIdle()
{
    vx = 0;

    if (Input::Press(KEY_INPUT_LEFT))
    {
        vx = -3;
        state = PlayerState::Run;
    }

    if (Input::Press(KEY_INPUT_RIGHT))
    {
        vx = 3;
        state = PlayerState::Run;
    }

    if(Input::Press(KEY_INPUT_Z))
    {
        vy = -10;
        state = PlayerState::Jump;
    }
}

void Player::UpdateRun()
{
    if (Input::Press(KEY_INPUT_LEFT))
        vx = -3;
    else if (Input::Press(KEY_INPUT_RIGHT))
        vx = 3;
    else
    {
        vx = 0;
        state = PlayerState::Idle;
    }

    if (Input::Trigger(KEY_INPUT_Z))
    {
        vy = -10;
        state = PlayerState::Jump;
    }
}

void Player::UpdateJump()
{
    vy += 0.6f; // d—Í

    if (vy > 0)
        state = PlayerState::Fall;
}

void Player::UpdateFall()
{
    vy += 0.6f;
    if (vy > 10) {
        vy = 10;
    }
    if (y > 150)
    {
        vy = 0;
        y = 150;
        state = PlayerState::Idle;
    }
}


void Player::Draw() const
{
    if (state == PlayerState::Idle)DrawString(20, 40, "idle", GetColor(255, 255, 255));
    if (state == PlayerState::Run)DrawString(20, 40, "run", GetColor(255, 255, 255));
    if (state == PlayerState::Fall)DrawString(20, 40, "fall", GetColor(255, 255, 255));
    if (state == PlayerState::Jump)DrawString(20, 40, "jump", GetColor(255, 255, 255));

    DrawBox(
        static_cast<int>(x),
        static_cast<int>(y),
        static_cast<int>(x + 16),
        static_cast<int>(y + 16),
        GetColor(255, 255, 255),
        TRUE
    );
}