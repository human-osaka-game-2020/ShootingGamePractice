#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include "Engine/Engine.h"
#include "Common/Vec.h"
#include <time.h>

int FlameCount = 0;				// フレームカウント

const int HpMAX = 3;			// プレイヤーの最大HP
bool PlayerHp[HpMAX];			// プレイヤーの残Hp
float g_Player_x = 50.0f;		// プレイヤーのx座標
float g_Player_y = 215.0f;		// プレイヤーのy座標
float PlayerHp_x[HpMAX];		// HpUIのx座標
float PlayerHp_y = 0.0f;		// HpUIのy座標
int ContactEnemyFlame = 20;		// 敵との衝突からのフレームカウント

const int EnemyStock = 10;		// 敵の出現可能数
bool EnemySpawn[EnemyStock];	// 敵が出現しているか判断
int EnemySpawnTime = 0;			// 敵が出現した時間
int EnemyElapsedTime = 0;		// 前の敵が出現して経過した時間
bool EnemyMoveSwitch = false;	// 敵の動きを変えるための変数
int EnemyKnockDownCount = 0;

const int BulletStock = 10;		// 弾の出現可能数
bool BulletSpawn[BulletStock];	// 弾が出現しているか判断
int BulletSpawnTime = 0;		// 弾の出現した時間
int BulletElapsedTime = 0;		// 前の弾が出現して経過した時間

const int  BackGroundStock = 2;
float BackGround_x[BackGroundStock] = { -200.0f,822.0f };
float BackGround_y[BackGroundStock] = { 0.0f,0.0f };

class Enemy						
{
public:
	float Enemy_x;			// 敵のx座標
	float Enemy_y;			// 敵のy座標		
	float Enemy_y_Save;		// 敵の出現したy座標
	float Center_x;			// 中心座標x
	float Center_y;			// 中心座標y
	float Angle;			// 角度
	float Length;			// 半径の長さ
	float DeleteCount;		// 敵を倒してからのフレーム数
	float Enemy_x_Delete;	// 敵の消滅したx座標
	float Enemy_y_Delete;	// 敵の消滅したy座標
};
Enemy EA[EnemyStock];		// EnemyをEnemyStock個分複製

class Bullet
{
public:
	float Bullet_x;		// 弾のx座標
	float Bullet_y;		// 弾のy座標
};
Bullet BulletPos[BulletStock];		// BulletをBulletStock個分複製

enum phase
{
	title,
	battle,
	result,
};
phase Phase;

// ゲーム処理
void GameProcessing();
// 描画処理
void DrawProcessing();
// サウンド処理
void SoundProcessing();

void Init();
void PlayerMove();
void PlayerDraw();;
void PlayerHpDraw();
void EnemyMove();
void EnemyUpDownMotion(int EnemyNum);
void EnemyCircularMotion(int EnemyNum);
void EnemyDelete();
void EnemyDraw();
void BulletMove();
void BulletDraw();
bool Contact_Player_Enemy(int num);
bool Contact_Bullet_Enemy(int num);
void BackGroundMove();
void BackGroundDraw();
void EnemyKnockDownUI();
void ResultDraw();
void MaterialLoadong();

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
	if (Engine::Initialize(640, 480, "SHOOTINGAME") == false)
	{
		return 0;
	}

	// 素材読み込み
	MaterialLoadong();
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

			// サウンド
			SoundProcessing();
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
		Init();	
		FlameCount++;
		if (FlameCount == 60)
		{
			FlameCount = 0;
		}
		if (Engine::IsKeyboardKeyPushed(DIK_RETURN) == true)
		{
			Phase = battle;
		}
		break;

	case battle:
		FlameCount++;
		if (FlameCount == 40)
		{
			FlameCount = 0;
		}
		PlayerMove();		
		EnemyMove();
		EnemyDelete();
		BulletMove();
		BackGroundMove();		
		ContactEnemyFlame++;
		if (PlayerHp[0] == false)
		{
			Phase = result;
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

	// テクスチャ描画
	if (Phase == title)
	{
		BackGroundDraw();
		Engine::DrawFont(220.0f, 140.0f, "SHOOTINGGAME", FontSize::Large, FontColor::White);
		if (FlameCount < 30)
		{
			Engine::DrawFont(210.0f, 280.0f, "PUSH ENTER KEY", FontSize::Large, FontColor::White);
		}
		else if (FlameCount < 60)
		{
			Engine::DrawFont(210.0f, 280.0f, "PUSH ENTER KEY", FontSize::Large, FontColor::Black);
		}
		Engine::DrawFont(466.0f, 420.0f, "十字キーで移動", FontSize::Regular, FontColor::White);
		Engine::DrawFont(443.0f, 450.0f, "スペースで弾発射", FontSize::Regular, FontColor::White);
	}
	else if (Phase == battle)
	{
		BackGroundDraw();
		PlayerDraw();
		PlayerHpDraw();
		EnemyDraw();
		BulletDraw();
		EnemyKnockDownUI();
	}
	else if(Phase == result)
	{
		ResultDraw();		
	}
	
	// 描画終了
	// 描画処理を終了する場合、必ず最後に実行する
	Engine::FinishDrawing();
}

