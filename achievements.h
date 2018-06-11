#pragma once
#include "interface.h"

///////////////////////////////////////////////////////////////////////
//
//  ���ъǗ��p�N���X/�V���O���g��
//
class achievements {
private:
	struct tolophy {
		std::array<bool, ConstClass::AchieveNumber> achieve = {};
		int32 beatEnemy = 0;

		// ���̂悤�ȃ����o�֐����`����
		template <class Archive>
		void serialize(Archive& archive)
		{
			archive(achieve,beatEnemy);
		}

	}tolo;

	achievements() {
		//�Z�[�u���e��ǂݍ��ށB

		if (FileSystem::Exists(ConstClass::AchieveFilePath))
		{
			load();
		}
		else
		{
			//�Z�[�u�f�[�^���Ȃ���΍��
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
		//count->�R���e�i�̓���̗v�f�̐��𒲂ׂ�A���������w�肷��Ȃ�count_if��
		return  (double)std::count(tolo.achieve.begin(), tolo.achieve.end(), true) / tolo.achieve.size() * 100.0;
	}

	
	void beatEnemy() {
		tolo.beatEnemy++;
	}

	void checkEnemy() {

		if (tolo.beatEnemy >= 100) {
			//No18. �G��100�̓|�����B�@���킪�Ȃ��Ƃ��킦��
			writeAchieve(18);
		}
		if (tolo.beatEnemy >= 200) {
			//No.19 �G��200�̓|�����B�@�L���}�V�[��
			writeAchieve(19);
		}
		if (tolo.beatEnemy >= 300) {
			//No.19 �G��300�̓|�����B�@�������c���Ȏ���
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