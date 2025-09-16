#pragma once

#include <iostream>
#include <raylib.h>

#include "Player.h"
#include "Enemy.h"


class BattleSystem
{
public:
	BattleSystem(Player* player, Enemy* enemy);
	~BattleSystem();

	void battleState();

private:
	

	Player* player;
	Enemy* enemy;

	bool battleActive = true;
	int userInput;

	enum State {
		START,
		PLAYERTURN,
		ACTIONRESOLUTION,
		ENEMYTURN,
		CHECKBATTLESTATUS,
		VICTORY,
		DEFEAT,
		END
	};
	State state{ START };



};

