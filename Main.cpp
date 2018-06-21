#include "interface.h"

///////////////////////////////////////////////////////////////////////
//
//  ゲームの基本情報
//
namespace GameInfo
{
	// ゲームのタイトル
	const String Title = ConstClass::Title;

	// ゲームのバージョン（空の文字列も OK）
	const String Version = L"";

	// ゲームの Web サイト（無い場合は空の文字列にする）
	const String WebURL = L"";

	// 結果ツイートの文章（TweetHead + score + TweetTail)
	const String TweetHead = L"CyberReflector で任務を遂行！ SCORE: ";

	const String TweetTail = L" #CyberReflector";

	// ゲームの背景色
	const ColorF BackgroundColor = ConstClass::backColor;

	// シーン切り替え時のフェードイン・アウトの色
	const ColorF FadeInColor = ColorF(0, 0, 0);

	// タイトルのフォントサイズ
	const int32 TitleFontSize = 72;

	// メニューのフォントサイズ
	const int32 MenuFontSize = 24;

	// メニュー項目「ゲームスタート」
	const String MenuGameStart = L"Game Start";

	// メニュー項目「スコア」
	const String MenuScore = L"Score";

	// メニュー項目「クレジット」
	const String MenuCredit = L"Credit";

	// メニュー項目「Web サイト」
	const String MenuWebURL = L"Trophy";

	// メニュー項目「終了」
	const String MenuExit = L"Exit";

	// セーブデータの保存場所
	const FilePath SaveFilePath = L"Save/Score.dat";

	//称号データの保存場所
	//const FilePath AchieveFilePath = L"Save/Achieve.dat";
	//->ConstClassに移行。

	// スタッフロールのクレジット（項目は増減できる）
	const Array<std::pair<String, Array<String>>> Credits
	{
		{ L"企画立案",{ L"DoubleZ" }},
		{ L"プログラマ",{ L"DoubleZ" } },
		{ L"ライブラリ(Siv3D)",{ L"Ryo Suzuki 様" } },
		//{ L"メニュー雛形",{ L"Ryo Suzuki 様" } },
		{ L"グラフィッカー",{ L"DoubleZ" } },
		{ L"エフェクト",{ L"Double Z" , L"Ryo Suzuki 様"} },
		{ L"BGM(自動作曲)",{ L"Music from Jukedeck" ,L"create your own at" ,L"http://jukedeck.com" } },
		{ L"効果音",{ L"音人 様" ,L"無料効果音で遊ぼう！", L"小森様",L"フリー効果音素材",L"くらげ工匠様"} },
		//{L"フォント",{ L"fontopoSOLID",L"Copyright(c) 2012 by fontopo.",L"All rights reserved."}},
		{ L"Special Thanks",{ L"CCSのみなさん", L"And You!" } },

	};
}

///////////////////////////////////////////////////////////////////////
//
//  タイトル画面の背景エフェクト
//
struct TitleBackGroundEffect : IEffect
{
	Line m_line;
	HSV m_color;

	TitleBackGroundEffect(HSV color)
	{
		const Vec2 pos = RandomVec2(Window::ClientRect());
		const Vec2 direction = Circular(Window::Width() + Window::Height(), Random(TwoPi));
		m_line.set(pos - direction, pos + direction);
		m_color = color;
	}

	bool update(double timeSec)
	{
		m_line.draw(2, ColorF(m_color, (1.0 - Abs(timeSec - 1.0)) * 0.3));
		return timeSec < 2.0;
	}
};

///////////////////////////////////////////////////////////////////////
//
//  タイトル画面のメニュー選択時のエフェクト
//
struct MenuEffect : IEffect
{
	Rect m_rect;

	MenuEffect(const Rect& rect)
		: m_rect(rect) {}

	bool update(double timeSec)
	{
		
		const double e = EaseOut<Easing::Quad>(timeSec);
		RectF(m_rect).stretched(e * 20).shearedX(20).draw(AlphaF((1.0 - e) * 0.4));
		return timeSec < 1.0;
	}
};

struct MenuEffect2 : IEffect
{
	Rect m_rect;

	MenuEffect2(const Rect& rect)
		: m_rect(rect) {}

	bool update(double timeSec)
	{

		const double e = EaseOut<Easing::Quad>(timeSec);
		RectF(m_rect).stretched(e * 20).shearedX(0).draw(AlphaF((1.0 - e) * 0.4));
		return timeSec < 1.0;
	}
};

struct MenuEffect3 : IEffect
{
	Rect m_rect;

	MenuEffect3(const Rect& rect)
		: m_rect(rect) {}

	bool update(double timeSec)
	{

		const double e = EaseOut<Easing::Quad>(timeSec);
		s3d::RoundRect(m_rect,25).stretched(e * 20).draw(AlphaF((1.0 - e) * 0.4));
		return timeSec < 1.0;
	}
};

///////////////////////////////////////////////////////////////////////
//
//  ボタンの描画
//
//テンプレートにすればどんな図形でも使えるよ！やったね！
template<class Figure>
void DrawSelectButton(const Figure& c, bool drawShadow)
{
	c.drawShadow({ 0, 2 }, 12, 2 + drawShadow * 2)
		.draw(Color(250, 180, 100))
		.drawFrame(1, 1, Color(240, 140, 80));
}

//アセット管理、Sozai.s3aからテクスチャファイルを登録する関数
void TextureRegist(s3d::wchar *registName, s3d::wchar *filePath) {
	TextureAsset::Register(
		registName,
		TextureAssetData::FromFileArchive(L"Sozai.s3a", filePath)
	);
}

void SoundRegist(s3d::wchar *registName, s3d::wchar *filePath) {
	SoundAsset::Register(
		registName,
		SoundAssetData::FromFileArchive(L"Sozai.s3a", filePath)
	);
}




///////////////////////////////////////////////////////////////////////
//
//  全てのシーンからアクセスできるデータ
//
struct GameData
{
	int lastMachine = 0;
	int lastLevel = 0;
	int32 lastScore = 0;
};

