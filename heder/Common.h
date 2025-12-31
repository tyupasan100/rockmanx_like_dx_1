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
constexpr int KEY_JUNP = KEY_INPUT_Z;
constexpr int KEY_SHOT = KEY_INPUT_X;


// =======================
// 列挙型・型定義
// =======================
enum class GameState
{
    Title,
    Playing,
    Pause,
    GameOver
};

// =======================
// 自作クラス
// =======================
#include "Player.h"
#include "Graphics.h"
#include "Input.h"