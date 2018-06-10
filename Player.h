#pragma once
#include "interface.h"

class gameManager;

class player:public object {
public:
	
	gameManager* game;

	//���݂̉����x���x�N�g���ŊǗ��B
	Vec2 acceleration;
	
	//���݂̑��x���x�N�g���ŊǗ��B
	Vec2 speed;

	//�V�[���h���������Ă��邩�H
	bool shield;

	//�X�e�[�^�X
	//�V�[���h�̊p�x
	double shieldArc;

	//��R�W��
	double resistance;

	//�q�b�g�|�C���g
	double hp;

	//�q�b�g�|�C���g�̍ő�l
	//�R���X�g���N�^�ȊO�ł͕ύX�s�\�ɂ�����
	//topic:const�ɂ��ă����o�C�j�V�����C�U�ŏ������A()���ɎO�����Z�q�Ƃ�
	//���Ɠ�()���Ŋ֐��Ăяo���Ă��������A�����
	double hpMax;

	//�V�[���h�����̂��߂̃G�l���M�[
	int energy;

	//�G�l���M�[�ő�l
	int energyMax;

	//�G�l���M�[�񕜗�
	int energyHeal;

	//�v���C���[�̉摜�̖��O
	String textureName;

	//�X�e�[�^�X�֘A�����܂�

	//���܂Ǝ��@�Ƃ̊p�x
	double iangle;

	//���̑傫��
	double fireSize;

	//�V���b�g��H����Ă���̕b��
	int shotedTime;

	//���\�����邽�߂̃N���X
	std::unique_ptr<Anime> fireAnime;

public:
	player(double startX,double startY,gameManager* setGame,int machineSelect);

	void update();

	void draw();

	void checkBulletHit();

	void checkEnemyTouch();

	void checkShotedTime();

};