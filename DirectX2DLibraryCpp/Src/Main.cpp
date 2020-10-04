#define _CRTDBG_MAP_ALLOC
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
// 敵の移動方法の管理
void EnemyController();
// 敵の移動処理
void EnemyMoving();
// リセット処理
void ResetProcessing();
// バリアの処理
void BarrierProcessing();


const static int BULLET_MAX = 3;
const static int ENEMY_MAX = 20;
const static int SPAWNRATE_MAX = 500;
const static int SPAWNRATE_MIN = 30;

const static float ENEMYSPEED_MAX = 1.5f;
const static float ENEMYSPEED_MIN = 0.5f;

Player player;
class Enemy enemy[ENEMY_MAX];
class Bullet bullet[BULLET_MAX];

// 背景のY座標
float BGpos_y = 0.0f;

double startTime = clock() / CLOCKS_PER_SEC;

int score = 0;

int resetCount = 0;

// 敵の出現する確率
int spawnRate = SPAWNRATE_MAX;

// 敵の数
int enemyCount = 0;

float enemySpeed = ENEMYSPEED_MIN;

// バリアのY軸
float barrierLine[3] = { 420, 440, 460 };
// バリアの壊された数
int breakingBarrier = 0;

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
		EnemyMoving();
		EnemyController();
		BarrierProcessing();
	}
	ResetProcessing();

}

void DrawProcessing()
{
	// 描画開始
	// 描画処理を実行する場合、必ず最初実行する
	Engine::StartDrawing(0);

	// 背景の描画
	Engine::LoadTexture("BackGround", "Res/BackGround.png");
	Engine::DrawTexture(0.0f, BGpos_y, "BackGround", UCHAR_MAX, 0.0f, 0.63f, 1.0f);
	BGpos_y += 0.2f;
	if (BGpos_y >= 0) {
		Engine::DrawTexture(0.0f, BGpos_y - 480, "BackGround", UCHAR_MAX, 0.0f, 0.63f, 1.0f);
	}
	if (BGpos_y - 480 >= 0) {
		BGpos_y = 0;
	}

	// プレイヤーの描画
	Engine::LoadTexture("Player", "Res/Player.png");
	if (player.isLive == true) {
		Engine::DrawTexture(player.pos_x, player.pos_y, "Player", UCHAR_MAX, 0.0f, player.textureScaling, player.textureScaling);
	}
	else {
		Engine::DrawTexture(player.pos_x, player.pos_y, "Player", 0.0f, 0.0f, player.textureScaling, player.textureScaling);
	}

	// 敵の描画
	Engine::LoadTexture("Enemy", "Res/Enemy.png");
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (enemy[i].isLive == true) {
			Engine::DrawTexture(enemy[i].pos_x, enemy[i].pos_y, "Enemy", UCHAR_MAX, 0.0f, enemy[i].textureScaling, enemy[i].textureScaling);
		}
	}

	// 弾の表示
	Engine::LoadTexture("Bullet", "Res/Bullet01.png");
	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet[i].isFired == true) {
			Engine::DrawTexture(bullet[i].pos_x + 17, bullet[i].pos_y, "Bullet");
		}
	}

	// 射撃モードの表示
	Engine::LoadTexture("SingleMode", "Res/BulletMode_Single.png");
	Engine::LoadTexture("BurstMode", "Res/BulletMode_Burst.png");
	if (player.threeWayMode == true) {
		Engine::DrawTexture(10.0f, 10.0f, "BurstMode", UCHAR_MAX, 0.0f, 2.0f, 2.0f);
	}
	else {
		Engine::DrawTexture(10.0f, 10.0f, "SingleMode", UCHAR_MAX, 0.0f, 2.0f, 2.0f);
	}

	// スコアの表示
	char buf_score[20];
	// スコアを文字列に変更する
	snprintf(buf_score, 20, "SCORE:%d", score);
	puts(buf_score);
	Engine::DrawFont(430.0f, 10.0f, buf_score, Large, White);

	// バリアの表示
	Engine::LoadTexture("Barrier", "Res/Barrier.png");
	Engine::DrawTexture(0, barrierLine[0], "Barrier");
	Engine::DrawTexture(0, barrierLine[1], "Barrier");
	Engine::DrawTexture(0, barrierLine[2], "Barrier");

	// ゲームオーバー画面の表示
	if (player.isLive == false) {
		Engine::LoadTexture("GameOverScreen", "Res/GameoverScreen.png");
		Engine::DrawTexture(0, 0, "GameOverScreen", UCHAR_MAX * 0.95f, 0, 0.63f, 1);
		Engine::DrawFont(240, 300, buf_score, Large, White);
	}

	// デバッグ情報
	//// X座標とY座標の表示
	//char buf_x[10];
	//char buf_y[10];
	//char printPos[30];

	//snprintf(buf_x, 10, "%.2f", player.pos_x);
	//puts(buf_x);
	//snprintf(buf_y, 10, "%.2f", player.pos_y);
	//puts(buf_y);

	//sprintf_s(printPos, "X:%s - Y:%s", buf_x, buf_y);
	//Engine::DrawFont(0.0f, 0.0f, printPos, Regular, Red);

	//// 敵とプレイヤーの距離
	//char aaaa[20];
	//snprintf(aaaa, 20, "%.2f", bullet[0].enemyDistance);
	//puts(aaaa);
	//Engine::DrawFont(0.0f, 50.0f, aaaa, Regular, Red);

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

	// プレイヤーの現在の位置を算出
	player.calcPosCenter();

	// プレイヤーと敵の当たり判定
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (enemy[i].isLive == true) {
			// 敵の現在の位置に基づいた当たり判定を算出
			enemy[i].calcPosCenter();
			enemy[i].playerDistance = sqrt(pow(player.posCenter_x - enemy[i].posCenter_x, 2.0f) + pow(player.posCenter_y - enemy[i].posCenter_y, 2.0f));
			if (enemy[i].isLive == true && player.isLive == true && enemy[i].playerDistance < (player.hitBox + enemy[i].hitBox)) {
				player.isLive = false;
			}
		}
	}

	// 敵とプレイヤーの弾の当たり判定
	for (int i = 0; i < BULLET_MAX; i++) {
		if (bullet[i].isFired == true) {
			// 弾の現在の位置に基づいた当たり判定を算出
			bullet[i].calcPosCenter();
			for (int j = 0; j < ENEMY_MAX; j++) {
				bullet[i].enemyDistance = sqrt(pow(bullet[i].posCenter_x - enemy[j].posCenter_x, 2.0f) + pow(bullet[i].posCenter_y - enemy[j].posCenter_y, 2.0f));
				if (bullet[i].isFired == true && enemy[j].isLive == true && bullet[i].enemyDistance < (bullet[i].hitBox + enemy[j].hitBox)) {
					score += enemy[j].destroyScore;
					enemy[j].isLive = false;
					enemyCount--;
					if (spawnRate >= SPAWNRATE_MIN) {
						spawnRate -= 10;
					}

					if (enemySpeed <= ENEMYSPEED_MAX) {
						enemySpeed += 0.02f;
					}
					bullet[i].isFired = false;
				}
			}
		}
	}

	// 敵が画面外に移動した場合は消滅させる
	for (int i = 0; i < ENEMY_MAX; i++) {
		if (enemy[i].isLive == true) {
			enemy[i].WindowOut();
			enemyCount--;
		}
	}
}

