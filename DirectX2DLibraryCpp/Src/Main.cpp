#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include "Engine/Engine.h"
#include "Common/Vec.h"

Vec2 Player_Pos = Vec2(0.0f, 240.0f);
Vec2 Enemy1_Pos = Vec2(610.0f, 240.0f);
Vec2 Enemy2_Pos = Vec2(610.0f, 240.0f);
Vec2 Enemy3_Pos = Vec2(610.0f, 240.0f);
Vec2 g_Scale = Vec2(1.0f, 1.0f);
float g_Angle = 0.0f;
int g_PivotType = PivotType::LeftTop;
float EnemyTimer = 0.0f;
float Timer = 0.0f;
int EnemyCount = 0;
int Enemy[10];
float tmp;
float speed = 2.0f;
float BackGround_1 = 0.0f;
float BackGround_2 = 1536.0f;
bool Spawn1 = false;
bool Spawn2 = false;
bool Spawn3 = false;
bool up = true;


// ゲーム処理
void GameProcessing();
// 描画処理
void DrawProcessing();

typedef struct
{
	float m_PosX;		// X座標
	float m_PosY;		// Y座標
	float m_Radius;		// 半径
} Circle;

//プレイヤーの当たり判定の範囲
Circle player_cir = {
	Player_Pos.X,
	Player_Pos.Y,
	16.0f
};

// 敵の当たり判定の範囲
Circle enemy_cir1 = {
	Enemy1_Pos.X,
	Enemy1_Pos.Y,
	16.0f,
};

Circle enemy_cir2 = {
	Enemy2_Pos.X,
	Enemy2_Pos.Y,
	16.0f,
};

Circle enemy_cir3 = {
	Enemy3_Pos.X,
	Enemy3_Pos.Y,
	16.0f,
};

bool OnCollisionCircle(Circle circle1, Circle circle2);

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

	// テクスチャ読み込み
	// 第一引数の文字列で読み込んだテクスチャを登録する
	// 描画や取得は登録した文字列で指定する
	Engine::LoadTexture("RMAKE1", "Res/RMAKE1.png");
	Engine::LoadTexture("Enemy1", "Res/Enemy1.png");
	Engine::LoadTexture("Haikei", "Res/Haikei.jpg");

	// サウンド読み込み
	// 第一引数の文字列で読み込んだサウンドを登録する
	// 描画や取得は登録した文字列で指定する
	Engine::LoadSoundFile("Bgm", "Res/Bgm.wav");
	Engine::LoadSoundFile("Se", "Res/Se.wav");


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

	g_Angle += 1.0f;

	// キーボードの入力取得
	if (Engine::IsKeyboardKeyHeld(DIK_LEFT) == true && Player_Pos.X > 0)
	{
		Player_Pos.X -= speed;
	}
	else if (Engine::IsKeyboardKeyHeld(DIK_RIGHT) == true && Player_Pos.X < 610)
	{
		Player_Pos.X += speed;
	}
	if (Engine::IsKeyboardKeyHeld(DIK_UP) == true && Player_Pos.Y > 30)
	{
		Player_Pos.Y -= speed;
	}
	else if (Engine::IsKeyboardKeyHeld(DIK_DOWN) == true && Player_Pos.Y < 480)
	{
		Player_Pos.Y += speed;
	}

	// 軸の切り替え
	if (Engine::IsKeyboardKeyPushed(DIK_RETURN) == true)
	{
		g_PivotType++;
		if (g_PivotType >= PivotType::MaxPivotType)
		{
			g_PivotType = PivotType::LeftTop;
		}
	}

	// 
	if (Engine::IsKeyboardKeyReleased(DIK_A))
	{
		// 重複再生
		// 指定されたキーワードのサウンドファイル再生する
		// StartPlayingDuplicateSoundでは同じサウンドファイルでも重複して再生可能
		Engine::PlayDuplicateSound("Se");
		Engine::PlayDuplicateSound("Se");
	}

	static bool is_add = true;
	float add = (is_add == true) ? 0.01f : -0.01f;
	g_Scale.X += add;
	g_Scale.Y += add;

	if (g_Scale.X > 3.0f ||
		g_Scale.X < 0.2f)
	{
		is_add = !is_add;
	}
	
	
	
}

