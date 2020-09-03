/**
* @file Bullet.h
* @brief �e�̏���ۑ�����N���X�̐錾
*/
#ifndef BULLET_H_
#define BULLET_H_

//=====================================================================//
//! �G�f�[�^�p�N���X
//=====================================================================//
class Bullet {

public:
	float pos_x = 0; // ���WX
	float posCenter_x = 0.0f; // �摜�̒��S�̍��WX
	float pos_y = 0; // ���WY
	float posCenter_y = 0.0f; // �摜�̒��S�̍��WY
	float hitBox = 1.0f; // �����蔻��
	float enemyDistance = 0.0f; // �G�܂ł̋���

	float textureCenterX = 0.0f; // �摜�̒��SX
	float textureCenterY = 0.0f; // �摜�̒��SY
	float textureScaleX = 16.0f; // �摜�̃T�C�YX
	float textureScaleY = 16.0f; // �摜�̃T�C�YY

	bool isFired = false; // ���˂���Ă��邩

public:

	// �摜�̒��S�Ƃ���𒆐S�ɂ������W�̎Z�o
	void calcPosCenter() {
		textureCenterX = textureScaleX / 2;
		textureCenterY = textureScaleY / 2;
		posCenter_x = pos_x + textureCenterX;
		posCenter_y = pos_y + textureCenterY;
	}

};

#endif
