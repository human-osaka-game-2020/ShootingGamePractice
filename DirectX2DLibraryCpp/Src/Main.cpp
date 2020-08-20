#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include "Engine/Engine.h"
#include "Common/Vec.h"
#include <time.h>
#define _USE_MATH_DEFINES
#include <math.h>

const int BulletStock = 5;	//弾表示数
const int EnemyStock = 20;	//敵表示数
const int Hp_Max = 3;		//HP最大値
const int NoDamageTime = 120;	//無敵時間

float Playerpos_x = 20.0f;	//プレイヤーのx座標
float Playerpos_y = 210.0f;	//プレイヤーのy座標

class Bullet
{
public:
	float Bulletpos_x;	//弾のx座標
	float Bulletpos_y;	//弾のy座標

	void BulletMove();
	void BulletDisappearance(int num);
};
bool BulletSpown[BulletStock] = {};	//Bulletを表示しているかを判断
Bullet BulletClone[BulletStock];	//BulletをBulletStock個複製


class Enemy
{
public:
	float Enemypos_x = 800.0f;	//敵のx座標
	float Enemypos_y = 0.0f;	//敵のy座標
	float Theta = 0;

	void EnemyMove(int num);
	void EnemyDisappearance(int num);
};
bool EnemySpown[EnemyStock] = {};	//Enemyを表示しているかを判断
Enemy EnemyClone[EnemyStock];		//EnemyをEnemyStock個複製
float ThetaSpeed = 4.0f;			//Enemyの回転速度

bool HpCount[Hp_Max] = {};

float BackGround_cloud_1 = 0.0f;	//背景のx座標
float BackGround_cloud_2 = 1536.0f;	//背景のx座標
float BackGround_grass_1 = 0.0f;	//背景のx座標
float BackGround_grass_2 = 1024.0f;	//背景のx座標

int FlameCount_Bullet = 30;	//弾を出してからどれだけ経過したか保存
int FlameCount_Enemy = 0;	//敵を出してからどれだけ経過したか保存
int EnemyReSpownTime = 0;	//次の敵を出せるまでの時間
int NoDamageTimeCount = NoDamageTime;//無敵時間の残り時間
int DestroyEnemy = 0;		//弾と接触した敵の数の保存
char c_DestroyEnemy[4];
bool initialize = false;	//ゲームオーバー後の初期化


enum phase
{
	title,
	battle,
	result
};
phase Phase = title;

