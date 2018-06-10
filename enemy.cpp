#include "interface.h"
#define enemyArange 0.6

class gameManager;

enemy::enemy(double ix, double iy, gameManager *setGame) {
	x = ix;
	y = iy;
	movev = ConstClass::EnemySpeed;
	enemyFlag = true;
	game = setGame;
	interval = 0;
	hitArea = 24;
	hp = 30;
	shotedTime = 999;
	score = 100;

}

void enemy::update() {
	//modeにより移動方法を変更。
	
		angle = atan2(game->myUnit->getX() - x, game->myUnit->getY() - y);
		//x += sin(angle)*movev;
		//y += cos(angle)*movev;
		interval++;

	

	if (interval % 100 >=70 && interval % 2 == 0) {
		game->bullets->addbullet(x - hitArea* sin(0), y + hitArea* cos(0),  5, 0, true,3);
		game->bullets->addbullet(x - hitArea* sin(-Pi/2), y + hitArea* cos(-Pi/2), 5, Pi/2,  true,3);
		game->bullets->addbullet(x - hitArea* sin(-Pi), y + hitArea* cos(-Pi), 5, Pi,  true,3);
		game->bullets->addbullet(x - hitArea* sin(-3*Pi/2), y + hitArea* cos(-3*Pi/2), 5, 3*Pi/2,  true,3);
	}

}

void enemy::draw() {



	TextureAsset(L"foreway").drawAt(x, y);
	//Circle(x, y, 24).draw(Palette::Skyblue);



}



enemy2::enemy2(double ix, double iy, gameManager *setGame) : enemy(ix, iy, setGame) {
	score = 200;
	hp = 100;
	angle = Random(0.0, Pi);
};

void enemy2::update()
{
	//modeにより移動方法を変更。

	//angle = atan2(game->myUnit->getX() - x, game->myUnit->getY() - y);
	angle += ConstClass::LazerRotateSpeed;
	//x += sin(angle)*movev;
	//y += cos(angle)*movev;
	
	if (interval > 30) {
		game->bullets->addbullet(x - hitArea* sin(-angle), y + hitArea* cos(-angle), 5, angle, true, 4);
		game->bullets->addbullet(x - hitArea* sin(-Pi / 2 - angle), y + hitArea* cos(-Pi / 2 - angle), 5, angle + Pi / 2, true, 4);
		game->bullets->addbullet(x - hitArea* sin(-Pi - angle), y + hitArea* cos(-Pi - angle), 5, angle + Pi, true, 4);
		game->bullets->addbullet(x - hitArea* sin(-3 * Pi / 2 - angle), y + hitArea* cos(-3 * Pi / 2 - angle), 5, angle + 3 * Pi / 2, true, 4);
	}

	interval++;
}

void enemy2::draw()
{
	TextureAsset(L"spinlazer").rotate(-angle + Pi).drawAt(x, y);
}


void enemy::checkBulletHit(gameManager *game)
{

	auto itbullet = game->bullets->bullets.begin();
	while (itbullet != game->bullets->bullets.end()) {

		//自機の弾が
		if ((*itbullet)->getEnemyFlag() == false) {
			//敵にあたったら
			if (Circle((*itbullet)->getX(), (*itbullet)->getY(), (*itbullet)->getHitArea()).intersects(Circle(x, y, hitArea))) {
				
				//てきにダメージを与える。
				hp -= (*itbullet)->damage;
				//hpを描写するために、shotedTimeを０にする。
				shotedTime = 0;
				//game->effect->add<TextEffect>(hp, Vec2(getX(), getY()));
				//もしＨＰが０以下になったら存在を抹消する。
				if (hp <= 0 && exist == true) {
					//スコアを足す
					game->score->addScore(score);
					exist = false;
					SoundAsset(L"bomb").playMulti();
				}

				//弾を消す。
				itbullet = game->bullets->bullets.erase(itbullet);
				//itbullet++;
			}
			else itbullet++;
		}
		else itbullet++;

	}
}

