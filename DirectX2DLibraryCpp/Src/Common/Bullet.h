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

	bool isFired = false; // 発射されているか

};

#endif