using MyApp = SceneManager<String, GameData>;

///////////////////////////////////////////////////////////////////////
//
//  セーブ用スコアデータ
//
struct ScoreData
{
	int32 score;

	Date date;

	int gameLevel;

	template <class Archive>
	void serialize(Archive& archive)
	{
		archive(score, date,gameLevel);
	}
};

///////////////////////////////////////////////////////////////////////
//
//  セーブ用トロフィーデータ
//

///////////////////////////////////////////////////////////////////////
//
//  デフォルトのハイスコア
//
const std::array<ScoreData, 5> defaultHighScores
{
	ScoreData{ 2000, Date(2017,1,1),1 },
	ScoreData{ 1500, Date(2017,1,1),1 },
	ScoreData{ 1000, Date(2017,1,1),1 },
	ScoreData{ 750, Date(2017,1,1),1 },
	ScoreData{ 500, Date(2017,1,1),1 },
};




///////////////////////////////////////////////////////////////////////
//
//  タイトル画面
//
class Title : public MyApp::Scene
{
private:

	Effect m_effect;

	Stopwatch m_effectBackgroundStopwatch{ true };

	Stopwatch m_effectMenuItemStopwatch{ true };

	Array<Rect> m_menuBoxes;

	Array<String> m_menuTexts =
	{
		GameInfo::MenuGameStart,
		GameInfo::MenuScore,
		GameInfo::MenuCredit,
		GameInfo::MenuWebURL,
		GameInfo::MenuExit
	};

public:

	~Title()
	{
		Cursor::SetStyle(CursorStyle::Default);
	}

	void init() override
	{
		SoundAsset(L"titleBgm").play();


		m_menuBoxes.resize(m_menuTexts.size());

		int32 boxWidth = 0;

		for (const auto& text : m_menuTexts)
		{
			boxWidth = Max(boxWidth, FontAsset(L"Menu")(text).region().w);
		}

		for (auto i : step(m_menuBoxes.size()))
		{
			m_menuBoxes[i].set(+ConstClass::ScreenX / 2 - (boxWidth + 65) / 2, 260 + i * 68, boxWidth + 65, 55);
		}
	}

	void update() override
	{


		bool handCursor = false;

		for (auto i : step(m_menuBoxes.size()))
		{
			const Quad item = m_menuBoxes[i].shearedX(20);

			handCursor |= item.mouseOver;

			if (item.mouseOver && m_effectMenuItemStopwatch.elapsed() > 300ms)
			{
				m_effect.add<MenuEffect>(m_menuBoxes[i]);

				m_effectMenuItemStopwatch.restart();
			}

			if (item.leftClicked)
			{
				SoundAsset(L"decision").play();

				if (i == 0)
				{
					changeScene(L"Level");
				}
				else if (i == 1)
				{
					changeScene(L"Score");
				}
				else if (i == 2)
				{
					changeScene(L"Credit");
				}
				else if (i == 3)
				{
					changeScene(L"Achieve");
				}
				else
				{
					System::Exit();
				}

				break;
			}
		}

		if (m_effectBackgroundStopwatch.elapsed() > 50ms)
		{
			m_effect.add<TitleBackGroundEffect>(HSV(90, 1, 1));

			m_effectBackgroundStopwatch.restart();
		}

		Cursor::SetStyle(handCursor ? CursorStyle::Hand : CursorStyle::Default);
	}

	void draw() const override
	{
		Graphics2D::SetBlendState(BlendState::Additive);

		m_effect.update();

		Graphics2D::SetBlendState(BlendState::Default);

		const double titleHeight = FontAsset(L"Title")(GameInfo::Title).region().h;

		TextureAsset(L"cyber").drawAt(Window::Center().x, titleHeight);

		for (auto i : step(m_menuBoxes.size()))
		{
			//shearedXで長方形を平方四辺形に
			m_menuBoxes[i].shearedX(20).draw(HSV(40 * i, 0.5, 1));

			FontAsset(L"Menu")(m_menuTexts[i]).drawAt(m_menuBoxes[i].center, Color(40));
		}

		const Size versionSize = FontAsset(L"Version")(GameInfo::Version).region().size;

		FontAsset(L"Version")(GameInfo::Version).drawAt(Window::Size().moveBy(-versionSize));
	}
};


///////////////////////////////////////////////////////////////////////
//
//  難易度選択画面
//
class Level : public MyApp::Scene
{
private:

	Effect m_effect;

	Stopwatch m_effectBackgroundStopwatch{ true };

	Stopwatch m_effectMenuItemStopwatch{ true };

	int isMouseOver;

	int isMouseOverBefore;

	Array<Rect> m_menuBoxes;

	Array<String> m_menuTexts =
	{
		L" Tutorial\n",
		L" Normal\n",
		L" Hard\n",
		L" Lunatic\n",
		L"Back to Title"

	};

	Array<String> m_menuTexts2 =
	{
		L"Tutorial\n操作方法を説明します",
		L"Normal\nすべての方におすすめ",
		L"Hard\n手応えがほしい方に　",
		L"Lunatic\nこのゲームを極めた方に  ",
		L""

	};

	Array<String> m_menuTextsjp =
	{
		L"\n操作方法を説明します",
		L"\nすべての方におすすめ",
		L"\n手応えがほしい方に　",
		L"\nこのゲームを極めた方に  ",
		L""

	};

public:

	~Level()
	{
		Cursor::SetStyle(CursorStyle::Default);
	}

	void init() override
	{
		m_menuBoxes.resize(m_menuTexts2.size());

		int32 boxWidth = 0;

		for (const auto& text : m_menuTexts2)
		{
			boxWidth = Max(boxWidth, FontAsset(L"Menu")(text).region().w);
		}

		for (auto i : step(m_menuBoxes.size()))
		{
			//四角形の大きさをset
			m_menuBoxes[i].set(ConstClass::ScreenX / 2 - (boxWidth + 80) / 2 -45*i+135 , 20 + i * 115, boxWidth + 80, 100);

		}
		
	}