void PlayerMachineMove();
void CanShotBulletSearch();
void EnemySpownControl();
bool Contact_Player_Enemy(int num);
bool Contact_Bullet_Enemy(int num);
void HpDecrease();
void BackGroundMove();

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
	Engine::LoadTexture("Bullet", "Res/Bullet2.png");
	Engine::LoadTexture("hart", "Res/akahart.png");
	Engine::LoadTexture("breakhart", "Res/sirohart.png");
	Engine::LoadTexture("sky", "Res/sky1.png");
	Engine::LoadTexture("cloud", "Res/sky2.PNG");
	Engine::LoadTexture("grass", "Res/grass2.png");
	Engine::LoadTexture("UI", "Res/white.png");

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
	case title:
		if (initialize == false)
		{
			for (int BulletNum = 0; BulletNum < BulletStock; BulletNum++)
			{
				BulletSpown[BulletNum] = false;
			}
			for (int EnemyNum = 0; EnemyNum < EnemyStock; EnemyNum++)
			{
				EnemySpown[EnemyNum] = false;
			}
			for (int i = 0; i < Hp_Max; i++)
			{
				HpCount[i] = true;
			}
			Playerpos_x = 20.0f;
			Playerpos_y = 210.0f;
			NoDamageTimeCount = NoDamageTime;
			DestroyEnemy = 0;
			initialize = true;
		}
		if (Engine::IsKeyboardKeyPushed(DIK_RETURN) == true)
		{
			Phase = battle;
		}
		break;
	case battle:
		PlayerMachineMove();

		BackGroundMove();

		if (Engine::IsKeyboardKeyPushed(DIK_SPACE) == true)
		{
			CanShotBulletSearch();
		}
		for (int BulletNum = 0; BulletNum < BulletStock; BulletNum++)
		{
			if (BulletSpown[BulletNum] == true)
			{
				BulletClone[BulletNum].BulletMove();
				BulletClone[BulletNum].BulletDisappearance(BulletNum);
			}
		}
		FlameCount_Bullet++;

		if (FlameCount_Enemy >= EnemyReSpownTime)
		{
			EnemySpownControl();
			FlameCount_Enemy = 0;
			EnemyReSpownTime = rand() % 50 + 50;
		}
		for (int EnemyNum = 0; EnemyNum < EnemyStock; EnemyNum++)
		{
			if (EnemySpown[EnemyNum] == true)
			{
				EnemyClone[EnemyNum].EnemyMove(EnemyNum);
				EnemyClone[EnemyNum].EnemyDisappearance(EnemyNum);
				if (Contact_Player_Enemy(EnemyNum) == true && NoDamageTimeCount >= NoDamageTime)
				{
					HpDecrease();
					if (HpCount[0] == false)
					{
						Phase = result;
						initialize = false;
					}
				}
			}
		}
		NoDamageTimeCount++;
		FlameCount_Enemy++;

		if (DestroyEnemy <= 99)
		{
			sprintf_s(c_DestroyEnemy, 4, "%d", DestroyEnemy);
		}
		else
		{
			DestroyEnemy = 100;
		}
		break;
	case result:
		if (Engine::IsKeyboardKeyPushed(DIK_RETURN) == true)
		{
			Phase = title;
		}
		break;
	}

}

