#pragma once
#include "interface.h"




class waveManager {

	//次に出る敵出現位置を格納
	std::vector <Point> token;

	//次に出る敵Idを格納
	std::vector <int> enemyIdToken;

	//現在のwave数
	int wave;

	//Lastwaveがいくつかを格納する。
	int lastWave;

	//次のwaveまでのインターバル
	int32 interval;

	//チュートリアルの表示間隔
	int tutorial;

	//ゲームマネージャのポインタ
	gameManager* game;

	//csvファイルを読み込んでおく
	CSVReader* csv;

	//csvファイルの現在行
	int csvRow;

	//トークンを取得する関数
	void tokenAdd();

	//クリアしてから経過したフレーム数
	int clearCount;

public:

	//クリアしたか、してないか
	bool clearFlag;

	//waveの更新とか、敵の追加とか
	void update();

	//wave数を表示
	void draw();

	void tutorialUpdate();

	void tutorialDraw();

	void tokenDraw();

	waveManager(gameManager *game);

	void achieveCheck();


};