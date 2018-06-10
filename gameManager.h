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
		//waveManagerでどのLvのcsvを読み込むかはgameLevelに依存するので先に読み込む。
		wave = std::make_unique<waveManager>(this);
		score = std::make_unique<scoreManager>(gameLevel);


	};
	
	//コンストラクタがprivateなのでGet関数以外からは生成不可
	//gameManager(const gameManager& r) = delete;//コピーコンストラクタの削除(コピー禁止)
	//gameManager& operator=(const gameManager& r) = delete;//代入演算子の削除(代入禁止)

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
	//ゲームのレベルを指定する変数、waveManagerに渡す
	int gameLevel;
	//機体の種類を指定する変数、playerに渡す
	int machineSelect;

	//処理時間確認用
	MillisecClock c;
	//ウィンドウの座標・大きさを示す。Siv3dのRectじゃないよ。windows.hのRECTだよ。
	RECT  r;
	//ウィンドウのハンドル
	HWND hwnd;

	//ポーズしているかどうか。
	bool isPause;

	//ゲームが終了したかどうか。
	bool isOver;

	//ゲームクリアしてからの秒数
	int gameClearCount;


	//背景用のカラー確保変数
	int color;
	int color2;
	int color3;

	/*
	シングルトン使わず
	ゲームやるたびinit関数で初期化するのはめんどいし...
	gameManager自体は普通に作って、gameManagerの生成を管理するシングルトンを他に作る手もある？
	static gameManager& get() {

		static gameManager inst;//static変数なので初めてGet関数が呼ばれたとき以外は実体が残る。
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

		//プレイヤーが死亡するか、ゲームクリアからしばらく経っていたらゲームを終了
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
			//マウス座標の制限をなくす。
			ClipCursor(NULL);
			s3d::RoundRect(Rect(500, 300).setCenter(Window::Center()), 20).draw(ColorF(0.2, 0.6, 0.8, 0.8));
			FontAsset(L"result")(L"PAUSE").drawCenter(Window::Center() - Point(0, 60));
			FontAsset(L"result")(L"SPACEキーで解除").drawCenter(Window::Center());
			FontAsset(L"result")(L"ESCキーでタイトル画面").drawCenter(Window::Center() + Point(0, 60));
			//FontAsset(L"result")(L"あと",pauseCount,L"回ポーズできます").drawCenter(Window::Center() + Point(0, 90));
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

			//マウス座標をウィンドウ内に収める為の代物
			//siv3dの機能じゃないからね。ごりおしだね。
			// ゲームのウィンドウのハンドルをタイトルから特定する。
			hwnd = FindWindow(NULL, ConstClass::Title.c_str());
			GetWindowRect(FindWindow(NULL, ConstClass::Title.c_str()), &r);
			GetClientRect(hwnd, &r);
			ClientToScreen(hwnd, (LPPOINT)&r.left);
			ClientToScreen(hwnd, (LPPOINT)&r.right);
			ClipCursor(&r);
		}

		//ゲームオーバー・ゲームクリアでロゴを表示
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


			//HPゲージの描写
		Rect(5, 5, 300, 20).drawFrame(0, 2, Palette::Darkred);
		Rect(5, 5, 300 * (myUnit->hp) / (myUnit->hpMax), 20).draw({ HSV(180,1,1), HSV(180 + 60 * (myUnit->hp) / (myUnit->hpMax),1,1), HSV(180 + 60 * (myUnit->hp) / (myUnit->hpMax),1,1), HSV(180,1,1) });

		//Energyゲージの描写
		Rect(5, 27, 250, 15).drawFrame(0, 2, Palette::Darkred);
		Rect(5, 27, 250 * (myUnit->energy) / (myUnit->energyMax), 15).draw({ HSV(120,1,1),HSV(120 - 60 * (myUnit->energy) / (myUnit->energyMax)),HSV(120 - 60 * (myUnit->energy) / (myUnit->energyMax)),HSV(120,1,1) });
		//もし右クリック中でEnergyが30%以下なら警告する。
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


		if (Input::MouseR.pressed) { //マウスの左ボタンが押されていたら

			TextureAsset(L"mousePurple").scale(0.3).drawAt(Mouse::Pos());
			
		}
		else if (Input::MouseL.pressed) {//マウスの右ボタンが押されていたら

			TextureAsset(L"mouseYellow").scale(0.3).drawAt(Mouse::Pos());
		}
		else {//どちらもおされていないならば
			TextureAsset(L"mouseYellow").scale(0.4).drawAt(Mouse::Pos());
		}
		//ボタンに関わらず描写。

		TextureAsset(L"mouseCercle").scale(0.6).rotate(Pi / 2000 * Time::GetMillisec()).drawAt(Mouse::Pos());

	}

};