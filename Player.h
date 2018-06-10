#pragma once
#include "interface.h"

class gameManager;

class player:public object {
public:
	
	gameManager* game;

	//現在の加速度をベクトルで管理。
	Vec2 acceleration;
	
	//現在の速度をベクトルで管理。
	Vec2 speed;

	//シールドが発動しているか？
	bool shield;

	//ステータス
	//シールドの角度
	double shieldArc;

	//抵抗係数
	double resistance;

	//ヒットポイント
	double hp;

	//ヒットポイントの最大値
	//コンストラクタ以外では変更不能にしたい
	//topic:constにしてメンバイニシャライザで初期化、()内に三項演算子とか
	//あと同()内で関数呼び出してもいいか、見難そう
	double hpMax;

	//シールド発動のためのエネルギー
	int energy;

	//エネルギー最大値
	int energyMax;

	//エネルギー回復量
	int energyHeal;

	//プレイヤーの画像の名前
	String textureName;

	//ステータス関連ここまで

	//たまと自機との角度
	double iangle;

	//炎の大きさ
	double fireSize;

	//ショットを食らってからの秒数
	int shotedTime;

	//炎表示するためのクラス
	std::unique_ptr<Anime> fireAnime;

public:
	player(double startX,double startY,gameManager* setGame,int machineSelect);

	void update();

	void draw();

	void checkBulletHit();

	void checkEnemyTouch();

	void checkShotedTime();

};