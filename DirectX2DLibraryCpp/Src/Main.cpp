#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include "Engine/Engine.h"
#include "Common/Vec.h"

Vec2 P_Position = Vec2(150.0f, 220.0f);               // プレイヤーの初期座標
Vec2 E_Position = Vec2(800.0f, 0.0f);               // 敵の初期座標

bool Enemy_Alive = true;




float Playerspeed = 10.0f;   // プレイヤーの速度
float Enemyspeed = 4.0f;   // 敵の速度


int Enemy_Array[]; // 敵の保存配列


int E_Counter = 0; // 敵のカウント


void Player();          // プレイヤーの動き
void Enemy();           // 敵の動き


void DrawPlayer();      // プレイヤーの描画
void DrawEnemy();       // 敵の描画


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
	if (Engine::Initialize(840, 480, "Shooting") == false)
	{
		return 0;
	}

	// テクスチャ読み込み
	Engine::LoadTexture("Player", "Res/Aircraft.png");
	Engine::LoadTexture("Enemy", "Res/Enemy.png");

	// サウンド読み込み


	// サウンド再生


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

void Player()
{
	// キーボードの入力取得
	if (Engine::IsKeyboardKeyHeld(DIK_LEFT) && P_Position.X > 5 == true)
	{
		P_Position.X -= Playerspeed;
	}
	else if (Engine::IsKeyboardKeyHeld(DIK_RIGHT) && P_Position.X < 775 == true)
	{
		P_Position.X += Playerspeed;
	}
	else if (Engine::IsKeyboardKeyHeld(DIK_UP) && P_Position.Y > 0 == true)
	{
		P_Position.Y -= Playerspeed;
	}
	else if (Engine::IsKeyboardKeyHeld(DIK_DOWN) && P_Position.Y < 460 == true)
	{
		P_Position.Y += Playerspeed;
	}
//	else if (Engine::IsKeyboardKeyPushed(DIK_SPACE) == true && Bullet_Alive == false)
//	{
//		Bullet_Alive = true;
//		B_Position.X = P_Position.X - 20.0f;
//		B_Position.Y = P_Position.Y - 50.0f;
//	}
}
void Enemy()
{
	E_Position.X -= Enemyspeed;
}

void DrawPlayer()
{
	// プレイヤーの描画
	Engine::DrawTexture(P_Position.X, P_Position.Y, "Player", 500, 0.0f, 1.0f, 1.0f);
}
void DrawEnemy()
{
	// 敵の描画
	if (Enemy_Alive == true)
	{
		Engine::DrawTexture(E_Position.X, E_Position.Y, "Enemy", 500, 0.0f, 0.5f, 0.56f);
	}
	/*
	for (Enemy_Array[] = 0;)
	{

	}
	*/
	if (E_Position.X < 0.0f)
	{
		Enemy_Alive = false;
	}
}

void GameProcessing()
{
	// 入力データの更新
	Engine::Update();

	Player();

	Enemy();
}

void DrawProcessing()
{
	// 描画開始
	Engine::StartDrawing(0);

	DrawPlayer();

	DrawEnemy();

	// 描画終了
	Engine::FinishDrawing();
}