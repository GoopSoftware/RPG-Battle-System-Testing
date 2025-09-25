#include <iostream>
#include <raylib.h>
#include <unordered_map>


#include "Entity.h"
#include "HealthComponent.h"
#include "CombatStatsComponent.h"
#include "BattleSystem.h"
#include "Tags.h"


/*
This is a text based testing of an RPG battle system

The initiation to the battle is bool battleActive.

The structure of

*/



int main() {

	Entity player = createEntity();
	healthStore[player] = { 100, 100 };
	statsStore[player] = { 10, 5, 9 };

	Entity enemy = createEntity();
	healthStore[enemy] = { 100, 100 };
	statsStore[enemy] = { 8, 2, 10 };
	
	std::vector<Entity> players;
	

	std::vector<Entity>enemies;


	BattleSystem battle(player, enemy);

	const int windowWidth = 720;
	const int windowHeight = 480;

	float deltaTime{};

	float waitTime = 5.f;
	int prevIntWait = static_cast<int>(waitTime);

	bool overWorldActive = true;
	
	SetRandomSeed(static_cast<unsigned int>(time(NULL)));

	InitWindow(windowWidth, windowHeight, "RPG Test");
	SetTargetFPS(60);


	std::cout << "Searching for a battle.";

	while (!WindowShouldClose())
	{

		deltaTime = GetFrameTime();
		//std::cout << deltaTime;

		int currentIntWait = static_cast<int>(waitTime);

		// This just prints a . to console every second then when waitTime = 0
		// battle starts
		if (currentIntWait < prevIntWait) {
			std::cout << ".";
			prevIntWait = currentIntWait;
		}
		if (waitTime > 0) {
			waitTime -= deltaTime;
		}
		if (waitTime <= 0) {
			battle.battleActive = true;
		}
		while (battle.isActive()) {
			battle.update();
		}
		// ------------------------------------------------------


		BeginDrawing();


		EndDrawing();



	}

	return 0;

	

}
	

	



