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
	float pos_y = WINDOW_HEIGHT - WINDOW_HEIGHT / 3; // 座標Y
	float speed_Default = 3.0f; // 標準時の移動速度
	float speed_Slow = 1.0f; // 低速モード時の移動速度
	
	bool threeWayMode = false; // 射撃モードの切り替え　false => 単発 , true => 3方向ショット

};

#endif
