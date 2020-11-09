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

int FramCount_Enemy = 0;				//エネミーの出現の時間管理
int FramCount_Bullet = 0;				//バレットのクールタイム管理
float player_posx=320.0f;				//プレイヤーの初期値
float player_posy=240.0f;
float Enemy_posx = 640.0f;				//エネミーの初期値;
float Enemy_posy = 240.0f;
float bullet_posx = 0;					//バレットの初期値
float bullet_posy = 0;
float background1_x = 0;				//背景の初期値
float background2_x = 962;
const int Enemys = 10;					//エネミーの数
const int Bullets = 10;					//バレットの数
const int HP_MAX = 3;					//プレイヤーのHP

static int No_DamageTime = 120;			//ノーダメージ時間 120
int No_DamageCount=0;

bool Enemy_Contact = false;
bool Bullet_Contact = false;
bool Player_Contact = false;
bool bulletAppearance[Bullets] = {};	//バレットの出現判断
bool ContactEnemy = false;

float Player_Radius = 20;				//プレイヤー半径
float Enemy_Radius = 10;				//エネミー半径
float Bullet_Radius = 4.5;				//バレット半径


class Enemy
{
public:
	float Enemy_posx = 620.0f;		//エネミーの初期値
	float Enemy_posy = 240.0f;
	int posy_speed = 3;
	float EnemyCount = 0;
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

class HP
{
public:
	float HP_posx;
};
HP hp[HP_MAX] = {};				//HPの配列管理
int Contact_C = 0;				//Enemyとの接触回数
bool contact_P[HP_MAX];

enum phase
{
	taitle,
	battle,
	gameover,
};
phase Phase = gameover;

void EnemyErase();
void EnemyClone();
void Playermove();
void BulletClone();
void BulletErase();
void Enemy_Player_Contact();
void Enemy_Bullet_Contact();
void BackGround_move();
void MAXHP_Appearance();
void HP_decrease();

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
	Engine::LoadTexture("Bullet", "Res/Bullet2.PNG");		//バレット
	Engine::LoadTexture("BG1", "Res/side01.jpg");			//背景
	Engine::LoadTexture("BG2", "Res/side02.jpg");
	Engine::LoadTexture("HP", "Res/heart.png");

	srand((unsigned)time(NULL));

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

	switch (Phase)
	{
	case taitle:
		if (Engine::IsKeyboardKeyPushed(DIK_RETURN))
		{
			Phase = battle;
		}
		break;
	case battle:
		
		FramCount_Enemy++;
		FramCount_Bullet++;
		Playermove();
		EnemyClone();
		EnemyErase();
		BulletClone();
		BulletErase();
		Enemy_Player_Contact();
		Enemy_Bullet_Contact();
		BackGround_move();
		MAXHP_Appearance();
		HP_decrease();
		for (int EnemyNUM = 0; EnemyNUM < Enemys; EnemyNUM++)
		{
			enemy[EnemyNUM].EnemyCount++;
			if (FramCount_Enemy > 40 && EnemyAppearance[EnemyNUM] == false)
			{
				EnemyAppearance[EnemyNUM] = true;
				enemy[EnemyNUM].Enemy_posx = 620;
				enemy[EnemyNUM].Enemy_posy = rand() % 350 + 55;
				FramCount_Enemy = 0;
				break;
			}
		}
		if (Contact_C == HP_MAX)//HP最大分当たるとgameover
		{
			Phase = gameover;
		}
		break;
	case gameover:
		if (Engine::IsKeyboardKeyPushed(DIK_RETURN))
		{
			Phase = taitle;
		}
		break;
	}	
}