void SoundProcessing()
{
	if (Phase == title)
	{
		Engine::StopSound("BGM2");
		Engine::StopSound("BGM3");
		Engine::PlaySoundA("OP", true);
	}
	else if (Phase == battle)
	{
		Engine::StopSound("OP");
		if (EnemyKnockDownCount < 50)
		{
			Engine::PlaySoundA("BGM1", true);
		}
		else
		{
			Engine::StopSound("BGM1");
			Engine::PlaySoundA("BGM2", true);
		}
	}
	else if (Phase == result)
	{
		if (EnemyKnockDownCount < 100)
		{
			Engine::StopSound("BGM1");
			Engine::StopSound("BGM2");
			Engine::PlaySoundA("BGM3", true);
		}
	}

	
}

// 初期化
void Init()
{
	g_Player_x = 50.0f;
	g_Player_y = 215.0f;
	ContactEnemyFlame = 20;
	EnemyKnockDownCount = 0;
	for (int i = HpMAX - 1; i >= 0; i--)
	{
		PlayerHp[i] = true;
	}
	EnemySpawnTime = timeGetTime();
	BulletSpawnTime = timeGetTime() - 400;
	for (int EnemyNum = 0; EnemyNum < EnemyStock; EnemyNum++)
	{
		EnemySpawn[EnemyNum] = false;
		EA[EnemyNum].Enemy_x = 640.0f;
		EA[EnemyNum].Enemy_y = 100.0f + rand() % 20 * 16;
	}
	for (int BulletNum = 0; BulletNum < BulletStock; BulletNum++)
	{
		BulletSpawn[BulletNum] = false;
		BulletPos[BulletNum].Bullet_x = g_Player_x + 66.0f;
		BulletPos[BulletNum].Bullet_y = g_Player_y + 33.0f;
	}
	BackGround_x[0] = { -200.0f };
	BackGround_x[1] = { 822.0f };
	BackGround_y[0] = { 0.0f };
	BackGround_y[1] = { 0.0f };


}

// プレイヤーの移動
void PlayerMove()
{
	if (PlayerHp[0] == true)
	{
		if (Engine::IsKeyboardKeyHeld(DIK_UP) && g_Player_y >= 50)
		{
			g_Player_y -= 3.0f;
		}
		if (Engine::IsKeyboardKeyHeld(DIK_DOWN) && g_Player_y <= 400)
		{
			g_Player_y += 3.0f;
		}
		if (Engine::IsKeyboardKeyHeld(DIK_LEFT) && g_Player_x >= 20)
		{
			g_Player_x -= 3.0f;
		}
		if (Engine::IsKeyboardKeyHeld(DIK_RIGHT) && g_Player_x <= 550)
		{
			g_Player_x += 3.0f;
		}
	}
}

