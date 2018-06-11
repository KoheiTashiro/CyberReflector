#pragma once
#include "interface.h"

//���݂������Ă����Ȃ��ƁA�z����|�C���g�^�������ł��Ȃ��B
class gameManager;
class enemy;

class enemyManager {
	

public:

	//������enemy�ւ̃X�}�[�g�|�C���^�̓��I�z��
	//�����_���A�N�Z�X�ł͂Ȃ��̂�list��
	std::vector<std::unique_ptr<enemy>> enemys;

	//gameManager�ւ̃|�C���^��gameManager����󂯎��A�Ȃ������enemy�ɓn���B
	gameManager *game;

	//�q�b�g�|�C���g
	double hp;

	enemyManager(gameManager *  setGame);

	void update();

	void draw();

	void addEnemyDebug(gameManager *game);

	void addEnemy(int enemyId,int x,int y);

	//�֐��̃I�[�o�[���[�h�@int2�̑����point���g����
	void addEnemy(int enemyId, Point point);

	int howManyEnemy();

	void checkBulletHit();

	void checkExist(gameManager *game);

};
