﻿#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include <string.h>
#include <time.h>
#include "Engine/Engine.h"
#include "Common/Vec.h"
#include "Common/Player.h"
#include "Common/Enemy.h"
#include "Common/Bullet.h"

// ゲーム処理
void GameProcessing();
// 描画処理
void DrawProcessing();

// プレイヤーの移動処理
void PlayerMove();
// 弾を撃つ処理
void ShootBullet();
// 弾の移動処理
void BulletMove();
// 射撃モード切替処理
void ToggleFireMode();
// 当たり判定の処理
void HitProcessing();


const int BULLET_MAX = 3;


Player player;
Enemy enemy;
class Bullet bullet[BULLET_MAX];

double startTime = clock() / CLOCKS_PER_SEC;


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
	if (Engine::Initialize(WINDOW_WIDTH, WINDOW_HEIGHT, "Sample") == false)
	{
		return 0;
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

// メイン処理
void GameProcessing()
{
	// 入力データの更新
	Engine::Update();

	if (player.isLive == true) {
		PlayerMove();
		ShootBullet();
		BulletMove();
		ToggleFireMode();
		HitProcessing();
	}

}

void DrawProcessing()
{
	// 描画開始
	// 描画処理を実行する場合、必ず最初実行する
	Engine::StartDrawing(0);

	// プレイヤーの描画
	Engine::LoadTexture("Player", "Res/Player.png");
	if (player.isLive == true) {
		Engine::DrawTexture(player.pos_x, player.pos_y, "Player", UCHAR_MAX, 0.0f, player.textureScaling, player.textureScaling);
	}
	else {
		Engine::DrawTexture(player.pos_x, player.pos_y, "Player", 0.0f, 0.0f, player.textureScaling, player.textureScaling);
	}

	// 敵の描画 ３秒ごとに出現、消滅する
	Engine::LoadTexture("Enemy", "Res/Enemy.png");
	if (enemy.isLive == true) {
		Engine::DrawTexture(enemy.pos_x, enemy.pos_y, "Enemy", UCHAR_MAX, 0.0f, enemy.textureScaling, enemy.textureScaling);
	}
	else {
		Engine::DrawTexture(enemy.pos_x, enemy.pos_y, "Enemy", 0.0f, 0.0f, enemy.textureScaling, enemy.textureScaling);
	}

	// 弾の表示
	Engine::LoadTexture("Bullet", "Res/Bullet01.png");
	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet[i].isFired == true) {
			Engine::DrawTexture(bullet[i].pos_x + 17, bullet[i].pos_y, "Bullet");
		}
	}

	// 射撃モードの表示
	if (player.threeWayMode == true) {
		Engine::DrawFont(280.0f, 0.0f, "3 Way Mode", Regular, Red);
	}
	else {
		Engine::DrawFont(280.0f, 0.0f, "Normal Mode", Regular, Red);
	}

	// X座標とY座標の表示
	char buf_x[10];
	char buf_y[10];
	char printPos[30];

	snprintf(buf_x, 10, "%.2f", player.pos_x);
	puts(buf_x);
	snprintf(buf_y, 10, "%.2f", player.pos_y);
	puts(buf_y);

	sprintf_s(printPos, "X:%s - Y:%s", buf_x, buf_y);
	Engine::DrawFont(0.0f, 0.0f, printPos, Regular, Red);

	// 敵とプレイヤーの距離
	char aaaa[20];
	snprintf(aaaa, 20, "%.2f", enemy.playerDistance);
	puts(aaaa);
	Engine::DrawFont(0.0f, 50.0f, aaaa, Regular, Red);

	// 描画終了
	// 描画処理を終了する場合、必ず最後に実行する
	Engine::FinishDrawing();
}

