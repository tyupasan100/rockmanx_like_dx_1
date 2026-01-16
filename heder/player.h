#pragma once

class Map;

class Player
{
public:
    Player();

    void Update(const Map& map);
    void DrawPlayer() const;

private:

    void UpdateIdle();  //立ち状態
    void UpdateRun();   //走り状態
    void UpdateJump();  //ジャンプ状態
    void UpdateFall();  //落ち状態
    void UpdateDash();  //ダッシュ状態

    void ApplyMovement(const Map& map);
    void HandleCommonTransition();
    void ResolveGroundCollision(const Map& map);
    void ResolveWallCollision(const Map& map);
    bool CheckOnGround(const Map& map);


    PlayerState state;  //player状態
    Facing facing;

    // 位置
    float x;
    float y;

    // 速度
    float vx;
    float vy;
    float sx;
    int move;

    //プレイヤーフラグ
    bool isGround;  //接地
    bool canDash;

    //プレイヤータイマー
    int dashTimer;
    

};