// プレイヤーの描画
void PlayerDraw()
{
	if (PlayerHp[0] == true)
	{
		if (FlameCount < 10)
		{
			Engine::DrawTexture(g_Player_x, g_Player_y, "Player1", 255, 0.0f, 1.2f, 1.2f);
		}
		else if (FlameCount < 20)
		{
			Engine::DrawTexture(g_Player_x, g_Player_y, "Player2", 255, 0.0f, 1.2f, 1.2f);
		}
		else if (FlameCount < 30)
		{
			Engine::DrawTexture(g_Player_x, g_Player_y, "Player3", 255, 0.0f, 1.2f, 1.2f);
		}
		else if (FlameCount < 40)
		{
			Engine::DrawTexture(g_Player_x, g_Player_y, "Player4", 255, 0.0f, 1.2f, 1.2f);
		}
	}
}

// プレイヤーのHPの描画
void PlayerHpDraw()
{
	for (int i = HpMAX - 1; i >= 0; i--)
	{
		PlayerHp_x[i] = i * 60;
		if (PlayerHp[i] == true)
		{
			Engine::DrawTexture(PlayerHp_x[i], 0.0f, "Heart");
		}
	}
}

// 敵の動き
void EnemyMove()
{
	for (int EnemyNum = 0; EnemyNum < EnemyStock; EnemyNum++)
	{
		EnemyElapsedTime = timeGetTime() - EnemySpawnTime;
		if (EnemySpawn[EnemyNum] == false && EnemyElapsedTime >= 1200 && EA[EnemyNum].DeleteCount == 0)
		{
			EnemySpawn[EnemyNum] = true;
			EA[EnemyNum].Enemy_x = 640.0f;
			EA[EnemyNum].Enemy_y = 100.0f + rand() % 20 * 16;
			EA[EnemyNum].Enemy_y_Save = EA[EnemyNum].Enemy_y;
			EA[EnemyNum].Center_x = EA[EnemyNum].Enemy_x + 10.0f;
			EA[EnemyNum].Center_y = EA[EnemyNum].Enemy_y + 10.0f;
			EA[EnemyNum].Length = 40.0f;
			EnemySpawnTime = timeGetTime();
		}
		if (EnemySpawn[EnemyNum] == true)
		{
			EA[EnemyNum].Enemy_x -= 3.0f;
			EnemyUpDownMotion(EnemyNum);
			EnemyCircularMotion(EnemyNum);
		}
	}
}

// 上下に揺れながら敵の動き
void EnemyUpDownMotion(int EnemyNum)
{
	if (EnemyNum == 2 || EnemyNum == 5 || EnemyNum == 8)
	{
		if (EA[EnemyNum].Enemy_y <= EA[EnemyNum].Enemy_y_Save - 100)
		{
			EnemyMoveSwitch = true;
		}
		else if (EA[EnemyNum].Enemy_y >= EA[EnemyNum].Enemy_y_Save + 100)
		{
			EnemyMoveSwitch = false;
		}
		if (EnemyMoveSwitch == false)
		{
			EA[EnemyNum].Enemy_y -= 3;
		}
		else if (EnemyMoveSwitch == true)
		{
			EA[EnemyNum].Enemy_y += 3;
		}
	}
}

// 円運動しながら進む敵の動き
void EnemyCircularMotion(int EnemyNum)
{
	if (EnemyNum == 4 || EnemyNum == 9)
	{
		float radius = EA[EnemyNum].Angle * 3.14f / 180.0f;

		// 三角関数を使用し、円の位置を割り出す。
		float add_x = cos(radius) * EA[EnemyNum].Length;
		float add_y = sin(radius) * EA[EnemyNum].Length;

		EA[EnemyNum].Center_x -= 1.0f;

		// 結果ででた位置を中心位置に加算し、それを描画位置とする
		EA[EnemyNum].Enemy_x = EA[EnemyNum].Center_x + add_x;
		EA[EnemyNum].Enemy_y = EA[EnemyNum].Center_y + add_y;

		// 角度更新
		EA[EnemyNum].Angle -= 5.0f;

	}
}

// 敵の消滅
void EnemyDelete()
{
	for (int num = 0; num < 10; num++)
	{
		if (EA[num].Enemy_x <= -60.0f ||
			Contact_Player_Enemy(num) == true ||
			Contact_Bullet_Enemy(num) == true)
		{
			EnemySpawn[num] = false;			
		}
	}
}

