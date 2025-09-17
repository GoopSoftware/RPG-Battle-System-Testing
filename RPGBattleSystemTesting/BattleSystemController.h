#pragma once

#include <iostream>
#include <raylib.h>
#include <cstdint>


using Entity = std::uint32_t;

class BattleSystemController
{
public:
	BattleSystemController(Entity player, Entity enemy);
	~BattleSystemController();

	void update();
	bool isActive() const { return battleActive; }

private:
	
	Entity player;
	Entity enemy;

	bool battleActive = true;
	int userInput{};

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

