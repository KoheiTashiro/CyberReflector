#include "interface.h"
//#define shieldArc (Pi)
#define shieldSurplus  5

player::player(double startX,double startY,gameManager* setGame,int machineSelect) {
	game = setGame;
	x = startX;
	y = startY;
	angle = atan2(Mouse::Pos().x - x, Mouse::Pos().y - y);
	fireSize = 0;
	hitArea = 20;
	shotedTime = 999;
	acceleration = Vec2(0,0);
	speed = Vec2(0,0);
	enemyFlag = false;
	fireAnime = std::make_unique<Anime>(TextureAsset(L"fireAnime"), 85, 1);

	switch (machineSelect) {
	case ConstClass::Machine::BLENIM:
		movev = ConstClass::PlayerSpeed;
		hpMax = ConstClass::PlayerHitPoint;
		energyMax = ConstClass::Energy;
		energyHeal = ConstClass::HealEnergy;
		shieldArc = Pi;
		resistance = 0.9;
		textureName = L"myUnit";
		break;
	case ConstClass::Machine::METEOR:
		movev = 13;
		hpMax = 180;
		energyMax = 140;
		energyHeal = 3;
		shieldArc = 5*Pi/3;
		resistance = 0.92;
		textureName = L"secretUnit";
		break;
	}

	hp = hpMax;
	energy = energyMax;
}
/*
void player::update() {

	angle = atan2(Mouse::Pos().x - x, Mouse::Pos().y - y);


	if (Input::MouseL.pressed) {
		

		double movedx = sin(angle)*movev + x;
		double movedy = cos(angle)*movev + y;

		//１Ｆの移動距離より近くにアイコンがあれば移動しない。
		if (!Circle(x,y,movev).intersects(Point(Mouse::Pos()) )){
			x += sin(angle)*movev;
			y += cos(angle)*movev;
		}

	}
	if (Input::MouseR.pressed) {
		//y += movev;
		angle = atan2(Mouse::Pos().x - x, Mouse::Pos().y - y);
		x -= sin(angle)*backv;
		y -= cos(angle)*backv;
	}

	x = Clamp((int)x, 0,ConstClass::ScreenX);
	y = Clamp((int)y, 0, ConstClass::ScreenY);

}
*/
void player::update() {

	angle = atan2(Mouse::Pos().x - x, Mouse::Pos().y - y);

	//加速度の設定
	acceleration = Vec2(0,0);

	if (Input::MouseL.pressed && !Circle(x,y,5).intersects(Mouse::Pos())) {
		
		acceleration.x = sin(angle)*0.7;
		acceleration.y = cos(angle)*0.7;
	}


	speed = resistance*(speed + acceleration);

	x += speed.x;

	y += speed.y;


	//ある値を逸脱していたら、戻す。壁！
	x = Clamp(x, hitArea, (double)ConstClass::GameScreenX - hitArea);
	y = Clamp(y, hitArea+ConstClass::GameStartY, (double)ConstClass::ScreenY -hitArea);

	//シールドを貼るか判定
	if(Input::MouseR.pressed){
		//エネルギーが切れていなければ
		if (energy > 0) {
			shield = true;
			--energy;
		}
		else {
			shield = false;
		}
	}
	else {
				shield = false;
				//エネルギーを回復
				energy = energy + energyHeal;
				energy = Clamp(energy,0,energyMax);
	}

	//hpが0以下なら、存在を消す
	if (hp <= 0) {
		game->effect->add<Ef02>(Vec2(x, y));

		exist = false;
	}
	
}

