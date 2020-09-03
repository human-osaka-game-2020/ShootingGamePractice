#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include <time.h>
#include <math.h>
#include "Engine/Engine.h"
#include "Common/Vec.h"

int num; 
const int ENEMY_MAX = 10;

Vec2 P_Position = Vec2(150.0f, 220.0f);                // プレイヤーの初期座標
Vec2 E_Position[ENEMY_MAX];                            // 敵の初期座標                 
Vec2 A_Position = Vec2(1000.0f, 150.0f);               // アヒルの初期座標
Vec2 K_Position = Vec2(1500.0f, 300.0f);               // コロシテくんの初期座標
Vec2 B_Position = Vec2(P_Position.X, P_Position.Y);    // 弾の初期座標
Vec2 BG_Position_A = Vec2(420.0f  , 240.0f);           // 背景の初期座標
Vec2 BG_Position_B = Vec2(1260.0f , 240.0f);
Vec2 BG_Position_C = Vec2(2100.0f  , 240.0f);

float K_Angle = 0.0f;

bool Player_Alive = true;          // プレイヤーの生存
bool Enemy_Alive[ENEMY_MAX] = {};  // 敵の生存
bool Ahiru_Alive = true;           // アヒルの生存
bool Korositekun_Alive = true;     // コロシテくんの生存
bool Bullet_Alive;                 // 弾の生存
bool Contact_Player_Enemy;         // プレイヤーと敵の衝突判定
bool Contact_Player_Ahiru;         // プレイヤーとアヒルの衝突判定
bool Contact_Player_Korositekun;   // プレイヤーとコロシテくんの衝突判定

bool Contact_Bullet_Enemy;         // 弾と敵の衝突判定
bool Contact_Bullet_Ahiru;         // 弾とアヒルの衝突判定
bool Contact_Bullet_Korositekun;   // 弾とコロシテくんの衝突判定

float Playerspeed      = 7.0f;    // プレイヤーの速度
float Enemyspeed       = 5.0f;     // 敵の速度
float Ahiruspeed_X     = 2.0f;     // アヒルの速度
float Ahiruspeed_Y     = 2.0f;     // アヒルの速度
float Korositekunspeed = 3.0f;     // コロシテくんの速度
float Bulletspeed      = 50.0f;    // 弾の速度
float BackGroundspeed  = 3.0f;     // 背景の速度

int Enemy_Array[ENEMY_MAX];        // 敵の保存配列
int Ahiru_Array[];                 // アヒルの保存配列
int Korositekun_Array[];           // コロシテくんの保存配列
int Bullet_Array[];                // 弾の保存配列

int Time_Counter = 0;              // 時間のカウント
int E_Counter    = 0;              // 敵のカウント
int A_Counter    = 0;              // アヒルのカウント
int K_Counter    = 0;              // コロシテくんのカウント

void Player();                     // プレイヤーの動き
void Enemy();                      // 敵の動き
void Ahiru();                      // アヒルの動き
void Korositekun();                // コロシテくんの動き
void Bullet();                     // 弾の動き
void BackGround();                 // 背景の動き

