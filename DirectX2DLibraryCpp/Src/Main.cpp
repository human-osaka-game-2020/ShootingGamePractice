#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include "Engine/Engine.h"
#include "Common/Vec.h"

float Playerpos_x = 20.0f;	//プレイヤーのx座標
float Playerpos_y = 210.0f;	//プレイヤーのy座標


void PlayerMachineMove();


// ゲーム処理
void GameProcessing();
// 描画処理
void DrawProcessing();

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
	if (Engine::Initialize(640, 480, "Sample") == false)
	{
		return 0;
	}

	Engine::LoadTexture("PlayerMachine", "Res/Robot_idle1.png");



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

void GameProcessing()
{
	// 入力データの更新
	Engine::Update();

	PlayerMachineMove();

}

void DrawProcessing()
{
	// 描画開始
	// 描画処理を実行する場合、必ず最初実行する
	Engine::StartDrawing(0);

	Engine::DrawTexture(Playerpos_x, Playerpos_y, "PlayerMachine");


	// 描画終了
	// 描画処理を終了する場合、必ず最後に実行する
	Engine::FinishDrawing();
}

//ゲーム処理関数

//プレイヤーの上下左右移動
void PlayerMachineMove()
{
	if (Engine::IsKeyboardKeyHeld(DIK_UP) == true)
	{
		Playerpos_y -= 3;
		if (Playerpos_y <= 0.0f)
		{
			Playerpos_y = 0.0f;
		}
	}
	if (Engine::IsKeyboardKeyHeld(DIK_DOWN) == true)
	{
		Playerpos_y += 4;

		if (Playerpos_y >= 420.0f)
		{
			Playerpos_y = 420.0f;
		}
	}
	if (Engine::IsKeyboardKeyHeld(DIK_RIGHT) == true)
	{
		Playerpos_x += 3;
		if (Playerpos_x >= 580.0f)
		{
			Playerpos_x = 580.0f;
		}
	}
	if (Engine::IsKeyboardKeyHeld(DIK_LEFT) == true)
	{
		Playerpos_x -= 5;
		if (Playerpos_x <= 0.0f)
		{
			Playerpos_x = 0.0f;
		}
	}

}