#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include "Engine/Engine.h"
#include "Common/Vec.h"

Vec2 P_Position = Vec2(150.0f, 220.0f);               // プレイヤーの初期座標
Vec2 E_Position = Vec2(800.0f, 0.0f);                 // 敵の初期座標
Vec2 A_Position = Vec2(800.0f, 150.0f);               // アヒルの初期座標
Vec2 K_Position = Vec2(800.0f, 300.0f);               // コロシテくんの初期座標
Vec2 B_Position = Vec2(P_Position.X, P_Position.Y);   // 弾の初期座標


float K_Angle = 0.0f;

bool Enemy_Alive = true;
bool Ahiru_Alive = true;
bool Korositekun_Alive = true;
bool Bullet_Alive;

float Playerspeed      = 10.0f;    // プレイヤーの速度
float Enemyspeed       = 4.0f;     // 敵の速度
float Ahiruspeed_X     = 3.0f;     // アヒルの速度
float Ahiruspeed_Y     = 4.0f;     // アヒルの速度
float Korositekunspeed = 5.0f;     // コロシテくんの速度
float Bulletspeed = 50.0f;         // 弾の速度

int Enemy_Array[];       // 敵の保存配列
int Ahiru_Array[];       // アヒルの保存配列
int Korositekun_Array[]; // コロシテくんの保存配列
int Bullet_Array[];      // 弾の保存配列

int E_Counter = 0; // 敵のカウント
int A_Counter = 0; // アヒルのカウント

void Player();          // プレイヤーの動き
void Enemy();           // 敵の動き
void Ahiru();           // アヒルの動き
void Korositekun();     // コロシテくんの動き
void Bullet();          // 弾の動き

void DrawPlayer();      // プレイヤーの描画
void DrawEnemy();       // 敵の描画
void DrawAhiru();       // アヒルの描画
void DrawKorositekun(); // コロシテくんの描画
void DrawBullet();      // 弾の描画

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
	Engine::LoadTexture("Ahiru", "Res/ahiru.png");
	Engine::LoadTexture("Korositekun", "Res/korositekun.png");
	Engine::LoadTexture("Bullet", "Res/kitune.png");

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
	else if (Engine::IsKeyboardKeyPushed(DIK_SPACE) == true && Bullet_Alive == false)
	{
		Bullet_Alive = true;
		B_Position.X = P_Position.X;
		B_Position.Y = P_Position.Y + 5.0f;
	}
}
void Enemy()
{
	E_Position.X -= Enemyspeed;
}
void Ahiru()
{
	A_Position.X -= Ahiruspeed_X;
	A_Position.Y += Ahiruspeed_Y;

	A_Counter++;

	if (A_Counter == 60)
	{
		Ahiruspeed_Y *= -1;
		A_Counter = 0;
	}
}
void Korositekun()
{
	K_Position.X -= Korositekunspeed;

	if (K_Position.X < 500.0f)
	{
		K_Angle -= 50.0f;
	}
}
void Bullet()
{
	B_Position.X += Bulletspeed;
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
	if (E_Position.X < -50.0f)
	{
		Enemy_Alive = false;
	}
}
void DrawAhiru()
{
	// Ahiruの描画
	if (Ahiru_Alive == true)
	{
		Engine::DrawTexture(A_Position.X, A_Position.Y, "Ahiru", 500, 0.0f, 0.5f, 0.3f);
	}

	if (A_Position.X < -50.0f)
	{
		Ahiru_Alive = false;
	}
}
void DrawKorositekun()
{
	// コロシテくんの描画
	if (Korositekun_Alive == true)
	{
		Engine::DrawTexture(K_Position.X, K_Position.Y, "Korositekun", 500, K_Angle, 0.1f, 0.1f);
	}

	if (K_Position.X < -10.0f)
	{
		Korositekun_Alive = false;
	}
}
void DrawBullet()
{
	// 弾の描画
	if (Bullet_Alive == true)
	{
		Engine::DrawTexture(B_Position.X, B_Position.Y, "Bullet", 500, 0.0f, 0.1f, 0.1f);
	}

	if (B_Position.X > 800)
	{
		Bullet_Alive = false;
	}
}

void GameProcessing()
{
	// 入力データの更新
	Engine::Update();

	Player();

	Enemy();

	Ahiru();

	Korositekun();

	Bullet();
}

void DrawProcessing()
{
	// 描画開始
	Engine::StartDrawing(0);

	DrawPlayer();

	DrawEnemy();

	DrawAhiru();

	DrawKorositekun();

	DrawBullet();

	// 描画終了
	Engine::FinishDrawing();
}