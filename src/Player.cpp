#include "Common.h"

Player::Player()
{
    x = 100.0f;
    y = 0.0f;
    vx = 0.0f;
    sx = 0.0f;
    vy = 0.0f;
    move = 0;
    state = PlayerState::Idle;
    facing = Facing::Right;
    isGround = false;
    canDash = true;
    dashTimer = 0;
}

void Player::Update(const Map& map)
{
    isGround = CheckOnGround(map);
    // 仮：重力だけ
    HandleCommonTransition();

    switch (state)
    {
    case PlayerState::Idle: UpdateIdle();   break;
    case PlayerState::Run:  UpdateRun();    break;
    case PlayerState::Jump: UpdateJump();   break;
    case PlayerState::Fall: UpdateFall();   break;
    case PlayerState::Dash: UpdateDash();   break;
    }

    ApplyMovement(map);
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
        dashTimer = 0;
        state = PlayerState::Fall;
    }

    
}

void Player::ApplyMovement(const Map& map)
{
    if (!isGround) vy += GRAVITY;
    if (vy > MAX_FALL_SPEED) vy = MAX_FALL_SPEED;

    if (state == PlayerState::Dash)
    {
        x += vx;
    }
    else
    {
        vx = sx * move;
        x += vx;
    }

    ResolveWallCollision(map);

    y += vy;

    ResolveGroundCollision(map);
}
 
bool Player::CheckOnGround(const Map& map)
{
    const int footY = y + PLAYER_HEIGHT;

    const int leftX = x + 2;
    const int rightX = x + PLAYER_WIDTH - 2;

    if (map.IsBlockAtPixel(leftX, footY))  return true;
    if (map.IsBlockAtPixel(rightX, footY)) return true;

    return false;
}

void Player::ResolveWallCollision(const Map& map)
{
    if (vx == 0) return;

    const int topY = y + 2;
    const int bottomY = y + PLAYER_HEIGHT - 2;

    if (vx > 0)
    {
        // 右壁
        int rightX = x + PLAYER_WIDTH;

        if (map.IsBlockAtPixel(rightX, topY) ||
            map.IsBlockAtPixel(rightX, bottomY))
        {
            int tileX = (rightX / TILE_SIZE) * TILE_SIZE;
            x = tileX - PLAYER_WIDTH;
            vx = 0;
            dashTimer = 0;
        }
    }
    else
    {
        // 左壁
        int leftX = x;

        if (map.IsBlockAtPixel(leftX, topY) ||
            map.IsBlockAtPixel(leftX, bottomY))
        {
            int tileX = (leftX / TILE_SIZE + 1) * TILE_SIZE;
            x = tileX;
            vx = 0;
            dashTimer = 0;
        }
    }
}


void Player::ResolveGroundCollision(const Map& map)
{
    if (vy <= 0)return; //落下中のみ

    int footY = y + PLAYER_HEIGHT;

    // 左右のチェック点
    int leftX = x + 2;
    int rightX = x + PLAYER_WIDTH - 2;

    if (!map.IsBlockAtPixel(leftX, footY) &&
        !map.IsBlockAtPixel(rightX, footY))
        return;

    int tileY = (footY / TILE_SIZE) * TILE_SIZE;

    y = tileY - PLAYER_HEIGHT;
    vy = 0;
    isGround = true;
}


void Player::UpdateIdle()
{
    sx = RUN_SPEED;
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


void Player::DrawPlayer() const
{
    if (state == PlayerState::Idle)DrawString(20, 40, "idle", GetColor(255, 255, 255));
    if (state == PlayerState::Run)DrawString(20, 40, "run", GetColor(255, 255, 255));
    if (state == PlayerState::Fall)DrawString(20, 40, "fall", GetColor(255, 255, 255));
    if (state == PlayerState::Jump)DrawString(20, 40, "jump", GetColor(255, 255, 255));
    if (state == PlayerState::Dash)DrawString(20, 40, "dash", GetColor(255, 255, 255));
    
    if(canDash)DrawString(20, 60, "true", GetColor(255, 255, 255));
    else DrawString(20, 60, "false", GetColor(255, 255, 255));

    DrawFormatString(20, 80, GetColor(255, 255, 255) , "%d", dashTimer);
    
    DrawBox(
        static_cast<int>(x),
        static_cast<int>(y),
        static_cast<int>(x + 16),
        static_cast<int>(y + 16),
        GetColor(255, 255, 255),
        TRUE
    );

    if (facing == Facing::Right)
    {
        DrawCircle(
            static_cast<int>(x + 16),
            static_cast<int>(y),
            2,
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
            2,
            GetColor(255, 0, 0),
            true,
            true
        );
    }
    
    DrawCircle(static_cast<int>(x + 2), static_cast<int>(y + PLAYER_HEIGHT), 1, GetColor(0, 255, 0), true, true);
    DrawCircle(static_cast<int>(x + PLAYER_WIDTH - 2), static_cast<int>(y + PLAYER_HEIGHT), 1, GetColor(0, 255, 0), true, true);
}