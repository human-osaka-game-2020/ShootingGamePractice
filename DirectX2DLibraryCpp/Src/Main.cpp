﻿#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include "Engine/Engine.h"
#include "Common/Vec.h"

#include<math.h>

#include"Enemy.h"

Vec2 g_Position = Vec2(0.0f, 0.0f);
Vec2 g_Scale = Vec2(1.0f, 1.0f);
float g_Angle = 0.0f;

float player_pos_x = 100.0f;
float player_pos_y = 100.0f;
const float X_posmax = 640.0f;
const float Y_posmax = 480.0f;
const float X_posmin = 0.0f;
const float Y_posmin = 0.0f;
const float pmove = 3.0f;
Enemy* penemy1[5];
extern int EnemyExistenceCount ;

static int bullet[10];
static bool BulletExistence = false;
static float bullet_pos_x;
static float bullet_pos_y;

// ゲーム処理
void GameProcessing();
// 描画処理
void DrawProcessing();



void BulletShoot(bool shoot);

void BulletMove();

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

	Engine::LoadTexture("Enemy", "Res/Enemies/EA1.PNG");
	Engine::LoadTexture("Robo", "Res/Robot/Robot_idle 1.PNG");
	Engine::LoadTexture("Bullet", "Res/Bullet/Bullet1.png");

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
}

void GameProcessing()
{
	// 入力データの更新
	Engine::Update();


	//========================================================
	// キーボードの入力取得
	//========================================================


	//==========[自機の移動処理]==========//
	if (Engine::IsKeyboardKeyHeld(DIK_UP) == true)
	{
		player_pos_y -= pmove;
		if (player_pos_y < Y_posmin)
			player_pos_y = Y_posmin;
	}

	if (Engine::IsKeyboardKeyHeld(DIK_DOWN) == true)
	{
		player_pos_y += pmove;
		if (player_pos_y > Y_posmax - Engine::GetTexture("Robo")->Height)
			player_pos_y = Y_posmax - Engine::GetTexture("Robo")->Height;
	}

	if (Engine::IsKeyboardKeyHeld(DIK_RIGHT) == true)
	{
		player_pos_x += pmove;
		if (player_pos_x > X_posmax - Engine::GetTexture("Robo")->Width)
			player_pos_x = X_posmax - Engine::GetTexture("Robo")->Width;
	}

	if (Engine::IsKeyboardKeyHeld(DIK_LEFT) == true)
	{
		player_pos_x -= pmove;
		if (player_pos_x < X_posmin)
			player_pos_x = X_posmin;
	}


	//==========[弾の処理]==========//

	if (Engine::IsKeyboardKeyPushed(DIK_SPACE) == true)
	{
		BulletExistence = true;
		bullet_pos_x = player_pos_x;
		bullet_pos_y = player_pos_y;
	}
	if (BulletExistence)
	{
		BulletMove();
	}
	

	/////////////////////////////////////////////////////
	
	EnemyExistenceCount++;

}

void DrawProcessing()
{
	// 描画開始
	// 描画処理を実行する場合、必ず最初実行する
	Engine::StartDrawing(0);


	Engine::DrawTexture(player_pos_x, player_pos_y, "Robo", 255, 0.0f,  1.0f,  1.0f);
	penemy1[0]->EnemyDrowing(1);//EnemyCreate(EnemyExistence(&enemy_pos_x, &enemy_pos_y));
	BulletShoot(BulletExistence);

	// 描画終了
	// 描画処理を終了する場合、必ず最後に実行する
	Engine::FinishDrawing();
}




void BulletShoot(bool shoot)
{
	if(shoot)
	Engine::DrawTexture(bullet_pos_x, bullet_pos_y, "Bullet", 255, 0.0f, 0.5f, 0.5f);
}
void BulletMove()
{
	bullet_pos_x += 5;
}


