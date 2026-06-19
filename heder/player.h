#pragma once

class Map;

class Player
{
public:
    Player();

    void Update(const Map& map);
    void DrawPlayer() const;

private:

    void ApplyMovement(const Map& map);
    void ApplyAirControl();
    void ApplyActionForces();

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

    MovementState movementState;
    ActionState actionState;
    Facing facing;

    float x;
    float y;
    float prevY;
    float prevX;

    float vx;
    float vy;
    float sx;
    int move;

    int dashTimer;
    int wallJumpLockTimer;
    int coyoteTimer;
    int jumpBufferTimer;
};
