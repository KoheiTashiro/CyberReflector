#pragma once
#include "interface.h"
class object{
protected:
	double x;
	double y;
	double angle;

	double movev;

	//当たり判定の大きさ
	double hitArea;
	
	//敵味方の識別
	bool enemyFlag;

	//存在しているか？していないか？
	bool exist;

	//グラフィックを指定
	Texture graphic;

public:

	object();

	//virtualなデストラクタ
    //継承先に存在するデストラクタを実行せよ
	//基底クラスのデストラクタがvirtualなら派生クラスにデストラクタを記述しなくても
	//暗黙でvirtualとなる。
	virtual ~object();

    //完全仮想関数
	//継承後のクラスで必ず、再定義します。
	virtual void update() = 0;
	//仮想関数は継承先で再定義可能な変数だが、=0とすることで継承元で定義しなくておｋとなる。

	//constメンバ関数
	//この関数はメンバ変数を変更することが出来ない。
	double getX() const;
	double getY() const;
	
	double getAngle() const;
	
	double getHitArea() const;
	
	bool getEnemyFlag() const;

	bool getExist() const;

	virtual void draw();








};