void DrawPlayer();                 // プレイヤーの描画
void DrawEnemy();                  // 敵の描画
void DrawAhiru();                  // アヒルの描画
void DrawKorositekun();            // コロシテくんの描画
void DrawBullet();                 // 弾の描画
void DrawBackGround();             // 背景の描画

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
    Engine::LoadTexture("BackGround", "Res/background.png");

	// 中心点を画像の重心にする
	Engine::SetPivotType(PivotType::CenterCenter);

	// サウンド読み込み


	// サウンド再生


	// Enemyの複製
	
	for (int i = 0; i < 10; i++)
	{
		E_Position[num] = Vec2(1000.0f, 0.0f);
	}


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
	if (Engine::IsKeyboardKeyHeld(DIK_LEFT) && P_Position.X > 35 == true)
	{
		P_Position.X -= Playerspeed;
	}
	else if (Engine::IsKeyboardKeyHeld(DIK_RIGHT) && P_Position.X < 810 == true)
	{
		P_Position.X += Playerspeed;
	}
	else if (Engine::IsKeyboardKeyHeld(DIK_UP) && P_Position.Y > 10 == true)
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

	/*if ()
	{

	}*/
}
void Enemy(int num)
{
	Time_Counter++;
	if (Time_Counter > 180)
	{
		E_Position[num].X -= Enemyspeed;
	}

	// プレイヤーとの衝突判定
	if (((P_Position.X         >= E_Position[num].X && P_Position.X         <= E_Position[num].X + 50.0f)  ||
		 (P_Position.X + 45.0f >= E_Position[num].X && P_Position.X + 45.0f <= E_Position[num].X + 50.0f)) &&
		((P_Position.Y         >= E_Position[num].Y && P_Position.Y         <= E_Position[num].Y + 150.0f)  ||
	     (P_Position.Y + 25.0f >= E_Position[num].Y && P_Position.Y + 25.0f <= E_Position[num].Y + 150.0f)))
	{
		Contact_Player_Enemy = true;
	}
}
void Ahiru()
{
	Time_Counter++;
	// 上下に移動
	if (Time_Counter > 300)
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

	// プレイヤーとの衝突判定
	if (((P_Position.X         >= A_Position.X && P_Position.X         <= A_Position.X + 65.0f)  ||
		 (P_Position.X + 45.0f >= A_Position.X && P_Position.X + 45.0f <= A_Position.X + 65.0f)) &&
	    ((P_Position.Y         >= A_Position.Y && P_Position.Y         <= A_Position.Y + 31.0f)  ||
		 (P_Position.Y + 25.0f >= A_Position.Y && P_Position.Y + 25.0f <= A_Position.Y + 31.0f)))
	{
		Contact_Player_Ahiru = true;
	}

	// 弾との衝突判定
	/*if (((B_Position.X          >= A_Position.X && B_Position.X             <= A_Position.X + 108.0f)  ||
		 (B_Position.X + 117.0f >= A_Position.X && B_Position.X + 117.0f      <= A_Position.X + 108.0f)) &&
		((B_Position.Y          >= A_Position.Y && B_Position.Y             <= A_Position.Y + 101.0f)  ||
		 (B_Position.Y + 127.0f >= A_Position.Y && B_Position.Y + 127.0f      <= A_Position.Y + 101.0f)))
	{
		Contact_Bullet_Ahiru = true;
	}*/
}
void Korositekun()
{
	K_Position.X -= Korositekunspeed;

	// 画像の中心をもとにして回る
	if (K_Position.X < 600.0f)
	{
		K_Angle -= 50.0f;
	}

	// プレイヤーとの衝突判定
	if (((P_Position.X         >= K_Position.X && P_Position.X         <= K_Position.X + 56.0f)  ||
		 (P_Position.X + 45.0f >= K_Position.X && P_Position.X + 45.0f <= K_Position.X + 56.0f)) &&
	    ((P_Position.Y         >= K_Position.Y && P_Position.Y         <= K_Position.Y + 44.0f)  ||
		 (P_Position.Y + 25.0f >= K_Position.Y && P_Position.Y + 25.0f <= K_Position.Y + 44.0f)))
	{
		Contact_Player_Korositekun = true;
	}

	// 弾との衝突判定
	/*if (((B_Position.X          >= A_Position.X && B_Position.X          <= A_Position.X + 496.0f)  ||
		 (B_Position.X + 117.0f >= A_Position.X && B_Position.X + 117.0f <= A_Position.X + 496.0f)) &&
		((B_Position.Y          >= A_Position.Y && B_Position.Y          <= A_Position.Y + 511.0f)  ||
		 (B_Position.Y + 127.0f >= A_Position.Y && B_Position.Y + 127.0f <= A_Position.Y + 511.0f)))
	{
		Contact_Bullet_Korositekun = true;
	}*/
}
void Bullet()
{
	B_Position.X += Bulletspeed;
}
void BackGround()
{
	// 1枚目
	BG_Position_A.X -= BackGroundspeed;

	// 2枚目
	BG_Position_B.X -= BackGroundspeed;

	// 3枚目
	BG_Position_C.X -= BackGroundspeed;
}

