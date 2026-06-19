#include "Common.h"

Player::Player()
{
    movementState = MovementState::Air;
    actionState = ActionState::None;
    facing = Facing::Right;

    x = 100.0f;
    y = 50.0f;
    prevY = 0.0f;
    prevX = 0.0f;

    vx = 0.0f;
    sx = 0.0f;
    vy = 0.0f;
    move = 0;

    dashTimer = 0;
    coyoteTimer = 0;
    jumpBufferTimer = 0;
    wallJumpLockTimer = 0;
}

void Player::Update(const Map& map)
{
    UpdateInput();
    UpdateMovementState(map);
    UpdateTimer();
    UpdateActionState();
    ApplyActionForces();

    ApplyMovement(map);
}

void Player::UpdateInput()
{
    move = 0;

    if (Input::Press(KEY_LEFT))  move--;
    if (Input::Press(KEY_RIGHT)) move++;

    if (move < 0) facing = Facing::Left;
    if (move > 0) facing = Facing::Right;
}

void Player::UpdateTimer()
{
    UpdateCoyoteTime();
    UpdateJumpBuffer();

    if (wallJumpLockTimer > 0)
        wallJumpLockTimer--;

    if (dashTimer > 0)
        dashTimer--;
}

void Player::UpdateCoyoteTime()
{
    if (movementState == MovementState::Ground)
    {
        coyoteTimer = COYOTE_TIME;
    }
    else if (coyoteTimer > 0)
    {
        coyoteTimer--;
    }
}

void Player::UpdateJumpBuffer()
{
    if (Input::Trigger(KEY_JUMP))
    {
        jumpBufferTimer = JUMP_BUFFER_TIME;
    }
    else if (jumpBufferTimer > 0)
    {
        jumpBufferTimer--;
    }
}

void Player::UpdateMovementState(const Map& map)
{
    bool onGround     = CheckOnGround(map);
    bool touchingWall = IsTouchingWall(map, move);

    if (onGround)
    {
        movementState = MovementState::Ground;
    }
    else if (touchingWall && vy > 0)
    {
        movementState = MovementState::Wall;
        vy = WALL_SLIDE_SPEED;
    }
    else
    {
        movementState = MovementState::Air;
    }
}

void Player::UpdateActionState()
{
    if ((actionState == ActionState::Dash && dashTimer <= 0) ||
        (actionState == ActionState::Run && move == 0))
        actionState = ActionState::None;

    if (move != 0)
        actionState = ActionState::Run;

    if (jumpBufferTimer > 0)
    {
        if (movementState == MovementState::Wall)
        {
            actionState = ActionState::WallJump;
            jumpBufferTimer = 0;
        }
        else if (movementState == MovementState::Ground || coyoteTimer > 0)
        {
            actionState = ActionState::Jump;
            jumpBufferTimer = 0;
        }
    }

    if (Input::Trigger(KEY_DASH) &&
        movementState == MovementState::Ground &&
        actionState != ActionState::Dash)
    {
        actionState = ActionState::Dash;
        dashTimer = DASH_DURATION;
    }
}

void Player::ApplyActionForces()
{
    if (actionState == ActionState::Run || actionState == ActionState::None)
    {
        sx = RUN_SPEED;
    }

    if (actionState == ActionState::Jump)
    {
        vy = -JUMP_SPEED;
        actionState = ActionState::None;
    }

    if (actionState == ActionState::WallJump)
    {
        vy = -WALL_JUMP_Y_SPEED;
        sx = WALL_JUMP_X_SPEED;
        vx = (facing == Facing::Right) ? -sx : sx;
        wallJumpLockTimer = WALL_JUMP_LOCK_FRAMES;
        actionState = ActionState::None;
    }

    if (actionState == ActionState::Dash)
    {
        if (dashTimer == DASH_DURATION)
        {
            sx = DASH_SPEED;
            vx = (facing == Facing::Right) ? sx : -sx;
        }
    }
}

void Player::ApplyMovement(const Map& map)
{
    if (movementState == MovementState::Air) vy += GRAVITY;
    if (vy > MAX_FALL_SPEED) vy = MAX_FALL_SPEED;

    prevY = y;
    y += vy;

    ResolveCeilingCollision(map);
    ResolveGroundCollision(map);
    ResolveGroundSnap(map);

    prevX = x;
    if (actionState == ActionState::Dash)
    {
        x += vx;
    }
    else
    {
        if (movementState != MovementState::Ground)
        {
            ApplyAirControl();
        }
        else
        {
            vx = sx * move;
        }
        x += vx;
    }

    ResolveWallCollision(map);
}