void DrawProcessing()
{
	// 描画開始
	// 描画処理を実行する場合、必ず最初実行する
	Engine::StartDrawing(0);

	Engine::DrawTexture(0.0f, 0.0f, "sky", 255, 0.0f, 1.5f, 1.5f);

	Engine::DrawTexture(BackGround_cloud_1, 0.0f, "cloud", 255, 0.0f, 1.5f, 1.5f);
	Engine::DrawTexture(BackGround_cloud_2, 0.0f, "cloud", 255, 0.0f, 1.5f, 1.5f);

	Engine::DrawTexture(BackGround_grass_1, 400.0f, "grass");
	Engine::DrawTexture(BackGround_grass_2, 400.0f, "grass");

	Engine::DrawTexture(0.0f, 0.0f, "UI", 255, 0.0f, 1.5f, 0.3f);

	switch (Phase)
	{
	case title:
		Engine::DrawFont(190.0f, 100.0f, "SHOOTING-GAME", FontSize::Large, FontColor::Black);
		Engine::DrawFont(170.0f, 300.0f, "ENTERを押してSTART", FontSize::Large, FontColor::Black);
		break;
	case battle:
		if (NoDamageTimeCount >= NoDamageTime || NoDamageTimeCount % 20 <= 10)
		{
			Engine::DrawTexture(Playerpos_x, Playerpos_y, "PlayerMachine");
		}
		for (int bulletnum = 0; bulletnum < BulletStock; bulletnum++)
		{
			if (BulletSpown[bulletnum] == true)
			{
				Engine::DrawTexture(BulletClone[bulletnum].Bulletpos_x, BulletClone[bulletnum].Bulletpos_y, "Bullet");
			}
		}

		for (int enemynum = 0; enemynum < EnemyStock; enemynum++)
		{
			if (EnemySpown[enemynum] == true)
			{
				Engine::DrawTexture(EnemyClone[enemynum].Enemypos_x, EnemyClone[enemynum].Enemypos_y, "Enemy", 255, 0.0f, 1.5f, 1.5f);
			}
		}

		Engine::DrawTexture(0.0f, 0.0f, "UI", 255, 0.0f, 1.5f, 0.3f);


		Engine::DrawFont(0.0f, 5.0f, "HP：", FontSize::Large, FontColor::Black);
		for (int HpNum = 0; HpNum < Hp_Max; HpNum++)
		{
			switch (HpCount[HpNum])
			{
			case true:
				Engine::DrawTexture(HpNum * 50 + 60, 5, "hart", 255, 0.0f, 0.5f, 0.5f);
				break;
			case false:
				Engine::DrawTexture(HpNum * 50 + 60, 5, "breakhart", 255, 0.0f, 0.5f, 0.5f);
				break;
			}

		}

		if (DestroyEnemy <= 99)
		{
			Engine::DrawFont(560, 0.0f, c_DestroyEnemy, FontSize::Large, FontColor::Black);
		}
		else
		{
			Engine::DrawFont(560, 0.0f, "99+", FontSize::Large, FontColor::Black);

		}
		Engine::DrawFont(530, 0.0f, "×", FontSize::Large, FontColor::Black);
		Engine::DrawTexture(480, 10.0f, "Enemy");

		break;
	case result:
		Engine::DrawFont(0.0f, 0.0f, "HP：", FontSize::Large, FontColor::Black);
		for (int HpNum = 0; HpNum < Hp_Max; HpNum++)
		{
			switch (HpCount[HpNum])
			{
			case true:
				Engine::DrawTexture(HpNum * 50 + 60, 5, "hart", 255, 0.0f, 0.5f, 0.5f);
				break;
			case false:
				Engine::DrawTexture(HpNum * 50 + 60, 5, "breakhart", 255, 0.0f, 0.5f, 0.5f);
				break;
			}

		}

		Engine::DrawTexture(Playerpos_x, Playerpos_y, "PlayerMachine");

		Engine::DrawFont(250.0f, 200.0f, "GAMEOVER", FontSize::Large, FontColor::Black);
		Engine::DrawFont(120.0f, 300.0f, "ENTERを押してTITLEへ戻る", FontSize::Large, FontColor::Black);
		break;
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
		if (Playerpos_y <= 30.0f)
		{
			Playerpos_y = 30.0f;
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

//弾を発射可能かを見て
void CanShotBulletSearch()
{
	for (int BulletNum = 0; BulletNum < BulletStock; BulletNum++)
	{
		if (BulletSpown[BulletNum] == false && FlameCount_Bullet >= 30)
		{
			BulletClone[BulletNum].Bulletpos_x = Playerpos_x + 38.0f;
			BulletClone[BulletNum].Bulletpos_y = Playerpos_y + 23.0f;
			BulletSpown[BulletNum] = true;
			FlameCount_Bullet = 0;
			break;
		}
	}
}

//弾速
void Bullet::BulletMove()
{
	Bulletpos_x += 10.0f;
}

//玉の消滅
void Bullet::BulletDisappearance(int num)
{
	if (BulletClone[num].Bulletpos_x >= 680.0f)
	{
		BulletSpown[num] = false;
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
			EnemyClone[EnemyNum].Enemypos_y = rand() % 260 + 100;
			EnemySpown[EnemyNum] = true;
			break;
		}
	}
}

//敵の移動
void Enemy::EnemyMove(int num)
{
	
	EnemyClone[num].Theta += ThetaSpeed;
	if (EnemyClone[num].Theta > 360)
	{
		EnemyClone[num].Theta = 0;
	}

	EnemyClone[num].Enemypos_x -= sinf(EnemyClone[num].Theta * M_PI/180) * 4;
	EnemyClone[num].Enemypos_y -= cosf(EnemyClone[num].Theta * M_PI/180) * 4;
//	if (EnemyClone[num].Theta >= 45 && EnemyClone[num].Theta <= 135)
//	{
		EnemyClone[num].Enemypos_x -= 2.0f;
//	}
}

//敵の消滅
void Enemy::EnemyDisappearance(int num)
{
	if (EnemyClone[num].Enemypos_x <= -60.0f ||
		Contact_Player_Enemy(num) == true)
	{
		EnemySpown[num] = false;
	}
	else if (Contact_Bullet_Enemy(num) == true)
	{
		DestroyEnemy++;
		EnemySpown[num] = false;
	}
}

//プレイヤーと敵の接触チェック
//接触時trueを返す
bool  Contact_Player_Enemy(int num)
{
	if (
		((Playerpos_x +  4.0f >= EnemyClone[num].Enemypos_x &&
		  Playerpos_x +  4.0f <= EnemyClone[num].Enemypos_x + 91.5f) ||
		 (Playerpos_x + 32.0f >= EnemyClone[num].Enemypos_x &&
		  Playerpos_x + 32.0f <= EnemyClone[num].Enemypos_x + 91.5f))
		&&
		((Playerpos_y + 14.0f <= EnemyClone[num].Enemypos_y &&
		  Playerpos_y + 50.0f >= EnemyClone[num].Enemypos_y ) ||
		 (Playerpos_y + 14.0f <= EnemyClone[num].Enemypos_y + 21.0f &&
		  Playerpos_y + 50.0f >= EnemyClone[num].Enemypos_y + 21.0f))
		)
	{
		return true;
	}
	return false;
}

//敵と弾の接触チェック
//接触時trueを返し、弾を消滅させる
bool Contact_Bullet_Enemy(int num)
{
	for (int BulletNum = 0; BulletNum < BulletStock; BulletNum++)
	{
		if (
			BulletSpown[BulletNum] == true
			&&
			((BulletClone[BulletNum].Bulletpos_x + 9.0f >= EnemyClone[num].Enemypos_x &&
			  BulletClone[BulletNum].Bulletpos_x + 9.0f <= EnemyClone[num].Enemypos_x + 90.5f) ||
			 (BulletClone[BulletNum].Bulletpos_x        >= EnemyClone[num].Enemypos_x &&
			  BulletClone[BulletNum].Bulletpos_x        <= EnemyClone[num].Enemypos_x + 90.5))
			&&
			((BulletClone[BulletNum].Bulletpos_y + 9.0f >= EnemyClone[num].Enemypos_y &&
			  BulletClone[BulletNum].Bulletpos_y + 9.0f <= EnemyClone[num].Enemypos_y + 21.0f) ||
			 (BulletClone[BulletNum].Bulletpos_y        >= EnemyClone[num].Enemypos_y &&
			  BulletClone[BulletNum].Bulletpos_y        <= EnemyClone[num].Enemypos_y + 21.0f))
			)
		{
			BulletSpown[BulletNum] = false;
			return true;
		}
	}
	return false;
}

//HPの減少
void HpDecrease()
{
	for (int i = Hp_Max - 1; i >= 0; i--)
	{
		if (HpCount[i] == true)
		{
			HpCount[i] = false;
			NoDamageTimeCount = 0;
			break;
		}
	}
}

//背景のスクロール
void BackGroundMove()
{
	BackGround_cloud_1 -= 0.5f;
	if (BackGround_cloud_1 <= -1536.0f)
	{
		BackGround_cloud_1 = 1536.0f;
	}
	BackGround_cloud_2 -= 0.5f;
	if (BackGround_cloud_2 <= -1536.0f)
	{
		BackGround_cloud_2 = 1536.0f;
	}

	BackGround_grass_1 -= 2.0f;
	if (BackGround_grass_1 <= -1024.0f)
	{
		BackGround_grass_1 = 1024.0f;
	}
	BackGround_grass_2 -= 2.0f;
	if (BackGround_grass_2 <= -1024.0f)
	{
		BackGround_grass_2 = 1024.0f;
	}

}

