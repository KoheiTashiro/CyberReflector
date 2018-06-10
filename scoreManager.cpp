#include "interface.h"

scoreManager::scoreManager(int setGameLevel)
{
	score = 0;
	gameLevel = setGameLevel;
}

void scoreManager::addScore(int plusScore)
{

		scoreManager::score += (int)((double)plusScore*(0.9+(double)(gameLevel+(gameLevel == 3 ? 2 : 1))*0.1));
	
	
}

void scoreManager::draw()
{
	FontAsset(L"Score")(L"SCORE:", score).drawCenter(400, 25);
}
