/**
* @file Enemy.h
* @brief �G�̏���ۑ�����N���X�̐錾
*/
#ifndef ENEMY_H_
#define ENEMY_H_

//=====================================================================//
//! �G�f�[�^�p�N���X
//=====================================================================//

// �ړ����@�̗�
enum MovingMode {
	Forward,
	Sway,
	Around
};

class Enemy {

public:
	float pos_x = WINDOW_WIDTH / 2 - 25; // ���WX
	float posCenter_x = 0.0f; // �摜�̒��S�̍��WX
	float pos_y = 0; // ���WY
	float posCenter_y = 0.0f; // �摜�̒��S�̍��WY
	float hitBox = 20.0f; // �����蔻��
	float playerDistance = 0.0f; // �v���C���[�܂ł̋���
	float moveSpeed = 0.5f; // �ړ����x
	MovingMode movingMode = Forward; // �ړ����@

	float textureCenterX = 0.0f; // �摜�̒��SX
	float textureCenterY = 0.0f; // �摜�̒��SY
	float textureScaleX = 16.0f; // �摜�̃T�C�YX
	float textureScaleY = 16.0f; // �摜�̃T�C�YY
	float textureScaling = 3.0f; // �摜�̊g�嗦

	bool swayTemp = false; // MoveSway�̂��߂�
	bool isLive = false; // �����Ă��邩�ǂ���
	bool isHitting = false; // �e�ɓ������Ă��邩�ǂ���
	int destroyScore = 10; // �|���ꂽ���ɉ��Z����X�R�A�̗�

public:

	void Reset() {
		isLive = false;
		isHitting = false;
		pos_x = rand() % 580;
		pos_y = 0;
		switch (rand() % 3)
		{
		default:
		case 0:movingMode = Forward;
			break;
		case 1:movingMode = Sway;
			break;
		case 2:movingMode = Around;
			break;
		}
	}
	
	// �摜�̒��S�Ƃ���𒆐S�ɂ������W�̎Z�o
	void calcPosCenter() {
		textureCenterX = textureScaleX * textureScaling / 2;
		textureCenterY = textureScaleY * textureScaling / 2;
		posCenter_x = pos_x + textureCenterX;
		posCenter_y = pos_y + textureCenterY;
	}

	// �^���ɓ���
	void MoveForward() {
		pos_y += moveSpeed;
	}

	// ���E�ɗh��Ȃ��牺�ɓ���
	void MoveSway() {
		pos_y += moveSpeed;

		if (posCenter_x >= WINDOW_WIDTH - 200) {
			swayTemp = true;
		}

		if (swayTemp == true && posCenter_x <= 200) {
			swayTemp = false;
		}

		switch (swayTemp)
		{
		case true:
			pos_x -= moveSpeed * 2;
			break;
		case false:
			pos_x += moveSpeed * 2;
			break;
		default:
			break;
		}

	}

	// �~�^�������Ȃ��牺�ɓ���
	void MoveAround() {
		pos_y += moveSpeed;
	}

	// �ړ�,�X�R�A����
	void EnemyMovingSwitch() {
		switch (movingMode)
		{
		case Forward:
			MoveForward();
			destroyScore = 10;
			break;
		case Sway:
			MoveSway();
			destroyScore = 20;
			break;
		case Around:
			MoveAround();
			destroyScore = 30;
			break;
		default:
			break;
		}
	}

	// ��ʊO����
	bool WindowOut(){
		if (posCenter_x <= 0 || posCenter_x >= WINDOW_WIDTH ||
			posCenter_y <= -50 || posCenter_y >= WINDOW_HEIGHT) {
			return true;
		}
		else {
			return false;
		}
	}

	// �C���N�������g
	Enemy() {
		srand((unsigned)time(NULL));
	}
};

#endif
