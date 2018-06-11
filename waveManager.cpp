#include "interface.h"
#define tutorialPace 660

void waveManager::tokenAdd() {
	for (csvRow = 2; csvRow <= csv->rows; ++csvRow) {
		if (csv->get<int>(csvRow - 1, 6 - 1) == wave + 1) {
			//�o�����ɉ����Ēǉ����邩�����߂�B
			if (RandomBool(csv->get<double>(csvRow - 1, 7 - 1) / 100.0)) {
				//x,y��999�Ȃ烉���_���Ȓl�����i�}�W�b�N�i���o�[�j
				//�y���l��999���z?�y�����_���Ȓl�z:�y���l���̂܂܁z
				token.push_back({ csv->get<int>(csvRow - 1, 4 - 1) == 999 ? Random(30, ConstClass::GameScreenX - 30) : csv->get<int>(csvRow - 1, 4 - 1),
					csv->get<int>(csvRow - 1, 5 - 1) == 999 ? Random(ConstClass::GameStartY + 30, ConstClass::ScreenY - 30) : csv->get<int>(csvRow - 1, 5 - 1) });
				enemyIdToken.push_back(csv->get<int>(csvRow - 1, 3 - 1));
			}
		}
	}
}

void waveManager::update()
{
	//�`���[�g���A���Ȃ�tutorialUpdate�݂̂��s���B
	if (game->gameLevel == 0) {
		tutorialUpdate();
		return;
	}
	//�g�[�N��������΁A�Q�[�W�����炷
	if (!token.empty())	--interval;

	//�N���A�t���O�������Ă��炸�A�c�莞�Ԃ�0�ȉ�or�G�����Ȃ���Wave0�łȂ��Ȃ�
	if ((interval <= 0 || game->enemys->howManyEnemy() == 0 && wave != 0) && clearFlag == false) {


		//�g�[�N�����ЂƂ��Ȃ���΁A�Q�[���N���A
		if (token.empty()) {
			achieveCheck();
			clearFlag = true;

		}
		else {
			//�g�[�N������ł�����Ȃ�B
			++wave;
			game->score->addScore(interval / 5);
			interval = ConstClass::waveInterval;
		
			//	�G��ǉ����Ă͗v�f�𖕎E
			auto it = token.begin();
			while (it != token.end()) {
				game->enemys->addEnemy(enemyIdToken.front(), { token.front() });


				//erase�̕Ԃ�l��it�Ŏ󂯂�i�Ԃ�l�͍폜�������̂̎��ɂȂ����Ă����v�f�ւ̃C�e���[�^�j
				it = token.erase(it);
				enemyIdToken.erase(enemyIdToken.begin());


			}

		}



		tokenAdd();
		//for (int i = 0; i < wave; ++i) {
		//	game->enemys->enemys.push_back(std::make_shared<enemyA>((double)Random(0, ConstClass::GameScreenX), (double)Random(ConstClass::GameStartY, ConstClass::ScreenY), game));
		//}
	}
	//Println(Mouse::Pos());
}

void waveManager::draw()
{
	//Wave�Q�[�W�̕`��

	Rect(500, 5, 290, 40).drawFrame(0, 2, Palette::Black);
	Rect(500, 5, 290 * interval / ConstClass::waveInterval, 40).draw({ HSV(360,1,1), HSV(360 - 60 * interval / ConstClass::waveInterval,1,1),HSV(360 - 60 * interval / ConstClass::waveInterval,1,1), HSV(360,1,1) });
	FontAsset(L"Score")(L"Wave:", wave, L"/", lastWave).drawCenter(645, 25);

	if (game->gameLevel == 0) {
		tutorialDraw();
	}



}

void waveManager::tutorialUpdate()
{
	++tutorial;
	interval = tutorialPace * 4 - tutorial;
	if (interval < 0) interval = ConstClass::waveInterval;
	if (interval > ConstClass::waveInterval) interval = ConstClass::waveInterval;

	if (tutorial == tutorialPace * 4) {
		//	�G��ǉ����Ă͗v�f�𖕎E
		auto it = token.begin();
		while (it != token.end()) {
			game->enemys->addEnemy(enemyIdToken.front(), { token.front() });
			++wave;

			//erase�̕Ԃ�l��it�Ŏ󂯂�i�Ԃ�l�͍폜�������̂̎��ɂȂ����Ă����v�f�ւ̃C�e���[�^�j
			it = token.erase(it);
			enemyIdToken.erase(enemyIdToken.begin());


		}
	}

}

