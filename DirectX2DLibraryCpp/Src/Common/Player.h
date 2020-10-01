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
	float posCenter_x = 0.0f; // �摜�̒��S�̍��WX
	float pos_y = WINDOW_HEIGHT - WINDOW_HEIGHT / 3; // ���WY
	float posCenter_y = 0.0f; // �摜�̒��S�̍��WY
	float speed_Default = 2.0f; // �W�����̈ړ����x
	float speed_Slow = 1.0f; // �ᑬ���[�h���̈ړ����x
	float bullet_Speed = 4.0f; // �e�̈ړ����x
	float hitBox = 3.0f; // �����蔻��

	float textureCenterX = 0.0f; // �摜�̒��SX
	float textureCenterY = 0.0f; // �摜�̒��SY
	float textureScaleX = 16.0f; // �摜�̃T�C�YX
	float textureScaleY = 16.0f; // �摜�̃T�C�YY
	float textureScaling = 3.0f; // �摜�̊g�嗦
	
	bool isLive = true; // �����Ă��邩�ǂ���
	bool threeWayMode = false; // �ˌ����[�h�̐؂�ւ��@false => �P�� , true => 3�����V���b�g
	bool isHitting = false; // �G�ɓ����������ǂ���

public:

	// �摜�̒��S�Ƃ���𒆐S�ɂ������W�̎Z�o
	void calcPosCenter() {
		textureCenterX = textureScaleX * textureScaling / 2;
		textureCenterY = textureScaleY * textureScaling / 2;
		posCenter_x = pos_x + textureCenterX;
		posCenter_y = pos_y + textureCenterY;
	}

	// ���Z�b�g����
	void Reset() {
		pos_x = WINDOW_WIDTH / 2 - 25;
		pos_y = WINDOW_HEIGHT - WINDOW_HEIGHT / 3;
		isHitting = false;
		isLive = true;
		threeWayMode = false;
	}

};

#endif
