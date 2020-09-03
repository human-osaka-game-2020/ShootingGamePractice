/**
* @file Bullet.h
* @brief 弾の情報を保存するクラスの宣言
*/
#ifndef BULLET_H_
#define BULLET_H_

//=====================================================================//
//! 敵データ用クラス
//=====================================================================//
class Bullet {

public:
	float pos_x = 0; // 座標X
	float posCenter_x = 0.0f; // 画像の中心の座標X
	float pos_y = 0; // 座標Y
	float posCenter_y = 0.0f; // 画像の中心の座標Y
	float hitBox = 1.0f; // 当たり判定
	float enemyDistance = 0.0f; // 敵までの距離

	float textureCenterX = 0.0f; // 画像の中心X
	float textureCenterY = 0.0f; // 画像の中心Y
	float textureScaleX = 16.0f; // 画像のサイズX
	float textureScaleY = 16.0f; // 画像のサイズY

	bool isFired = false; // 発射されているか

public:

	// 画像の中心とそれを中心にした座標の算出
	void calcPosCenter() {
		textureCenterX = textureScaleX / 2;
		textureCenterY = textureScaleY / 2;
		posCenter_x = pos_x + textureCenterX;
		posCenter_y = pos_y + textureCenterY;
	}

};

#endif
