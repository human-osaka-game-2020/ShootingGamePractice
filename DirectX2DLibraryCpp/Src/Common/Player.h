/**
* @file Player.h
* @brief �v���C���[�̏���ۑ�����N���X�̐錾
*/
#ifndef PLAYER_H_
#define PLAYER_H_

//=====================================================================//
//! �v���C���[�f�[�^�p�N���X
//=====================================================================//
class Player {

public:
	double pos_x = WINDOW_WIDTH / 2 - 25; // ���WX
	double pos_y = WINDOW_HEIGHT - WINDOW_HEIGHT / 3; // ���WY
	double speed_Default = 3.0f; // �W�����̈ړ����x
	double speed_Slow = 1.5f; // �ᑬ���[�h���̈ړ����x

};

#endif
