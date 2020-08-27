#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include "Engine/Engine.h"
#include "Common/Vec.h"
#include <time.h>

float g_Player_x = 50.0f;		// プレイヤーのx座標
float g_Player_y = 215.0f;		// プレイヤーのy座標
const int EnemyStock = 10;		// 敵の出現可能数
bool EnemySpon[EnemyStock];		// 敵が出現しているか判断
int EnemySponTime = 0;			// 敵が出現した時間
int EnemyElapsedTime = 0;		// 前の敵が出現して経過した時間
const int BulletStock = 10;		// 弾の出現可能数
bool BulletSpon[BulletStock];	// 弾が出現しているか判断
int BulletSponTime = 0;			// 弾の出現した時間
int BulletElapsedTime = 0;		// 前の弾が出現して経過した時間

class Enemy						
{
public:
	float Enemy_x;		// 敵のx座標
	float Enemy_y;		// 敵のy座標
};
Enemy EA[EnemyStock];		// EnemyをEnemyStock個分複製

class Bullet
{
public:
	float Bullet_x;		// 弾のx座標
	float Bullet_y;		// 弾のy座標
};
Bullet BulletPos[BulletStock];		// BulletをBulletStock個分複製

// ゲーム処理
void GameProcessing();
// 描画処理
void DrawProcessing();

void PlayerMove();
void EnemyMove();
void EnemyDraw();
void BulletMove();
void BulletDraw();

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

	// 画像読み込み
	Engine::LoadTexture("Player", "Res/Player.png");
	Engine::LoadTexture("Enemy1", "Res/Enemy1.png");
	Engine::LoadTexture("Bullet1", "Res/Bullet1.png");

	srand((unsigned)time(NULL));

	EnemySponTime = timeGetTime();
	BulletSponTime = timeGetTime() - 400;

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

	PlayerMove();

	EnemyMove();
	
	BulletMove();
}

void DrawProcessing()
{
	// 描画開始
	// 描画処理を実行する場合、必ず最初実行する
	Engine::StartDrawing(0);

	// テクスチャ描画
	Engine::DrawTexture(g_Player_x, g_Player_y, "Player",255,0.0f,1.2f,1.2f);
	EnemyDraw();
	BulletDraw();

	// 描画終了
	// 描画処理を終了する場合、必ず最後に実行する
	Engine::FinishDrawing();
}

// プレイヤーの移動
void PlayerMove()
{
	if (Engine::IsKeyboardKeyHeld(DIK_UP)&& g_Player_y >= 0)
	{	
		g_Player_y -= 2.0f;		
	}
	if (Engine::IsKeyboardKeyHeld(DIK_DOWN)&& g_Player_y <= 420)
	{
		g_Player_y += 2.0f;
	}
	if (Engine::IsKeyboardKeyHeld(DIK_LEFT)&& g_Player_x >= 0)
	{
		g_Player_x -= 2.0f;	
	}
	if (Engine::IsKeyboardKeyHeld(DIK_RIGHT)&& g_Player_x <= 580)
	{	
		g_Player_x += 2.0f;	
	}
}

// 敵の動き
void EnemyMove()
{
	for (int EnemyNum = 0; EnemyNum < EnemyStock; EnemyNum++)
	{
		EnemyElapsedTime = timeGetTime() - EnemySponTime;
		if (EnemySpon[EnemyNum] == false && EnemyElapsedTime >= 1200)
		{
			EnemySpon[EnemyNum] = true;
			EA[EnemyNum].Enemy_x = 640.0f;
			EA[EnemyNum].Enemy_y = 100.0f + rand() % 20 * 16;
			EnemySponTime = timeGetTime();
		}
		if (EnemySpon[EnemyNum] == true)
		{
			EA[EnemyNum].Enemy_x -= 2.0f;
			if (EA[EnemyNum].Enemy_x <= -60.0f)
			{
				EnemySpon[EnemyNum] = false;
			}
		}
	}
}

// 敵の描写
void EnemyDraw()
{
	for (int EnemyNum = 0; EnemyNum < EnemyStock; EnemyNum++)
	{
		if (EnemySpon[EnemyNum] == true)
		{
			Engine::DrawTexture(EA[EnemyNum].Enemy_x, EA[EnemyNum].Enemy_y, "Enemy1",255,0.0f,1.2f,1.2f);
		}
	}
}

// 弾の発射
void BulletMove()
{
	for (int BulletNum = 0; BulletNum < BulletStock; BulletNum++)
	{
		BulletElapsedTime = timeGetTime() - BulletSponTime;
		if (BulletSpon[BulletNum] == false && BulletElapsedTime >= 400)
		{
			if (Engine::IsKeyboardKeyPushed(DIK_SPACE))
			{
				BulletSpon[BulletNum] = true;
				BulletPos[BulletNum].Bullet_x = g_Player_x + 66.0f;
				BulletPos[BulletNum].Bullet_y = g_Player_y + 33.0f;
				BulletSponTime = timeGetTime();
			}
		}
		if (BulletSpon[BulletNum] == true)
		{
			BulletPos[BulletNum].Bullet_x += 3.0f;
			if (BulletPos[BulletNum].Bullet_x >= 650.0f)
			{
				BulletSpon[BulletNum] = false;
			}
		}
	}
}

// 弾の描写
void BulletDraw()
{
	for (int BulletNum = 0; BulletNum < BulletStock; BulletNum++)
	{
		if (BulletSpon[BulletNum] == true)
		{
			Engine::DrawTexture(BulletPos[BulletNum].Bullet_x, BulletPos[BulletNum].Bullet_y,"Bullet1",255,0.0f,0.75f,0.75f);
		}
	}
}