	void update() override
	{
		bool handCursor = false;
		

		for (auto i : step(m_menuBoxes.size()))
		{
			const Quad item = m_menuBoxes[i].shearedX(20);

			handCursor |= item.mouseOver;

			if (item.mouseOver && m_effectMenuItemStopwatch.elapsed() > 300ms)
			{
				m_effect.add<MenuEffect>(m_menuBoxes[i]);

				m_effectMenuItemStopwatch.restart();
			}

		
			

			if (item.leftClicked)
			{
				SoundAsset(L"decision").play();

				#ifdef _DEBUG
				if (Input::KeyA.pressed) {
				m_data->lastMachine=1;
				}else m_data->lastMachine = 0;
				#endif


				if (i == 0)
				{
					m_data->lastLevel = ConstClass::Level::TUTORIAL;
					changeScene(L"Machine");

				}
				else if (i == 1)
				{
					m_data->lastLevel = ConstClass::Level::NORMAL;
					changeScene(L"Machine");
				}
				else if (i == 2)
				{
					m_data->lastLevel = ConstClass::Level::HARD;
					changeScene(L"Machine");
				}
				else if (i==3)
				{
					m_data->lastLevel = ConstClass::Level::LUNA;
					changeScene(L"Machine");
				}
				else {
					changeScene(L"Title");

				}

				break;
			}
			else if (Input::KeyEscape.clicked)
			{
				SoundAsset(L"decision").play();
			changeScene(L"Title");
			}
		}

		if (m_effectBackgroundStopwatch.elapsed() > 50ms)
		{
			m_effect.add<TitleBackGroundEffect>(HSV(90, 1, 1));

			m_effectBackgroundStopwatch.restart();
		}

		Cursor::SetStyle(handCursor ? CursorStyle::Hand : CursorStyle::Default);
	}

	void draw() const override
	{
		Graphics2D::SetBlendState(BlendState::Additive);

		m_effect.update();

		Graphics2D::SetBlendState(BlendState::Default);

		FontAsset(L"Level")(L"Level\n   Select").draw(10, 0);

		for (auto i : step(m_menuBoxes.size()))
		{
			m_menuBoxes[i].shearedX(20).draw(HSV(40*i,0.5,1));
			
			//日本語の行があるかないかで、表示方法を変える
			if (m_menuTextsjp[i] == L"") {
				FontAsset(L"Menu")(m_menuTexts[i]).drawAt(m_menuBoxes[i].center, Color(40));
			}
			else {
				FontAsset(L"Menu")(m_menuTexts[i]).draw(m_menuBoxes[i].center - Point(FontAsset(L"MenuJp")(m_menuTexts2[i]).region().w / 2, FontAsset(L"MenuJp")(m_menuTexts2[i]).region().h / 2), Color(40));
			}
			FontAsset(L"MenuJp")(m_menuTextsjp[i]).drawAt(m_menuBoxes[i].center, Color(40));
		}
				
	}
};


///////////////////////////////////////////////////////////////////////
//
//  機体選択画面
//
class Machine : public MyApp::Scene
{
private:
	//meteorの解放判断用
	std::array<ScoreData, 5> m_highScores = defaultHighScores;

	Effect m_effect;

	Stopwatch m_effectBackgroundStopwatch{ true };

	Stopwatch m_effectMenuItemStopwatch{ true };

	int isMouseOver;

	int isMouseOverBefore;

	Array<Rect> m_menuBoxes;

	const Rect titleButton = Rect(70, 520, FontAsset(L"ResultButton")(L"Go to Title").region().w + 20, FontAsset(L"ResultButton")(L"Go to Title").region().h);


public:

	~Machine()
	{
		Cursor::SetStyle(CursorStyle::Default);
	}

	void init() override
	{
		
			if (FileSystem::Exists(GameInfo::SaveFilePath))
			{
				Deserializer<BinaryReader>{GameInfo::SaveFilePath}(m_highScores);
			}
			else
			{
				Serializer<BinaryWriter>{GameInfo::SaveFilePath}(m_highScores);
			}
		

		m_menuBoxes.resize(3);

		int32 boxWidth = 0;
	

		for (auto i : step(m_menuBoxes.size()-1))
		{
			//四角形の大きさをset
			m_menuBoxes[i].set(70, 120 + i * 200, ConstClass::ScreenX - 70*2, 170);

		}
		
		m_menuBoxes[2]= titleButton;

	}

	void update() override
	{
		#ifdef _DEBUG
		ClearPrint();
		Println(Mouse::Pos());
		#endif

		bool handCursor = false;


		for (auto i : step(m_menuBoxes.size()))
		{
			const Quad item = m_menuBoxes[i].shearedX(0);

			handCursor |= item.mouseOver;

			if (item.mouseOver && m_effectMenuItemStopwatch.elapsed() > 300ms)
			{
				if (i == m_menuBoxes.size()-1) {
					//Go to Titleのやつ
					m_effect.add<MenuEffect2>(m_menuBoxes[i]);
				}
				else {
					m_effect.add<MenuEffect3>(m_menuBoxes[i]);
				}

				m_effectMenuItemStopwatch.restart();
			}

			if (item.leftClicked)
			{
			
				if (i == 0)
				{
					SoundAsset(L"decision").play();
					m_data->lastMachine = ConstClass::Machine::BLENIM;
					changeScene(L"Game");
				}
				else if(i==1){
					if (m_highScores[0].score >= 5000) {
						SoundAsset(L"decision").play();
						m_data->lastMachine = ConstClass::Machine::METEOR;
						changeScene(L"Game");
					}else SoundAsset(L"error").play();

				}
				else {
					SoundAsset(L"decision").play();
					changeScene(L"Title");

				}

				break;
			}
			else if (Input::KeyEscape.clicked)
			{
				SoundAsset(L"decision").play();
				changeScene(L"Level");
			}
			else if (Input::KeyA.clicked) {
				m_highScores[0].score = 5000;
			}
		}

	

		if (m_effectBackgroundStopwatch.elapsed() > 50ms)
		{
			m_effect.add<TitleBackGroundEffect>(HSV(90, 1, 1));

			m_effectBackgroundStopwatch.restart();
		}

		Cursor::SetStyle(handCursor ? CursorStyle::Hand : CursorStyle::Default);
	}

