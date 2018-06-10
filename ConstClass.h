#pragma once
#include "interface.h"
class ConstClass
{
public:
	static const int ScreenX;
	static const int ScreenY;
	static const int GameScreenX;
	static const int GameStartY;

	static const int PlayerSpeed;
	static const int PlayerBackSpeed;
	static const int EnemySpeed;

	static const int EnemyBulletSpeed;
	static const double LazerRotateSpeed;

	static const int PlayerHitPoint;

	static const int Energy;
	static const int HealEnergy;

	static const int waveInterval;

	static const Color backColor;
	static const String Title;
	static const FilePath AchieveFilePath;

	//enumはstaticを指定しなくても静的
	enum Level {
		TUTORIAL = 0,
		NORMAL = 1,
		HARD = 2,
		LUNA = 3,
	};

	enum Machine {
		BLENIM = 0,
		METEOR = 1,
	};

	enum Enemy {
		FOURWAY_FORT = 1,
		CHASER = 2,
		SPINNER = 3,
		SPIN_LAZER = 4,
		STALKER = 5,
	};

		//値をここで明確にしないと、配列の要素数として使えない！
	static const int AchieveNumber=20;
	
	//ちなみにintやenum以外はクラス定義で初期化できない、どうしよう！？
	//× static const double test = 5.0;

	//コンパイル時定数にすればいいよ！やったね！
	//○ static constexpr double test = 5.0;

	//逆にクラス定義で初期化しないとダメなんですよね、まぁコンパイル時に決めないといかんからだろうが
	//× static constexpr double test;


	//このへん、全部constexprにしていい気がするんですがどうなんでしょ
	//アセンブラコード変わったりするんですかね
	//あっ、でも！クラス定義書き換えるとコンパイルやたら走るらしいし
	//実装は分けて書けるconstにも分があったり？

};