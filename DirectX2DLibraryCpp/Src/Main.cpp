#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include "Engine/Engine.h"
#include "Common/Vec.h"

// プレイヤーの初期座標を宣言し、初期化する
Vec2 P_Position = Vec2(150.0f, 220.0f);

// プレイヤーの基本の移動速度を宣言し、初期化する
float playerspeed = 1.0f;

// プレイヤーの動きを変更するための関数を宣言する
void player();

// プレイヤーの描画するための関数を宣言
void DrawPlayer();

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
	if (Engine::Initialize(1920, 1080, "Sample") == false)
	{
		return 0;
	}

	// テクスチャ読み込み
	Engine::LoadTexture("player", "Res/Robot.png");


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

void player()
{
	// 入力データの更新
	Engine::Update();
	if (Engine::IsKeyboardKeyHeld(DIK_LEFT) == true)
	{
		P_Position.X -= playerspeed;
	}
	
	else if (Engine::IsKeyboardKeyHeld(DIK_RIGHTARROW) == true)
	{
		P_Position.X += playerspeed;
	}
	else if (Engine::IsKeyboardKeyHeld(DIK_UPARROW) == true)
	{
		P_Position.Y -= playerspeed;
	}

	else if (Engine::IsKeyboardKeyHeld(DIK_DOWNARROW) == true)
	{
		P_Position.Y += playerspeed;
	}
}

void DrawPlayer()
{
	Engine::DrawTexture(P_Position.X, P_Position.Y, "player", 255, 0.0f, 1.0f, 1.0f);
}

void GameProcessing()
{
	// 入力データの更新
	Engine::Update();

	player();

}

void DrawProcessing()
{
	// 描画開始
	// 描画処理を実行する場合、必ず最初実行する
	Engine::StartDrawing(0);

	DrawPlayer();


	// 描画終了
		// 描画処理を終了する場合、必ず最後に実行する
	Engine::FinishDrawing();
	
}

