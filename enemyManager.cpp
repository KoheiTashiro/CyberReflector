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

	//指定された座標に敵を生成
	#ifdef _DEBUG
	if (Input::KeyN.clicked) {
		//enemysはshared_ptr<enemy>のlistなので、make_sharedでポインタを渡そう。
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

//関数のオーバーロード
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
	case 4://spin lazer(つおい)
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
		//もしexistがfalseなら
		if ((*it)->getExist() == false) {
			
			//爆発エフェクト
			game->effect->add<Ef02>(Vec2((*it)->getX(), (*it)->getY()));
			
			//消去処理を行う。
			//エフェクトの前に消去したらバグるので注意（あたりまえだのクラッカー）
			//eraseの返り値をitで受ける（返り値は削除したものの次につながっていた要素へのイテレータ）
			it = enemys.erase(it);

			//敵を倒したとしてカウント
			achievements::get().beatEnemy();

			
		}else it++;
	}
}


