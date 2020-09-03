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
	float Bulletpos_x;			//弾のx座標
	float Bulletpos_y;			//弾のy座標
	bool BulletSpown = false;	//Bulletを表示しているかを判断

	void BulletMove();
	void BulletDisappearance();
};
Bullet BulletClone[BulletStock];	//BulletをBulletStock個複製


class Enemy
{
public:
	float Enemypos_x = 800.0f;	//敵のx座標
	float Enemypos_y = 0.0f;	//敵のy座標
	float Theta = 0;
	bool EnemySpown = false;	//Enemyを表示しているかを判断

	void EnemyMove(int num);
	void EnemyDisappearance(int num);
};
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

class explosion
{
public:
	float pos_x;
	float pos_y;
	int EffectTime = 60;
	bool ExplosionSpown = false;

};
explosion Explosion[EnemyStock];

int Timer;			//制限時間の記録
char c_Timer[7];

enum phase
{
	title,
	battle,
	gameover,
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

void PlayMove();
void EnemyMove(int num);
void explosionMove(int x, int y, int num);

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
	if (Engine::Initialize(640, 480, "SHOOTING-GAME") == false)
	{
		return 0;
	}

	Engine::LoadTexture("PlayerMachine", "Res/Robot_idle1.png");
	Engine::LoadTexture("PlayerMachine2", "Res/Robot_idle2.png");
	Engine::LoadTexture("PlayerMachine3", "Res/Robot_idle3.png");
	Engine::LoadTexture("Enemy", "Res/EA1.png");
	Engine::LoadTexture("Enemy2", "Res/EA2.png");
	Engine::LoadTexture("Enemy3", "Res/EA3.png");
	Engine::LoadTexture("Enemy4", "Res/EA4.png");
	Engine::LoadTexture("Bullet", "Res/Bullet2.png");
	Engine::LoadTexture("hart", "Res/akahart.png");
	Engine::LoadTexture("breakhart", "Res/sirohart.png");
	Engine::LoadTexture("sky", "Res/sky1.png");
	Engine::LoadTexture("cloud", "Res/sky2.PNG");
	Engine::LoadTexture("grass", "Res/grass2.png");
	Engine::LoadTexture("UI", "Res/white.png");
	Engine::LoadTexture("explosion1", "Res/explosion/explosion1.png");
	Engine::LoadTexture("explosion2", "Res/explosion/explosion2.png");
	Engine::LoadTexture("explosion3", "Res/explosion/explosion3.png");
	Engine::LoadTexture("explosion4", "Res/explosion/explosion4.png");
	Engine::LoadTexture("explosion5", "Res/explosion/explosion5.png");
	Engine::LoadTexture("explosion6", "Res/explosion/explosion6.png");
	Engine::LoadTexture("explosion7", "Res/explosion/explosion7.png");
	Engine::LoadTexture("explosion8", "Res/explosion/explosion8.png");
	Engine::LoadTexture("explosion9", "Res/explosion/explosion9.png");
	Engine::LoadTexture("explosion10", "Res/explosion/explosion10.png");
	Engine::LoadTexture("explosion11", "Res/explosion/explosion11.png");

	

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
				BulletClone[BulletNum].BulletSpown = false;
			}
			for (int EnemyNum = 0; EnemyNum < EnemyStock; EnemyNum++)
			{
				EnemyClone[EnemyNum].EnemySpown = false;
			}
			for (int i = 0; i < Hp_Max; i++)
			{
				HpCount[i] = true;
			}
			Playerpos_x = 20.0f;
			Playerpos_y = 210.0f;
			NoDamageTimeCount = NoDamageTime;
			DestroyEnemy = 0;
			Timer = 0;
			initialize = true;
		}
		if (Engine::IsKeyboardKeyPushed(DIK_RETURN) == true)
		{
			Phase = battle;
		}
		break;
	case battle:

		Timer++;

		PlayerMachineMove();

		BackGroundMove();

		for (int i = 0; i < 5; i++)
		{
			if (Explosion[i].EffectTime > 60)
			{
				Explosion[i].ExplosionSpown = false;
			}
		}

		if (Engine::IsKeyboardKeyPushed(DIK_SPACE) == true)
		{
			CanShotBulletSearch();
		}
		for (int BulletNum = 0; BulletNum < BulletStock; BulletNum++)
		{
			if (BulletClone[BulletNum].BulletSpown == true)
			{
				BulletClone[BulletNum].BulletMove();
				BulletClone[BulletNum].BulletDisappearance();
			}
		}
		FlameCount_Bullet++;

		if (FlameCount_Enemy >= EnemyReSpownTime)
		{
			EnemySpownControl();
			FlameCount_Enemy = 0;
			EnemyReSpownTime = rand() % 50 + 20;
		}
		for (int EnemyNum = 0; EnemyNum < EnemyStock; EnemyNum++)
		{
			if (EnemyClone[EnemyNum].EnemySpown == true)
			{
				EnemyClone[EnemyNum].EnemyMove(EnemyNum);
				EnemyClone[EnemyNum].EnemyDisappearance(EnemyNum);
				if (Contact_Player_Enemy(EnemyNum) == true && NoDamageTimeCount >= NoDamageTime)
				{
					HpDecrease();
					if (HpCount[0] == false)
					{
						Phase = gameover;
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

		sprintf_s(c_Timer, 4, "%d",100 - Timer / 60);
		if (100 - Timer / 60 <= 0)
		{
			Phase = result;
			initialize = false;
		}

		break;
	case gameover:
		if (Engine::IsKeyboardKeyPushed(DIK_RETURN) == true)
		{
			Phase = title;
		}
		break;
	case result:
		sprintf_s(c_DestroyEnemy, 4, "%d", DestroyEnemy);
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
		Engine::DrawFont(24.0f, 440.0f, "↑", FontSize::Regular, FontColor::Black);
		Engine::DrawFont(0.0f, 450.0f, "←↓→で移動,", FontSize::Regular, FontColor::Black);
		Engine::DrawFont(150.0f, 450.0f, "SPACEで弾を発射", FontSize::Regular, FontColor::Black);



		Engine::DrawFont(190.0f, 100.0f, "SHOOTING-GAME", FontSize::Large, FontColor::Black);
		Engine::DrawFont(170.0f, 300.0f, "ENTERを押してSTART", FontSize::Large, FontColor::Black);
		break;
	case battle:
		if (NoDamageTimeCount >= NoDamageTime || NoDamageTimeCount % 20 <= 10)
		{
			PlayMove();
		}
		for (int BulletNum = 0; BulletNum < BulletStock; BulletNum++)
		{
			if (BulletClone[BulletNum].BulletSpown == true)
			{
				Engine::DrawTexture(BulletClone[BulletNum].Bulletpos_x, BulletClone[BulletNum].Bulletpos_y, "Bullet");
			}
		}

		for (int EnemyNum = 0; EnemyNum < EnemyStock; EnemyNum++)
		{
			if (EnemyClone[EnemyNum].EnemySpown == true)
			{
				EnemyMove(EnemyNum);
			}
		}

		Engine::DrawTexture(0.0f, 0.0f, "UI", 255, 0.0f, 1.5f, 0.3f);

		if (Timer < 5400)
		{
			Engine::DrawFont(300.0f, 0.0f, c_Timer, FontSize::Large, FontColor::Black);
		}
		else if (Timer > 5400)
		{
			Engine::DrawFont(300.0f, 0.0f, c_Timer, FontSize::Large, FontColor::Red);
		}
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

		for (int i = 0; i < 5; i++)
		{
			explosionMove(Explosion[i].pos_x, Explosion[i].pos_y,i);
		}

		break;
	case gameover:
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
	case result:
		Engine::DrawFont(350, 250.0f, c_DestroyEnemy, FontSize::Large, FontColor::Black);
		Engine::DrawFont(300, 250.0f, "×", FontSize::Large, FontColor::Black);
		Engine::DrawTexture(240, 260.0f, "Enemy");
		Engine::DrawFont(250.0f, 200.0f, " TIMEUP ", FontSize::Large, FontColor::Black);
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
		if (BulletClone[BulletNum].BulletSpown == false && FlameCount_Bullet >= 10)
		{
			BulletClone[BulletNum].Bulletpos_x = Playerpos_x + 38.0f;
			BulletClone[BulletNum].Bulletpos_y = Playerpos_y + 23.0f;
			BulletClone[BulletNum].BulletSpown = true;
			FlameCount_Bullet = 0;
			break;
		}
	}
}

//弾速
void Bullet::BulletMove()
{
	Bulletpos_x += 15.0f;
}

//玉の消滅
void Bullet::BulletDisappearance()
{
	if (Bulletpos_x >= 680.0f)
	{
		BulletSpown = false;
	}
}

//敵の出現
void EnemySpownControl()
{
	for (int EnemyNum = 0; EnemyNum < EnemyStock; EnemyNum++)
	{
		if (EnemyClone[EnemyNum].EnemySpown == false)
		{
			EnemyClone[EnemyNum].Enemypos_x = 650.0f;
			EnemyClone[EnemyNum].Enemypos_y = rand() % 260 + 100;
			EnemyClone[EnemyNum].EnemySpown = true;
			break;
		}
	}
}

//敵の移動
void Enemy::EnemyMove(int num)
{
	
	Theta += ThetaSpeed;
	if (Theta > 360)
	{
		Theta = 0;
	}
	if (num % 2 == 0)
	{
		Enemypos_x -= sinf(Theta * M_PI / 180) * 4;
		Enemypos_y -= cosf(Theta * M_PI / 180) * 4;
		Enemypos_x -= 2.0f;
	}
	else
	{
		Enemypos_x -= (num % 3 + 3);
 	}
}

//敵の消滅
void Enemy::EnemyDisappearance(int num)
{
	if (Enemypos_x <= -90.0f ||
		Contact_Player_Enemy(num) == true)
	{
		EnemySpown = false;
	}
	else if (Contact_Bullet_Enemy(num) == true)
	{
		DestroyEnemy++;
		EnemySpown = false;
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
			BulletClone[BulletNum].BulletSpown == true
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
			BulletClone[BulletNum].BulletSpown = false;
			for (int i = 0; i < 5; i++)
			{
				if (Explosion[i].ExplosionSpown == false)
				{
					Explosion[i].pos_x = EnemyClone[num].Enemypos_x - 10.5f;
					Explosion[i].pos_y = EnemyClone[num].Enemypos_y;
					Explosion[i].ExplosionSpown = true;
					Explosion[i].EffectTime = 0;
					break;
				}
			}


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

//描画処理関数

//プレイヤーアニメーション
void PlayMove()
{
	int count = Timer % 30;
	if (count < 10)
	{
		Engine::DrawTexture(Playerpos_x, Playerpos_y, "PlayerMachine");
	}
	else if (count >= 10 && count < 20)
	{
		Engine::DrawTexture(Playerpos_x, Playerpos_y, "PlayerMachine2");
	}
	else
	{
		Engine::DrawTexture(Playerpos_x, Playerpos_y, "PlayerMachine3");
	}
}

//エネミーアニメーション
void EnemyMove(int num)
{
	int count = Timer % 40;
	if (count < 10)
	{
		Engine::DrawTexture(EnemyClone[num].Enemypos_x, EnemyClone[num].Enemypos_y, "Enemy", 255, 0.0f, 1.5f, 1.5f);
	}
	else if (count >= 10 && count < 20)
	{
		Engine::DrawTexture(EnemyClone[num].Enemypos_x, EnemyClone[num].Enemypos_y, "Enemy2", 255, 0.0f, 1.5f, 1.5f);
	}
	else if (count >= 20 && count < 30)
	{
		Engine::DrawTexture(EnemyClone[num].Enemypos_x, EnemyClone[num].Enemypos_y, "Enemy3", 255, 0.0f, 1.5f, 1.5f);
	}
	else
	{
		Engine::DrawTexture(EnemyClone[num].Enemypos_x, EnemyClone[num].Enemypos_y, "Enemy4", 255, 0.0f, 1.5f, 1.5f);
	}
}

//爆発エフェクト
void explosionMove(int x, int y, int num)
{
	if (Explosion[num].ExplosionSpown == true)
	{
		Explosion[num].EffectTime++;
		int count = Explosion[num].EffectTime;
		if (count < 5)
		{
			Engine::DrawTexture(x, y, "explosion1");
		}
		else if (count >= 5 && count < 10)
		{
			Engine::DrawTexture(x, y, "explosion2");
		}
		else if (count >= 10 && count < 15)
		{
			Engine::DrawTexture(x, y, "explosion3");
		}
		else if (count >= 15 && count < 20)
		{
			Engine::DrawTexture(x, y, "explosion4");
		}
		else if (count >= 20 && count < 25)
		{
			Engine::DrawTexture(x, y, "explosion5");
		}
		else if (count >= 25 && count < 30)
		{
			Engine::DrawTexture(x, y, "explosion6");
		}
		else if (count >= 30 && count < 35)
		{
			Engine::DrawTexture(x, y, "explosion7");
		}
		else if (count >= 35 && count < 40)
		{
			Engine::DrawTexture(x, y, "explosion8");
		}
		else if (count >= 40 && count < 45)
		{
			Engine::DrawTexture(x, y, "explosion9");
		}
		else if (count >= 45 && count < 50)
		{
			Engine::DrawTexture(x, y, "explosion10");
		}
		else if (count >= 50 && count < 55)
		{
			Engine::DrawTexture(x, y, "explosion11");
		}
	}
}

