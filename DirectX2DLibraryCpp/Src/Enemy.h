#ifndef ENEMY_H
#define ENEMY_H




extern float player_pos_x;
extern float player_pos_y;

struct EnemyParam
{

	const float enemy_initialize_x = 400.0f;
	const float enemy_initialize_y = 200.0f;
	float enemy_pos_x ;
	float enemy_pos_y ;
	const float emove = 1.0f;
	float radian;
	int EnemyExistenceCount ;
	bool isDead;

	EnemyParam()
	{
		enemy_pos_x = enemy_initialize_x;
		enemy_pos_y = enemy_initialize_y;
		radian = 0;
		EnemyExistenceCount = 0;
		isDead = false;
	}

};

class Enemy
{
public:
	EnemyParam* pParam;
	
	Enemy();
	~Enemy();

	float Pai();

	void EnemyPos();

	float EnemyAngle(float r);

	void EnemyDrowing(bool cancel);

	bool EnemyExistence(float* epx, float* epy);



};













#endif //ENEMY_H