// 敵の描画
void EnemyDraw()
{
	for (int EnemyNum = 0; EnemyNum < EnemyStock; EnemyNum++)
	{
		if (EnemySpawn[EnemyNum] == true)
		{
			if (FlameCount < 10)
			{
				Engine::DrawTexture(EA[EnemyNum].Enemy_x, EA[EnemyNum].Enemy_y, "Enemy1", 255, 0.0f, 1.2f, 1.2f);
			}
			else if (FlameCount < 20)
			{
				Engine::DrawTexture(EA[EnemyNum].Enemy_x, EA[EnemyNum].Enemy_y, "Enemy2", 255, 0.0f, 1.2f, 1.2f);
			}
			else if (FlameCount < 30)
			{
				Engine::DrawTexture(EA[EnemyNum].Enemy_x, EA[EnemyNum].Enemy_y, "Enemy3", 255, 0.0f, 1.2f, 1.2f);
			}
			else if (FlameCount < 40)
			{
				Engine::DrawTexture(EA[EnemyNum].Enemy_x, EA[EnemyNum].Enemy_y, "Enemy4", 255, 0.0f, 1.2f, 1.2f);
			}						
		}
		else if (EA[EnemyNum].DeleteCount > 0)
		{
			EA[EnemyNum].DeleteCount++;
			if (EA[EnemyNum].DeleteCount < 5)
			{
				Engine::DrawTexture(EA[EnemyNum].Enemy_x_Delete, EA[EnemyNum].Enemy_y_Delete, "explosion1");
			}
			else if (EA[EnemyNum].DeleteCount < 10)
			{
				Engine::DrawTexture(EA[EnemyNum].Enemy_x_Delete, EA[EnemyNum].Enemy_y_Delete, "explosion2");
			}
			else if (EA[EnemyNum].DeleteCount < 15)
			{
				Engine::DrawTexture(EA[EnemyNum].Enemy_x_Delete, EA[EnemyNum].Enemy_y_Delete, "explosion3");
			}
			else if (EA[EnemyNum].DeleteCount < 20)
			{
				Engine::DrawTexture(EA[EnemyNum].Enemy_x_Delete, EA[EnemyNum].Enemy_y_Delete, "explosion4");
			}
			else if (EA[EnemyNum].DeleteCount < 25)
			{
				Engine::DrawTexture(EA[EnemyNum].Enemy_x_Delete, EA[EnemyNum].Enemy_y_Delete, "explosion5");
			}
			else if (EA[EnemyNum].DeleteCount < 30)
			{
				Engine::DrawTexture(EA[EnemyNum].Enemy_x_Delete, EA[EnemyNum].Enemy_y_Delete, "explosion6");
			}
			else if (EA[EnemyNum].DeleteCount < 35)
			{
				Engine::DrawTexture(EA[EnemyNum].Enemy_x_Delete, EA[EnemyNum].Enemy_y_Delete, "explosion7");
			}
			else if (EA[EnemyNum].DeleteCount < 40)
			{
				Engine::DrawTexture(EA[EnemyNum].Enemy_x_Delete, EA[EnemyNum].Enemy_y_Delete, "explosion8");
			}
			else if (EA[EnemyNum].DeleteCount < 45)
			{
				Engine::DrawTexture(EA[EnemyNum].Enemy_x_Delete, EA[EnemyNum].Enemy_y_Delete, "explosion9");
			}
			else if (EA[EnemyNum].DeleteCount < 50)
			{
				Engine::DrawTexture(EA[EnemyNum].Enemy_x_Delete, EA[EnemyNum].Enemy_y_Delete, "explosion10");
			}
			else if (EA[EnemyNum].DeleteCount < 55)
			{
				Engine::DrawTexture(EA[EnemyNum].Enemy_x_Delete, EA[EnemyNum].Enemy_y_Delete, "explosion11");
			}
			else
			{
				EA[EnemyNum].DeleteCount = 0;
			}
		}
	}
}

