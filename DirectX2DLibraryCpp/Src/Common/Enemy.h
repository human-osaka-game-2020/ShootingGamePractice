/**
* @file Enemy.h
* @brief 敵の情報を保存するクラスの宣言
*/
#ifndef ENEMY_H_
#define ENEMY_H_

//=====================================================================//
//! 敵データ用クラス
//=====================================================================//

// 移動方法の列挙
enum MovingMode {
	Forward,
	Sway,
	Around
};

class Enemy {

public:
	float pos_x = WINDOW_WIDTH / 2 - 25; // 座標X
	float posCenter_x = 0.0f; // 画像の中心の座標X
	float pos_y = 0; // 座標Y
	float posCenter_y = 0.0f; // 画像の中心の座標Y
	float hitBox = 20.0f; // 当たり判定
	float playerDistance = 0.0f; // プレイヤーまでの距離
	float moveSpeed = 0.5f; // 移動速度
	MovingMode movingMode = Forward; // 移動方法

	float textureCenterX = 0.0f; // 画像の中心X
	float textureCenterY = 0.0f; // 画像の中心Y
	float textureScaleX = 16.0f; // 画像のサイズX
	float textureScaleY = 16.0f; // 画像のサイズY
	float textureScaling = 3.0f; // 画像の拡大率

	bool swayTemp = false; // MoveSwayのために
	bool isLive = false; // 生きているかどうか
	bool isHitting = false; // 弾に当たっているかどうか
	int destroyScore = 10; // 倒された時に加算するスコアの量

public:

	void Reset() {
		isLive = false;
		isHitting = false;
		pos_x = rand() % 580;
		pos_y = 0;
		switch (rand() % 3)
		{
		default:
		case 0:movingMode = Forward;
			break;
		case 1:movingMode = Sway;
			break;
		case 2:movingMode = Around;
			break;
		}
	}
	
	// 画像の中心とそれを中心にした座標の算出
	void calcPosCenter() {
		textureCenterX = textureScaleX * textureScaling / 2;
		textureCenterY = textureScaleY * textureScaling / 2;
		posCenter_x = pos_x + textureCenterX;
		posCenter_y = pos_y + textureCenterY;
	}

	// 真下に動く
	void MoveForward() {
		pos_y += moveSpeed;
	}

	// 左右に揺れながら下に動く
	void MoveSway() {
		pos_y += moveSpeed;

		if (posCenter_x >= WINDOW_WIDTH - 200) {
			swayTemp = true;
		}

		if (swayTemp == true && posCenter_x <= 200) {
			swayTemp = false;
		}

		switch (swayTemp)
		{
		case true:
			pos_x -= moveSpeed * 2;
			break;
		case false:
			pos_x += moveSpeed * 2;
			break;
		default:
			break;
		}

	}

	// 円運動をしながら下に動く
	void MoveAround() {
		pos_y += moveSpeed;
	}

	// 移動,スコア調整
	void EnemyMovingSwitch() {
		switch (movingMode)
		{
		case Forward:
			MoveForward();
			destroyScore = 10;
			break;
		case Sway:
			MoveSway();
			destroyScore = 20;
			break;
		case Around:
			MoveAround();
			destroyScore = 30;
			break;
		default:
			break;
		}
	}

	// 画面外判定
	bool WindowOut(){
		if (posCenter_x <= 0 || posCenter_x >= WINDOW_WIDTH ||
			posCenter_y <= -50 || posCenter_y >= WINDOW_HEIGHT) {
			return true;
		}
		else {
			return false;
		}
	}

	// インクリメント
	Enemy() {
		srand((unsigned)time(NULL));
	}
};

#endif
