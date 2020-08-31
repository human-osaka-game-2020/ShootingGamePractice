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

int FramCount_Enemy = 0;			//エネミーの出現の時間管理
int FramCount_Bullet = 0;			//バレットのクールタイム管理
float player_posx=320.0f;			//プレイヤーの初期値
float player_posy=240.0f;
float Enemy_posx = 640.0f;			//エネミーの初期値
float Enemy_posy = 240.0f;
float bullet_posx = 0;				//バレットの初期値
float bullet_posy = 0;
const int Enemys = 3;				//エネミーの数
const int Bullets = 10;				//バレットの数
bool Enemy_Contact = false;
bool Bullet_Contact = false;
bool Player_Contact = false;
bool bulletAppearance[Bullets] = {};//バレットの出現判断

float Player_Radius = 15;			//プレイヤー半径
float Enemy_Radius = 7;				//エネミー半径
float Bullet_Radius = 4.5;			//バレット半径


class Enemy
{
public:
	float Enemy_posx = 620.0f;		//エネミーの初期値
	float Enemy_posy = 240.0f;
};
bool EnemyAppearance[Enemys] = {};	//エネミーの出現判断
Enemy enemy[Enemys];				//エネミーを複製するための変数

class Bullet
{
public:
	float bullet_posx = player_posx;			//バレットの初期値
	float bullet_posy = player_posy;
};
Bullet bullet[Bullets];				//バレット複製

void EnemyErase();
void EnemyClone();
void Playermove();
void BulletClone();
void BulletErase();
void Enemy_Player_Contact();
void Enemy_Bullet_Contact();

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
	Engine::LoadTexture("Bullet", "Res/Bullet1.PNG");		//バレット

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
	FramCount_Enemy++;
	FramCount_Bullet++;
	Playermove();
	EnemyClone();
	EnemyErase();
	BulletClone();
	BulletErase();
	Enemy_Player_Contact();
	Enemy_Bullet_Contact();
	for (int EnemyNUM = 0; EnemyNUM < Enemys; EnemyNUM++) 
	{
		if (FramCount_Enemy > 60 && EnemyAppearance[EnemyNUM]==false)
		{
			EnemyAppearance[EnemyNUM] = true;
			enemy[EnemyNUM].Enemy_posx = 620;
			FramCount_Enemy = 0;
			break;
		}
	}
	
}

void DrawProcessing()
{
	// 描画開始
	// 描画処理を実行する場合、必ず最初実行する
	Engine::StartDrawing(0);
	Engine::DrawTexture(player_posx, player_posy, "PlayerMachine");
	for (int BulletNUM=0;BulletNUM<Bullets;BulletNUM++)//半径18前後
	{
		if (bulletAppearance[BulletNUM] == true)
		{
			Engine::DrawTexture(bullet[BulletNUM].bullet_posx, bullet[BulletNUM].bullet_posy, "Bullet");
		}
	}
	
	
	for (int EnemyNUM = 0; EnemyNUM < Enemys; EnemyNUM++)
	{
		if (EnemyAppearance[EnemyNUM] == true)
		{
			Engine::DrawTexture(enemy[EnemyNUM].Enemy_posx, enemy[EnemyNUM].Enemy_posy, "Enemy");
		}
	}
	
	
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
	if (Engine::IsKeyboardKeyPushed(DIK_SPACE) == true)
	{
		for (int bulletNUM = 0; bulletNUM < Bullets; bulletNUM++)
		{
			if (FramCount_Bullet >5 && bulletAppearance[bulletNUM] == false)
			{
				bulletAppearance[bulletNUM] = true;
				bullet[bulletNUM].bullet_posx = player_posx+59;
				bullet[bulletNUM].bullet_posy = player_posy+25;
				FramCount_Bullet = 0;
			}
		}
	}
}

//エネミーを複製
void EnemyClone()
{
	for (int EnemyCount = 0; EnemyCount < Enemys; EnemyCount++)
	{
		if (EnemyAppearance[EnemyCount] == true)
		{
			enemy[EnemyCount].Enemy_posx -= 3;
			enemy[EnemyCount].Enemy_posy = 240;
		}
	}
}

//エネミーを消す
void EnemyErase()
{
	for (int enemyNUM = 0; enemyNUM < Enemys; enemyNUM++)
	{
		if (enemy[enemyNUM].Enemy_posx < 0)
		{
			EnemyAppearance[enemyNUM] = false;
		}
	}
	
}

//バレット複製
void BulletClone()
{
	for (int bulletNUM=0;bulletNUM<Bullets;bulletNUM++)
	{
		if (bulletAppearance[bulletNUM] == true)
		{
			bullet[bulletNUM].bullet_posx += 10;
		}
	}
}

//バレットを消す
void BulletErase()
{
	for (int bulletNUM = 0; bulletNUM < Bullets; bulletNUM++)
	{
		if (bullet[bulletNUM].bullet_posx > 640)
		{
			bulletAppearance[bulletNUM] = false;
		}
	}
}

//エネミーとプレイヤーの当たり判定
void Enemy_Player_Contact()
{
	float Player_center_x = player_posx + 32;
	float Player_center_y = player_posy + 25;
	for (int EnemyNUM = 0; EnemyNUM < Enemys; EnemyNUM++)
	{
		float Enemy_center_x = enemy[EnemyNUM].Enemy_posx + 30.5;
		float Enemy_center_y = enemy[EnemyNUM].Enemy_posy + 7;
		
		if ((enemy[EnemyNUM].Enemy_posx - Player_center_x) * (enemy[EnemyNUM].Enemy_posx - Player_center_x)  +
			(enemy[EnemyNUM].Enemy_posy - Player_center_y) * (enemy[EnemyNUM].Enemy_posy - Player_center_y) <=
			(Player_Radius + Enemy_Radius) * (Player_Radius + Enemy_Radius))
		{
			EnemyAppearance[EnemyNUM] = false;
		}
	}
	
}

//バレットとエネミーの当たり判定
void Enemy_Bullet_Contact()
{
	for (int EnemyNUM = 0; EnemyNUM < Enemys; EnemyNUM++)
	{
		float Enemy_center_x = enemy[EnemyNUM].Enemy_posx + 30.5;
		float Enemy_center_y = enemy[EnemyNUM].Enemy_posy + 7;
		for (int BulletNUM = 0; BulletNUM < Bullets; BulletNUM++)
		{
			float Bullet_center_x = bullet[BulletNUM].bullet_posx + 4.5;
			float Bullet_center_y = bullet[BulletNUM].bullet_posy + 4.5;
			if ((enemy[EnemyNUM].Enemy_posx - Bullet_center_x) * (enemy[EnemyNUM].Enemy_posx - Bullet_center_x) +
				(enemy[EnemyNUM].Enemy_posy - Bullet_center_y) * (enemy[EnemyNUM].Enemy_posy - Bullet_center_y) <=
				(Enemy_Radius + Bullet_Radius) * (Enemy_Radius + Bullet_Radius) 
					&& bulletAppearance[BulletNUM] == true 
					&& EnemyAppearance[EnemyNUM]==true)
			{
				EnemyAppearance[EnemyNUM] = false;
				bulletAppearance[BulletNUM] = false;
			}
		}
	}
}
