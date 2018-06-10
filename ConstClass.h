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

	//enum��static���w�肵�Ȃ��Ă��ÓI
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

		//�l�������Ŗ��m�ɂ��Ȃ��ƁA�z��̗v�f���Ƃ��Ďg���Ȃ��I
	static const int AchieveNumber=20;
	
	//���Ȃ݂�int��enum�ȊO�̓N���X��`�ŏ������ł��Ȃ��A�ǂ����悤�I�H
	//�~ static const double test = 5.0;

	//�R���p�C�����萔�ɂ���΂�����I������ˁI
	//�� static constexpr double test = 5.0;

	//�t�ɃN���X��`�ŏ��������Ȃ��ƃ_���Ȃ�ł���ˁA�܂��R���p�C�����Ɍ��߂Ȃ��Ƃ����񂩂炾�낤��
	//�~ static constexpr double test;


	//���̂ւ�A�S��constexpr�ɂ��Ă����C�������ł����ǂ��Ȃ�ł���
	//�A�Z���u���R�[�h�ς�����肷���ł�����
	//�����A�ł��I�N���X��`����������ƃR���p�C���₽�瑖��炵����
	//�����͕����ď�����const�ɂ�������������H

};