void player::draw() {
	

	//HPが０
	if (exist == false) return;
		
	
	Graphics2D::SetBlendState(BlendState::Additive);

	if (Input::MouseL.pressed) {
		fireSize += 0.0015;
	}
	else {
		fireSize -= 0.0015;
	}

	fireSize = Clamp(fireSize, 0.0, 0.06);

	
	//(TextureAsset(L"fireme").scale(0.2 + fireSize + Random(-0.005 - fireSize / 10, 0.005 + fireSize / 10))).rotate(-angle + Random(-0.04, 0.04)).drawAt(x + hitArea*(1.7 + fireSize * 10) * sin(-angle), y - hitArea*(1.7 + fireSize * 10) * cos(-angle));
	fireAnime->update();
	fireAnime->draw(Vec2{x + hitArea*(2.2 + fireSize * 5) * sin(-angle),y - hitArea*(2.2 + fireSize * 5) * cos(-angle)},0.5+fireSize,-angle);

	Graphics2D::SetBlendState(BlendState::Default);

	TextureAsset(textureName).rotate(-angle+Pi).drawAt(x,y);

	if (shield == true) {
	//シールドを描写

	//	Circle(x, y, hitArea + 8).drawArc(-angle + Pi - shieldArc / 2, shieldArc, 5.0, 0.0, HSV(40+energy, 1, (double)energy / ConstClass::Energy));
	//	Circle(x, y, hitArea + 8).drawArc(-angle + Pi - shieldArc / 2, shieldArc, 2.0, 0.0, HSV(40+energy, 1, 1));

		//Circle(x, y, hitArea + 8).drawArc(-angle + Pi - shieldArc / 2, shieldArc*energy / ConstClass::Energy, 5.0, 0.0, HSV(120, 1, 1));
		Circle(x, y, hitArea + 8).drawPie(-angle + Pi - shieldArc / 2, shieldArc*energy / energyMax, { HSV(120, 1, 1),150 });
		//Circle(x, y, hitArea + 8).drawArc(-angle + Pi - Pi, 2*Pi*energy / ConstClass::Energy, 5.0, 0.0, HSV(120, 1, 1));
		Circle(x, y, hitArea + 8).drawArc(-angle + Pi - shieldArc / 2, shieldArc, 5.0, 0.0, HSV(40, 1, 1));

	}

	checkShotedTime();

}

void player::checkBulletHit()
{
	auto itbullet = game->bullets->bullets.begin();

	while (itbullet != game->bullets->bullets.end()) {


		//敵の弾が
		if ((*itbullet)->getEnemyFlag() == true) {
						
			iangle = atan2((*itbullet)->getX() - x, (*itbullet)->getY() - y);
			iangle = fmod(abs(iangle - angle), 2 * Pi);
			
			const bool isIntersects = Circle((*itbullet)->getX(), (*itbullet)->getY(), (*itbullet)->getHitArea()).
				intersects(Circle(x, y, hitArea + shieldSurplus));
			const bool isFrontShoted= (iangle < shieldArc / 2 || iangle >2 * Pi - shieldArc / 2);
			
			//自機が弾と接触していて
			//前方から弾に当たっていて
			//シールドを貼っているなら
			if (isIntersects && isFrontShoted && shield) {

				//弾を反射する。
				(*itbullet)->bulletReflect();

				itbullet++;

			}else
			//自機にあたったら
			if (Circle((*itbullet)->getX(), (*itbullet)->getY(), (*itbullet)->getHitArea()).intersects(Circle(x,y,hitArea))) {
				
				
				//じきにダメージを与える。
				hp -= (*itbullet)->damage;
				//itbullet++;
				SoundAsset(L"damage").playMulti();
				game->effect->add<Efplayer>(Vec2((*itbullet)->getX(), (*itbullet)->getY()));
				//hpを描写するために、shotedTimeを０にする。
				shotedTime = 0;
				//弾を消す。
				itbullet = game->bullets->bullets.erase(itbullet);
			}
			else itbullet++;

		}
		else itbullet++;

	}



}

void player::checkEnemyTouch()
{

	auto itenemy = game->enemys->enemys.begin();

	while (itenemy != game->enemys->enemys.end()) {

		
		//自機が敵にあたったら
				if (Circle((*itenemy)->getX(), (*itenemy)->getY(), (*itenemy)->getHitArea()).intersects(Circle(x, y, hitArea))) {


					//じきにダメージを与える。
					hp -= 0.3;
					SoundAsset(L"damage").playMulti(0.05);
					checkShotedTime();
					shotedTime = 0;
				}

				itenemy++;

		}

}

void player::checkShotedTime()
{/*
	// 1.0 秒で消える
	if (shotedTime <= 30) {
		FontAsset(L"font").drawCenter(Format((int)hp), Vec2(getX(), getY() - 30), HSV(260 - hp/6));
	}
	else if (shotedTime <= 60) {
		//0.5秒以上経過で暗くなる。
		FontAsset(L"font").drawCenter(Format((int)hp), Vec2(getX(), getY() - 30), HSV(260 - hp/6).toColorF(1 - 1 / 30.0 * (shotedTime - 30)));
	}
	*/
	
	++shotedTime;
}
