#pragma once

// =======================
// 外部ライブラリ
// =======================
#include "DxLib.h"

// =======================
// テンプレート関数
// =======================
template <typename T>

constexpr T Clamp(T value, T minVal, T maxVal)
{
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}

// =======================
// 定数・設定
// =======================
//main.
constexpr int SCREEN_WIDTH = 320;
constexpr int SCREEN_HEIGHT = 240;
constexpr int TARGET_FPS = 60;


//graphics.
constexpr int SCALE = 3;           //画面の整数倍 .


//input.
constexpr int KEY_LEFT = KEY_INPUT_LEFT;
constexpr int KEY_RIGHT = KEY_INPUT_RIGHT;
constexpr int KEY_JUMP = KEY_INPUT_Z;
constexpr int KEY_SHOT = KEY_INPUT_C;
constexpr int KEY_DASH = KEY_INPUT_X;


//player.
constexpr int PLAYER_HEIGHT = 16;
constexpr int PLAYER_WIDTH = 16;
//player speed.
constexpr float RUN_SPEED = 1.0f;
constexpr float DASH_SPEED = 2.5;
constexpr float JUMP_SPEED = 5.5f;
constexpr float MAX_FALL_SPEED = 6.5f;
constexpr float WALL_SLIDE_SPEED = 1.0f;
constexpr float WALL_JUMP_X_SPEED = 2.5;
constexpr float WALL_JUMP_Y_SPEED = 6.0f;
constexpr float AIR_MAX_SPEED = DASH_SPEED;
constexpr float AIR_ACCEL = 0.2f;
constexpr float AIR_DRAG = 0.95f;
constexpr float GRAVITY = 0.32f;
constexpr float AIR_CONTROL_RATE = 0.1f;
//player timer.
constexpr int DASH_DURATION = 30;
constexpr int WALL_JUMP_LOCK_FRAMES = 8; 
constexpr int COYOTE_TIME = 30;
constexpr int JUMP_BUFFER_TIME = 30;
//player size.
constexpr int DASH_SNAP_HEIGHT = 4;
constexpr int NORMAL_SNAP_HEIGHT = 1;
constexpr int MAX_STEP_HEIGHT = 4;
constexpr int RESPITE = 2;


//map.
constexpr int TILE_SIZE = 16;
constexpr int MAP_W = SCREEN_WIDTH / TILE_SIZE;
constexpr int MAP_H = SCREEN_HEIGHT / TILE_SIZE;


// =======================
// 列挙型・型定義
// =======================
//seane.
enum class GameState
{
    Title,
    Playing,
    Pause,
    GameOver
};

//player.
enum class PlayerState 
{
    Idle,
    Run,
    Jump,
    Fall,
    Dash,
    Wall
};

enum class MovementState
{
    Ground,
    Air,
    Wall
};

enum class ActionState
{
    None,
    Run,
    Jump,
    Dash,
    WallJump
};

enum class Facing 
{
    Left,
    Right
};

//map.
enum class TileType
{
    Air,
    Block,
    SlopeUpRight,
    SlopeUpLeft
};

// =======================
// 自作クラス
// =======================
#include "Player.h"
#include "Graphics.h"
#include "Input.h"
#include "Map.h"