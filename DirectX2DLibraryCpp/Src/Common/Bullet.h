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
	float pos_y = 0; // ���WY
	float hitBox = textureScaleX; // �����蔻��

	float textureCenterX = textureScaleX / 2; // �摜�̒��SX
	float textureCenterY = textureScaleY / 2; // �摜�̒��SY
	float textureScaleX = 16.0f; // �摜�̃T�C�YX
	float textureScaleY = 16.0f; // �摜�̃T�C�YY

	bool isFired = false; // ���˂���Ă��邩

};

#endif
