#include "Common.h"

// ==============================
// エントリーポイント
// ==============================
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int){
    
    SetOutApplicationLogValidFlag(FALSE);// ログ非表示

    ChangeWindowMode(TRUE);// ウィンドウ設定
    SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);
    
    if (DxLib_Init() == -1) return -1;// DXライブラリ初期化
    SetDrawScreen(DX_SCREEN_BACK);

    int prevTime = GetNowCount();// フレーム制御用
    Player player;
    // ==============================
    // メインループ
    // ==============================
    while (ProcessMessage() == 0)
    {
        ClearDrawScreen();
        // --------------------------
        // フレーム開始時間
        // --------------------------
        int currentTime = GetNowCount();
        int deltaTime = currentTime - prevTime;
        prevTime = currentTime;

        // --------------------------
        // 入力更新（後で関数化）
        // --------------------------
        // GetHitKeyStateAll などをここに置く予定

        // --------------------------
        // 更新（Update）
        // --------------------------
        // プレイヤー更新
        // 当たり判定
        // 状態遷移
        player.Update();

        // --------------------------
        // 描画（Draw）
        // --------------------------
        player.Draw();
        DrawString(20, 20, "Game Loop Running", GetColor(255, 255, 255));


        ScreenFlip();

        // --------------------------
        // FPS制御
        // --------------------------
        int frameTime = GetNowCount() - currentTime;
        int waitTime = (1000 / TARGET_FPS) - frameTime;
        if (waitTime > 0)
        {
            WaitTimer(waitTime);
        }
    }

    // ==============================
    // 終了処理
    // ==============================
    DxLib_End();
    return 0;
}