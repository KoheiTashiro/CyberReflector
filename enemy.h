#pragma once
#include "interface.h"

//enemy�̓����o�ϐ��Ƃ���gameManager�ւ̃|�C���^�����̂ŁA���O�ɂ��̑��݂������˂΂Ȃ�Ȃ��B
//�N���X�̑O���錾�����Ă����B
class gameManager;

class enemy :public object {
protected:
	//�|������Ⴆ�链�_
	int score;

public:
	int interval;
	int hp;
	//�Ō�ɒe��������Ă���ǂ̂��炢���Ԃ��������̂��B
	int shotedTime;
	gameManager* game;


	enemy(double ix, double iy, gameManager *setGame);

	virtual void update();

	virtual void draw();

	void checkBulletHit(gameManager *game);

	void checkShotedTime();

};

class enemy2 : public enemy {
	public:
	enemy2(double ix, double iy, gameManager *setGame);

	void update();

	void draw();
	
};

class enemyA :public enemy {
public:
	enemyA(double ix, double iy, gameManager *setGame);

	void update();

	void draw();

};

class enemyA2 :public enemy {
public:
	enemyA2(double ix, double iy, gameManager *setGame);

	void update();

	void draw();

};
class enemyB :public enemy {

	//�e�ۂ̔��ˊp�x
	double bulletAngle;

	//�e�ۂ������n�߂��Ƃ��̎��@�̊p�x
	double oldAngle;

public:
	enemyB(double ix, double iy, gameManager *setGame);

	void update();

	void draw();

};