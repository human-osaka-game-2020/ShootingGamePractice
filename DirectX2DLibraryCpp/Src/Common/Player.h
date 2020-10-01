/**
* @file Player.h
* @brief プレイヤーの情報を保存するクラスの宣言
*/
#ifndef PLAYER_H_
#define PLAYER_H_

//=====================================================================//
//! プレイヤーデータ用クラス
//=====================================================================//
class Player {

public:
	float pos_x = WINDOW_WIDTH / 2 - 25; // 座標X
	float posCenter_x = 0.0f; // 画像の中心の座標X
	float pos_y = WINDOW_HEIGHT - WINDOW_HEIGHT / 3; // 座標Y
	float posCenter_y = 0.0f; // 画像の中心の座標Y
	float speed_Default = 2.0f; // 標準時の移動速度
	float speed_Slow = 1.0f; // 低速モード時の移動速度
	float bullet_Speed = 4.0f; // 弾の移動速度
	float hitBox = 3.0f; // 当たり判定

	float textureCenterX = 0.0f; // 画像の中心X
	float textureCenterY = 0.0f; // 画像の中心Y
	float textureScaleX = 16.0f; // 画像のサイズX
	float textureScaleY = 16.0f; // 画像のサイズY
	float textureScaling = 3.0f; // 画像の拡大率
	
	bool isLive = true; // 生きているかどうか
	bool threeWayMode = false; // 射撃モードの切り替え　false => 単発 , true => 3方向ショット
	bool isHitting = false; // 敵に当たったかどうか

public:

	// 画像の中心とそれを中心にした座標の算出
	void calcPosCenter() {
		textureCenterX = textureScaleX * textureScaling / 2;
		textureCenterY = textureScaleY * textureScaling / 2;
		posCenter_x = pos_x + textureCenterX;
		posCenter_y = pos_y + textureCenterY;
	}

	// リセット処理
	void Reset() {
		pos_x = WINDOW_WIDTH / 2 - 25;
		pos_y = WINDOW_HEIGHT - WINDOW_HEIGHT / 3;
		isHitting = false;
		isLive = true;
		threeWayMode = false;
	}

};

#endif
