#include "Common.h"

Player::Player()
{
    state = PlayerState::Idle;
    movementState = MovementState::Air;
    actionState = ActionState::None;
    facing = Facing::Right;

    x = 100.0f;
    y = 50.0f;
    prevY = 0.0;
    prevX = 0.0;
    
    vx = 0.0f;
    sx = 0.0f;
    vy = 0.0f;
    move = 0;

    isGround = false;
    canDash = true;
    canJump = false;
    canWallJump = false;

    dashTimer = 0;
    coyoteTimer = 0;
    jumpBufferTimer = 0;
    wallJumpLockTimer = 0;
}

void Player::Update(const Map& map)
{
    isGround = CheckOnGround(map);
    
    UpdateInput();
    UpdateTimer();

    UpdateMovementState(map);
    UpdateActionState();
    ApplyActionForces();
    //TryJump();
    //TryDash();

    //HandleCommonTransition(map);

    //switch (state)
    //{
    //case PlayerState::Idle: UpdateIdle();   break;
    //case PlayerState::Run:  UpdateRun();    break;
    //case PlayerState::Jump: UpdateJump();   break;
    //case PlayerState::Fall: UpdateFall();   break;
    //case PlayerState::Dash: UpdateDash();   break;
    //case PlayerState::Wall: UpdateWall();   break;
    //}

    ApplyMovement(map);
}

void Player::UpdateInput()
{
    move = 0;

    if (Input::Press(KEY_LEFT))   move--;
    if (Input::Press(KEY_RIGHT))  move++;

    if (move < 0) facing = Facing::Left;
    if (move > 0) facing = Facing::Right;
}

void Player::UpdateTimer()
{
    UpdateCoyoteTime();
    
    UpdateJumpBuffer();

    if (wallJumpLockTimer > 0)
        wallJumpLockTimer--;

    if(dashTimer > 0)
        dashTimer--;
}

void Player::UpdateCoyoteTime()
{
    if (isGround)
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
    bool touchingWall = IsTouchingWall(map, move);

    if (isGround)
    {
        movementState = MovementState::Ground;
        //coyoteTimer = COYOTE_TIME;
    }
    else
    {
        if (touchingWall && vy > 0)
        {
            movementState = MovementState::Wall;
            vy = WALL_SLIDE_SPEED;
        }
        else
        {
            movementState = MovementState::Air;
        }
    }
}

void Player::UpdateActionState()
{
    if ((actionState == ActionState::Dash && dashTimer <= 0)||
        (actionState == ActionState::Run && move == 0))
        actionState = ActionState::None;

    if (move != 0)
        actionState = ActionState::Run;

    if (jumpBufferTimer > 0)
    {
        if (movementState == MovementState::Wall)
        {
            actionState == ActionState::WallJump;
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
            //ダッシュのスピードを
            sx = DASH_SPEED;
            vx = (facing == Facing::Right) ? sx : -sx;
        }
    }


}

void Player::UpdateIdle()
{
    sx = RUN_SPEED;
    if (!isGround) state = PlayerState::Fall;
    else if (move != 0) state = PlayerState::Run;

}

void Player::UpdateRun()
{
    sx = RUN_SPEED;
    if (!isGround)state = PlayerState::Fall;
    else if (move == 0) state = PlayerState::Idle;
}

void Player::UpdateJump()
{
    if (vy >= 0) state = PlayerState::Fall;
}

void Player::UpdateFall()
{
    if (isGround) state = PlayerState::Idle;
}

void Player::UpdateDash()
{
    //最初にスピードを代入
    if (dashTimer == DASH_DURATION)
    {
        //ダッシュのスピードを
        sx = DASH_SPEED;
        vx = (facing == Facing::Right) ? sx : -sx;
    }

    if (dashTimer <= 0)
        state = isGround ? PlayerState::Idle : PlayerState::Fall;
}

void Player::UpdateWall()
{
    if (isGround) state = PlayerState::Idle;
    sx = RUN_SPEED;
}

void Player::TryJump()
{
    if (jumpBufferTimer <= 0) return;

    if (CanWallJump())
    {
        DoWallJump();
        return;
    }

    if (CanGroundJump())
    {
        DoGroundJump();
        return;
    }
}

void Player::TryDash()
{
    if (!canDash) return;
    if (!isGround)return;
    if (!Input::Trigger(KEY_DASH))return;

    dashTimer = DASH_DURATION;
    canDash = false;

    state = PlayerState::Dash;
}

