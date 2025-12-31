#pragma once

// =======================
// 外部ライブラリ
// =======================
#include "DxLib.h"

// =======================
// 定数・設定
// =======================
constexpr int SCREEN_WIDTH = 320;
constexpr int SCREEN_HEIGHT = 180;
constexpr int TARGET_FPS = 60;
constexpr int SCALE = 3;

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