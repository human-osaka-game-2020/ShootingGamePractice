#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include "Engine/Engine.h"
#include "Common/Vec.h"

const int EnemyStock = 20;	//敵表示数

float Playerpos_x = 20.0f;	//プレイヤーのx座標
float Playerpos_y = 210.0f;	//プレイヤーのy座標

class Enemy
{
public:
	float Enemypos_x = 800.0f;	//敵のx座標
	float Enemypos_y = 0.0f;	//敵のy座標

	void EnemyMove();
	void EnemyDisappearance(int num);
};
bool EnemySpown[EnemyStock] = {};	//Enemyを表示しているかを判断
Enemy EnemyClone[EnemyStock];		//EnemyをEnemyStock個複製

int FlameCount_Enemy = 0;	//敵を出してからどれだけ経過したか保存
int EnemyReSpownTime = 0;	//次の敵を出せるまでの時間


void PlayerMachineMove();
void EnemySpownControl();


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
	Engine::LoadTexture("Enemy", "Res/EA1.png");



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

	if (FlameCount_Enemy >= EnemyReSpownTime)
	{
		EnemySpownControl();
		FlameCount_Enemy = 0;
		EnemyReSpownTime = rand() % 30 + 10;
	}
	for (int EnemyNum = 0; EnemyNum < EnemyStock; EnemyNum++)
	{
		if (EnemySpown[EnemyNum] == true)
		{
			EnemyClone[EnemyNum].EnemyMove();
			EnemyClone[EnemyNum].EnemyDisappearance(EnemyNum);
		}
	}
	FlameCount_Enemy++;

}

void DrawProcessing()
{
	// 描画開始
	// 描画処理を実行する場合、必ず最初実行する
	Engine::StartDrawing(0);

	Engine::DrawTexture(Playerpos_x, Playerpos_y, "PlayerMachine");

	for (int enemynum = 0; enemynum < EnemyStock; enemynum++)
	{
		if (EnemySpown[enemynum] == true)
		{
			Engine::DrawTexture(EnemyClone[enemynum].Enemypos_x, EnemyClone[enemynum].Enemypos_y, "Enemy", 255, 0.0f, 1.5f, 1.5f);
		}
	}

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

//敵の出現
void EnemySpownControl()
{
	for (int EnemyNum = 0; EnemyNum < EnemyStock; EnemyNum++)
	{
		if (EnemySpown[EnemyNum] == false)
		{
			EnemyClone[EnemyNum].Enemypos_x = 650.0f;
			EnemyClone[EnemyNum].Enemypos_y = rand() % 400 + 40;
			EnemySpown[EnemyNum] = true;
			break;
		}
	}
}

//敵の移動速度
void Enemy::EnemyMove()
{
	Enemypos_x -= 3.0f;
}

//敵の消滅
void Enemy::EnemyDisappearance(int num)
{
	if (EnemyClone[num].Enemypos_x <= -20.0f)
	{
		EnemySpown[num] = false;
	}
}