bool Player::CanGroundJump()
{
    return isGround || coyoteTimer> 0;
}

bool Player::CanWallJump()
{
    return state == PlayerState::Wall;
}

void Player::DoGroundJump()
{
    vy = -JUMP_SPEED;
    coyoteTimer = 0;
    jumpBufferTimer = 0;
    dashTimer = 0;
    
    state = PlayerState::Jump;
}

void Player::DoWallJump()
{
    coyoteTimer = 0;
    jumpBufferTimer = 0;

    vy = -WALL_JUMP_Y_SPEED;

    sx = WALL_JUMP_X_SPEED;
    vx = (facing == Facing::Right) ? -sx : sx;

    wallJumpLockTimer = WALL_JUMP_LOCK_FRAMES;
    state = PlayerState::Jump;
}

void Player::HandleCommonTransition(const Map& map)
{
    move = 0;
    

    if (isGround && dashTimer <= 0 && !canDash)
    {
        state = PlayerState::Idle;
    }

    if (state != PlayerState::Dash)
    {

        if (isGround && (state == PlayerState::Fall || move == 0))
        {
            state = PlayerState::Idle;
        }

        if (isGround && move != 0) {
            state = PlayerState::Run;
        }

        if (isGround && canDash && Input::Trigger(KEY_DASH))
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

    if (state != PlayerState::Wall && canJump)
    {
        canJump = false;
        coyoteTimer = 0;
        jumpBufferTimer = 0;
        dashTimer = 0;
        vy = -JUMP_SPEED;
        state = PlayerState::Jump;
    }

    if (!isGround && canWallJump)
    {
        canWallJump = false;
        coyoteTimer = 0;
        jumpBufferTimer = 0;
        vy = -WALL_JUMP_Y_SPEED;
        sx = WALL_JUMP_X_SPEED;
        vx = (facing == Facing::Right) ? -sx : sx;
        wallJumpLockTimer = WALL_JUMP_LOCK_FRAMES;
        state = PlayerState::Jump;
    }

    if (vy > 0)
    {
        dashTimer = 0;
        state = PlayerState::Fall;
    }

    if (!isGround && vy > 0)
    {
        if (IsTouchingWall(map, move))
        {
            state = PlayerState::Wall;
            vy = WALL_SLIDE_SPEED;
        }
    }
}

void Player::ApplyMovement(const Map& map)
{
    
    if (!isGround && movementState != MovementState::Wall) vy += GRAVITY;
    if (vy > MAX_FALL_SPEED) vy = MAX_FALL_SPEED;

    prevY = y;
    y += vy;

    //地面にいるかどうか.
    //isGround = CheckOnGround(map);

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
        if (!isGround) {
            //空中での速度制御.
            //壁ジャンプでのkeylock.
            ApplyAirControl();
        }
        else
        {
            //地上での速度.
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
        vx += move * AIR_ACCEL * sx * ((wallJumpLockTimer > 0)?AIR_CONTROL_RATE:1);
        vx = Clamp(vx, -1 * sx, sx);
    }
    else
    {
        vx *= AIR_DRAG;
    }
}
 
bool Player::CheckOnGround(const Map& map)
{
    //playerの足.
    const int footY = y + PLAYER_HEIGHT;

    //playerの左右の足.
    const int leftX = x + RESPITE;
    const int rightX = x + PLAYER_WIDTH - RESPITE;

    //左右の足の地面のyを取得.
    int gL = map.GetGroundY(leftX, footY);
    int gR = map.GetGroundY(rightX, footY);

    //左右の足が地面のyより高いか低いかの判定.
    if (gL > 0 && gL <= footY) return true;
    if (gR > 0 && gR <= footY) return true;

    return false;
}

void Player::ResolveWallCollision(const Map& map)
{

    if (vx == 0) return;

    const int topY = y + 2;
    const int bottomY = y + PLAYER_HEIGHT - 6;

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

    //落下中のみ.
    if (vy < 0)return;

    // 今の足のyと1フレーム前の足のyを取得.
    int prevFootY = prevY + PLAYER_HEIGHT;
    int currFootY = y + PLAYER_HEIGHT;

    // 左右の足.
    int leftX = x + RESPITE;
    int rightX = x + PLAYER_WIDTH - RESPITE;

    /*
    //sweep
    int bestSweepGroundY = -1;
    // sweep 範囲（足の通過した X 範囲）
    int sweepLeft = min(prevX + FOOT_RESPITE, x + FOOT_RESPITE);
    int sweepRight = max(prevX + PLAYER_WIDTH - FOOT_RESPITE,
        x + PLAYER_WIDTH - FOOT_RESPITE);
    // 横方向 sweep
    for (int px = sweepLeft; px <= sweepRight; px++)
    {
        // 少し上まで見る（斜面・段差用）
        int g = map.GetGroundY(px, currFootY);
        if (g >= 0)
        {
            if (bestSweepGroundY < 0)
                bestSweepGroundY = g;
            else
                bestSweepGroundY = min(bestSweepGroundY, g);
        }
    }

    //初期化.
    int gL = bestSweepGroundY;
    int gR = bestSweepGroundY;
    */

    //初期化.
    int gL = -1;
    int gR = -1;

    //少し上まで見る.
    for (int offset = 0; offset <= MAX_STEP_HEIGHT; offset++)
    {
        //左右の足の地面のyを取得する.
        int checkY = currFootY - offset;
        int groundYL = map.GetGroundY(leftX, checkY);
        int groundYR = map.GetGroundY(rightX, checkY);

        //高い位置のyを保存.
        if (gL < 0)gL = groundYL;
        else if (groundYL > 0) gL = min(gL, groundYL);

        if (gR < 0)gR = groundYR;
        else if (groundYR > 0) gR = min(gR, groundYR);
    }

    //デバッグ用
    test1 = gL;
    test2 = gR;

    //高いほうのyを取得.
    int targetFootY = -1;
    if (gL > 0 && gR > 0)targetFootY = min(gL, gR);
    else targetFootY = max(gL, gR);

    //距離によってどうか.
    if (targetFootY >= 0)
    {
        //playerのyに変換.
        int targetY = targetFootY - PLAYER_HEIGHT;
        int step = y - targetY;

        //足が地面のyより下か.
        if (currFootY >= targetFootY)
        {
            y = targetY;
            vy = 0;
            isGround = true;
            return;
        }

        /*
        if (step > 0 && step <= MAX_STEP_HEIGHT)
        {
            y -= step;
            vy = 0;
            isGround = true;
            return;
        }
        */
    }
}

void Player::ResolveCeilingCollision(const Map& map)
{
    if (vy >= 0)return;

    int headY = y;
    int leftX = x + RESPITE;
    int rightX = x + PLAYER_WIDTH - RESPITE;

    if (!map.IsBlockAtPixel(leftX, headY) &&
        !map.IsBlockAtPixel(rightX, headY))
        return;

    int tileY = (headY / TILE_SIZE + 1) * TILE_SIZE;
    y = tileY;
    vy = 0;
}

void Player::ResolveGroundSnap(const Map& map)
{
    // 上昇中・ジャンプ中は吸いつかせない.
    if (vy < 0) return;

    // playerの足.
    int footY = y + PLAYER_HEIGHT;

    // playerの左右の足.
    int leftX = x + RESPITE;
    int rightX = x + PLAYER_WIDTH - RESPITE;

    int gL = -1;
    int gR = -1;

    //少し下から少し上までのブロックを見る.
    for (int offset = -MAX_STEP_HEIGHT; offset <= MAX_STEP_HEIGHT; offset++) 
    {
        //左右の足の地面の高さを取得する.
        int checkY = footY - offset;
        int groundYL = map.GetGroundY(leftX, checkY);
        int groundYR = map.GetGroundY(rightX, checkY);

        //高い位置のyを保存.
        if (gL < 0)gL = groundYL;
        else if (groundYL > 0) gL = min(gL, groundYL);

        if (gR < 0)gR = groundYR;
        else if (groundYR > 0) gR = min(gR, groundYR);
    }

    //左右の高いほうの地面のyを取得.
    int targetFootY = -1;
    if (gL >= 0 && gR >= 0) targetFootY = min(gL, gR);
    else targetFootY = max(gL, gR);

    //空中なら終わり.
    if (targetFootY < 0) return;

    //足と地面の差を取得.
    int delta = targetFootY - footY;

    int snapHeight = (actionState == ActionState::Dash) ? DASH_SNAP_HEIGHT : NORMAL_SNAP_HEIGHT;

    // 「少し下にある地面だけ」吸いつく.
    if (delta >= 0 && delta <= snapHeight)
    {
        y += delta;
        vy = 0;
        isGround = true;
    }
}

bool Player::IsTouchingWall(const Map& map, int dir)
{
    if (dir == 0)return false;

    int checkX = (dir > 0) ? (x + PLAYER_WIDTH) : x - 1;
    int topY = y + 4;
    int bottomY = y + PLAYER_HEIGHT - 4;

    return map.IsBlockAtPixel(checkX, topY) ||
        map.IsBlockAtPixel(checkX, bottomY);

}

void Player::DrawPlayer() const
{
    if (movementState == MovementState::Air)DrawString(20, 40, "Air", GetColor(255, 255, 255));
    if (movementState == MovementState::Ground)DrawString(20, 40, "Ground", GetColor(255, 255, 255));
    if (movementState == MovementState::Wall)DrawString(20, 40, "Wall", GetColor(255, 255, 255));
    if (actionState == ActionState::None)DrawString(20, 60, "None", GetColor(255, 255, 255));
    if (actionState == ActionState::Run)DrawString(20, 60, "Run", GetColor(255, 255, 255));
    if (actionState == ActionState::Jump)DrawString(20, 60, "Jump", GetColor(255, 255, 255));
    if (actionState == ActionState::Dash)DrawString(20, 60, "Dash", GetColor(255, 255, 255));
    if (actionState == ActionState::WallJump)DrawString(20, 60, "WallJump", GetColor(255, 255, 255));
    
    if(canDash)DrawString(20, 80, "true", GetColor(255, 255, 255));
    else DrawString(20, 80, "false", GetColor(255, 255, 255));
    if (isGround)DrawString(20, 120, "true", GetColor(255, 255, 255));
    else DrawString(20, 120, "false", GetColor(255, 255, 255));
    DrawFormatString(20, 100, GetColor(255, 255, 255) , "%d", dashTimer);
    DrawFormatString(60, 100, GetColor(255, 255, 255), "%d", test1);
    DrawFormatString(100, 100, GetColor(255, 255, 255), "%d", test2);
    DrawFormatString(140, 100, GetColor(255, 255, 255), "%d", coyoteTimer);
    DrawFormatString(180, 100, GetColor(255, 255, 255), "%d", jumpBufferTimer);
    

    DrawBox(static_cast<int>(x), static_cast<int>(y), static_cast<int>(x + 16), static_cast<int>(y + 16), GetColor(255, 255, 255), TRUE);

    if (facing == Facing::Right)//キャラの向き
    {
        DrawCircle(static_cast<int>(x + 16), static_cast<int>(y), 1, GetColor(255, 0, 0), true, true);
    }
    if (facing == Facing::Left)
    {
        DrawCircle(static_cast<int>(x), static_cast<int>(y), 1, GetColor(255, 0, 0), true, true);
    }
    
    DrawCircle(static_cast<int>(x + RESPITE), static_cast<int>(y + PLAYER_HEIGHT), 1, GetColor(0, 255, 0), true, true);
    DrawCircle(static_cast<int>(x + PLAYER_WIDTH - RESPITE), static_cast<int>(y + PLAYER_HEIGHT), 1, GetColor(0, 255, 0), true, true);
    DrawCircle(static_cast<int>(x + PLAYER_WIDTH / 2), static_cast<int>(y + PLAYER_HEIGHT), 1, GetColor(0, 255, 255), true, true);
    
    DrawCircle(static_cast<int>(x), static_cast<int>(y + 2), 1, GetColor(255, 255, 0), true, true);
    DrawCircle(static_cast<int>(x), static_cast<int>(y + PLAYER_HEIGHT - 4), 1, GetColor(255, 255, 0), true, true);
    
    DrawCircle(static_cast<int>(x + PLAYER_WIDTH), static_cast<int>(y + 2), 1, GetColor(255, 255, 0), true, true);
    DrawCircle(static_cast<int>(x + PLAYER_WIDTH), static_cast<int>(y + PLAYER_HEIGHT - 4), 1, GetColor(255, 255, 0), true, true);

    DrawCircle(static_cast<int>(x + RESPITE), static_cast<int>(y), 1, GetColor(255, 0, 255), true, true);
    DrawCircle(static_cast<int>(x + PLAYER_WIDTH - RESPITE), static_cast<int>(y), 1, GetColor(255, 0, 255), true, true);
}