void DrawProcessing()
{
	// 描画開始
	// 描画処理を実行する場合、必ず最初実行する
	Engine::StartDrawing(0);
	//背景描画
	switch (Phase)
	{
	case taitle:
		Engine::DrawTexture(background1_x, 0, "BG1", 255, 0.0, 0.47, 0.47);
		Engine::DrawFont(160, 220, "シューティングゲーム", FontSize::Large, FontColor::White);
		Engine::DrawFont(180, 380, "Enterキーを押してスタート", FontSize::Regular , FontColor::White);
		break;
	case battle:
		Engine::DrawTexture(background1_x, 0, "BG1", 255, 0.0, 0.47, 0.47);
		Engine::DrawTexture(background2_x, 0, "BG2", 255, 0.0, 0.47, 0.47);
		//プレイヤー描画
		Engine::DrawTexture(player_posx, player_posy, "PlayerMachine");
		//バレット描画
		for (int BulletNUM = 0; BulletNUM < Bullets; BulletNUM++)
		{
			if (bulletAppearance[BulletNUM] == true)
			{
				Engine::DrawTexture(bullet[BulletNUM].bullet_posx, bullet[BulletNUM].bullet_posy, "Bullet");
			}
		}
		//エネミー描画
		for (int EnemyNUM = 0; EnemyNUM < Enemys; EnemyNUM++)
		{
			if (EnemyAppearance[EnemyNUM] == true)
			{
				Engine::DrawTexture(enemy[EnemyNUM].Enemy_posx, enemy[EnemyNUM].Enemy_posy, "Enemy");
			}
		}
		//HP描画
		for (int hpNUM = 0; hpNUM < HP_MAX - Contact_C; hpNUM++)
		{
			if (contact_P[hpNUM] == true)
			{
				hp[hpNUM].HP_posx = hpNUM * 51;
				Engine::DrawTexture(hp[hpNUM].HP_posx, 0, "HP", 255, 0.0, 0.2, 0.2);
			}
		}
		break;
	case gameover:
		Engine::DrawTexture(background1_x, 0, "BG1", 255, 0.0, 0.47, 0.47);
		Engine::DrawFont(240, 180, "GameOver", FontSize::Large, FontColor::White);
		Engine::DrawFont(180, 380, "Enterキーでタイトルへ戻る", FontSize::Regular, FontColor::White);
		break;
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
			enemy[EnemyCount].Enemy_posx -= 2;
			enemy[EnemyCount].Enemy_posy -= enemy[EnemyCount].posy_speed;
			if (enemy[EnemyCount].EnemyCount > 40)
			{
				enemy[EnemyCount].posy_speed *= -1;
				enemy[EnemyCount].EnemyCount = 0;
			}
			
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
	if(ContactEnemy == true)
	{
		return;
	}
	else
	{
		float Player_center_x = player_posx + 32;
		float Player_center_y = player_posy + 32;
		for (int EnemyNUM = 0; EnemyNUM < Enemys; EnemyNUM++)
		{
			if ((enemy[EnemyNUM].Enemy_posx - Player_center_x) * (enemy[EnemyNUM].Enemy_posx - Player_center_x) +
				(enemy[EnemyNUM].Enemy_posy - Player_center_y) * (enemy[EnemyNUM].Enemy_posy - Player_center_y) <=
				(Player_Radius + Enemy_Radius) * (Player_Radius + Enemy_Radius))
			{
				EnemyAppearance[EnemyNUM] = false;
				ContactEnemy = true;
				Contact_C++;
				break;
			}
		}
	}
}

//バレットとエネミーの当たり判定
void Enemy_Bullet_Contact()
{
	for (int EnemyNUM = 0; EnemyNUM < Enemys; EnemyNUM++)
	{
		
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

void BackGround_move()
{
	background1_x -= 2.5;
	background2_x -= 2.5;
	if (background1_x < -962)
	{
		background1_x = 962;
	}
	if (background2_x < -962)
	{
		background2_x = 962;
	}
}

void MAXHP_Appearance()//表示
{
	for (int i= 0; i < HP_MAX - Contact_C;i++)
	{
		contact_P[i] = true;
	}	
}

void HP_decrease()//消去
{	
	if (ContactEnemy == true)
	{
		No_DamageCount++;
		if (No_DamageCount == No_DamageTime)
		{
			No_DamageCount = 0;
			ContactEnemy = false;
		}
	}
	
}
