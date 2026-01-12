#pragma once

// =======================
// 外部ライブラリ
// =======================
#include "DxLib.h"

// =======================
// 定数・設定
// =======================
//main
constexpr int SCREEN_WIDTH = 320;
constexpr int SCREEN_HEIGHT = 180;
constexpr int TARGET_FPS = 60;

//graphics
constexpr int SCALE = 3;           //画面の整数倍 

//input
constexpr int KEY_LEFT = KEY_INPUT_LEFT;
constexpr int KEY_GIGHT = KEY_INPUT_RIGHT;
constexpr int KEY_JUMP = KEY_INPUT_Z;
constexpr int KEY_SHOT = KEY_INPUT_C;
constexpr int KEY_DASH = KEY_INPUT_X;

//player
constexpr int RUN_SPEED = 2;
constexpr int DASH_SPEED = 5;
constexpr int JUMP_SPEED = 8;
constexpr int DASH_DURATION = 30;
constexpr int MAX_FALL_SPEED = 7;
constexpr float GRAVITY = 0.6f;


// =======================
// 列挙型・型定義
// =======================
//seane
enum class GameState
{
    Title,
    Playing,
    Pause,
    GameOver
};

//player
enum class PlayerState {
    Idle,
    Run,
    Jump,
    Fall,
    Dash
};

enum class Facing {
    Left,
    Right
};

// =======================
// 自作クラス
// =======================
#include "Player.h"
#include "Graphics.h"
#include "Input.h"