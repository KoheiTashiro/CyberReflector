#pragma once
#include "interface.h"

//存在を教えておかないと、配列やらポイント型が準備できない。
class gameManager;
class enemy;

class enemyManager {
	

public:

	//ここにenemyへのスマートポインタの動的配列
	//ランダムアクセスではないのでlistで
	std::vector<std::unique_ptr<enemy>> enemys;

	//gameManagerへのポインタをgameManagerから受け取る、なおこれはenemyに渡す。
	gameManager *game;

	//ヒットポイント
	double hp;

	enemyManager(gameManager *  setGame);

	void update();

	void draw();

	void addEnemyDebug(gameManager *game);

	void addEnemy(int enemyId,int x,int y);

	//関数のオーバーロード　int2つの代わりにpointが使える
	void addEnemy(int enemyId, Point point);

	int howManyEnemy();

	void checkBulletHit();

	void checkExist(gameManager *game);

};
