/*

#define _CRTDBG_MAP_ALLOC
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
	penemy1[0]->EnemyDrowing(true);//EnemyCreate(EnemyExistence(&enemy_pos_x, &enemy_pos_y));
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


*/



#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include "Engine/Engine.h"
#include "Common/Vec.h"

#include<math.h>

Vec2 g_Position = Vec2(0.0f, 0.0f);
Vec2 g_Scale = Vec2(1.0f, 1.0f);
float g_Angle = 0.0f;

static float player_pos_x = 100.0f;
static float player_pos_y = 100.0f;
const float enemy_initialize_x = 680.0f;
const float enemy_initialize_y = 240.0f;
static float enemy1_pos_x = enemy_initialize_x;
static float enemy1_pos_y = enemy_initialize_y;
static float enemy2_pos_x = enemy_initialize_x;
static float enemy2_pos_y = enemy_initialize_y;
static float enemy3_pos_x = enemy_initialize_x;
static float enemy3_pos_y = enemy_initialize_y;
static float enemy4_pos_x = enemy_initialize_x;
static float enemy4_pos_y = player_pos_y;
static float rotation = 0.0f;
const float X_posmax = 640.0f;
const float Y_posmax = 480.0f;
const float X_posmin = 0.0f;
const float Y_posmin = 0.0f;
const float pmove = 3.0f;
const float emove = 2.0f;
const float Angle_finer_tuning = 10.0f;
static int PlayerHP = 3;
static int Enemy1HP = 1;
static int Enemy2HP = 1;
static int Enemy3HP = 1;
static int Enemy4HP = 1;
static int DestroyCount;
static bool NoDamage = false;
static int NoDamageTime = 0;
static bool Collision;

static bool BulletExistence = false;
static float bullet_pos_x;
static float bullet_pos_y;

static float background_pos_x = 0.0f;
static float background_pos_y = 0.0f;
static bool  ScrollEnd = false;
static int GameEndCount = 0;
static int ElapsedTime ;
static char ElapsedTimeString[20];

float tmp_wid; 
float tmp_hei; 
float y;
float x;
float tmp_dia; 
float diagonal; 

static float radian;
float Y_increment;
float X_increment;



enum phase
{
	Title,
	Battle,
	Result
};
phase Scene = Title;

// ゲーム処理
void GameProcessing();
// 描画処理
void DrawProcessing();

float Pai();

float Angle(float r);

void Enemy1Create(bool cancel);

bool Enemy1Existence(float epx, float epy);

void Enemy2Create(bool cancel);

bool Enemy2Existence(float epx, float epy);

void Enemy3Create(bool cancel);

bool Enemy3Existence(float epx, float epy);

void Enemy4Create(bool cancel);

bool Enemy4Existence(float epx, float epy);

void BulletShoot(bool shoot);

void BulletMove();

void BackGround();

void SceneTransfer(bool ScrollEnd);

void PlayerHPControler(bool Collision_Player);

bool Collision_Player();

