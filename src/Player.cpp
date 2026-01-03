#include "Common.h"

Player::Player()
{
    x = 100.0f;
    y = 0.0f;
    vx = 0.0f;
    vy = 0.0f;
    state = PlayerState::Idle;
    facing = Facing::Right;
}

void Player::Update()
{
    // 仮：重力だけ

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
    case PlayerState::Dash:
        UpdateDash();
        break;
    }

    x += vx;
    y += vy;
}

void Player::UpdateIdle()
{
    int move = 0;
    vx = 0;

    if (Input::Press(KEY_INPUT_LEFT))   move--;
    if (Input::Press(KEY_INPUT_RIGHT))  move++;
    vx = move * RUN_SPEED;

    if (move < 0) facing = Facing::Left;
    if (move > 0) facing = Facing::Right;


    if (vx != 0) {
        state = PlayerState::Run;
    }

    if(Input::Press(KEY_INPUT_Z))
    {
        vy = -JUMP_SPEED;
        state = PlayerState::Jump;
    }
}

void Player::UpdateRun()
{
    int move = 0;
    if (Input::Press(KEY_INPUT_LEFT))   move--;
    if (Input::Press(KEY_INPUT_RIGHT))  move++;
    vx = move * RUN_SPEED;

    if (move < 0) facing = Facing::Left;
    if (move > 0) facing = Facing::Right;


    if(vx == 0)
    {
        vx = 0;
        state = PlayerState::Idle;
    }

    if (Input::Trigger(KEY_INPUT_Z))
    {
        vy = -JUMP_SPEED;
        state = PlayerState::Jump;
    }

    if (Input::Press(KEY_INPUT_X))
    {
        state = PlayerState::Dash;
    }
}

void Player::UpdateJump()
{
    vy += GRAVITY; // 重力

    int move = 0;
    if (Input::Press(KEY_INPUT_LEFT))   move--;
    if (Input::Press(KEY_INPUT_RIGHT))  move++;
    vx = move * RUN_SPEED;

    if (move < 0) facing = Facing::Left;
    if (move > 0) facing = Facing::Right;


    if (vy > 0)
        state = PlayerState::Fall;
}

void Player::UpdateFall()
{
    vy += GRAVITY;
    if (vy > JUMP_SPEED) {
        vy = JUMP_SPEED;
    }

    int move = 0;
    if (Input::Press(KEY_INPUT_LEFT))   move--;
    if (Input::Press(KEY_INPUT_RIGHT))  move++;
    vx = move * RUN_SPEED;

    if (move < 0) facing = Facing::Left;
    if (move > 0) facing = Facing::Right;


    if (y > 150)
    {
        vy = 0;
        y = 150;
        state = PlayerState::Idle;
    }
}

void Player::UpdateDash()
{
    int move = 0;
    if (Input::Press(KEY_INPUT_LEFT))  move--;
    if (Input::Press(KEY_INPUT_RIGHT)) move++;

    if (move == 0)
    {
        vx = 0;
        state = PlayerState::Idle;
        return;
    }

    // 向き更新
    if (move < 0) facing = Facing::Left;
    if (move > 0) facing = Facing::Right;

    vx = move * 6; // ダッシュ速度

    // ジャンプ
    if (Input::Trigger(KEY_INPUT_Z))
    {
        vy = -10;
        state = PlayerState::Jump;
    }

    // ダッシュ解除
    if (!Input::Press(KEY_INPUT_X))
    {
        state = PlayerState::Run;
    }
}


void Player::Draw() const
{
    if (state == PlayerState::Idle)DrawString(20, 40, "idle", GetColor(255, 255, 255));
    if (state == PlayerState::Run)DrawString(20, 40, "run", GetColor(255, 255, 255));
    if (state == PlayerState::Fall)DrawString(20, 40, "fall", GetColor(255, 255, 255));
    if (state == PlayerState::Jump)DrawString(20, 40, "jump", GetColor(255, 255, 255));
    
    if (facing == Facing::Right)
    {
        DrawCircle(
            static_cast<int>(x + 16),
            static_cast<int>(y),
            3,
            GetColor(255, 0, 0),
            true,
            true
        );
    }
    if (facing == Facing::Left)
    {
        DrawCircle(
            static_cast<int>(x),
            static_cast<int>(y),
            3,
            GetColor(255, 0, 0),
            true,
            true
        );
    }
    DrawBox(
        static_cast<int>(x),
        static_cast<int>(y),
        static_cast<int>(x + 16),
        static_cast<int>(y + 16),
        GetColor(255, 255, 255),
        TRUE
    );
}