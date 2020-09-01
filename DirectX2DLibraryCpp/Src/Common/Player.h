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
	float pos_x = WINDOW_WIDTH / 2 - 25; // ���WX
	float pos_y = WINDOW_HEIGHT - WINDOW_HEIGHT / 3; // ���WY
	float speed_Default = 3.0f; // �W�����̈ړ����x
	float speed_Slow = 1.0f; // �ᑬ���[�h���̈ړ����x
	
	bool threeWayMode = false; // �ˌ����[�h�̐؂�ւ��@false => �P�� , true => 3�����V���b�g

};

#endif
