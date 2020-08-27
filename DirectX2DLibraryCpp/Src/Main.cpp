#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include "Engine/Engine.h"
#include "Common/Vec.h"
#include "Common/Player.h"

// ゲーム処理
void GameProcessing();
// 描画処理
void DrawProcessing();

Player player;

/*
	エントリポイント
*/
int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmpLine,
	_In_ INT       nCmdShow)
{
	// エンジンの初期化
	// ゲームループ開始前に1度だけ実行する
	if (Engine::Initialize(WINDOW_WIDTH, WINDOW_HEIGHT, "Sample") == false)
	{
		return 0;
	}

	while (true)
	{
		bool message_ret = false;
		MSG msg;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			// ゲーム処理
			GameProcessing();

			// 描画開始
			DrawProcessing();
		}
	}

	// エンジン終了
	// ゲームループ終了後に1度だけ実行する
	// テクスチャとサウンドファイルも全て解放する
	Engine::Release();

	// メモリリーク検出
	_CrtDumpMemoryLeaks();

	return 0;

} // プログラム終了

// メイン処理
void GameProcessing()
{
	// 入力データの更新
	Engine::Update();

	float moveSpeed = 0.0f;

	// LShiftキーを押している間は低速モードになる
	if (Engine::IsKeyboardKeyHeld(DIK_LSHIFT)) {
		moveSpeed = player.speed_Slow;
	}
	else if (!Engine::IsKeyboardKeyHeld(DIK_LSHIFT)) {
		moveSpeed = player.speed_Default;
	}

	// 矢印キーで移動　画面外には行けない
	if (Engine::IsKeyboardKeyHeld(DIK_UP)) {
		if (player.pos_y > 0) {
			player.pos_y -= moveSpeed;
		}
	}
	else if (Engine::IsKeyboardKeyHeld(DIK_DOWN)) {
		if (player.pos_y < CALC_WINDOW_Y) {
			player.pos_y += moveSpeed;
		}
	}
	
	if (Engine::IsKeyboardKeyHeld(DIK_LEFT)) {
		if (player.pos_x > 0) {
			player.pos_x -= moveSpeed;
		}
	}
	else if (Engine::IsKeyboardKeyHeld(DIK_RIGHT)) {
		if (player.pos_x < CALC_WINDOW_X) {
			player.pos_x += moveSpeed;
		}
	}

}

void DrawProcessing()
{
	// 描画開始
	// 描画処理を実行する場合、必ず最初実行する
	Engine::StartDrawing(0);

	Engine::LoadTexture("Player", "Res/Chocolate.png");
	Engine::DrawTexture(player.pos_x, player.pos_y, "Player");

	char buf[20];

	snprintf(buf, 20, "%.2f", player.pos_x);
	puts(buf);
	Engine::DrawFont(200.0f, 200.0f, buf, Large, Red);

	char bufy[20];

	snprintf(bufy, 20, "%.2f", player.pos_y);
	puts(bufy);
	Engine::DrawFont(400.0f, 200.0f, bufy, Large, Red);


	// 描画終了
	// 描画処理を終了する場合、必ず最後に実行する
	Engine::FinishDrawing();
}
