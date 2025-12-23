#pragma once

class Player
{
public:
    Player();

    void Update();
    void Draw() const;

private:
    // ˆÊ’u
    float x;
    float y;

    // ‘¬“x
    float vx;
    float vy;
};