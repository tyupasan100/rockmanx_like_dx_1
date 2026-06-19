#pragma once

// =======================
// �O�����C�u����
// =======================
#include "DxLib.h"

// =======================
// �e���v���[�g�֐�
// =======================
template <typename T>

constexpr T Clamp(T value, T minVal, T maxVal)
{
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}

// =======================
// �萔�E�ݒ�
// =======================
//main.
constexpr int SCREEN_WIDTH = 320;
constexpr int SCREEN_HEIGHT = 240;
constexpr int TARGET_FPS = 60;


//graphics.
constexpr int SCALE = 3;           //��ʂ̐����{ .


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
// �񋓌^�E�^��`
// =======================
//seane.
enum class GameState
{
    Title,
    Playing,
    Pause,
    GameOver
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
// ����N���X
// =======================
#include "Player.h"
#include "Graphics.h"
#include "Input.h"
#include "Map.h"