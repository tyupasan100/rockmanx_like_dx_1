#pragma once

class Player
{
public:
    Player();

    void Update();
    void Draw() const;

private:

    void UpdateIdle();  //立ち状態
    void UpdateRun();   //走り状態
    void UpdateJump();  //ジャンプ状態
    void UpdateFall();  //落ち状態

    PlayerState state;  //player状態

    // 位置
    float x;
    float y;

    // 速度
    float vx;
    float vy;
};