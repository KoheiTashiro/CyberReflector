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
	//unique_ptr�̓R�s�[�ł��Ȃ��̂ŎQ�Ƃœn���Ă���
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
	//����������
	//�C�e���[�^�[�𗘗p����bullet��draw���Ă�
	for (auto&& iter : bullets) {
		iter->draw();

	}
}

void bulletManager::addbullet(double x, double y, int v, double angle,bool setEnemy,int bulletKind) {

	
	//�w�肳�ꂽ���W�ɒe�𐶐�
	switch (bulletKind) {
	case 1://�܂邢����
		bullets.push_back(std::make_unique<bullet>(bullet(x, y, v, angle, setEnemy)));
		break;
	case 2://�Ƃ�
		bullets.push_back(std::make_unique<needle>(needle(x, y, v, angle, setEnemy)));
		break;
	case 3://��[���[
		bullets.push_back(std::make_unique<lazer>(lazer(x, y, v, angle, setEnemy)));
		break;
	case 4://�݂����߂�[���[
		bullets.push_back(std::make_unique<lazerm>(lazerm(x, y, v, angle, setEnemy)));
		break;
	}

}
