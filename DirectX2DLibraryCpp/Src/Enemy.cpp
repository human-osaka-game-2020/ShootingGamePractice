#include"Enemy.h"
#include "Engine/Engine.h"
#include<math.h>




Enemy::Enemy()
{

}
Enemy::~Enemy()
{


}

float Enemy::Pai()
{
	return atan(1.0) * 4.0f;
}

void Enemy::EnemyPos()
{
	//==========[Enemy�̒ǐՏ���]==========//

	float tmp_wid = (player_pos_x - pParam->enemy_pos_x) * (player_pos_x - pParam->enemy_pos_x);
	float tmp_hei = (player_pos_y - pParam->enemy_pos_y) * (player_pos_y - pParam->enemy_pos_y);
	float y = sqrt(tmp_hei);
	float x = sqrt(tmp_wid);
	float tmp_dia = tmp_hei + tmp_wid;
	float diagonal = sqrt(tmp_dia);
	pParam->radian = asin(y / diagonal);
	float Y_increment = y / diagonal;
	float X_increment = x / diagonal;



	if (player_pos_x > pParam->enemy_pos_x)
		pParam->enemy_pos_x += pParam->emove * X_increment;
	else
		pParam->enemy_pos_x -= pParam->emove * X_increment;


	if (player_pos_y > pParam->enemy_pos_y)
		pParam->enemy_pos_y += pParam->emove * Y_increment;
	else
		pParam->enemy_pos_y -= pParam->emove * Y_increment;
}

float Enemy::EnemyAngle(float r)		//enemy�̊p�x
{

	if (player_pos_x > pParam->enemy_pos_x)
	{

		if (player_pos_y == pParam->enemy_pos_y)				//y = 0
		{
			return 180.0f;
		}
		else if (player_pos_y > pParam->enemy_pos_y)			//Enemy�����ی�
		{
			return (r * 180.0f / Pai()) + 180.0f;
		}
		else											//Enemy����O�ی�
		{
			return ((Pai() / 2 - r) * 180.0f / Pai()) + 90.0f;

		}
	}
	else if (player_pos_x < pParam->enemy_pos_x)
	{
		if (player_pos_y == pParam->enemy_pos_y)				//y = 0
		{
			return 0.0f;
		}
		else if (player_pos_y > pParam->enemy_pos_y)			//Enemy�����ی�
		{
			return ((Pai() / 2 - r) * 180.0f / Pai()) + 270.0f;
		}
		else											//Enemy����l�ی�
		{
			return r * 180.0f / Pai();
		}
	}
	else if (player_pos_x == pParam->enemy_pos_x)				//x = 0
	{
		if (player_pos_y > pParam->enemy_pos_y)
		{
			return 90.0f;
		}
		else if (player_pos_y < pParam->enemy_pos_y)			//x = 0
		{
			return 270.0f;
		}
		else											//����d�Ȃ�
			return 0.0f;
	}
}

void Enemy::EnemyDrowing(bool cancel)
{
	if (cancel) {
		Engine::DrawTexture(pParam->enemy_pos_x, pParam->enemy_pos_y, "Enemy", 255, EnemyAngle(pParam->radian), 1.0f, 1.0f);
	}
	else
	{

	}
}

bool Enemy::EnemyExistence(float* epx, float* epy)
{
	if ((pParam->EnemyExistenceCount / 300) % 2 == 0)
	{
		return true;
	}
	else
	{
		*epx = pParam->enemy_initialize_x;				//global���Ɗ֐��O����ϐ������������Ă����邪
		*epy = pParam->enemy_initialize_y;				//�Ԃ����ʓ|�L���̂�pointer�g�p
		return false;
	}
}