void waveManager::tutorialDraw()
{
	s3d::RoundRect(50, 400, 620, 170, 20).draw({ Palette::Black,150 });
	s3d::RoundRect(50, 400, 620, 170, 20).drawFrame(3);
	if (tutorial < tutorialPace) {
		FontAsset(L"Tutorial")(L"���N���b�N���������ƂŁA�}�E�X�J�[�\��").draw(70, 410);
		FontAsset(L"Tutorial")(L"�̕����Ɉړ����邱�Ƃ��o���܂��B").draw(70, 445);
		FontAsset(L"Tutorial")(L"�ړ��ɂ͊����������̂ŁA�s�������ꏊ��").draw(70, 480);
		FontAsset(L"Tutorial")(L"������O�Ń{�^���𗣂��̂��R�c�ł��B").draw(70, 515);
		if (Input::KeyZ.clicked) tutorial = tutorialPace;
	}
	else if (tutorial < tutorialPace * 2) {
		FontAsset(L"Tutorial")(L"����ɂ�����Q�[�W��HP�ł��B").draw(70, 410);
		FontAsset(L"Tutorial")(L"�G�̒e�ɂ�����ƃQ�[�W������A").draw(70, 445);
		FontAsset(L"Tutorial")(L"0�ɂȂ�ƃQ�[���I�[�o�[�ł��B").draw(70, 480);
		//	FontAsset(L"Tutorial")(L"������O�Ń{�^���𗣂��̂��R�c�ł��B").draw(70, 515);
		if (Input::KeyZ.clicked) tutorial = tutorialPace * 2;
	}
	else if (tutorial < tutorialPace * 3) {
		FontAsset(L"Tutorial")(L"�΂̃Q�[�W�̓G�l���M�[�̎c�ʂ�\���܂�").draw(70, 410);
		FontAsset(L"Tutorial")(L"�E�N���b�N�������ƃG�l���M�[������A").draw(70, 445);
		FontAsset(L"Tutorial")(L"���@�O���Ƀo���A��\��܂��B").draw(70, 480);
		FontAsset(L"Tutorial")(L"�G�̒e�𔽎˂��A�_���[�W��^�����܂�").draw(70, 515);
		if (Input::KeyZ.clicked) tutorial = tutorialPace * 3;
	}
	else if (tutorial < tutorialPace * 4) {
		FontAsset(L"Tutorial")(L"�E��̐Ԃ��Q�[�W�͎��̓G������܂ł�").draw(70, 410);
		FontAsset(L"Tutorial")(L"�c�莞�Ԃł��B���ꂪ�����Ȃ�ƁA").draw(70, 445);
		FontAsset(L"Tutorial")(L"��ʏ�!�Ƃ����ꂽ�ꏊ�ɓG������܂�").draw(70, 480);
		FontAsset(L"Tutorial")(L"����̉񐔂����G�W�c��|���΃N���A�ł�").draw(70, 515);
		if (Input::KeyZ.clicked) tutorial = tutorialPace * 4 - 1;
	}
	else if (game->enemys->howManyEnemy() != 0) {
		tutorial = tutorialPace * 4;
		FontAsset(L"Tutorial")(L"����ł͓G��|���Ă݂܂��傤").draw(70, 410);
		FontAsset(L"Tutorial")(L"���N���b�N�ňړ�").draw(70, 445);
		FontAsset(L"Tutorial")(L"�E�N���b�N�Ńo���A�ł�").draw(70, 480);

	}
	else if (tutorial <tutorialPace * 5) {

		FontAsset(L"Tutorial")(L"�X�y�[�X�L�[�������ƃ|�[�Y��ʂɂȂ�").draw(70, 410);
		FontAsset(L"Tutorial")(L"�J�[�\�����b�N����������܂�").draw(70, 445);
		FontAsset(L"Tutorial")(L"�ȏ�Ń`���[�g���A���͏I���ł�").draw(70, 480);
		FontAsset(L"Tutorial")(L"�����l�ł����B").draw(70, 515);
		if (Input::KeyZ.clicked) tutorial = tutorialPace * 5;

	}
	else {
		achieveCheck();
		clearFlag = true;
	}

	//Rect(60, 580, 250, 15).drawFrame(0, 2, Palette::Darkred);
	Rect(60, 575, 250 * (tutorialPace - (tutorial%tutorialPace)) / tutorialPace, 20).draw({ HSV(180,1,1),HSV(180 - 60 * (tutorialPace - tutorial) % tutorialPace / tutorialPace),HSV(180 - 60 * (tutorialPace - tutorial) % tutorialPace / tutorialPace),HSV(180,1,1) });
	FontAsset(L"Next")(L"Z�L�[�Ŏ��ɐi�݂܂�").draw(450, 570);

}

void waveManager::tokenDraw() {
	for (auto& iter : token) {
		Circle(iter, 8).drawArc(0, 2 * Pi*interval / ConstClass::waveInterval, 0, 5, { Palette::Red,150 });
		Circle(iter, 8).drawFrame(0, 2, Palette::Pink);
		FontAsset(L"font")(L"!").drawAt(iter, Palette::Pink);
	}
}