	void draw() const override
	{
		Graphics2D::SetBlendState(BlendState::Additive);

		m_effect.update();

		Graphics2D::SetBlendState(BlendState::Default);

		FontAsset(L"MachineSelect")(L"Machine  Select").drawAt(Window::Center().x, 60);

		for (auto i : step(m_menuBoxes.size()-1))
		{
			m_menuBoxes[i](TextureAsset(L"machine" + ToString(i))).draw();
		}

		if (m_highScores[0].score < 5000) {
			m_menuBoxes[1](TextureAsset(L"lockedmachine1")).draw();
		}


		DrawSelectButton(m_menuBoxes[m_menuBoxes.size()-1], m_menuBoxes[m_menuBoxes.size() - 1].mouseOver);
		FontAsset(L"ResultButton")(L"Go to Title").drawAt(titleButton.center, Color(40));
	}
};

///////////////////////////////////////////////////////////////////////
//
//  ゲーム
//
class Game : public MyApp::Scene
{
private:
	std::unique_ptr<gameManager> game;
	//処理時間測定用
	MillisecClock c;
public:

	Game() {
		
		}

	~Game()
	{
		Cursor::SetStyle(CursorStyle::Default);
	}

	void init() override
	{
		m_data->lastScore = 0;
		game = std::make_unique<gameManager>(m_data->lastLevel,m_data->lastMachine);
		//カーソルを非表示にします。
		Cursor::SetStyle(CursorStyle::None);
	}

	void update() override
	{
	
		//キャラクタの移動など
		game->update();
	
		if (game->isOver == true) {
			if (game->isPause == false) {
				m_data->lastScore = game->score->score;
				changeScene(L"Result", 2000);
			}
			else { 
				changeScene(L"Title", 2000); 
			}
		}
	
		
	}

	void draw() const override
	{
		//キャラクタの描写など
		game->draw();

	}
};

///////////////////////////////////////////////////////////////////////
//
//  結果画面
//
class Result : public MyApp::Scene
{
private:

	//どんなヒントをだすか
	int Rand;

	String hintString;

	Image hintImage;

	Texture hintTexture;

	bool isHiScore;

	std::array<ScoreData, 5> m_highScores = defaultHighScores;
	
	const Rect titleButton = Rect(Window::Center().x - 220-FontAsset(L"ResultButton")(L"Go to Title").region().w/2, Window::Height() * 0.7+37, FontAsset(L"ResultButton")(L"Go to Title").region().w+20, FontAsset(L"ResultButton")(L"Go to Title").region().h);

	const Rect tweetButton = Rect(Window::Center().x + 180 - FontAsset(L"ResultButton")(L"Go to Title").region().w / 2, Window::Height() * 0.7 +37, FontAsset(L"ResultButton")(L"Go to Title").region().w+20, FontAsset(L"ResultButton")(L"Go to Title").region().h);

public:

	~Result()
	{
		Cursor::SetStyle(CursorStyle::Default);
	}

	void init() override
	{
		//表示される攻略のヒントを決定します。
		switch (Random(0,5)) {
		case 0:
			hintString = L"バリアを貼った状態で敵に突っ込めば  大量のダメージを与えられるぞ";
			break;
		case 1:
			hintString = L"カドでバリアを張れば死角が減り  安全に反射できるぞ";
			break;
		case 2:
			hintString = L"包囲されるとHPは一気に減る  有利な位置取りを心がけよう";
			break;
		case 3:
			hintString = L"エネルギーが無くなる度逃げ回る　ヒットアンドアウェイ戦法が有効だ";
			break;
		case 4:
			hintString = L"次のWaveの敵の出現位置を　意識した位置取りをしてみよう";
			break;
		case 5:
			hintString = L"素早く敵を倒すことで　多くのスコアが得られるぞ";
			break;
		}
		
		const Rect rectO = FontAsset(L"Hint").region(hintString);
		hintImage.resize(rectO.size);
		FontAsset(L"Hint").overwrite(hintImage, hintString, {0,0}, Palette::White);
		hintTexture = Texture(hintImage);


		if (FileSystem::Exists(GameInfo::SaveFilePath))
		{
			Deserializer<BinaryReader>{GameInfo::SaveFilePath}(m_highScores);
		}
		else
		{
			Serializer<BinaryWriter>{GameInfo::SaveFilePath}(m_highScores);
		}

		if (m_highScores.back().score <= m_data->lastScore)
		{
			isHiScore = true;
			m_highScores.back() = { m_data->lastScore, Date::Today() ,m_data->lastLevel};

			std::sort(m_highScores.begin(), m_highScores.end(), [](const ScoreData& a, const ScoreData& b)
			{
				return a.score > b.score ? true : a.score == b.score ? a.date > b.date : false;
			});

			Serializer<BinaryWriter>{GameInfo::SaveFilePath}(m_highScores);
		}
		else isHiScore = false;

		//スコア＆倒した敵の数によって、称号を追加
		achieveCheck();
		

	}

	void update() override
	{
		if (titleButton.leftClicked || Input::KeyEscape.clicked)
		{
			SoundAsset(L"decision").play();

			changeScene(L"Title");
		}

		if (tweetButton.leftClicked)
		{
			SoundAsset(L"decision").play();

			const String tweetMessage = Format(GameInfo::TweetHead, m_data->lastScore, GameInfo::TweetTail);

			Twitter::OpenTweetWindow(tweetMessage);
		}

		const bool handCursor = titleButton.mouseOver || tweetButton.mouseOver;

		Cursor::SetStyle(handCursor ? CursorStyle::Hand : CursorStyle::Default);
	}

