#pragma once
#include "interface.h"
class object{
protected:
	double x;
	double y;
	double angle;

	double movev;

	//�����蔻��̑傫��
	double hitArea;
	
	//�G�����̎���
	bool enemyFlag;

	//���݂��Ă��邩�H���Ă��Ȃ����H
	bool exist;

	//�O���t�B�b�N���w��
	Texture graphic;

public:

	object();

	//virtual�ȃf�X�g���N�^
    //�p����ɑ��݂���f�X�g���N�^�����s����
	//���N���X�̃f�X�g���N�^��virtual�Ȃ�h���N���X�Ƀf�X�g���N�^���L�q���Ȃ��Ă�
	//�Öق�virtual�ƂȂ�B
	virtual ~object();

    //���S���z�֐�
	//�p����̃N���X�ŕK���A�Ē�`���܂��B
	virtual void update() = 0;
	//���z�֐��͌p����ōĒ�`�\�ȕϐ������A=0�Ƃ��邱�ƂŌp�����Œ�`���Ȃ��Ă����ƂȂ�B

	//const�����o�֐�
	//���̊֐��̓����o�ϐ���ύX���邱�Ƃ��o���Ȃ��B
	double getX() const;
	double getY() const;
	
	double getAngle() const;
	
	double getHitArea() const;
	
	bool getEnemyFlag() const;

	bool getExist() const;

	virtual void draw();








};