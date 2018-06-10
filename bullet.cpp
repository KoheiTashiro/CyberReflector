#include "interface.h"

bullet::bullet(double startX, double startY, int setMovev,double setAngle,bool setEnemy) {
	x = startX;
	y = startY;
	movev = setMovev;
	angle = setAngle;
	hitArea = 8;
	enemyFlag = setEnemy;
	damage = 10;
	
}


void bullet::update() {

	


			x += sin(angle)*movev;
			y += cos(angle)*movev;
	


}

void bullet::draw() {
	//ìGÇ©ñ°ï˚Ç≈íeÇÃêFÇïœÇ¶ÇÈÅB

	Graphics2D::SetBlendState(BlendState::Additive);
	if (enemyFlag == true) {
		Circle(x, y, 8).draw(Color(Palette::Yellow,200));
	//	(TextureAsset(L"ball")).drawAt(x, y);
		//(TextureAsset(L"ball")).drawAt(x, y);
	}
	else {
		Circle(x, y, 8).draw(Color(Palette::Skyblue, 200));
	}

	Graphics2D::SetBlendState(BlendState::Default);
}

void bullet::bulletReflect()
{
	enemyFlag = false;
	angle = angle+Pi;
	SoundAsset(L"reflection").playMulti(0.7);
}


needle::needle(double startX, double startY, int setMovev, double setAngle, bool setEnemy) : bullet (startX,startY, setMovev,setAngle, setEnemy) {
}

void needle::draw()
{
	if (enemyFlag == true) {
		//Graphics2D::SetBlendState(BlendState::Additive);

		Triangle({x,y-8} ,{x+8,y+12} ,{x-8,y+12}).rotated(-angle + Pi).draw(Color(Palette::Yellow, 200));

		//(TextureAsset(L"needle")).rotate(-angle + Pi).drawAt(x, y);
	//Graphics2D::SetBlendState(BlendState::Default);

	}
	else {
		Triangle({ x,y - 8 }, { x + 8,y + 12 }, { x - 8,y + 12 }).rotated(-angle + Pi).draw(Color(Palette::Skyblue, 200));
	}
}
;

lazer::lazer(double startX, double startY, int setMovev, double setAngle, bool setEnemy) : bullet(startX, startY, setMovev, setAngle,  setEnemy) {
	damage = 2;
	hitArea = 6;
}

void lazer::draw()
{

	Graphics2D::SetBlendState(BlendState::Additive);
	if (enemyFlag == true) {
		(TextureAsset(L"lazer")).rotate(-angle + Pi).drawAt(x, y);
	}
	else {
		(TextureAsset(L"myLazer")).rotate(-angle + Pi).drawAt(x, y);
	}
	
	
	Graphics2D::SetBlendState(BlendState::Default);
}

void lazer::bulletReflect()
{
	enemyFlag = false;
	angle = angle + Pi;
	SoundAsset(L"reflection").playMulti(0.2);
}

lazerm::lazerm(double startX, double startY, int setMovev, double setAngle,  bool setEnemy) : bullet(startX, startY, setMovev, setAngle,  setEnemy) {
	
	//launchedTime = Time::GetMillisec();
	damage = 2;
	spinAngle = 0;
	hitArea = 6;

}

void lazerm::bulletReflect()
{
		enemyFlag = false;
		angle = angle + Pi;
		SoundAsset(L"reflection").playMulti(0.2);

}

void lazerm::draw()
{

	Graphics2D::SetBlendState(BlendState::Additive);
	if (enemyFlag == true) {
		(TextureAsset(L"lazerm")).rotate(-angle-spinAngle + Pi).drawAt(x, y);
	}
	else {
		(TextureAsset(L"myLazerm")).rotate(-angle-spinAngle + Pi).drawAt(x, y);
	}

	Graphics2D::SetBlendState(BlendState::Default);
	

}
