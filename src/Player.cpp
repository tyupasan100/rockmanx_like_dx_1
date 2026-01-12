#include "Common.h"

Player::Player()
{
    x = 100.0f;
    y = 0.0f;
    vx = 0.0f;
    sx = 0.0f;
    vy = 0.0f;
    state = PlayerState::Idle;
    facing = Facing::Right;
    isGround = true;
    canDash = true;
    dashTimer = 0;
}

void Player::Update()
{
    // âºÅFèdóÕÇæÇØ
    HandleCommonTransition();

    switch (state)
    {
    case PlayerState::Idle: UpdateIdle();   break;
    case PlayerState::Run:  UpdateRun();    break;
    case PlayerState::Jump: UpdateJump();   break;
    case PlayerState::Fall: UpdateFall();   break;
    case PlayerState::Dash: UpdateDash();   break;
    }

    ApplyMovement();
}

void Player::HandleCommonTransition()
{
    move = 0;

    if (isGround && dashTimer <= 0 && !canDash)
    {
        state = PlayerState::Idle;
    }

    if (state != PlayerState::Dash)
    {
        if (Input::Press(KEY_INPUT_LEFT))   move--;
        if (Input::Press(KEY_INPUT_RIGHT))  move++;
        if (move < 0) facing = Facing::Left;
        if (move > 0) facing = Facing::Right;

        if (isGround && (state == PlayerState::Fall || move == 0))
        {
            state = PlayerState::Idle;
        }

        if (isGround && move != 0) {
            state = PlayerState::Run;
        }

        if (isGround && canDash && Input::Press(KEY_INPUT_X))
        {
            dashTimer = DASH_DURATION;
            canDash = false;
            state = PlayerState::Dash;
        }
    }


    if (dashTimer <= 0 && isGround && (!Input::Press(KEY_DASH)))
    {
        canDash = true;
    }

    if (isGround && Input::Trigger(KEY_JUMP))
    {
        dashTimer = 0;
        vy = -JUMP_SPEED;
        state = PlayerState::Jump;
    }

    if (vy > 0)
    {
        state = PlayerState::Fall;
    }

    
}

void Player::ApplyMovement()
{
    isGround = false;


    if (!isGround) vy += GRAVITY;

    if (vy > MAX_FALL_SPEED) vy = MAX_FALL_SPEED;

    if (state == PlayerState::Dash)
    {
        x += vx;
        y += vy;
    }
    else
    {
        x += sx * move;
        y += vy;
    }

    ResolveGroundCollision();
}

void Player::ResolveGroundCollision()
{
    if (y > 150)
    {
        vy = 0;
        y = 150;
        isGround = true;
    }
}


void Player::UpdateIdle()
{
    sx = 0;
}

void Player::UpdateRun()
{
    sx = RUN_SPEED;
}

void Player::UpdateJump()
{

}

void Player::UpdateFall()
{

    if (vy > JUMP_SPEED) {
        vy = JUMP_SPEED;
    }
}

void Player::UpdateDash()
{
    if(dashTimer == DASH_DURATION)
    {   
        sx = DASH_SPEED;
        vx = (facing == Facing::Right) ? sx : -sx;
    }
    
    dashTimer--;
}


void Player::Draw() const
{
    if (state == PlayerState::Idle)DrawString(20, 40, "idle", GetColor(255, 255, 255));
    if (state == PlayerState::Run)DrawString(20, 40, "run", GetColor(255, 255, 255));
    if (state == PlayerState::Fall)DrawString(20, 40, "fall", GetColor(255, 255, 255));
    if (state == PlayerState::Jump)DrawString(20, 40, "jump", GetColor(255, 255, 255));
    if (state == PlayerState::Dash)DrawString(20, 40, "dash", GetColor(255, 255, 255));
    
    if(canDash)DrawString(20, 60, "true", GetColor(255, 255, 255));
    else DrawString(20, 60, "false", GetColor(255, 255, 255));

    DrawFormatString(20, 80, GetColor(255, 255, 255) , "%d", dashTimer);
    
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