#include <iostream>
#include <raylib.h>
#include <unordered_map>


#include "Entity.h"
#include "HealthComponent.h"
#include "CombatStatsComponent.h"
#include "BattleSystem.h"
#include "Tags.h"
#include "nameComponent.h"


/*
This is a text based testing of an RPG battle system

The initiation to the battle is bool battleActive.

The structure of

*/



int main() {

	std::unordered_map<Entity, HealthComponent> healthStore;
	std::unordered_map<Entity, CombatStatsComponent> statsStore;
	std::unordered_map<Entity, NameComponent> nameStore;

	Entity player1 = createEntity();
	nameStore[player1] = { "Dave" };
	healthStore[player1] = { 100, 100 };
	statsStore[player1] = { 10, 5, 15 };

	Entity enemy1 = createEntity();
	nameStore[enemy1] = { "Goblin" };
	healthStore[enemy1] = { 100, 100 };
	statsStore[enemy1] = { 8, 2, 10 };

	Entity enemy2 = createEntity();
	nameStore[enemy2] = { "Goblin 2" };
	healthStore[enemy2] = { 100, 100 };
	statsStore[enemy2] = { 8, 2, 10 };
	
	std::vector<Entity> players;
	players.push_back(player1);

	std::vector<Entity>enemies;
	enemies.push_back(enemy1);
	enemies.push_back(enemy2);

	BattleSystem battle(players, enemies, healthStore, statsStore, nameStore);

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