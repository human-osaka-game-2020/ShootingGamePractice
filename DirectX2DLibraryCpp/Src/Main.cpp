#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include "Engine/Engine.h"
#include "Common/Vec.h"
#include <time.h>

// ゲーム処理
void GameProcessing();
// 描画処理
void DrawProcessing();

float player_posx=320.0f;	//プレイヤーの初期値
float player_posy=240.0f;
float Enemy_posx = 620.0f;	//エネミーの初期値
float Enemy_posy = 240.0f;
float CreateTimer = 0.0;
int FramCount=0.0f;

void Playermove();
void Enemymove();

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
	

	Engine::LoadTexture("PlayerMachine", "Res/Robot.PNG");	//ロボットの画像読み込み
	Engine::LoadTexture("Enemy", "Res/EA1.PNG");			//エネミー

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
	FramCount++;
	Playermove();
	Enemymove();
}

void DrawProcessing()
{
	// 描画開始
	// 描画処理を実行する場合、必ず最初実行する
	Engine::StartDrawing(0);
	Engine::DrawTexture(player_posx, player_posy, "PlayerMachine");
	Engine::DrawTexture(Enemy_posx, Enemy_posy, "Enemy");


	// 描画終了
	// 描画処理を終了する場合、必ず最後に実行する
	Engine::FinishDrawing();
}

//移動処理
void Playermove()	
{
	if (Engine::IsKeyboardKeyHeld(DIK_UP)==true&& player_posy > 0)
	{
		player_posy -= 5;
	}

	if (Engine::IsKeyboardKeyHeld(DIK_DOWN)==true&&player_posy<420)
	{
		player_posy += 5;
	}
	if (Engine::IsKeyboardKeyHeld(DIK_RIGHT)==true&&player_posx<580)
	{
		player_posx += 5;
	}
	if (Engine::IsKeyboardKeyHeld(DIK_LEFT)==true&&player_posx>0)
	{
		player_posx -= 5;
	}
}

void Enemymove()
{

	if (FramCount>=120)
	{
		Enemy_posx -= 5;
	}
}