void Player::ApplyAirControl()
{
    if (move != 0)
    {
        vx += move * AIR_ACCEL * sx * ((wallJumpLockTimer > 0) ? AIR_CONTROL_RATE : 1);
        vx = Clamp(vx, -1 * sx, sx);
    }
    else
    {
        vx *= AIR_DRAG;
    }
}

bool Player::CheckOnGround(const Map& map)
{
    const int footY = static_cast<int>(y) + PLAYER_HEIGHT;
    const int leftX = static_cast<int>(x) + RESPITE;
    const int rightX = static_cast<int>(x) + PLAYER_WIDTH - RESPITE;

    int gL = map.GetGroundY(leftX, footY);
    int gR = map.GetGroundY(rightX, footY);

    if (gL > 0 && gL <= footY) return true;
    if (gR > 0 && gR <= footY) return true;

    return false;
}

void Player::ResolveWallCollision(const Map& map)
{
    if (vx == 0) return;

    const int topY    = static_cast<int>(y) + 2;
    const int bottomY = static_cast<int>(y) + PLAYER_HEIGHT - 6;

    if (vx > 0)
    {
        int rightX = static_cast<int>(x) + PLAYER_WIDTH;

        if (map.IsBlockAtPixel(rightX, topY) ||
            map.IsBlockAtPixel(rightX, bottomY))
        {
            int tileX = (rightX / TILE_SIZE) * TILE_SIZE;
            x = static_cast<float>(tileX - PLAYER_WIDTH);
            vx = 0;
            dashTimer = 0;
        }
    }
    else
    {
        int leftX = static_cast<int>(x);

        if (map.IsBlockAtPixel(leftX, topY) ||
            map.IsBlockAtPixel(leftX, bottomY))
        {
            int tileX = (leftX / TILE_SIZE + 1) * TILE_SIZE;
            x = static_cast<float>(tileX);
            vx = 0;
            dashTimer = 0;
        }
    }
}

void Player::ResolveGroundCollision(const Map& map)
{
    if (vy < 0) return;

    int currFootY = static_cast<int>(y) + PLAYER_HEIGHT;
    int leftX     = static_cast<int>(x) + RESPITE;
    int rightX    = static_cast<int>(x) + PLAYER_WIDTH - RESPITE;

    int gL = -1;
    int gR = -1;

    for (int offset = 0; offset <= MAX_STEP_HEIGHT; offset++)
    {
        int checkY   = currFootY - offset;
        int groundYL = map.GetGroundY(leftX, checkY);
        int groundYR = map.GetGroundY(rightX, checkY);

        if (gL < 0) gL = groundYL;
        else if (groundYL > 0) gL = min(gL, groundYL);

        if (gR < 0) gR = groundYR;
        else if (groundYR > 0) gR = min(gR, groundYR);
    }

    int targetFootY = -1;
    if (gL > 0 && gR > 0) targetFootY = min(gL, gR);
    else                   targetFootY = max(gL, gR);

    if (targetFootY >= 0 && currFootY >= targetFootY)
    {
        y = static_cast<float>(targetFootY - PLAYER_HEIGHT);
        vy = 0;
        movementState = MovementState::Ground;
    }
}

void Player::ResolveCeilingCollision(const Map& map)
{
    if (vy >= 0) return;

    int headY  = static_cast<int>(y);
    int leftX  = static_cast<int>(x) + RESPITE;
    int rightX = static_cast<int>(x) + PLAYER_WIDTH - RESPITE;

    if (!map.IsBlockAtPixel(leftX, headY) &&
        !map.IsBlockAtPixel(rightX, headY))
        return;

    int tileY = (headY / TILE_SIZE + 1) * TILE_SIZE;
    y = static_cast<float>(tileY);
    vy = 0;
}

void Player::ResolveGroundSnap(const Map& map)
{
    if (vy < 0) return;

    int footY  = static_cast<int>(y) + PLAYER_HEIGHT;
    int leftX  = static_cast<int>(x) + RESPITE;
    int rightX = static_cast<int>(x) + PLAYER_WIDTH - RESPITE;

    int gL = -1;
    int gR = -1;

    for (int offset = -MAX_STEP_HEIGHT; offset <= MAX_STEP_HEIGHT; offset++)
    {
        int checkY   = footY - offset;
        int groundYL = map.GetGroundY(leftX, checkY);
        int groundYR = map.GetGroundY(rightX, checkY);

        if (gL < 0) gL = groundYL;
        else if (groundYL > 0) gL = min(gL, groundYL);

        if (gR < 0) gR = groundYR;
        else if (groundYR > 0) gR = min(gR, groundYR);
    }

    int targetFootY = -1;
    if (gL >= 0 && gR >= 0) targetFootY = min(gL, gR);
    else                     targetFootY = max(gL, gR);

    if (targetFootY < 0) return;

    int delta      = targetFootY - footY;
    int snapHeight = (actionState == ActionState::Dash) ? DASH_SNAP_HEIGHT : NORMAL_SNAP_HEIGHT;

    if (delta >= 0 && delta <= snapHeight)
    {
        y += static_cast<float>(delta);
        vy = 0;
        movementState = MovementState::Ground;
    }
}