void EnemyController() {
	if (rand() % spawnRate == 0) {
		if (enemyCount < ENEMY_MAX) {
			for (int i = 0; i < ENEMY_MAX; i++) {
				if (enemy[i].isLive == false) {
					enemy[i].Reset();
					enemy[i].moveSpeed = enemySpeed;
					enemy[i].isLive = true;
					enemyCount++;
					return;
				}
			}
		}
	}
}

void EnemyMoving() {
	for (int i = 0; i < ENEMY_MAX; i++) {
		enemy[i].EnemyMovingSwitch();
	}
}

void ResetProcessing() {
	if (Engine::IsKeyboardKeyHeld(DIK_R)) {
		resetCount++;
	}
	else {
		resetCount = 0;
	}

	if (resetCount > 120) {
		resetCount = 0;
		score = 0;
		enemyCount = 0;
		spawnRate = SPAWNRATE_MAX;
		for (int i = 0; i < ENEMY_MAX; i++) {
			enemy[i].Reset();
			enemy[i].moveSpeed = ENEMYSPEED_MIN;
		}
		player.Reset();
	}
}

void BarrierProcessing() {
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < ENEMY_MAX; j++) {
			if (enemy[j].isLive == true && enemy[j].posCenter_y > barrierLine[i]) {
				enemy[j].isLive = false;
				enemyCount -= 1;
				spawnRate *= 0.95f;
				enemySpeed *= 0.95f;
				breakingBarrier = i + 1;
			}
		}
	}

	switch (breakingBarrier)
	{
	case 1:
		barrierLine[0] -= 1;
		if (barrierLine[0] < 0) {
			barrierLine[0] = 1000;
			breakingBarrier = 0;
		}
		break;
	case 2:
		barrierLine[1] -= 1;
		if (barrierLine[1] < 0) {
			barrierLine[1] = 1000;
			breakingBarrier = 0;
		}
		break;
	case 3:
		player.isLive = false;
		break;
	default:
		break;
	}
}