// 弾の発射
void BulletMove()
{
	for (int BulletNum = 0; BulletNum < BulletStock; BulletNum++)
	{
		BulletElapsedTime = timeGetTime() - BulletSpawnTime;
		if (BulletSpawn[BulletNum] == false && BulletElapsedTime >= 400)
		{
			if (Engine::IsKeyboardKeyPushed(DIK_SPACE))
			{
				BulletSpawn[BulletNum] = true;
				BulletPos[BulletNum].Bullet_x = g_Player_x + 66.0f;
				BulletPos[BulletNum].Bullet_y = g_Player_y + 33.0f;
				BulletSpawnTime = timeGetTime();
				Engine::PlayDuplicateSound("SE1");
			}
		}
		if (BulletSpawn[BulletNum] == true)
		{
			BulletPos[BulletNum].Bullet_x += 5.0f;
			if (BulletPos[BulletNum].Bullet_x >= 650.0f)
			{
				BulletSpawn[BulletNum] = false;
			}
		}
	}
}

// 弾の描画
void BulletDraw()
{
	for (int BulletNum = 0; BulletNum < BulletStock; BulletNum++)
	{
		if (BulletSpawn[BulletNum] == true)
		{
			Engine::DrawTexture(BulletPos[BulletNum].Bullet_x, BulletPos[BulletNum].Bullet_y,"Bullet1",255,0.0f,0.75f,0.75f);
		}
	}
}

// 自機と敵の衝突判定
bool Contact_Player_Enemy(int num)
{
	if (EnemySpawn[num] == true &&
		((g_Player_x +  4.0f >= EA[num].Enemy_x &&
		  g_Player_x +  4.0f <= EA[num].Enemy_x + 55.0f) ||
		 (g_Player_x + 35.0f >= EA[num].Enemy_x &&
		  g_Player_x + 35.0f <= EA[num].Enemy_x + 55.0f))
		&&
		((g_Player_y + 12.0f <= EA[num].Enemy_y &&
		  g_Player_y + 56.0f >= EA[num].Enemy_y) ||
		 (g_Player_y + 12.0f <= EA[num].Enemy_y + 10.0f &&
		  g_Player_y + 56.0f >= EA[num].Enemy_y + 10.0f))
	   )
	{
		for (int i = HpMAX - 1; i >= 0; i--)
		{
			if (PlayerHp[i] == true && ContactEnemyFlame >= 20)
			{
				PlayerHp[i] = false;
				ContactEnemyFlame = 0;
				Engine::PlayDuplicateSound("SE2");
				break;
			}
		}
		return true;		
	}
	return false;
}

// 弾と敵の衝突判定
bool Contact_Bullet_Enemy(int num)
{
	for (int BulletNum = 0; BulletNum <= BulletStock; BulletNum++)
	{
		if ( BulletSpawn[BulletNum]== true && EnemySpawn[num] == true &&
			((BulletPos[BulletNum].Bullet_x         >= EA[num].Enemy_x &&
			  BulletPos[BulletNum].Bullet_x         <= EA[num].Enemy_x + 55.0f) ||
			 (BulletPos[BulletNum].Bullet_x + 10.0f >= EA[num].Enemy_x &&
	   		  BulletPos[BulletNum].Bullet_x + 10.0f <= EA[num].Enemy_x + 55.0f))
			&&
			((BulletPos[BulletNum].Bullet_y         <= EA[num].Enemy_y &&
 			  BulletPos[BulletNum].Bullet_y + 10.0f >= EA[num].Enemy_y) ||
			 (BulletPos[BulletNum].Bullet_y         <= EA[num].Enemy_y +  9.0f &&
	 		  BulletPos[BulletNum].Bullet_y + 10.0f >= EA[num].Enemy_y +  9.0f)||
			 (BulletPos[BulletNum].Bullet_y         <= EA[num].Enemy_y + 17.0f &&
			  BulletPos[BulletNum].Bullet_y + 10.0f >= EA[num].Enemy_y + 17.0f))
		   )
		{
			BulletSpawn[BulletNum] = false;
			EA[num].Enemy_x_Delete = EA[num].Enemy_x;
			EA[num].Enemy_y_Delete = EA[num].Enemy_y - 25.0f;
			EA[num].DeleteCount++;
			EnemyKnockDownCount++;
			Engine::PlayDuplicateSound("SE3");
			return true;
		}
	}
	return false;
}

