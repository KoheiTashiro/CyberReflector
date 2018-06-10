#include "interface.h"
#define tutorialPace 660

void waveManager::tokenAdd() {
	for (csvRow = 2; csvRow <= csv->rows; ++csvRow) {
		if (csv->get<int>(csvRow - 1, 6 - 1) == wave + 1) {
			//出現率に応じて追加するかを決める。
			if (RandomBool(csv->get<double>(csvRow - 1, 7 - 1) / 100.0)) {
				//x,yが999ならランダムな値を代入（マジックナンバー）
				//【数値が999か】?【ランダムな値】:【数値そのまま】
				token.push_back({ csv->get<int>(csvRow - 1, 4 - 1) == 999 ? Random(30, ConstClass::GameScreenX - 30) : csv->get<int>(csvRow - 1, 4 - 1),
					csv->get<int>(csvRow - 1, 5 - 1) == 999 ? Random(ConstClass::GameStartY + 30, ConstClass::ScreenY - 30) : csv->get<int>(csvRow - 1, 5 - 1) });
				enemyIdToken.push_back(csv->get<int>(csvRow - 1, 3 - 1));
			}
		}
	}
}

void waveManager::update()
{
	//チュートリアルならtutorialUpdateのみを行う。
	if (game->gameLevel == 0) {
		tutorialUpdate();
		return;
	}
	//トークンがあれば、ゲージを減らす
	if (!token.empty())	--interval;

	//クリアフラグが立っておらず、残り時間が0以下or敵がいなくてWave0でないなら
	if ((interval <= 0 || game->enemys->howManyEnemy() == 0 && wave != 0) && clearFlag == false) {


		//トークンがひとつもなければ、ゲームクリア
		if (token.empty()) {
			achieveCheck();
			clearFlag = true;

		}
		else {
			//トークンが一つでもあるなら。
			++wave;
			game->score->addScore(interval / 5);
			interval = ConstClass::waveInterval;
		
			//	敵を追加しては要素を抹殺
			auto it = token.begin();
			while (it != token.end()) {
				game->enemys->addEnemy(enemyIdToken.front(), { token.front() });


				//eraseの返り値をitで受ける（返り値は削除したものの次につながっていた要素へのイテレータ）
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
	//Waveゲージの描写

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
		//	敵を追加しては要素を抹殺
		auto it = token.begin();
		while (it != token.end()) {
			game->enemys->addEnemy(enemyIdToken.front(), { token.front() });
			++wave;

			//eraseの返り値をitで受ける（返り値は削除したものの次につながっていた要素へのイテレータ）
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
		FontAsset(L"Tutorial")(L"左クリックを押すことで、マウスカーソル").draw(70, 410);
		FontAsset(L"Tutorial")(L"の方向に移動することが出来ます。").draw(70, 445);
		FontAsset(L"Tutorial")(L"移動には慣性が働くので、行きたい場所の").draw(70, 480);
		FontAsset(L"Tutorial")(L"少し手前でボタンを離すのがコツです。").draw(70, 515);
		if (Input::KeyZ.clicked) tutorial = tutorialPace;
	}
	else if (tutorial < tutorialPace * 2) {
		FontAsset(L"Tutorial")(L"左上にある青いゲージはHPです。").draw(70, 410);
		FontAsset(L"Tutorial")(L"敵の弾にあたるとゲージが減り、").draw(70, 445);
		FontAsset(L"Tutorial")(L"0になるとゲームオーバーです。").draw(70, 480);
		//	FontAsset(L"Tutorial")(L"少し手前でボタンを離すのがコツです。").draw(70, 515);
		if (Input::KeyZ.clicked) tutorial = tutorialPace * 2;
	}
	else if (tutorial < tutorialPace * 3) {
		FontAsset(L"Tutorial")(L"緑のゲージはエネルギーの残量を表します").draw(70, 410);
		FontAsset(L"Tutorial")(L"右クリックを押すとエネルギーを消費し、").draw(70, 445);
		FontAsset(L"Tutorial")(L"自機前方にバリアを貼ります。").draw(70, 480);
		FontAsset(L"Tutorial")(L"敵の弾を反射し、ダメージを与えられます").draw(70, 515);
		if (Input::KeyZ.clicked) tutorial = tutorialPace * 3;
	}
	else if (tutorial < tutorialPace * 4) {
		FontAsset(L"Tutorial")(L"右上の赤いゲージは次の敵がくるまでの").draw(70, 410);
		FontAsset(L"Tutorial")(L"残り時間です。これが無くなると、").draw(70, 445);
		FontAsset(L"Tutorial")(L"画面上!とかかれた場所に敵が現れます").draw(70, 480);
		FontAsset(L"Tutorial")(L"既定の回数だけ敵集団を倒せばクリアです").draw(70, 515);
		if (Input::KeyZ.clicked) tutorial = tutorialPace * 4 - 1;
	}
	else if (game->enemys->howManyEnemy() != 0) {
		tutorial = tutorialPace * 4;
		FontAsset(L"Tutorial")(L"それでは敵を倒してみましょう").draw(70, 410);
		FontAsset(L"Tutorial")(L"左クリックで移動").draw(70, 445);
		FontAsset(L"Tutorial")(L"右クリックでバリアです").draw(70, 480);

	}
	else if (tutorial <tutorialPace * 5) {

		FontAsset(L"Tutorial")(L"スペースキーを押すとポーズ画面になり").draw(70, 410);
		FontAsset(L"Tutorial")(L"カーソルロックが解除されます").draw(70, 445);
		FontAsset(L"Tutorial")(L"以上でチュートリアルは終了です").draw(70, 480);
		FontAsset(L"Tutorial")(L"お疲れ様でした。").draw(70, 515);
		if (Input::KeyZ.clicked) tutorial = tutorialPace * 5;

	}
	else {
		achieveCheck();
		clearFlag = true;
	}

	//Rect(60, 580, 250, 15).drawFrame(0, 2, Palette::Darkred);
	Rect(60, 575, 250 * (tutorialPace - (tutorial%tutorialPace)) / tutorialPace, 20).draw({ HSV(180,1,1),HSV(180 - 60 * (tutorialPace - tutorial) % tutorialPace / tutorialPace),HSV(180 - 60 * (tutorialPace - tutorial) % tutorialPace / tutorialPace),HSV(180,1,1) });
	FontAsset(L"Next")(L"Zキーで次に進みます").draw(450, 570);

}

void waveManager::tokenDraw() {
	for (auto& iter : token) {
		Circle(iter, 8).drawArc(0, 2 * Pi*interval / ConstClass::waveInterval, 0, 5, { Palette::Red,150 });
		Circle(iter, 8).drawFrame(0, 2, Palette::Pink);
		FontAsset(L"font")(L"!").drawAt(iter, Palette::Pink);
	}
}

waveManager::waveManager(gameManager *gamePointer) {
	//コンストラクタでゲームマネージャのポインタをげっとだぜ
	game = gamePointer;
	clearFlag = false;

	clearCount = 0;

	interval = ConstClass::waveInterval / 2;
	wave = 0;
	lastWave = 0;
	//チュートリアル用のインターバル変数
	tutorial = 0;
	//敵をCSVで読み込んだりらじばんだり
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

//clearFlagがtrueになる直前、一回だけ実行する
//体力などが補正される前に実行する
void waveManager::achieveCheck()
{
	if (clearFlag == true) {
		return;
	}

	switch (game->gameLevel) {
	case ConstClass::Level::TUTORIAL:
		//No.1 はじめの一歩 チュートリアルをクリア
		achievements::get().writeAchieve(1);
	break;

	case ConstClass::Level::NORMAL:
		switch (game->machineSelect) {
		case ConstClass::Machine::BLENIM:
			//No.2 勝利の証 Blenimノーマルクリア
			achievements::get().writeAchieve(2);
			break;
		case ConstClass::Machine::METEOR:
			//No8.Meteorでノーマルをクリア　流星の使い手
			achievements::get().writeAchieve(8);
			break;
		}
	break;

	case ConstClass::Level::HARD:
		switch (game->machineSelect) {
		case ConstClass::Machine::BLENIM:
			//No.3 ナイスシューター Blenimハードクリア
			achievements::get().writeAchieve(3);
			break;
		case ConstClass::Machine::METEOR:
			//No9.Meteorでハードをクリア　音速の貴公子
			achievements::get().writeAchieve(9);
			break;
		}
	break;

	case ConstClass::Level::LUNA:
		switch (game->machineSelect) {
		case ConstClass::Machine::BLENIM:
			//No.4 苦境を乗り越え Blenimルナティッククリア
			achievements::get().writeAchieve(4);
			break;
		case ConstClass::Machine::METEOR:
			//No10.Meteorでルナティックをクリア　逆境を跳ね返す者
			achievements::get().writeAchieve(10);
			break;
		}
	break;
	}

	//hpが50%以上残っていたなら
	if (game->myUnit->hpMax < game->myUnit->hp * 2) {
		switch (game->gameLevel) {
		case ConstClass::Level::NORMAL:
			switch (game->machineSelect) {
			case ConstClass::Machine::BLENIM:
				//No5.Blenimでノーマル耐久50%以上残してクリア　余裕綽綽
				achievements::get().writeAchieve(5);
				break;
			case ConstClass::Machine::METEOR:
				//No11.Meteorでノーマル耐久50%以上残してクリア　防御は最大の攻撃　
				achievements::get().writeAchieve(11);
				break;
			}
			break;

		case ConstClass::Level::HARD:
			switch (game->machineSelect) {
			case ConstClass::Machine::BLENIM:
				//No6.Blenimでハード耐久50%以上残してクリア　熟練の腕前
				achievements::get().writeAchieve(6);
				break;
			case ConstClass::Machine::METEOR:
				//No12.Meteorでハード耐久50%以上残してクリア　敗北を知らず
				achievements::get().writeAchieve(12);
				break;
			}
			break;

		case ConstClass::Level::LUNA:
			switch (game->machineSelect) {
			case ConstClass::Machine::BLENIM:
				//No7.Blenimルナティックで耐久50%以上残してクリア　鏡壁の守り手
				achievements::get().writeAchieve(7);
				break;
			case ConstClass::Machine::METEOR:
				//No13.Meteorルナティックで耐久50%以上残してクリア　生ける伝説
				achievements::get().writeAchieve(13);
				break;
			}
			break;
		}
	}

}