void DrawPlayer()
{
	// プレイヤーの描画
	if(Player_Alive == true)
	{
		Engine::DrawTexture(P_Position.X, P_Position.Y, "Player", 500, 0.0f, 1.0f, 1.0f);
	}

	// 敵との衝突判定
	if (Contact_Player_Enemy == true)
	{
		Player_Alive = false;
	}
	else if (Contact_Player_Ahiru == true)
	{
		Player_Alive = false;
	}
	else if (Contact_Player_Korositekun == true)
	{
		Player_Alive = false;
	}

	// 消滅したら画面外に飛ばす
	if (Player_Alive == false)
	{
		P_Position.X = 0.0f;
		P_Position.Y = -1000.0f;
	}
}
void DrawEnemy(int num)
{
	srand((unsigned)time(NULL));
	int Enemyrand = rand() % 460;
	// 敵の描画
	Time_Counter++;
	if (Time_Counter > num * 180)
	{
		if (Enemy_Alive[num] == false)
		{
			Engine::DrawTexture(E_Position[num].X, E_Position[num].Y + Enemyrand, "Enemy", 500, 0.0f, 0.5f, 0.56f);
		}
	}
	
	// 画面外で消滅
	if (E_Position[num].X < -50.0f)
	{
		Enemy_Alive[num] = true;
	}
}
void DrawAhiru()
{
	Time_Counter++;
	if (Time_Counter > 300)
	{
		if (Ahiru_Alive == true)
		{
			Engine::DrawTexture(A_Position.X, A_Position.Y, "Ahiru", 500, 0.0f, 0.5f, 0.3f);
		}
	}

	// プレイヤーとの衝突判定
	if (Contact_Player_Ahiru == true)
	{
		Ahiru_Alive = false;
	}
	
	// 弾との衝突判定
	if (Contact_Bullet_Ahiru == true)
	{
		Ahiru_Alive = false;
	}

	// 画面外で消滅
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

	// プレイヤーとの衝突判定
	if (Contact_Player_Korositekun == true)
	{
		Korositekun_Alive = false;
	}

	// 弾との衝突判定
	if (Contact_Bullet_Korositekun == true)
	{
		Korositekun_Alive = false;
	}

	// 画面外で消滅
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

	// 敵との衝突判定
	if (Contact_Bullet_Enemy == true)
	{
		Bullet_Alive = false;
	}
	else if (Contact_Bullet_Ahiru == true)
	{
		Bullet_Alive = false;
	}
	else if (Contact_Bullet_Korositekun == true)
	{
		Bullet_Alive = false;
	}

	// 画面外で消滅
	if (B_Position.X > 800)
	{
		Bullet_Alive = false;
	}
}
void DrawBackGround()
{
	Engine::DrawTexture(BG_Position_A.X, BG_Position_A.Y, "BackGround", 500, 0.0f, 1.65f, 1.87f);

	Engine::DrawTexture(BG_Position_B.X, BG_Position_B.Y, "BackGround", 500, 0.0f, 1.65f, 1.87f);

	Engine::DrawTexture(BG_Position_C.X, BG_Position_C.Y, "BackGround", 500, 0.0f, 1.65f, 1.87f);

	if (BG_Position_A.X < -840.0f)
	{
		BG_Position_A.X = 1660.0f;
	}
	else if (BG_Position_B.X < -840.0f)
	{
		BG_Position_B.X = 1660.0f;
	}
	else if (BG_Position_C.X < -840.0f)
	{
		BG_Position_C.X = 1660.0f;
	}
}

void GameProcessing()
{
	// 入力データの更新
	Engine::Update();

    BackGround();

	Player();
	
	for (int i = 0; i < 10; i++)
	{
		Enemy(i);
	}

	Ahiru();

	Korositekun();

	Bullet();
	
}

void DrawProcessing()
{
	// 描画開始
	Engine::StartDrawing(0);

    DrawBackGround();

	DrawPlayer();
	
	for (int i = 0; i < 10; i++)
	{
		DrawEnemy(i);
	}

	DrawAhiru();

	DrawKorositekun();

	DrawBullet();

	// 描画終了
	Engine::FinishDrawing();
}