	void draw() const override
	{
		#ifdef _DEBUG
		ClearPrint();
		Println(Mouse::Pos());
		#endif

		const double resultHeight = FontAsset(L"Result")(L"x", m_data->lastScore).region().h;

		const Rect frame(20, 535, 760, 50);
		const Rect frameMessage(25, 535, 750, 50);
		
		frameMessage(hintTexture).draw();

		frame.stretched(2).drawFrame(0, 2, Palette::White);

		frame.stretched(-2).drawFrame(0, 2, Palette::White);
		
		const  s3d::RoundRect ranking(25, 15, 750, 415, 20);
		ranking.draw(ColorF(0.4, 0.8, 1, 0.4));
		ranking.drawFrame(2,0, ColorF(0.4, 0.8, 1));

		FontAsset(L"Ranking")(L"Ranking").drawAt(Window::Center().x, 50, Color(Palette::Orange));
		

		for (auto i : step(m_highScores.size()))
		{
			Color rankingColor;

			//今回の記録とランキング内の記録が一致したら
			if (isHiScore == true &&
				m_highScores[i].score == m_data->lastScore&&
				m_highScores[i].gameLevel == m_data->lastLevel&&
				m_highScores[i].date == Date::Today()) {
				rankingColor = Color(Palette::Orange);
				FontAsset(L"RankingThin")(i+1,L"位にランクイン！").drawAt(Window::Center().x, 397, rankingColor);
			}
			else {
				rankingColor = Color(Palette::White);
			}

			FontAsset(L"Ranking")(i+1,L"位").draw(120- FontAsset(L"Ranking")(i + 1, L"位").region().w,70 + i * 50, rankingColor);
			FontAsset(L"Ranking")(m_highScores[i].score).draw(260 - FontAsset(L"Ranking")(m_highScores[i].score).region().w, 70 + i * 50,rankingColor);

			switch (m_highScores[i].gameLevel) {
			case 1://NORMAL
				FontAsset(L"Ranking")(L"Normal").draw(300, 70 + i * 50,rankingColor);
				break;
			case 2://HARD
				FontAsset(L"Ranking")(L"Hard").draw(300, 70 + i * 50,rankingColor);
				break;
			case 3://LUNATIC
				FontAsset(L"Ranking")(L"Lunatic").draw(300, 70 + i * 50,rankingColor);
				break;
			}
			FontAsset(L"Ranking")(m_highScores[i].date).draw(495,70 + i * 50,rankingColor);

		}
		switch (m_data->lastLevel) {
		case 1://NORMAL
			FontAsset(L"Ranking")(L"YourScore:", m_data->lastScore,L" Normal").drawAt(Window::Center().x,345, Palette::Orange);
			break;
		case 2://HARD
			FontAsset(L"Ranking")(L"YourScore:", m_data->lastScore, L" Hard").drawAt(Window::Center().x, 345, Palette::Orange);
			break;
		case 3://LUNATIC
			FontAsset(L"Ranking")(L"YourScore:", m_data->lastScore, L" Lunatic").drawAt(Window::Center().x, 345, Palette::Orange);
			break;
		}

		if (isHiScore == false) {
			FontAsset(L"RankingThin")(L"残念！ランクインならず").drawAt(Window::Center().x, 397, Palette::Orange);
		}
		

		DrawSelectButton(titleButton, titleButton.mouseOver);
		FontAsset(L"ResultButton")(L"Go to Title").drawAt(titleButton.center, Color(40));

		DrawSelectButton(tweetButton, tweetButton.mouseOver);
		FontAsset(L"ResultButton")(L"Tweet").drawAt(tweetButton.center, Color(40));
	}

	void achieveCheck(){

		//敵の数をセーブ！
		achievements::get().save();

		if (m_data->lastScore > 3000) {
			//No14.SCORE3000を突破　最初の壁
			achievements::get().writeAchieve(14);
		}

		if (m_data->lastScore > 5000) {
			//No15.SCORE5000を突破　中級者の壁
			achievements::get().writeAchieve(15);
		}

		if (m_data->lastScore > 8000) {
			//No16.SCORE8000を突破　上級者の壁
			achievements::get().writeAchieve(16);
		}

		if (m_data->lastScore > 9000) {
			//No17.SCORE9000を突破　限界を突破せよ　
			achievements::get().writeAchieve(17);
		}

		//敵を倒した数をチェック！
		achievements::get().checkEnemy();

	}

};

///////////////////////////////////////////////////////////////////////
//
//  ハイスコア一覧
//
class Score : public MyApp::Scene
{
private:

	std::array<ScoreData, 5> m_highScores = defaultHighScores;
	
	Effect m_effect;

	Stopwatch m_effectBackgroundStopwatch{ true };

public:

	void init() override
	{
		if (FileSystem::Exists(GameInfo::SaveFilePath))
		{
			Deserializer<BinaryReader>{GameInfo::SaveFilePath}(m_highScores);
		}
		else
		{
			Serializer<BinaryWriter>{GameInfo::SaveFilePath}(m_highScores);
		}
	}

	void update() override
	{
		if ((Input::MouseL | Input::KeyEscape).clicked)
		{
			SoundAsset(L"decision").play();

			changeScene(L"Title");
		}

		if (m_effectBackgroundStopwatch.elapsed() > 50ms)
		{
			m_effect.add<TitleBackGroundEffect>(HSV(20, 1, 1));

			m_effectBackgroundStopwatch.restart();
		}
	}

