#pragma once
#include "interface.h"

//enemyはメンバ変数としてgameManagerへのポインタを持つので、事前にその存在を教えねばならない。
//クラスの前方宣言をしておく。
class gameManager;

class enemy :public object {
protected:
	//倒したら貰える得点
	int score;

public:
	int interval;
	int hp;
	//最後に弾をくらってからどのぐらい時間がたったのか。
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

	//弾丸の発射角度
	double bulletAngle;

	//弾丸を撃ち始めたときの自機の角度
	double oldAngle;

public:
	enemyB(double ix, double iy, gameManager *setGame);

	void update();

	void draw();

};