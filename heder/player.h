#pragma once

class Map;

class Player
{
public:
    Player();

    void Update(const Map& map);
    void DrawPlayer() const;

private:

    void UpdateIdle();  //立ち状態.
    void UpdateRun();   //走り状態.
    void UpdateJump();  //ジャンプ状態.
    void UpdateFall();  //落ち状態.
    void UpdateDash();  //ダッシュ状態.
    void UpdateWall();  //壁ずり状態.

    void ApplyMovement(const Map& map);
    void ApplyAirControl();
    void ApplyActionForces();
    void HandleCommonTransition(const Map& map);

    void ResolveGroundCollision(const Map& map);
    void ResolveWallCollision(const Map& map);
    void ResolveCeilingCollision(const Map& map);
    void ResolveGroundSnap(const Map& map);
    bool CheckOnGround(const Map& map);
    bool IsTouchingWall(const Map& map, int dir);

    void UpdateInput();
    void UpdateTimer();
    void UpdateMovementState(const Map& map);
    void UpdateActionState();
    void UpdateCoyoteTime();
    void UpdateJumpBuffer();

    void TryJump();

    bool CanWallJump();
    bool CanGroundJump();

    void DoGroundJump();
    void DoWallJump();

    void TryDash();

    MovementState movementState;
    ActionState actionState;
    PlayerState state;  //player状態.
    Facing facing;

    // 位置.
    float x;
    float y;
    float prevY;
    float prevX;

    // 速度.
    float vx;
    float vy;
    float sx;
    int move;

    //フラグ.
    bool isGround;  //接地.
    bool canDash;
    bool canJump;
    bool canWallJump;

    //タイマー.
    int dashTimer;
    int wallJumpLockTimer;
    int coyoteTimer;
    int jumpBufferTimer;

    //test value.
    int test1;
    int test2;
    

};