	void draw() const override
	{

		m_effect.update();

		const  s3d::RoundRect ranking(25, 155, 750, 330, 20);
		ranking.draw(ColorF(0.4, 0.8, 1, 0.4));
		ranking.drawFrame(2, 0, ColorF(0.4, 0.8, 1));

		FontAsset(L"MachineSelect")(L"Ranking").drawAt(Window::Center().x, 100);
		
		for (auto i : step(m_highScores.size()))
		{
			FontAsset(L"Ranking")(i + 1, L"位").draw(120 - FontAsset(L"Ranking")(i + 1, L"位").region().w, 190 + i * 50, HSV(40*i,0.5,1));
			FontAsset(L"Ranking")(m_highScores[i].score).draw(260 - FontAsset(L"Ranking")(m_highScores[i].score).region().w, 190 + i * 50, HSV(40 * i, 0.5, 1));

			switch (m_highScores[i].gameLevel) {
			case 1://NORMAL
				FontAsset(L"Ranking")(L"Normal").draw(300, 190 + i * 50, HSV(40 * i, 0.5, 1));
				break;
			case 2://HARD
				FontAsset(L"Ranking")(L"Hard").draw(300, 190 + i * 50, HSV(40 * i, 0.5, 1));
				break;
			case 3://LUNATIC
				FontAsset(L"Ranking")(L"Lunatic").draw(300, 190 + i * 50, HSV(40 * i, 0.5, 1));
				break;
			}
			FontAsset(L"Ranking")(m_highScores[i].date).draw(495, 190 + i * 50, HSV(40 * i, 0.5, 1));

		}
		

		
	}
};

///////////////////////////////////////////////////////////////////////
//
//  実績一覧
//
class Achieve : public MyApp::Scene
{
private:

	Effect m_effect;

	Stopwatch m_effectBackgroundStopwatch{ true };

	Stopwatch m_effectMenuItemStopwatch{ true };

	//int isMouseOver;

	//int isMouseOverBefore;

	const Rect titleButton = Rect(Window::Center().x + 200 - FontAsset(L"ResultButton")(L"Go to Title").region().w / 2, Window::Height() * 0.8 + 37, FontAsset(L"ResultButton")(L"Go to Title").region().w + 20, FontAsset(L"ResultButton")(L"Go to Title").region().h);

	struct tolophyInfo {
		String Texts;
		String Explain;
		String Condition;
	};
	
	std::array<tolophyInfo,ConstClass::AchieveNumber> tolophy
	{ {//メンバ変数として配列を持つので二重括弧でくくらなきゃいけないとかなんとか
		{L"はじめの一歩",L"チュートリアルをクリアした証",L"はじめの一歩を踏み出そう"},
		{L"勝利の証",L"Blenimでノーマルをクリアした証",L"Blenimでノーマルをクリアしよう"},
		{L"ナイスシューター",L"Blenimでハードをクリアした証",L"Blenimでハードをクリアしよう"},
		{L"苦境を乗り越え",L"Blenimでルナティックをクリアした証",L"Blenimでルナティックをクリアしよう"},
		{L"余裕綽々",L"Blenimでノーマル耐久50%以上残してクリアした証",L"Blenimノーマルで半分以上HPを残そう"},
		{L"熟練の腕前",L"Blenimでハード耐久50%以上残してクリアした証",L"Blenimハードで半分以上HPを残そう" },
		{L"鏡壁の守り手",L"Blenimのルナティック耐久半分残してクリアした証",L"Blenimルナティックで半分以上HPを残そう" },
		{L"流星の使い手",L"Meteorでノーマルをクリアした証",L"Meteorでノーマルをクリアしよう" },
		{L"音速の貴公子",L"Meteorでハードをクリアした証",L"Meteorでハードをクリアしよう" },
		{L"逆境を跳ね返す者",L"Meteorでルナティックをクリアした証",L"Meteorでルナティックをクリアしよう" },
		{L"防御は最大の攻撃",L"Meteorでノーマル耐久50%以上残してクリアした証",L"Meteorノーマルで半分以上HPを残そう" },
		{L"敗北を知らず",L"Meteorでハード耐久50%以上残してクリアした証",L"Meteorハードで半分以上HPを残そう" },
		{L"Cyber Reflector",L"Meteorのルナティック耐久半分残してクリアした証",L"Meteorルナティックで半分以上HPを残そう" },
		{L"最初の壁",L"SCORE3000を突破した証",L"SCORE3000を目指そう" },
		{L"中級者の壁",L"SCORE5000を突破した証",L"SCORE5000を目指そう" },
		{L"上級者の壁",L"SCORE8000を突破した証",L"SCORE8000を目指そう" },
		{L"限界を突破せよ",L"SCORE9000を突破した証",L"SCORE9000を目指そう" },
		{L"武器無き戦い",L"敵を100体倒した証",L"敵を100体倒そう" },
		{L"キルマシーン",L"敵を200体倒した証",L"敵を200体倒そう" },
		{L"美しく残酷な死を",L"敵を300体倒した証",L"敵を300体倒そう" },
		}
	};

	std::array<bool, ConstClass::AchieveNumber> achieve;


public:

	void init() override
	{
		for (int i = 0; i < ConstClass::AchieveNumber; i++) {
			achieve[i] = achievements::get()[i];
		}
	}

	void update() override
	{

		if (m_effectBackgroundStopwatch.elapsed() > 50ms)
		{
			m_effect.add<TitleBackGroundEffect>(HSV(60, 1, 1));

			m_effectBackgroundStopwatch.restart();
		}

		if (titleButton.leftClicked || Input::KeyEscape.clicked)
		{
			SoundAsset(L"decision").play();

			changeScene(L"Title");
		}


		const bool handCursor = titleButton.mouseOver;

		Cursor::SetStyle(handCursor ? CursorStyle::Hand : CursorStyle::Default);

	}

	void draw() const override
	{
	
		Graphics2D::SetBlendState(BlendState::Additive);

		m_effect.update();

		Graphics2D::SetBlendState(BlendState::Default);

		

		const  s3d::RoundRect tolophys(20, 100, 760, 315, 20);
		tolophys.draw(Color(ConstClass::backColor,200));
		tolophys.drawFrame(2, 0, ColorF(0.2, 0.4, 0.5));

		FontAsset(L"TolophyRate")(L"達成率:", std::floor(achievements::get().clearRate() * 10)/10, L"%  撃墜数:",achievements::get().getEnemy()).draw(30, 530);
		
		showTolophys();
	
		const Rect frame(20, 435, 760, 50);

		frame.stretched(2).drawFrame(0, 2, Palette::White);
		frame.stretched(-2).draw(Color(ConstClass::backColor,200));
		frame.stretched(-2).drawFrame(0, 2, Palette::White);
		
		showInfo();

		FontAsset(L"TolophyLogo")(L"Tolophy").drawAt(Window::Center().x, 55);

		DrawSelectButton(titleButton, titleButton.mouseOver);
		FontAsset(L"ResultButton")(L"Go to Title").drawAt(titleButton.center, Color(40));
	}

