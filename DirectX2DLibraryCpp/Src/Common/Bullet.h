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
	float pos_y = 0; // 座標Y
	float hitBox = textureScaleX; // 当たり判定

	float textureCenterX = textureScaleX / 2; // 画像の中心X
	float textureCenterY = textureScaleY / 2; // 画像の中心Y
	float textureScaleX = 16.0f; // 画像のサイズX
	float textureScaleY = 16.0f; // 画像のサイズY

	bool isFired = false; // 発射されているか

};

#endif
