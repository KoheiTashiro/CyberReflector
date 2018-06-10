#pragma once
#include "interface.h"

///////////////////////////////////////////////////////////////////////
//
//  実績管理用クラス/シングルトン
//
class achievements {
private:
	struct tolophy {
		std::array<bool, ConstClass::AchieveNumber> achieve = {};
		int32 beatEnemy = 0;

		// このようなメンバ関数を定義する
		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(achieve,beatEnemy);
		}

	}tolo;

	achievements() {
		//セーブ内容を読み込む。

		if (FileSystem::Exists(ConstClass::AchieveFilePath))
		{
			load();
		}
		else
		{
			//セーブデータがなければ作る
			save();
		}
	}

	achievements(const achievements& r) {}
	achievements& operator=(const achievements& r) {}

public:

	static achievements& get() {
		static achievements inst;
		return inst;
	}

	bool operator[](int n) {
		assert(n < ConstClass::AchieveNumber);
		return tolo.achieve[n];
	}

	void writeAchieve(int n) {
		assert(n <= ConstClass::AchieveNumber);
		if (tolo.achieve[n - 1] == false) {
			tolo.achieve[n - 1] = true;
			save();
		}
	}

	double clearRate() {
		//count->コンテナの特定の要素の数を調べる、条件式を指定するならcount_ifだ
		return  (double)std::count(tolo.achieve.begin(), tolo.achieve.end(), true) / tolo.achieve.size() * 100.0;
	}

	
	void beatEnemy() {
		tolo.beatEnemy++;
	}

	void checkEnemy() {

		if (tolo.beatEnemy >= 100) {
			//No18. 敵を100体倒した。　武器がなくとも戦える
			writeAchieve(18);
		}
		if (tolo.beatEnemy >= 200) {
			//No.19 敵を200体倒した。　キルマシーン
			writeAchieve(19);
		}
		if (tolo.beatEnemy >= 300) {
			//No.19 敵を300体倒した。　美しく残酷な死を
			writeAchieve(20);
		}
	}

	int getEnemy() {
		return tolo.beatEnemy;
	}

	void save() {
		Serializer<BinaryWriter>{ConstClass::AchieveFilePath}(tolo);
	}

	void load() {
		Deserializer<BinaryReader>{ConstClass::AchieveFilePath}(tolo);
	}

};