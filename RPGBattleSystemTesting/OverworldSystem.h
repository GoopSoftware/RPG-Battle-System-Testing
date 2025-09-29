#pragma once
#include "Entity.h"


enum OverworldState {
	EXPLORING,
	ENCOUNTER_CHECK,
	BATTLE,
	MENU,
	GAME_OVER
};

class OverworldSystem
{
public:
	OverworldSystem();
	~OverworldSystem();
	
	void update();
	void encounterTriggered();
	void generateEncounter();


private:

	OverworldState state;
	float deltatTime;

	Entity player;

	float waitTime = 5.f;
	int prevIntWait = static_cast<int>(waitTime);



};

