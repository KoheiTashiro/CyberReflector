#pragma once
#include "interface.h"

class bullet: public object {
public:
	
	bullet(double startX, double startY, int setMovev, double setAngle,bool setEnemy);

	//�_���[�W��
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

	//�e���������ꂽ���̎��Ԃ�ێ�����ϐ�
	u_int launchedTime;

	double spinAngle;
public:
	lazerm(double startX, double startY, int setMovev, double setAngle,  bool setEnemy);

	void bulletReflect();

	void draw();
};