void DrawProcessing()
{
	// 描画開始
	// 描画処理を実行する場合、必ず最初実行する
	Engine::StartDrawing(0);

	EnemyTimer = timeGetTime();
	Timer = timeGetTime();

	// テクスチャ描画
	// キーワードで指定されたテクスチャを描画する
	// DrawTextureはテクスチャをそのまま描画する
	// 一部切り取って描画する場合はDrawTextureUVを使用する
	Engine::SetPivotType(PivotType::LeftTop);

	Engine::DrawTexture(BackGround_1, 0.0f, "Haikei", 255, 0.0f, 1.5f, 1.5f);
	Engine::DrawTexture(BackGround_2, 0.0f, "Haikei", 255, 0.0f, 1.5f, 1.5f);

	if (EnemyTimer >= 2.0f)
	{
		srand(unsigned(timeGetTime()));
		EnemyCount += 1;
		if (EnemyCount == 1 || Spawn1 == true)
		{
			Engine::DrawTexture(Enemy1_Pos.X, Enemy1_Pos.Y, "Enemy1", 500, -90.0f, 0.5f, 0.5f);
			Spawn1 = true;
		}
		if (EnemyCount == 2 || Spawn2 == true)
		{
			Engine::DrawTexture(Enemy2_Pos.X, Enemy2_Pos.Y, "Enemy1", 500, -90.0f, 0.5f, 0.5f);
			Spawn2 = true;
		}
		if (EnemyCount == 3 || Spawn3 == true)
		{
			Engine::DrawTexture(Enemy3_Pos.X, Enemy3_Pos.Y, "Enemy1", 500, -90.0f, 0.5f, 0.5f);
			Spawn3 = true;
		}
		EnemyTimer = 0.0f;
	}
	Enemy1_Pos.X -= speed;
	Enemy2_Pos.X -= 1.5f;
	if (Enemy2_Pos.Y <= 130)
	{
			up = false;
	}
	else if (Enemy2_Pos.Y >= 350)
	{
			up = true;
	}
	if (up == true)
	{
		Enemy2_Pos.Y -= 2.0f;
	}
	else
	{
		Enemy2_Pos.Y += 2.0f;
	}
	
	Engine::DrawTexture(Player_Pos.X, Player_Pos.Y, "RMAKE1", 500, -90.0f, 0.5f, 0.5f);

	//プレイヤーと衝突時の敵の描画
	if (OnCollisionCircle(player_cir, enemy_cir1) == true)
	{
		Engine::DrawTexture(Enemy1_Pos.X, Enemy1_Pos.Y, "Enemy1", 0, -90.0f, 0.5f, 0.5f);
		Spawn1 = false;
	}
	if (OnCollisionCircle(player_cir, enemy_cir2) == true)
	{
		Spawn2 = false;
	}
	if (OnCollisionCircle(player_cir, enemy_cir3) == true)
	{
		Spawn3 = false;
	}



	//背景のスクロール
	BackGround_1 -= 0.5f;
	if (BackGround_1 <= -1536.0f)
	{
		BackGround_1 = 1536.0f;
	}
	BackGround_2 -= 0.5f;
	if (BackGround_2 <= -1536.0f)
	{
		BackGround_2 = 1536.0f;
	}
	// 描画終了
	// 描画処理を終了する場合、必ず最後に実行する
	Engine::FinishDrawing();
}
	//当たり判定計算用関数の定義
bool OnCollisionCircle(Circle circle1, Circle circle2)
{
	float a = circle1.m_PosX - circle2.m_PosX;
	float b = circle1.m_PosY - circle2.m_PosY;
	tmp = a * a;
	tmp += b * b;
	float c = sqrt(tmp);
	float sum_radius = circle1.m_Radius + circle2.m_Radius;

	if (c <= sum_radius)
	{
		return true;
	}
	return false;
}
