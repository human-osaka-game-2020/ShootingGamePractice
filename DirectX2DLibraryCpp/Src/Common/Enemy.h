/**
* @file Enemy.h
* @brief �G�̏���ۑ�����N���X�̐錾
*/
#ifndef ENEMY_H_
#define ENEMY_H_

//=====================================================================//
//! �G�f�[�^�p�N���X
//=====================================================================//
class Enemy {

public:
	float pos_x = WINDOW_WIDTH / 2 - 25; // ���WX
	float posCenter_x = 0.0f; // �摜�̒��S�̍��WX
	float pos_y = 0; // ���WY
	float posCenter_y = 0.0f; // �摜�̒��S�̍��WY
	float hitBox = 20.0f; // �����蔻��
	float playerDistance = 0.0f; // �v���C���[�܂ł̋���

	float textureCenterX = 0.0f; // �摜�̒��SX
	float textureCenterY = 0.0f; // �摜�̒��SY
	float textureScaleX = 16.0f; // �摜�̃T�C�YX
	float textureScaleY = 16.0f; // �摜�̃T�C�YY
	float textureScaling = 3.0f; // �摜�̊g�嗦

	bool isLive = true; // �����Ă��邩�ǂ���
	bool isHitting = false; // �e�ɓ������Ă��邩�ǂ���

public:

	// �摜�̒��S�Ƃ���𒆐S�ɂ������W�̎Z�o
	void calcPosCenter() {
		textureCenterX = textureScaleX * textureScaling / 2;
		textureCenterY = textureScaleY * textureScaling / 2;
		posCenter_x = pos_x + textureCenterX;
		posCenter_y = pos_y + textureCenterY;
	}

};

#endif