bool Collision_Bullet(int* Enemy1HP, int* Enemy2HP, int* Enemy3HP, int* Enemy4HP);

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

	Engine::LoadTexture("Enemy1", "Res/Enemies/EA4 1.PNG");		//追跡
	Engine::LoadTexture("Enemy2", "Res/Enemies/EA6 1.PNG");		//円運動
	Engine::LoadTexture("Enemy3", "Res/Enemies/EA6 4.PNG");		//上下
	Engine::LoadTexture("Enemy4", "Res/Enemies/EA4 2.PNG");		//一定
	Engine::LoadTexture("Unit", "Res/Unit/unit_normal.PNG");
	Engine::LoadTexture("Bullet", "Res/Bullet/Bullet1.png");

	Engine::LoadTexture("BG", "Res/BackGround/side01.png");

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

	switch (Scene)
	{
	case Title:
		if (Engine::IsKeyboardKeyHeld(DIK_RETURN) == true)
		{
			Scene = Battle;
		}
		break;
	case Battle:




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
			if (player_pos_y > Y_posmax - Engine::GetTexture("Unit")->Height + 30.0f)
				player_pos_y = Y_posmax - Engine::GetTexture("Unit")->Height + 30.0f;
		}

		if (Engine::IsKeyboardKeyHeld(DIK_RIGHT) == true)
		{
			player_pos_x += pmove;
			if (player_pos_x > X_posmax - Engine::GetTexture("Unit")->Width + 120.0f)
				player_pos_x = X_posmax - Engine::GetTexture("Unit")->Width + 120.0f;
		}

		if (Engine::IsKeyboardKeyHeld(DIK_LEFT) == true)
		{
			player_pos_x -= pmove;
			if (player_pos_x < X_posmin + 30.0f)
				player_pos_x = X_posmin + 30.0f;
		}


		//==========[弾の処理]==========//

		if (Engine::IsKeyboardKeyPushed(DIK_SPACE) == true && BulletExistence == false)
		{
			BulletExistence = true;
			bullet_pos_x = player_pos_x - 5.0f;
			bullet_pos_y = player_pos_y + 15.0f;
		}
		if (BulletExistence)
		{
			BulletMove();
		}

		//==========[Enemy1の追跡処理]==========//

		tmp_wid = (player_pos_x + Angle_finer_tuning - enemy1_pos_x) * (player_pos_x + Angle_finer_tuning - enemy1_pos_x);
		tmp_hei = (player_pos_y + Angle_finer_tuning - enemy1_pos_y) * (player_pos_y + Angle_finer_tuning - enemy1_pos_y);
		y = sqrt(tmp_hei);
		x = sqrt(tmp_wid);
		tmp_dia = tmp_hei + tmp_wid;
		diagonal = sqrt(tmp_dia);

		radian = asin(y / diagonal);
		Y_increment = y / diagonal;
		X_increment = x / diagonal;

		if (player_pos_x + Angle_finer_tuning > enemy1_pos_x)
			enemy1_pos_x += emove * X_increment;
		else
			enemy1_pos_x -= emove * X_increment;


		if (player_pos_y + Angle_finer_tuning > enemy1_pos_y)
			enemy1_pos_y += emove * Y_increment;
		else
			enemy1_pos_y -= emove * Y_increment;


		if (Enemy1Existence(enemy1_pos_x, enemy1_pos_y) == false)
		{
			enemy1_pos_x = enemy_initialize_x;
			enemy1_pos_y = enemy_initialize_y;
			if (Enemy1HP == 0)
			{
				DestroyCount += 1;
				Enemy1HP = 1;
			}
			else
				Enemy1HP = 1;
		}

		//==========[Enemy2と3で使うrotation]==========//

			rotation += 0.1f;
		//==========[Enemy2の移動処理：回転]==========//
		enemy2_pos_x -= 0.5f + (10.0f * cos(rotation));
		enemy2_pos_y -= 10.0f * sin(rotation);

		if (Enemy2Existence(enemy2_pos_x, enemy2_pos_y)==false)
		{
			enemy2_pos_x = enemy_initialize_x;
			enemy2_pos_y = enemy_initialize_y;
			if (Enemy2HP == 0)
			{
				DestroyCount += 1;
				Enemy2HP = 1;
			}
			else
				Enemy2HP = 1;
		}

		//==========[Enemy3の移動処理：上下]==========//
		enemy3_pos_x -= emove / 2;
		enemy3_pos_y += 10.0f  * cos(rotation);

		if (Enemy2Existence(enemy3_pos_x, enemy3_pos_y) == false)
		{
			enemy3_pos_x = enemy_initialize_x;
			enemy3_pos_y = enemy_initialize_y;
			if (Enemy3HP == 0)
			{
				DestroyCount += 1;
				Enemy3HP = 1;
			}
			else
				Enemy3HP = 1;
		}
		//==========[Enemy4の追跡処理：一定]==========//
		enemy4_pos_x -= emove;

		if (Enemy2Existence(enemy4_pos_x, enemy4_pos_y) == false)
		{
			enemy4_pos_x = enemy_initialize_x;
			enemy4_pos_y = player_pos_y;
			if (Enemy3HP == 0)
			{
				DestroyCount += 1;
				Enemy4HP = 1;
			}
			else
				Enemy4HP = 1;
		}
		//==========[背景のスクロール]==========//
		BackGround();
		SceneTransfer(ScrollEnd);



		//==========[経過時間]==========//
		GameEndCount = 1400.0f + background_pos_x;
		ElapsedTime = GameEndCount ;


		//==========[自機のHP管理]==========//
		PlayerHPControler(Collision_Player());
		if(PlayerHP==0)
			Scene = Result;


		break;

	case Result:
		
		
		break;
	
	
	
	
	}

}

