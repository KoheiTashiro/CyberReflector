#include "interface.h"

class gameManager;

enemyManager::enemyManager(gameManager  * setGame) {

	game = setGame;
}

void enemyManager::update() {

	for (auto&& iter :enemys) {
		iter->update();
		
	}

}

void enemyManager::draw() {

	for (auto&& iter :enemys) {
		iter->draw();
		iter->checkShotedTime();
	}
}

void enemyManager::addEnemyDebug(gameManager* setGame) {

	//�w�肳�ꂽ���W�ɓG�𐶐�
	#ifdef _DEBUG
	if (Input::KeyN.clicked) {
		//enemys��shared_ptr<enemy>��list�Ȃ̂ŁAmake_shared�Ń|�C���^��n�����B
		enemys.push_back(std::make_unique<enemy>((double)Random(20, ConstClass::GameScreenX), (double)Random(ConstClass::GameStartY, ConstClass::ScreenY), game));
	}
	if (Input::KeyX.clicked) {
		enemys.push_back(std::make_unique<enemyA>((double)Random(20, ConstClass::GameScreenX), (double)Random(ConstClass::GameStartY, ConstClass::ScreenY), game));
	}
	if (Input::KeyC.clicked) {
		enemys.push_back(std::make_unique<enemyB>((double)Random(20, ConstClass::GameScreenX), (double)Random(ConstClass::GameStartY, ConstClass::ScreenY), game));
	}
	if (Input::KeyV.clicked) {
		enemys.push_back(std::make_unique<enemy2>((double)Random(20, ConstClass::GameScreenX), (double)Random(ConstClass::GameStartY, ConstClass::ScreenY), game));
	}
	if (Input::KeyB.clicked) {
		enemys.push_back(std::make_unique<enemyA2>((double)Random(20, ConstClass::GameScreenX), (double)Random(ConstClass::GameStartY, ConstClass::ScreenY), game));
	}
	#endif
	
}

void enemyManager::addEnemy(int enemyId, int x, int y)
{
	switch (enemyId) {
	case 100://Random
		enemyId = Random(1, 3);
		break;
	}


	switch (enemyId) {
	case 1://fourway-fort
		enemys.push_back(std::make_unique<enemy>((double)x,(double)y , game));
		break;
	case 2://chaser
		enemys.push_back(std::make_unique<enemyA>((double)x, (double)y, game));
		break;
	case 3://spinner
		enemys.push_back(std::make_unique<enemyB>((double)x, (double)y, game));
		break;
	case 4://spin lazer
		enemys.push_back(std::make_unique<enemy2>((double)x, (double)y, game));
		break;
	case 5://stalker
		enemys.push_back(std::make_unique<enemyA2>((double)x, (double)y, game));
		break;
	}
	


}

//�֐��̃I�[�o�[���[�h
void enemyManager::addEnemy(int enemyId, Point point)
{


	switch (enemyId) {
	case 100://Random
		enemyId = Random(1, 3);
		break;
	}

	switch (enemyId) {
	case 1://fourway-fort
		enemys.push_back(std::make_unique<enemy>((double)point.x, (double)point.y, game));
		break;
	case 2://chaser
		enemys.push_back(std::make_unique<enemyA>((double)point.x, (double)point.y, game));
		break;
	case 3://spinner
		enemys.push_back(std::make_unique<enemyB>((double)point.x, (double)point.y, game));
		break;
	case 4://spin lazer(����)
		enemys.push_back(std::make_unique<enemy2>((double)point.x, (double)point.y, game));
		break;
	case 5://stalker
		enemys.push_back(std::make_unique<enemyA2>((double)point.x, (double)point.y, game));
		break;
	}


}

int enemyManager::howManyEnemy()
{
	return size(enemys);
}

void enemyManager::checkBulletHit()
{
	for (auto&& it : enemys) {
		if (it->getExist() == true) it->checkBulletHit(game);
	}
	
}

void enemyManager::checkExist(gameManager *game)
{
	auto it = enemys.begin();


	while (it != enemys.end()) {
		//����exist��false�Ȃ�
		if ((*it)->getExist() == false) {
			
			//�����G�t�F�N�g
			game->effect->add<Ef02>(Vec2((*it)->getX(), (*it)->getY()));
			
			//�����������s���B
			//�G�t�F�N�g�̑O�ɏ���������o�O��̂Œ��Ӂi������܂����̃N���b�J�[�j
			//erase�̕Ԃ�l��it�Ŏ󂯂�i�Ԃ�l�͍폜�������̂̎��ɂȂ����Ă����v�f�ւ̃C�e���[�^�j
			it = enemys.erase(it);

			//�G��|�����Ƃ��ăJ�E���g
			achievements::get().beatEnemy();

			
		}else it++;
	}
}