// 背景のスクロール
void BackGroundMove()
{
	for (int BGNum = 0; BGNum < BackGroundStock; BGNum++)
	{
		BackGround_x[BGNum] -= 1.0f;
		if (BackGround_x[BGNum] <= -1100.0f)
		{
			BackGround_x[BGNum] = 822.0f;
			BackGround_y[BGNum] = 0.0f;
		}
	}
}

// 背景の描画
void BackGroundDraw()
{
	for (int BGNum = 0; BGNum < BackGroundStock; BGNum++)
	{
		Engine::DrawTexture(BackGround_x[BGNum], BackGround_y[BGNum],"BackGround");
	}
}

// 倒した敵の数のUI
void EnemyKnockDownUI()
{
	Engine::DrawTexture(450.0f, 5.0f, "Enemy1", 255, 0.0f, 1.2f, 1.2f);
	char EnemyKnockDownCountString[8];
	if (EnemyKnockDownCount < 100)
	{
		sprintf_s(EnemyKnockDownCountString, 7, "× %02d", EnemyKnockDownCount);
	}
	else
	{
		sprintf_s(EnemyKnockDownCountString, 7, "× 99+", EnemyKnockDownCount);
	}
	
	Engine::DrawFont(530.0f, 0.0f, EnemyKnockDownCountString, FontSize::Large, FontColor::White);
}


// リザルトの描画
void ResultDraw()
{
	Engine::DrawTexture(-200.0f, -10.0f, "Result");
	Engine::DrawFont(280.0f, 140.0f, "RESULT", FontSize::Large, FontColor::White);
	Engine::DrawFont(200.0f, 230.0f, "倒した敵の数：", FontSize::Large, FontColor::White);
	char EnemyKnockDownCountString[4];
	sprintf_s(EnemyKnockDownCountString, 4, "%d", EnemyKnockDownCount);
	Engine::DrawFont(430.0f, 233.0f, EnemyKnockDownCountString, FontSize::Large, FontColor::White);
	if (EnemyKnockDownCount < 50)
	{
		Engine::DrawFont(270.0f, 280.0f, "頑張れ！", FontSize::Large, FontColor::White);
	}
	else if (EnemyKnockDownCount < 99)
	{
		Engine::DrawFont(220.0f, 280.0f, "あともう少し！", FontSize::Large, FontColor::White);
	}
	else
	{
		Engine::DrawFont(220.0f, 280.0f, "ゲームクリア！", FontSize::Large, FontColor::White);
		Engine::DrawFont(230.0f, 320.0f, "おめでとう！", FontSize::Large, FontColor::White);
	}
}

// 素材読み込み
void MaterialLoadong()
{
	// 画像読み込み
	Engine::LoadTexture("Player1", "Res/Player1.png");
	Engine::LoadTexture("Player2", "Res/Player2.png");
	Engine::LoadTexture("Player3", "Res/Player3.png");
	Engine::LoadTexture("Player4", "Res/Player4.png");
	Engine::LoadTexture("Enemy1", "Res/Enemy1.png");
	Engine::LoadTexture("Enemy2", "Res/Enemy2.png");
	Engine::LoadTexture("Enemy3", "Res/Enemy3.png");
	Engine::LoadTexture("Enemy4", "Res/Enemy4.png");
	Engine::LoadTexture("Bullet1", "Res/Bullet1.png");
	Engine::LoadTexture("BackGround", "Res/background.jpg");
	Engine::LoadTexture("Heart", "Res/Heart.png");
	Engine::LoadTexture("Result", "Res/result.jpg");
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
	// サウンド読み込み
	Engine::LoadSoundFile("OP", "Res/bgm_maoudamashii_neorock82.wav");
	Engine::LoadSoundFile("BGM1", "Res/game_maoudamashii_1_battle34.wav");
	Engine::LoadSoundFile("BGM2", "Res/song_shiho_shining_star.wav");
	Engine::LoadSoundFile("BGM3", "Res/bgm_maoudamashii_acoustic52.wav");
	Engine::LoadSoundFile("SE1", "Res/se_maoudamashii_battle14.wav");
	Engine::LoadSoundFile("SE2", "Res/se_maoudamashii_battle06.wav");
	Engine::LoadSoundFile("SE3", "Res/se_maoudamashii_battle18.wav");
}