void PlayerMove() {

	float moveSpeed = 0.0f;

	// LShiftキーを押している間は低速モードになる
	if (Engine::IsKeyboardKeyHeld(DIK_LSHIFT)) {
		moveSpeed = player.speed_Slow;
	}
	else if (!Engine::IsKeyboardKeyHeld(DIK_LSHIFT)) {
		moveSpeed = player.speed_Default;
	}

	// 矢印キーで移動　画面外には行けない
	if (Engine::IsKeyboardKeyHeld(DIK_UP)) {
		if (player.pos_y > 0) {
			player.pos_y -= moveSpeed;
		}
	}
	else if (Engine::IsKeyboardKeyHeld(DIK_DOWN)) {
		if (player.pos_y < CALC_WINDOW_Y) {
			player.pos_y += moveSpeed;
		}
	}

	if (Engine::IsKeyboardKeyHeld(DIK_LEFT)) {
		if (player.pos_x > 0) {
			player.pos_x -= moveSpeed;
		}
	}
	else if (Engine::IsKeyboardKeyHeld(DIK_RIGHT)) {
		if (player.pos_x < CALC_WINDOW_X) {
			player.pos_x += moveSpeed;
		}
	}
}

void ShootBullet() {
	if (Engine::IsKeyboardKeyPushed(DIK_SPACE)) {
		
		if (player.threeWayMode == true) {
			for (int i = 0; i < BULLET_MAX; i++) {
				if (bullet[i].isFired == false) {

				}
				else {
					break;;
				}

				if (i == BULLET_MAX - 1) {
					for (int i = 0; i < BULLET_MAX; i++) {
						bullet[i].isFired = true;
					}
				}
			}
		}
		else {
			for (int i = 0; i < BULLET_MAX; i++) {
				if (bullet[i].isFired == false) {
					bullet[i].isFired = true;
					break;
				}
				else {

				}
			}


		}
	}
}

void BulletMove() {

	// 撃たれている時は上に移動　そうでないときはプレイヤーに追従
	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet[i].isFired == true) {
			bullet[i].pos_y -= player.bullet_Speed;

			// 3wayはゴリ押し 修正予定
			if (player.threeWayMode == true) {
				if (i == 1) {
					bullet[i].pos_x += player.bullet_Speed / 5;
				}
				if (i == 2) {
					bullet[i].pos_x -= player.bullet_Speed / 5;
				}
			}

			if (bullet[i].pos_y < 0) {
				bullet[i].isFired = false;
			}
		}
		else {
			bullet[i].pos_x = player.pos_x;
			bullet[i].pos_y = player.pos_y;
		}
	}
}

void ToggleFireMode() {
	if (Engine::IsKeyboardKeyPushed(DIK_V)) {
		for (int i = 0; i < BULLET_MAX; i++) {
			if (bullet[i].isFired == false) {

			}
			else {
				break;;
			}

			if (i == BULLET_MAX - 1) {
				player.threeWayMode = !player.threeWayMode;
			}
		}
	}
}

void HitProcessing() {

	// いろいろ計算
	player.calcPosCenter();
	enemy.calcPosCenter();
	for (int i = 0; i < BULLET_MAX; i++) {
		bullet[i].calcPosCenter();
	}

	// 敵とプレイヤーの距離
	enemy.playerDistance = sqrt(pow(player.posCenter_x - enemy.posCenter_x, 2.0f) + pow(player.posCenter_y - enemy.posCenter_y, 2.0f));
	// 敵と弾の距離
	for (int i = 0; i < BULLET_MAX; i++) {
		bullet[i].enemyDistance = sqrt(pow(bullet[i].posCenter_x - enemy.posCenter_x, 2.0f) + pow(bullet[i].posCenter_y - enemy.posCenter_y, 2.0f));
	}

	// 当たったかどうか
	if (enemy.isLive == true && player.isLive == true && enemy.playerDistance < (player.hitBox + enemy.hitBox)) {
		player.isLive = false;
	}

	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet[i].isFired == true && enemy.isLive == true && bullet[i].enemyDistance < (bullet[i].hitBox + enemy.hitBox)) {
			enemy.isLive = false;
		}
	}

}