void DrawProcessing()
{
	// 描画開始
	// 描画処理を実行する場合、必ず最初実行する
	Engine::StartDrawing(0);

	switch (Scene)
	{
	case Title:
		Engine::DrawFont(190.0f, 150.0f, "SHOOTING-GAME", FontSize::Large, FontColor::White);
		Engine::DrawFont(190.0f, 300.0f, "ENTERを押してSTART", FontSize::Regular, FontColor::White);
		break;
	case Battle:

		Engine::DrawTexture(background_pos_x, background_pos_y, "BG", 255, 0.0f, 1.0f, 0.5f);
		Engine::DrawTexture(player_pos_x, player_pos_y, "Unit", 255, 90.0f, 0.3f, 0.3f);
		Enemy1Create(Enemy1Existence(enemy1_pos_x, enemy1_pos_y));
		Enemy2Create(Enemy2Existence(enemy2_pos_x, enemy2_pos_y));
		Enemy3Create(Enemy3Existence(enemy3_pos_x, enemy3_pos_y));
		Enemy4Create(Enemy4Existence(enemy4_pos_x, enemy4_pos_y));

		BulletShoot(BulletExistence);


		sprintf_s(ElapsedTimeString, 20, "残り時間：%d", ElapsedTime);
		Engine::DrawFont(0.0f, 0.0f, ElapsedTimeString, FontSize::Small, FontColor::Black);
		char HpStatus[10];
		sprintf_s(HpStatus, 10, "残りHP：%d", PlayerHP);
		Engine::DrawFont(0.0f, 15.0f, HpStatus, FontSize::Small, FontColor::Black);
		break;
	
	case Result:
		Engine::DrawFont(190.0f, 150.0f, "GAMEOVER", FontSize::Large, FontColor::White);
		char DestroyScoreString[20];
		sprintf_s(DestroyScoreString, 20, "Score：%d", DestroyCount);
		Engine::DrawFont(190.0f, 300.0f, DestroyScoreString, FontSize::Regular, FontColor::White);
		break;


	}

	// 描画終了
	// 描画処理を終了する場合、必ず最後に実行する
	Engine::FinishDrawing();
}




float Pai()
{
	return atan(1.0) * 4.0f;
}

float Angle(float r)		//enemyの角度
{

	if (player_pos_x + Angle_finer_tuning > enemy1_pos_x)
	{

		if (player_pos_y + Angle_finer_tuning == enemy1_pos_y)				//y = 0
		{
			return 180.0f;
		}
		else if (player_pos_y + Angle_finer_tuning > enemy1_pos_y)			//Enemyが第二象限
		{
			return (r * 180.0f / Pai()) + 180.0f;
		}
		else											//Enemyが第三象限
		{
			return ((Pai() / 2 - r) * 180.0f / Pai()) + 90.0f;

		}
	}
	else if (player_pos_x + Angle_finer_tuning < enemy1_pos_x)
	{
		if (player_pos_y + Angle_finer_tuning == enemy1_pos_y)				//y = 0
		{
			return 0.0f;
		}
		else if (player_pos_y + Angle_finer_tuning > enemy1_pos_y)			//Enemyが第一象限
		{
			return ((Pai() / 2 - r) * 180.0f / Pai()) + 270.0f;
		}
		else											//Enemyが第四象限
		{
			return r * 180.0f / Pai();
		}
	}
	else if (player_pos_x + Angle_finer_tuning == enemy1_pos_x)				//x = 0
	{
		if (player_pos_y + Angle_finer_tuning > enemy1_pos_y)
		{
			return 90.0f;
		}
		else if (player_pos_y + Angle_finer_tuning < enemy1_pos_y)			//x = 0
		{
			return 270.0f;
		}
		else											//判定重なり
			return 0.0f;
	}
}