bool Player::IsTouchingWall(const Map& map, int dir)
{
    if (dir == 0) return false;

    int checkX  = (dir > 0) ? (static_cast<int>(x) + PLAYER_WIDTH) : static_cast<int>(x) - 1;
    int topY    = static_cast<int>(y) + 4;
    int bottomY = static_cast<int>(y) + PLAYER_HEIGHT - 4;

    return map.IsBlockAtPixel(checkX, topY) ||
           map.IsBlockAtPixel(checkX, bottomY);
}

void Player::DrawPlayer() const
{
    if (movementState == MovementState::Air)    DrawString(20, 40, "Air",    GetColor(255, 255, 255));
    if (movementState == MovementState::Ground) DrawString(20, 40, "Ground", GetColor(255, 255, 255));
    if (movementState == MovementState::Wall)   DrawString(20, 40, "Wall",   GetColor(255, 255, 255));

    if (actionState == ActionState::None)     DrawString(20, 60, "None",     GetColor(255, 255, 255));
    if (actionState == ActionState::Run)      DrawString(20, 60, "Run",      GetColor(255, 255, 255));
    if (actionState == ActionState::Jump)     DrawString(20, 60, "Jump",     GetColor(255, 255, 255));
    if (actionState == ActionState::Dash)     DrawString(20, 60, "Dash",     GetColor(255, 255, 255));
    if (actionState == ActionState::WallJump) DrawString(20, 60, "WallJump", GetColor(255, 255, 255));

    DrawFormatString(20,  100, GetColor(255, 255, 255), "%d", dashTimer);
    DrawFormatString(60,  100, GetColor(255, 255, 255), "%d", coyoteTimer);
    DrawFormatString(100, 100, GetColor(255, 255, 255), "%d", jumpBufferTimer);

    if (movementState == MovementState::Ground) DrawString(20, 120, "true",  GetColor(255, 255, 255));
    else                                        DrawString(20, 120, "false", GetColor(255, 255, 255));

    DrawBox(static_cast<int>(x), static_cast<int>(y),
            static_cast<int>(x + PLAYER_WIDTH), static_cast<int>(y + PLAYER_HEIGHT),
            GetColor(255, 255, 255), TRUE);

    if (facing == Facing::Right)
        DrawCircle(static_cast<int>(x + PLAYER_WIDTH), static_cast<int>(y), 1, GetColor(255, 0, 0), true, true);
    if (facing == Facing::Left)
        DrawCircle(static_cast<int>(x),                static_cast<int>(y), 1, GetColor(255, 0, 0), true, true);

    DrawCircle(static_cast<int>(x + RESPITE),                static_cast<int>(y + PLAYER_HEIGHT), 1, GetColor(0, 255, 0),   true, true);
    DrawCircle(static_cast<int>(x + PLAYER_WIDTH - RESPITE), static_cast<int>(y + PLAYER_HEIGHT), 1, GetColor(0, 255, 0),   true, true);
    DrawCircle(static_cast<int>(x + PLAYER_WIDTH / 2),       static_cast<int>(y + PLAYER_HEIGHT), 1, GetColor(0, 255, 255), true, true);

    DrawCircle(static_cast<int>(x),                static_cast<int>(y + 2),                 1, GetColor(255, 255, 0), true, true);
    DrawCircle(static_cast<int>(x),                static_cast<int>(y + PLAYER_HEIGHT - 4), 1, GetColor(255, 255, 0), true, true);
    DrawCircle(static_cast<int>(x + PLAYER_WIDTH), static_cast<int>(y + 2),                 1, GetColor(255, 255, 0), true, true);
    DrawCircle(static_cast<int>(x + PLAYER_WIDTH), static_cast<int>(y + PLAYER_HEIGHT - 4), 1, GetColor(255, 255, 0), true, true);

    DrawCircle(static_cast<int>(x + RESPITE),                static_cast<int>(y), 1, GetColor(255, 0, 255), true, true);
    DrawCircle(static_cast<int>(x + PLAYER_WIDTH - RESPITE), static_cast<int>(y), 1, GetColor(255, 0, 255), true, true);
}
