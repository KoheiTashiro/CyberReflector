#include "interface.h"


	Ef02::Ef02(const Vec2& pos) {
		/*effectPos.resize(8);    // 配列サイズを4個にする

		for (auto& i : effectPos) { // 範囲ループで代入する
			i = pos;
		}
		*/
		effectPos = pos;
		attackEffect = std::make_unique<Anime>(TextureAsset(L"attackEffect2"), 21,1);
	}
	
	// 更新関数 引数tは時間(秒数)
	bool Ef02::update(double t)  {
		Graphics2D::SetBlendState(BlendState::Additive);

		attackEffect->draw(effectPos, 0.7, 0);
		attackEffect->update();
		

		Graphics2D::SetBlendState(BlendState::Default);

		/*
		effectPos[0].x += 2;        // 座標を移動させる
		effectPos[1].y += 2;
		effectPos[2].x -= 2;
		effectPos[3].y -= 2;

		effectPos[4].x += 1.4;        // 座標を移動させる
		effectPos[4].y += 1.4;
		effectPos[5].x -= 1.4;
		effectPos[5].y -= 1.4;

		effectPos[6].x += 1.4;        // 座標を移動させる
		effectPos[6].y -= 1.4;
		effectPos[7].x -= 1.4;
		effectPos[7].y += 1.4;


		for (auto& i : effectPos) {
			Rect(i.x - 4, i.y - 4, 8, 8).draw(ColorF(Palette::Red, 1.0 - t * 2)); // 四角を描画
		}
		*/
		return attackEffect->m_index != attackEffect->m_size-1; // ここがfalseになるか、10秒たつと消滅
	}

	Fire::Fire()
	{
	}

	bool Fire::update(double t)
	{
		return false;
	}

	float randomFire(float r)
	{
		
			return r * (Random(0, 20000) - 10000) / 10000.0f;
		
	}

	Efplayer::Efplayer(const Vec2 & pos)
	{
		effectPos.resize(8);    // 配列サイズを4個にする

		for (auto& i : effectPos) { // 範囲ループで代入する
			i = pos;
		}
	}

	bool Efplayer::update(double t)
	{
		movePos();

		for (auto& i : effectPos) {
			//Rect(i.x - 4, i.y - 4, 8, 8).draw(ColorF(Palette::White, 1.0 - t * 2)); // 四角を描画
			Circle(i.x , i.y, 3).draw(ColorF(HSV(40-t*80,1,1), 1.0 - t * 2));
		}
		return t < 0.5; // ここがfalseになるか、10秒たつと消滅
	}

	void Efplayer::movePos() {
		// 座標を移動させる
		effectPos[0].x += 1;
		effectPos[1].y += 1;
		effectPos[2].x -= 1;
		effectPos[3].y -= 1;

		effectPos[4].x += 0.7;
		effectPos[4].y += 0.7;
		effectPos[5].x -= 0.7;
		effectPos[5].y -= 0.7;

		effectPos[6].x += 0.7;
		effectPos[6].y -= 0.7;
		effectPos[7].x -= 0.7;
		effectPos[7].y += 0.7;
	}