	void showTolophys() const {
		
		int i;

		for (i = 0; i < 8; i++) {
			if (achieve[i] == true) {
				FontAsset(L"Tolophy")(L"No.", i + 1, L" ", tolophy[i].Texts).draw(35, 110 + 37 * i,Palette::Orange);
			}
			else {
				FontAsset(L"Tolophy")(L"No.", i + 1, L" ?????????????").draw(35, 110 + 37 * i);
			}
		}

		for (; i < 16; i++) {
			if (achieve[i] == true) {
				FontAsset(L"Tolophy")(L"No.", i + 1, L" ", tolophy[i].Texts).draw(283, 110 + 37 * (i%8), Palette::Orange);
			}
			else {
				FontAsset(L"Tolophy")(L"No.", i + 1, L" ????????????").draw(283, 110 + 37 * (i%8));
			}
		}

		for (; i < ConstClass::AchieveNumber; i++) {
			if (achieve[i] == true) {
				FontAsset(L"Tolophy")(L"No.", i + 1, L" ", tolophy[i].Texts).draw(535, 110 + 37 * (i % 8), Palette::Orange);
			}
			else {
				FontAsset(L"Tolophy")(L"No.", i + 1, L" ????????????").draw(535, 110 + 37 * (i % 8));
			}
		}

	}

	void showInfo() const {
		int i;

		for (i = 0; i < 8; i++) {
			if (Mouse::Pos().x>35 && Mouse::Pos().x<283 && Mouse::Pos().y>110+37*i && Mouse::Pos().y<110+37*(i+1)) {
				if (achievements::get()[i]) {
					FontAsset(L"TolophyRate")(tolophy[i].Explain).drawAt(400, 460);
				}
				else {
					FontAsset(L"TolophyRate")(tolophy[i].Condition).drawAt(400, 460);
				}
			}
		}

		for (; i < 16; i++) {
			if (Mouse::Pos().x>283 && Mouse::Pos().x<535 && Mouse::Pos().y>110 + 37 * (i%8) && Mouse::Pos().y<110 + 37 * ((i%8) + 1)) {
				if (achievements::get()[i]) {
					FontAsset(L"TolophyRate")(tolophy[i].Explain).drawAt(400, 460);
				}
				else {
					FontAsset(L"TolophyRate")(tolophy[i].Condition).drawAt(400, 460);
				}
			}
		}

		for (; i < ConstClass::AchieveNumber; i++) {
			if (Mouse::Pos().x > 535 && Mouse::Pos().y > 110 + 37 * (i % 8) && Mouse::Pos().y < 110 + 37 * ((i % 8) + 1)) {
				if (achievements::get()[i]) {
					FontAsset(L"TolophyRate")(tolophy[i].Explain).drawAt(400, 460);
				}
				else {
					FontAsset(L"TolophyRate")(tolophy[i].Condition).drawAt(400, 460);
				}
			}
		}
	}

};

///////////////////////////////////////////////////////////////////////
//
//  スタッフクレジット
//
class Credit : public MyApp::Scene
{
private:

	Array<std::tuple<String, double, bool>> m_credits;

	double m_height = 0.0;

	Stopwatch m_stopwatch{ true };

	double yOffset() const
	{
		return Window::Height() + 60 - m_stopwatch.ms() / 20.0;
	}

public:

	void init()
	{
		double y = 0;

		for (const auto& credit : GameInfo::Credits)
		{
			m_credits.emplace_back(credit.first, y, true);
			y += 70;

			for (const auto& name : credit.second)
			{
				m_credits.emplace_back(name, y, false);
				y += 60;
			}

			y += 60;
		}

		m_height = y;
	}

	void update() override
	{
		if ((Input::MouseL | Input::KeyEscape).clicked)
		{
			SoundAsset(L"decision").play();

			changeScene(L"Title");
		}

		if ((m_height + yOffset()) < 0)
		{
			m_stopwatch.restart();
		}
	}

	void draw() const override
	{
		const double offset = yOffset();

		for (const auto& credit : m_credits)
		{
			FontAsset(std::get<bool>(credit) ? L"CreditBig" : L"CreditSmall")(std::get<String>(credit))
				.drawAt(Window::Center().x, std::get<double>(credit) + offset);
		}
	}
};