void Enemy1Create(bool cancel)
{
	if (cancel) {
		Engine::DrawTexture(enemy1_pos_x, enemy1_pos_y, "Enemy1", 255, Angle(radian), 1.0f, 1.0f);
	}
	else
	{

	}
}

bool Enemy1Existence(float epx, float epy)
{
	if (Enemy1HP > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Enemy2Create(bool cancel)
{
	if (cancel) {
		Engine::DrawTexture(enemy2_pos_x, enemy2_pos_y, "Enemy2", 255, 0.0f, 1.0f, 1.0f);
	}
	else
	{

	}
}

bool Enemy2Existence(float epx, float epy)
{
	if (Enemy2HP > 0 && epx > -10.0f)
	{
		return true;
	}
	else
	{		
		return false;
	}
}

void Enemy3Create(bool cancel)
{
	if (cancel) {
		Engine::DrawTexture(enemy3_pos_x, enemy3_pos_y, "Enemy3", 255, 0.0f, 1.0f, 1.0f);
	}
	else
	{

	}
}

bool Enemy3Existence(float epx, float epy)
{
	if (Enemy3HP > 0 && epx > -10.0f)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Enemy4Create(bool cancel)
{
	if (cancel) {
		Engine::DrawTexture(enemy4_pos_x, enemy4_pos_y, "Enemy4", 255, 0.0f, 1.0f, 1.0f);
	}
	else
	{	

	}
}

bool Enemy4Existence(float epx, float epy)
{
	if (Enemy4HP > 0 && epx > -10.0f)
	{
		return true;
	}
	else
	{		
		return false;
	}
}

void BulletShoot(bool shoot)
{
	if (shoot)
		Engine::DrawTexture(bullet_pos_x, bullet_pos_y, "Bullet", 255, 0.0f, 0.5f, 0.5f);
}
void BulletMove()
{
	bullet_pos_x += 8;
	if (bullet_pos_x > 640 || Collision_Bullet(&Enemy1HP, &Enemy2HP, &Enemy3HP, &Enemy4HP) == true)
		BulletExistence = false;
}



void BackGround()
{
	if (background_pos_x > -1400.0f)
		background_pos_x -= 0.3f;
	else
		ScrollEnd = true;

}

void SceneTransfer(bool ScrollEnd)
{

	if (ScrollEnd == true)
		Scene = Result;
}

void PlayerHPControler(bool Collision_Player)
{
	NoDamage = true;
	if (NoDamageTime == 0)
		PlayerHP--;
	else if (NoDamageTime > 200)
	{
		NoDamageTime = 0;
		NoDamage = false;
	}

	if (NoDamage)
		NoDamageTime++;
}

bool Collision_Player()
{

	if (((player_pos_x >= enemy1_pos_x && player_pos_x <= enemy1_pos_x + 16.0f) || (player_pos_x + 68.0f >= enemy1_pos_x && player_pos_x + 9.0f <= enemy1_pos_x + 16.0f)) &&
		((player_pos_y >= enemy1_pos_y && player_pos_y <= enemy1_pos_y + 16.0f) || (player_pos_y + 60.0f >= enemy1_pos_y && player_pos_y + 9.0f <= enemy1_pos_y + 16.0f)))
	return true;

	if (((player_pos_x >= enemy2_pos_x && player_pos_x <= enemy2_pos_x + 16.0f) || (player_pos_x + 68.0f >= enemy2_pos_x && player_pos_x + 9.0f <= enemy2_pos_x + 16.0f)) &&
		((player_pos_y >= enemy2_pos_y && player_pos_y <= enemy2_pos_y + 16.0f) || (player_pos_y + 60.0f >= enemy2_pos_y && player_pos_y + 9.0f <= enemy2_pos_y + 16.0f)))
	return true;
	
	if (((player_pos_x >= enemy3_pos_x && player_pos_x <= enemy3_pos_x + 16.0f) || (player_pos_x + 68.0f >= enemy3_pos_x && player_pos_x + 9.0f <= enemy3_pos_x + 16.0f)) &&
		((player_pos_y >= enemy3_pos_y && player_pos_y <= enemy3_pos_y + 16.0f) || (player_pos_y + 60.0f >= enemy3_pos_y && player_pos_y + 9.0f <= enemy3_pos_y + 16.0f)))
	return true;

	if (((player_pos_x >= enemy4_pos_x && player_pos_x <= enemy4_pos_x + 16.0f) || (player_pos_x + 68.0f >= enemy4_pos_x && player_pos_x + 9.0f <= enemy4_pos_x + 16.0f)) &&
		((player_pos_y >= enemy4_pos_y && player_pos_y <= enemy4_pos_y + 16.0f) || (player_pos_y + 60.0f >= enemy4_pos_y && player_pos_y + 9.0f <= enemy4_pos_y + 16.0f)))
	return true;

	return false;
}

bool Collision_Bullet(int* Enemy1HP, int* Enemy2HP, int* Enemy3HP, int* Enemy4HP)
{

	if (((bullet_pos_x >= enemy1_pos_x && bullet_pos_x <= enemy1_pos_x + 16.0f) || (bullet_pos_x + 9.0f >= enemy1_pos_x && bullet_pos_x + 9.0f <= enemy1_pos_x + 16.0f)) &&
		((bullet_pos_y >= enemy1_pos_y && bullet_pos_y <= enemy1_pos_y + 16.0f) || (bullet_pos_y + 9.0f >= enemy1_pos_y && bullet_pos_y + 9.0f <= enemy1_pos_y + 16.0f)))
	{
		*Enemy1HP -= 1;
		return true;
	}
	if (((bullet_pos_x >= enemy2_pos_x && bullet_pos_x <= enemy2_pos_x + 16.0f) || (bullet_pos_x + 9.0f >= enemy2_pos_x && bullet_pos_x + 9.0f <= enemy2_pos_x + 23.0f)) &&
		((bullet_pos_y >= enemy2_pos_y && bullet_pos_y <= enemy2_pos_y + 16.0f) || (bullet_pos_y + 9.0f >= enemy2_pos_y && bullet_pos_y + 9.0f <= enemy2_pos_y + 23.0f)))
	{
		*Enemy2HP -= 1;
		return true;
	}
	if (((bullet_pos_x >= enemy3_pos_x && bullet_pos_x <= enemy3_pos_x + 16.0f) || (bullet_pos_x + 9.0f >= enemy3_pos_x && bullet_pos_x + 9.0f <= enemy3_pos_x + 16.0f)) &&
		((bullet_pos_y >= enemy3_pos_y && bullet_pos_y <= enemy3_pos_y + 16.0f) || (bullet_pos_y + 9.0f >= enemy3_pos_y && bullet_pos_y + 9.0f <= enemy3_pos_y + 16.0f)))
	{
		*Enemy3HP -= 1;
		return true;
	}
	if (((bullet_pos_x >= enemy4_pos_x && bullet_pos_x <= enemy4_pos_x + 16.0f) || (bullet_pos_x + 9.0f >= enemy4_pos_x && bullet_pos_x + 9.0f <= enemy4_pos_x + 16.0f)) &&
		((bullet_pos_y >= enemy4_pos_y && bullet_pos_y <= enemy4_pos_y + 16.0f) || (bullet_pos_y + 9.0f >= enemy4_pos_y && bullet_pos_y + 9.0f <= enemy4_pos_y + 16.0f)))
	{
		*Enemy4HP -= 1;
		return true;
	}

	return false;
}



