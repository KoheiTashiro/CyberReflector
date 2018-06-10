#pragma once
#include "interface.h"

class scoreManager {

	int gameLevel;

	public:
		
	int score;

	scoreManager(int setGameLevel);

	void addScore(int plusScore);

	void draw();


};