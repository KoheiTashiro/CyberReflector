#include "interface.h"

object::object(){ exist = true; }

object::~object(){}

double object::getX() const { return x; }

double object::getY() const { return y; }

double object::getAngle() const{ return angle; }

double object::getHitArea() const { return hitArea; }

bool object::getEnemyFlag() const { return enemyFlag; }

bool object::getExist() const
{
	return exist;
}

void object::draw() {
	graphic.rotate(-angle + 3.1415).drawAt(x, y);
}