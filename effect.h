#pragma once
#include "interface.h"

float randomFire(float r);

struct Ef02 : IEffect {
	// 使用する変数をかく
	//Array<Vec2> effectPos; // 配列(個数変動することを想定して可変長配列)
	
	Vec2 effectPos;

	std::unique_ptr<Anime> attackEffect;
							// コンストラクター
	Ef02(const Vec2& pos);

	// 更新関数 引数tは時間(秒数)
	bool update(double t) override;


};

struct Efplayer : IEffect {
	// 使用する変数をかく
	Array<Vec2> effectPos; // 配列(個数変動することを想定して可変長配列)

						   // コンストラクター
	Efplayer(const Vec2& pos);

	// 更新関数 引数tは時間(秒数)
	bool update(double t) override;

	// 座標を移動させる
	void movePos();


};

struct Fire : IEffect {
	
	Fire();

	bool update(double t) override;

};

struct TextEffect : IEffect{
	//表示する数値を代入
	int m_value;


	//表示する位置を代入
	const Vec2 m_position;

	TextEffect(int value, const Vec2& position)
		:m_value(value)
		, m_position(position) 
		
	{}

	bool update(double t) override
	{
		// 1.0 秒で消える
		if (t >= 1.0 )
		{
			return false;
		}

		if (t <= 0.5) {
			FontAsset(L"font").regionCenter(Format(999), m_position + Vec2(0, -30)).draw(Palette::Black);
			FontAsset(L"font").drawCenter(Format(m_value), m_position + Vec2(0, -30), HSV(60 - m_value));
		}
		else {
			//0.5秒以上経過で暗くなる。
			FontAsset(L"font").regionCenter(Format(999), m_position + Vec2(0, -30)).draw(Palette::Black);
			FontAsset(L"font").drawCenter(Format(m_value), m_position + Vec2(0, -30), HSV(60 - m_value).toColorF(1-2*(t-0.5)));
		}
		return true;
	}
	
};
;

