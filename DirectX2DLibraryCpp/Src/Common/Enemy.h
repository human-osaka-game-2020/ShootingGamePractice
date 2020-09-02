/**
* @file Enemy.h
* @brief 敵の情報を保存するクラスの宣言
*/
#ifndef ENEMY_H_
#define ENEMY_H_

//=====================================================================//
//! 敵データ用クラス
//=====================================================================//
class Enemy {

public:
	float pos_x = WINDOW_WIDTH / 2 - 25; // 座標X
	float posCenter_x = 0.0f; // 画像の中心の座標X
	float pos_y = WINDOW_HEIGHT / 3; // 座標Y
	float posCenter_y = 0.0f; // 画像の中心の座標Y
	float hitBox = 24.0f; // 当たり判定
	float playerDistance = 0.0f; // プレイヤーまでの距離
	float bulletDistance = 0.0f; // 弾までの距離

	float textureCenterX = 0.0f; // 画像の中心X
	float textureCenterY = 0.0f; // 画像の中心Y
	float textureScaleX = 16.0f; // 画像のサイズX
	float textureScaleY = 16.0f; // 画像のサイズY
	float textureScaling = 3.0f; // 画像の拡大率

	bool isLive = true; // 生きているかどうか
	bool isHitting = false; // 弾に当たっているかどうか

};

#endif
