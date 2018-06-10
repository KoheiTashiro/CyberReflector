#pragma once
#include "interface.h"

class IPause;

//-------------------------------------------------------
// PauseManager
//-------------------------------------------------------

//シングルトンで実装
class PauseManager final : Uncopyable
{

	//すごーい！あなたは非公開メンバにアクセスできるフレンズなんだね！
	friend IPause;
private:
	PauseManager() = default;

	//管理するリスト
	std::list<IPause*> m_pauseList;

	void pauseAll();
	void resumeAll();

	//リストへデータの追加
	void add(IPause*const pause);

	//リストからデータの削除
	void erase(const IPause*const pause);


	static PauseManager& Instance();

	static void Add(IPause*const pause);

	static void Erase(const IPause*const pause);

public:

	//すべてポーズ
	static void Pause();

	//すべてリジューム
	//static void Resume();

	//すべてアップデート
	static void update();
};

//-------------------------------------------------------
// IPause
//-------------------------------------------------------

//ポーズさせたいクラスはこれを継承
class IPause
{
public:
	IPause();

	virtual ~IPause();

	virtual void pause() = 0;
	//virtual void resume() = 0;
	virtual void update() = 0;

};

//-------------------------------------------------------


//テンプレートは実装にかけないらしい
//何度も定義されてエラーはかないんかな？
template<class Type>
class Pauseable :
	public IPause,
	public Type
{
public:
	//コンストラクタの引継ぎ
	using Type::Type;

	void pause()override
	{
		this->Type::pause();
	}

	/*void resume()override
	{
		this->Type::resume();
	}*/

	void update()override
	{
		this->Type::resume();
	}
};
