#include <iostream>
#include <raylib.h>
#include <unordered_map>


#include "Entity.h"
#include "HealthComponent.h"
#include "CombatStatsComponent.h"
#include "BattleSystemController.h"
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
	statsStore[player] = { 10, 5 };

	Entity enemy = createEntity();
	healthStore[enemy] = { 100, 100 };
	statsStore[enemy] = { 8, 2 };

	BattleSystemController battle(player, enemy);

	const int windowWidth = 720;
	const int windowHeight = 480;

	float deltaTime{};

	SetRandomSeed(static_cast<unsigned int>(time(NULL)));

	InitWindow(windowWidth, windowHeight, "RPG Test");
	SetTargetFPS(60);



	while (!WindowShouldClose())
	{

		deltaTime = GetFrameTime();
		//std::cout << deltaTime;


		while (battle.isActive()) {
			battle.update();
		}



		BeginDrawing();


		EndDrawing();



	}

	return 0;

	

}
	

	