///////////////////////////////////////////////////////////////////////
//
//  メイン関数
//
void Main()
{
	Window::Resize(800, 600);
	Window::SetTitle(GameInfo::Title);
	Graphics::SetBackground(GameInfo::BackgroundColor);
	System::SetExitEvent(WindowEvent::CloseButton);

	//Archive::Create(L"Sozai", L"Sozai.s3a");
	FileArchive archive(L"Sozai.s3a");
	if (!archive)	{
		MessageBox::Show(L"リソースファイル Sozai.s3a をロードできません");
		return;
	}


	//////////////////////////////////////////////////////////////
	//
	//  使用するシーン
	//
	MyApp manager;
	manager.setFadeColor(GameInfo::FadeInColor);
	manager.add<Title>(L"Title");
	manager.add<Game>(L"Game");
	manager.add<Result>(L"Result");
	manager.add<Score>(L"Score");
	manager.add<Credit>(L"Credit");
	manager.add<Level>(L"Level");
	manager.add<Machine>(L"Machine");
	manager.add<Achieve>(L"Achieve");


	//////////////////////////////////////////////////////////////
	//
	//フォントファイルをゲットだぜ！
	FontManager::Register(archive.load(L"Sozai/Akashi.ttf"));
	//  使用するフォント
	//
	FontAsset::Register(L"Title", GameInfo::TitleFontSize, Typeface::Heavy, FontStyle::Outline);
	FontAsset(L"Title").changeOutlineStyle(TextOutlineStyle(Color(60), Color(255), GameInfo::TitleFontSize * 0.05));
	FontAsset::Register(L"Menu", GameInfo::MenuFontSize,  L"Akashi");
	FontAsset::Register(L"MenuJp", GameInfo::MenuFontSize, Typeface::Bold);
	FontAsset::Register(L"TolophyRate", 22, Typeface::Bold);
	FontAsset::Register(L"Version", 14, Typeface::Regular);
	FontAsset::Register(L"Tolophy", 14, Typeface::Bold);
	//FontAsset::Register(L"TolophyGet", 14,Typeface::Bold);
	FontAsset::Register(L"CountDown", 72, Typeface::Bold);
	FontAsset::Register(L"Result", 60, Typeface::Bold);
	FontAsset::Register(L"ResultButton", 30, L"Akashi");
	FontAsset::Register(L"GameTime", 40, Typeface::Light);
	FontAsset::Register(L"ScoreList", 50, Typeface::Heavy);
	FontAsset::Register(L"ScoreListDate", 25, Typeface::Regular, FontStyle::Italic);
	FontAsset::Register(L"CreditBig", 32, Typeface::Bold);
	FontAsset::Register(L"CreditSmall", 28, Typeface::Regular);
	FontAsset::Register(L"Hint", 24);
	FontAsset::Register(L"Tutorial", 20);
	FontAsset::Register(L"Ranking", 30,Typeface::Bold);
	FontAsset::Register(L"RankingThin", 23,Typeface::Regular);

	FontAsset::Register(L"Level",34, L"Akashi", FontStyle::Outline);
	FontAsset(L"Level").changeOutlineStyle(TextOutlineStyle(Palette::White, Palette::Red, 36 * 0.05));
	FontAsset::Register(L"MachineSelect", 34, L"Akashi", FontStyle::Outline);
	FontAsset(L"MachineSelect").changeOutlineStyle(TextOutlineStyle(Palette::White, Palette::Orange, 36 * 0.05));
	FontAsset::Register(L"TolophyLogo", 34, L"Akashi", FontStyle::Outline);
	FontAsset(L"TolophyLogo").changeOutlineStyle(TextOutlineStyle(Palette::White, Palette::Yellowgreen, 36 * 0.05));

	
	FontAsset::Register(L"Score", 18, Typeface::Bold);
	//フォントを登録、プログラムのどこからでも参照可能。
	FontAsset::Register(L"font", 10, Typeface::Bold);
	FontAsset::Register(L"Next", 14, Typeface::Bold);
	//マウスカーソルのテクスチャを登録、プログラムのどこからでも（ｒｙ
	//TextureRegistをTextureAsset::Registerにかえるとアーカイブではなく直接読み込む
	TextureRegist(L"mouseCercle", L"Sozai/cercle.png");
	TextureRegist(L"mouseYellow", L"Sozai/cursor2.png");
	TextureRegist(L"mousePurple", L"Sozai/cursor3.png");
	//プレイヤーの画像
	TextureRegist(L"myUnit", L"Sozai/player6.png");
	TextureRegist(L"secretUnit",L"Sozai/enemyA.png");
	//敵キャラクタの画像
	TextureRegist(L"chaser", L"Sozai/enemy-90.png");
	TextureRegist(L"stalker",L"Sozai/enemy-150.png");
	TextureRegist(L"foreway", L"Sozai/en2.png");
	TextureRegist(L"spinlazer", L"Sozai/en3.png");
	TextureRegist(L"spinner", L"Sozai/enemyA-90.png");
	//ボタンの画像
	TextureRegist(L"machine0", L"Sozai/easymachine.png");
	TextureRegist(L"machine1", L"Sozai/hardmachine.png");
	TextureRegist(L"lockedmachine1", L"Sozai/lockedmachine.png");
	//弾の画像
	TextureRegist(L"lazer", L"Sozai/lazer2n.png");
	TextureRegist(L"myLazer", L"Sozai/lazer3n.png");
	TextureRegist(L"lazerm", L"Sozai/lazer2m.png");
	TextureRegist(L"myLazerm", L"Sozai/lazer3m.png");
	TextureRegist(L"ball", L"Sozai/ball.png");
	TextureRegist(L"needle", L"Sozai/needle2.png");
	//エフェクト用の画像
	TextureRegist(L"fire", L"Sozai/fire.png");
	TextureRegist(L"firenext", L"Sozai/firenext.jpg");
	TextureRegist(L"fireme", L"Sozai/fireme.jpg");
	TextureRegist(L"fireAnime", L"Sozai/fire.bmp");
	TextureRegist(L"attackEffect", L"Sozai/attackeffect.bmp");
	TextureRegist(L"attackEffect2", L"Sozai/attackeffect2.bmp");

	//タイトルロゴ
	TextureRegist(L"cyber", L"Sozai/cyberlogo.png");
	TextureRegist(L"clear", L"Sozai/clear.png");
	TextureRegist(L"gameover",L"Sozai/gameover.png");
	//BGM
	SoundRegist(L"titleBgm", L"Sozai/BumpyHours3.wav");
	SoundAsset(L"titleBgm").setLoop(true);
	SoundAsset(L"titleBgm").setVolume(0.4);

	//効果音
	SoundRegist(L"alert", L"Sozai/alert.mp3");
	SoundRegist(L"reflection",L"Sozai/reflection.mp3");
	SoundRegist(L"bomb", L"Sozai/bomb.mp3");
	SoundRegist(L"damage", L"Sozai/sen_ge_gun13.mp3");
	SoundRegist(L"select", L"Sozai/select.mp3");
	SoundRegist(L"decision", L"Sozai/decision.mp3");
	SoundRegist(L"error", L"Sozai/error.mp3");

	//////////////////////////////////////////////////////////////
	//
	//  メインループ
	//
	while (System::Update())
	{
		if (!manager.updateAndDraw())
		{
			break;
		}
	}

}

