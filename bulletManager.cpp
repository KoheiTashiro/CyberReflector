#include "interface.h"
#define margin 10

enum type {
	bulletType,
	needleType,
	lazerType,
};

bulletManager::bulletManager()
{
	//bullets.reserve(5000);
}

void bulletManager::update() {
	//unique_ptrはコピーできないので参照で渡しておく
	auto erasable = std::remove_if(bullets.begin(), bullets.end(), 
		[](std::unique_ptr<bullet>& it) {
		return it->getX() < 0 - margin || it->getX() > ConstClass::ScreenX + margin 
			|| it->getY() < 0 - margin || it->getY() > ConstClass::ScreenY + margin; });

	bullets.erase(erasable, bullets.end());

	for (auto&& iter : bullets) {
		iter->update();
	}
}




void bulletManager::draw() {
	//ここを実装
	//イテレーターを利用してbulletのdrawを呼ぶ
	for (auto&& iter : bullets) {
		iter->draw();

	}
}

void bulletManager::addbullet(double x, double y, int v, double angle,bool setEnemy,int bulletKind) {

	
	//指定された座標に弾を生成
	switch (bulletKind) {
	case 1://まるいたま
		bullets.push_back(std::make_unique<bullet>(bullet(x, y, v, angle, setEnemy)));
		break;
	case 2://とげ
		bullets.push_back(std::make_unique<needle>(needle(x, y, v, angle, setEnemy)));
		break;
	case 3://れーざー
		bullets.push_back(std::make_unique<lazer>(lazer(x, y, v, angle, setEnemy)));
		break;
	case 4://みじかめれーざー
		bullets.push_back(std::make_unique<lazerm>(lazerm(x, y, v, angle, setEnemy)));
		break;
	}

}
