#pragma once
#include "interface.h"

class player;
class enemy;
class enemyManager;
class bulletManager;
class scoreManager;
class Game;

class gameManager
{

public:
	gameManager(int setGameLevel, int setMachineSelect) {
		bullets = std::make_unique<bulletManager>();
		enemys = std::make_unique<enemyManager>(this);
		myUnit = std::make_unique<player>(400, 300, this, setMachineSelect);
		effect = std::make_unique<Effect>();
		isOver = false;
		isPause = false;
		gameLevel = setGameLevel;
		machineSelect = setMachineSelect;
		gameClearCount = 0;
		//waveManager�łǂ�Lv��csv��ǂݍ��ނ���gameLevel�Ɉˑ�����̂Ő�ɓǂݍ��ށB
		wave = std::make_unique<waveManager>(this);
		score = std::make_unique<scoreManager>(gameLevel);


	};
	
	//�R���X�g���N�^��private�Ȃ̂�Get�֐��ȊO����͐����s��
	//gameManager(const gameManager& r) = delete;//�R�s�[�R���X�g���N�^�̍폜(�R�s�[�֎~)
	//gameManager& operator=(const gameManager& r) = delete;//������Z�q�̍폜(����֎~)

	~gameManager() {
		ClipCursor(NULL);
	}

public:
	std::unique_ptr<enemyManager> enemys;
	std::unique_ptr<player> myUnit;
	std::unique_ptr<bulletManager> bullets;
	std::unique_ptr<Effect> effect;
	std::unique_ptr<scoreManager> score;
	std::unique_ptr<waveManager> wave;
	//�Q�[���̃��x�����w�肷��ϐ��AwaveManager�ɓn��
	int gameLevel;
	//�@�̂̎�ނ��w�肷��ϐ��Aplayer�ɓn��
	int machineSelect;

	//�������Ԋm�F�p
	MillisecClock c;
	//�E�B���h�E�̍��W�E�傫���������BSiv3d��Rect����Ȃ���Bwindows.h��RECT����B
	RECT  r;
	//�E�B���h�E�̃n���h��
	HWND hwnd;

	//�|�[�Y���Ă��邩�ǂ����B
	bool isPause;

	//�Q�[�����I���������ǂ����B
	bool isOver;

	//�Q�[���N���A���Ă���̕b��
	int gameClearCount;


	//�w�i�p�̃J���[�m�ەϐ�
	int color;
	int color2;
	int color3;

	/*
	�V���O���g���g�킸
	�Q�[����邽��init�֐��ŏ���������̂͂߂�ǂ���...
	gameManager���͕̂��ʂɍ���āAgameManager�̐������Ǘ�����V���O���g���𑼂ɍ��������H
	static gameManager& get() {

		static gameManager inst;//static�ϐ��Ȃ̂ŏ��߂�Get�֐����Ă΂ꂽ�Ƃ��ȊO�͎��̂��c��B
		return inst;
	}
	*/

	void update() {

		if (Input::KeySpace.clicked) {
			isPause = !isPause;
		}

		if (isPause) {
			return;
		}

		//�v���C���[�����S���邩�A�Q�[���N���A���炵�΂炭�o���Ă�����Q�[�����I��
		if (myUnit->getExist() == false || gameClearCount >= 200) {
			isOver = true;
			return;
		}

		if (wave->clearFlag == true) {
			++gameClearCount;
			myUnit->hp = myUnit->hpMax;
			score->addScore(gameLevel * 2);
		}


		collision();
		bullets->update();
		wave->update();
		enemys->addEnemyDebug(this);

		myUnit->update();
		enemys->update();

		//	bullets->update();

		enemys->checkExist(this);



	};

	void collision() {

		enemys->checkBulletHit();
		myUnit->checkBulletHit();
		myUnit->checkEnemyTouch();

	};


	void draw() {
		//ClearPrint();
		//Println(L"bullet:",bullets->bullets.size());
		//Println(L"bullet:", bullets->bullets.capacity());


		if (isPause) {
			//�}�E�X���W�̐������Ȃ����B
			ClipCursor(NULL);
			s3d::RoundRect(Rect(500, 300).setCenter(Window::Center()), 20).draw(ColorF(0.2, 0.6, 0.8, 0.8));
			FontAsset(L"result")(L"PAUSE").drawCenter(Window::Center() - Point(0, 60));
			FontAsset(L"result")(L"SPACE�L�[�ŉ���").drawCenter(Window::Center());
			FontAsset(L"result")(L"ESC�L�[�Ń^�C�g�����").drawCenter(Window::Center() + Point(0, 60));
			//FontAsset(L"result")(L"����",pauseCount,L"��|�[�Y�ł��܂�").drawCenter(Window::Center() + Point(0, 90));
			if (Input::KeyEscape.clicked) {
				isOver = true;

			}
		}
		else {
			backGround();
			wave->tokenDraw();
			effect->update();
			myUnit->draw();
			enemys->draw();
			bullets->draw();
			score->draw();
			gauge();
			wave->draw();

			//�}�E�X���W���E�B���h�E���Ɏ��߂�ׂ̑㕨
			//siv3d�̋@�\����Ȃ�����ˁB���肨�����ˁB
			// �Q�[���̃E�B���h�E�̃n���h�����^�C�g��������肷��B
			hwnd = FindWindow(NULL, ConstClass::Title.c_str());
			GetWindowRect(FindWindow(NULL, ConstClass::Title.c_str()), &r);
			GetClientRect(hwnd, &r);
			ClientToScreen(hwnd, (LPPOINT)&r.left);
			ClientToScreen(hwnd, (LPPOINT)&r.right);
			ClipCursor(&r);
		}

		//�Q�[���I�[�o�[�E�Q�[���N���A�Ń��S��\��
		if (gameClearCount > 0) {
			TextureAsset(L"clear").drawAt(Window::Center(), Alpha(Min(255, gameClearCount * 4)));
		}

		/*else if (gameOverCount > 0) {
			TextureAsset(L"gameover").drawAt(Window::Center(), Alpha(Min(255,gameOverCount * 4)));
		}*/

		WriteCursor();
		FontAsset(L"font")(Profiler::FPS(), L"fps").draw(750, 580);

	};


