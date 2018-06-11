#pragma once
#include "interface.h"




class waveManager {

	//���ɏo��G�o���ʒu���i�[
	std::vector <Point> token;

	//���ɏo��GId���i�[
	std::vector <int> enemyIdToken;

	//���݂�wave��
	int wave;

	//Lastwave�����������i�[����B
	int lastWave;

	//����wave�܂ł̃C���^�[�o��
	int32 interval;

	//�`���[�g���A���̕\���Ԋu
	int tutorial;

	//�Q�[���}�l�[�W���̃|�C���^
	gameManager* game;

	//csv�t�@�C����ǂݍ���ł���
	CSVReader* csv;

	//csv�t�@�C���̌��ݍs
	int csvRow;

	//�g�[�N�����擾����֐�
	void tokenAdd();

	//�N���A���Ă���o�߂����t���[����
	int clearCount;

public:

	//�N���A�������A���ĂȂ���
	bool clearFlag;

	//wave�̍X�V�Ƃ��A�G�̒ǉ��Ƃ�
	void update();

	//wave����\��
	void draw();

	void tutorialUpdate();

	void tutorialDraw();

	void tokenDraw();

	waveManager(gameManager *game);

	void achieveCheck();


};