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
	double pos_x = WINDOW_WIDTH / 2 - 25; // 座標X
	double pos_y = WINDOW_HEIGHT - WINDOW_HEIGHT / 3; // 座標Y
	double speed_Default = 3.0f; // 標準時の移動速度
	double speed_Slow = 1.5f; // 低速モード時の移動速度

};

#endif
