#pragma once
#include "interface.h"



class bulletManager {


public:

	enum type;

	//ここにbulletへのスマートポインタの動的配列
	std::vector<std::unique_ptr<bullet>> bullets;

	bulletManager();



	void update();


	void draw();

	void addbullet(double x, double y, int v, double angle,bool setEnemy,int bullletKind);

};