waveManager::waveManager(gameManager *gamePointer) {
	//�R���X�g���N�^�ŃQ�[���}�l�[�W���̃|�C���^�������Ƃ���
	game = gamePointer;
	clearFlag = false;

	clearCount = 0;

	interval = ConstClass::waveInterval / 2;
	wave = 0;
	lastWave = 0;
	//�`���[�g���A���p�̃C���^�[�o���ϐ�
	tutorial = 0;
	//�G��CSV�œǂݍ��񂾂�炶�΂񂾂�
	FileArchive archive(L"Sozai.s3a");

	switch (game->gameLevel) {
	case 0://tutorial
		csv = new CSVReader(archive.load(L"Sozai/tutorial.csv"));
		break;
	case 1://normal
		csv = new CSVReader(archive.load(L"Sozai/normal.csv"));
		break;
	case 2://hard
		csv = new CSVReader(archive.load(L"Sozai/hard.csv"));
		break;
	case 3://lunatic
		csv = new CSVReader(archive.load(L"Sozai/lunatic.csv"));
		//	csv = new CSVReader(L"tester.csv");
		break;
	}

	for (csvRow = 2; csvRow <= csv->rows; ++csvRow) {
		if (csv->get<int>(csvRow - 1, 6 - 1) > lastWave) {
			lastWave = csv->get<int>(csvRow - 1, 6 - 1);
		}
	}

	tokenAdd();

	//	Println(L"wave: ", csv->get<int>(csvRow - 1, 6 - 1));
}

//clearFlag��true�ɂȂ钼�O�A��񂾂����s����
//�̗͂Ȃǂ��␳�����O�Ɏ��s����
void waveManager::achieveCheck()
{
	if (clearFlag == true) {
		return;
	}

	switch (game->gameLevel) {
	case ConstClass::Level::TUTORIAL:
		//No.1 �͂��߂̈�� �`���[�g���A�����N���A
		achievements::get().writeAchieve(1);
	break;

	case ConstClass::Level::NORMAL:
		switch (game->machineSelect) {
		case ConstClass::Machine::BLENIM:
			//No.2 �����̏� Blenim�m�[�}���N���A
			achievements::get().writeAchieve(2);
			break;
		case ConstClass::Machine::METEOR:
			//No8.Meteor�Ńm�[�}�����N���A�@�����̎g����
			achievements::get().writeAchieve(8);
			break;
		}
	break;

	case ConstClass::Level::HARD:
		switch (game->machineSelect) {
		case ConstClass::Machine::BLENIM:
			//No.3 �i�C�X�V���[�^�[ Blenim�n�[�h�N���A
			achievements::get().writeAchieve(3);
			break;
		case ConstClass::Machine::METEOR:
			//No9.Meteor�Ńn�[�h���N���A�@�����̋M���q
			achievements::get().writeAchieve(9);
			break;
		}
	break;

	case ConstClass::Level::LUNA:
		switch (game->machineSelect) {
		case ConstClass::Machine::BLENIM:
			//No.4 �ꋫ�����z�� Blenim���i�e�B�b�N�N���A
			achievements::get().writeAchieve(4);
			break;
		case ConstClass::Machine::METEOR:
			//No10.Meteor�Ń��i�e�B�b�N���N���A�@�t���𒵂˕Ԃ���
			achievements::get().writeAchieve(10);
			break;
		}
	break;
	}

	//hp��50%�ȏ�c���Ă����Ȃ�
	if (game->myUnit->hpMax < game->myUnit->hp * 2) {
		switch (game->gameLevel) {
		case ConstClass::Level::NORMAL:
			switch (game->machineSelect) {
			case ConstClass::Machine::BLENIM:
				//No5.Blenim�Ńm�[�}���ϋv50%�ȏ�c���ăN���A�@�]�T�^�^
				achievements::get().writeAchieve(5);
				break;
			case ConstClass::Machine::METEOR:
				//No11.Meteor�Ńm�[�}���ϋv50%�ȏ�c���ăN���A�@�h��͍ő�̍U���@
				achievements::get().writeAchieve(11);
				break;
			}
			break;

		case ConstClass::Level::HARD:
			switch (game->machineSelect) {
			case ConstClass::Machine::BLENIM:
				//No6.Blenim�Ńn�[�h�ϋv50%�ȏ�c���ăN���A�@�n���̘r�O
				achievements::get().writeAchieve(6);
				break;
			case ConstClass::Machine::METEOR:
				//No12.Meteor�Ńn�[�h�ϋv50%�ȏ�c���ăN���A�@�s�k��m�炸
				achievements::get().writeAchieve(12);
				break;
			}
			break;

		case ConstClass::Level::LUNA:
			switch (game->machineSelect) {
			case ConstClass::Machine::BLENIM:
				//No7.Blenim���i�e�B�b�N�őϋv50%�ȏ�c���ăN���A�@���ǂ̎���
				achievements::get().writeAchieve(7);
				break;
			case ConstClass::Machine::METEOR:
				//No13.Meteor���i�e�B�b�N�őϋv50%�ȏ�c���ăN���A�@������`��
				achievements::get().writeAchieve(13);
				break;
			}
			break;
		}
	}

}