	void backGround() {

		for (int i = 0; i < 20; i++) {
			int lineX = ConstClass::ScreenX / 20 * i + Time::GetMillisec() * 0.05;
			if (lineX > ConstClass::ScreenX) {
				lineX = lineX % ConstClass::ScreenX;
			}

			Line(lineX, 0.0, lineX, ConstClass::ScreenY).draw(2, HSV(90, 1, 1));
		}
		for (int j = 0; j < 15; j++) {
			int lineY = ConstClass::ScreenY / 15 * j + Time::GetMillisec() * 0.05;
			if (lineY > ConstClass::ScreenY) {
				lineY = lineY % ConstClass::ScreenY;
			}
			//Line(0.0, lineY, ConstClass::ScreenX, lineY).draw(HSV(90,1, abs(sin(Time::GetMillisec() * 0.000314 + lineY*0.01) )));
			Line(0.0, lineY, ConstClass::ScreenX, lineY).draw(2, HSV(90, 1, 1));
		}

		color = 255 * abs(sin(Time::GetMillisec() * Pi / 10000));
		color2 = 255 * abs(sin(Time::GetMillisec() * Pi / 10000 + 1));
		color3 = 255 * abs(sin(Time::GetMillisec() * Pi / 10000 + 2));
		//Rect(0, 0,800,600).draw({ Color(Color(11,22,33),color), Color(Color(11,22,33),color2),Color(Color(11,22,33),color2), Color(Color(11,22,33),color3)});
		Rect(0, 0, 800, 600).draw({ Color(ConstClass::backColor,color), Color(ConstClass::backColor,color2),Color(ConstClass::backColor,color2), Color(ConstClass::backColor,color3) });
		//	Line(0.0, myUnit->getY(), ConstClass::ScreenX, myUnit->getY()).draw(2, { HSV(0, 1, 1),150 });
		//	Line(myUnit->getX(), 0.0, myUnit->getX(), ConstClass::ScreenY).draw(2, { HSV(0, 1, 1),150 });
	};

	void gauge() {
		//	Line(650, 0, 650, 600).draw(3);

		Line(0, 50, 800, 50).draw(3);
		//	Rect(0, 0, 800, 50).draw();


			//HP�Q�[�W�̕`��
		Rect(5, 5, 300, 20).drawFrame(0, 2, Palette::Darkred);
		Rect(5, 5, 300 * (myUnit->hp) / (myUnit->hpMax), 20).draw({ HSV(180,1,1), HSV(180 + 60 * (myUnit->hp) / (myUnit->hpMax),1,1), HSV(180 + 60 * (myUnit->hp) / (myUnit->hpMax),1,1), HSV(180,1,1) });

		//Energy�Q�[�W�̕`��
		Rect(5, 27, 250, 15).drawFrame(0, 2, Palette::Darkred);
		Rect(5, 27, 250 * (myUnit->energy) / (myUnit->energyMax), 15).draw({ HSV(120,1,1),HSV(120 - 60 * (myUnit->energy) / (myUnit->energyMax)),HSV(120 - 60 * (myUnit->energy) / (myUnit->energyMax)),HSV(120,1,1) });
		//�����E�N���b�N����Energy��30%�ȉ��Ȃ�x������B
		if (Input::MouseR.pressed && (double)(myUnit->energy) / (double)ConstClass::Energy < 0.3) {
			if (!SoundAsset(L"alert").isPlaying()) {
				SoundAsset(L"alert").play();
			}

		}
		else {
			if (SoundAsset(L"alert").isPlaying()) {
				SoundAsset(L"alert").stop();
			}
		}


	}

	void WriteCursor() {


		if (Input::MouseR.pressed) { //�}�E�X�̍��{�^����������Ă�����

			TextureAsset(L"mousePurple").scale(0.3).drawAt(Mouse::Pos());
			
		}
		else if (Input::MouseL.pressed) {//�}�E�X�̉E�{�^����������Ă�����

			TextureAsset(L"mouseYellow").scale(0.3).drawAt(Mouse::Pos());
		}
		else {//�ǂ����������Ă��Ȃ��Ȃ��
			TextureAsset(L"mouseYellow").scale(0.4).drawAt(Mouse::Pos());
		}
		//�{�^���Ɋւ�炸�`�ʁB

		TextureAsset(L"mouseCercle").scale(0.6).rotate(Pi / 2000 * Time::GetMillisec()).drawAt(Mouse::Pos());

	}

};