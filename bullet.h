#pragma once
#include "interface.h"

class bullet: public object {
public:
	
	bullet(double startX, double startY, int setMovev, double setAngle,bool setEnemy);

	//ダメージ量
	int damage;

	virtual void update();

	virtual void draw();

	virtual void bulletReflect();

};

class needle : public bullet {
public:

	needle(double startX, double startY, int setMovev, double setAngle,  bool setEnemy);
	
	//void update();

	void draw();

};

class lazer : public bullet {
public:
	lazer(double startX, double startY, int setMovev, double setAngle,  bool setEnemy);
	
	void draw();

	void bulletReflect();
};

class lazerm : public bullet {

	//弾が生成された時の時間を保持する変数
	u_int launchedTime;

	double spinAngle;
public:
	lazerm(double startX, double startY, int setMovev, double setAngle,  bool setEnemy);

	void bulletReflect();

	void draw();
};