void enemy::checkShotedTime()
{
	// 1.0 秒で消える
	if (shotedTime <= 30) {
		FontAsset(L"font").drawCenter(Format(hp), Vec2(getX(), getY() - 30), HSV(60 - hp));
	}
	else if (shotedTime <= 60) {
		//0.5秒以上経過で暗くなる。
		FontAsset(L"font").drawCenter(Format(hp), Vec2(getX(), getY() - 30), HSV(60 - hp).toColorF(1 - 1 / 30.0 * (shotedTime - 30)));
	}
	++shotedTime;
}

enemyA::enemyA(double ix, double iy, gameManager *setGame) : enemy(ix, iy, setGame) {


};

void enemyA::update() {
	//modeにより移動方法を変更。

	angle = atan2(game->myUnit->getX() - x, game->myUnit->getY() - y);
	
	interval++;

	if (interval % 120< 40) {
		x += sin(angle)*movev;
		y += cos(angle)*movev;
	}


	if (interval % 30 ==0) {
		game->bullets->addbullet(x - hitArea*enemyArange* sin(-angle) , y + hitArea*enemyArange* cos(-angle),6, angle, true,1);
	}

}

void enemyA::draw() {


	TextureAsset(L"chaser").rotate(-angle + Pi).drawAt(x, y);
	//Circle(x, y, 24).draw(Palette::Yellow);

	

}

enemyA2::enemyA2(double ix, double iy, gameManager * setGame) : enemy(ix, iy, setGame)
{
	score = 150;
	hp = 40;
	movev = 2.5;
}

void enemyA2::update() {

	angle = atan2(game->myUnit->getX() - x, game->myUnit->getY() - y);

	interval++;

	if (interval > 45) {
		x += sin(angle)*movev;
		y += cos(angle)*movev;
	}


	if (interval % 30 == 0 && interval >45) {
		game->bullets->addbullet(x - hitArea*enemyArange* sin(-angle), y + hitArea*enemyArange* cos(-angle), 7, angle,  true, 1);
	}
}

void enemyA2::draw() {
	TextureAsset(L"stalker").rotate(-angle + Pi).drawAt(x, y);
	//Circle(x, y, 24).draw(Palette::Yellow);


}

enemyB::enemyB(double ix, double iy, gameManager * setGame):enemy(ix,iy,setGame)
{
	bulletAngle = 0;
	interval = Random(0, 179);
	movev = 1;
	hp = 50;
	score = 120;
}

void enemyB::update()
{

	angle = atan2(game->myUnit->getX() - x, game->myUnit->getY() - y);

	interval++;

	//if (interval % 120< 40) {
		//距離が遠ければ
	
//	}

	if (interval % 240 >=180) {
		if (interval % 2 == 0) {
			game->bullets->addbullet(x - hitArea* sin(-bulletAngle-angle), y + hitArea* cos(-bulletAngle-angle), 3, angle+ bulletAngle,  true, 2);
			//game->bullets->addbullet(x, y, 4, -bulletAngle,  true);
		}
		bulletAngle += 2 * Pi / 60;
	}
	else {
		if (pow((game->myUnit->getX() - x), 2) + pow((game->myUnit->getY() - y), 2)>pow(300, 2)) {
			//近づく
			x += sin(angle)*movev;
			y += cos(angle)*movev;
		}
		//距離が近ければ
		if (pow((game->myUnit->getX() - x), 2) + pow((game->myUnit->getY() - y), 2)<pow(200, 2)) {
			//離れる
			x -= sin(angle)*movev;
			y -= cos(angle)*movev;
		}
		bulletAngle = 0;
	}

	x = Clamp(x, hitArea, (double)ConstClass::GameScreenX - hitArea);
	y = Clamp(y, hitArea + ConstClass::GameStartY, (double)ConstClass::ScreenY - hitArea);
}

void enemyB::draw()
{
	TextureAsset(L"spinner").rotate(-bulletAngle-angle + Pi).drawAt(x, y);
	//Circle(x, y, 24).draw(Palette::Yellow);


}

