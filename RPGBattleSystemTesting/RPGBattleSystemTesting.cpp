#include <iostream>
#include <raylib.h>


#include "Player.h"
#include "Enemy.h"
#include "BattleSystem.h"


/*
This is a text based testing of an RPG battle system

The initiation to the battle is bool battleActive.

The structure of

*/



int main() {



	const int windowWidth = 720;
	const int windowHeight = 480;

	float deltaTime{};

	float waitTime = 5.f;
	int prevIntWait = static_cast<int>(waitTime);

	Player player;
	Enemy enemy;

	BattleSystem battleSystem(&player, &enemy);

	SetRandomSeed(time(NULL));

	bool overworldActive = true;
	bool battleActive = false;
	int userInput;

	InitWindow(windowWidth, windowHeight, "RPG Test");
	SetTargetFPS(60);

	std::cout << "You are walking around the overworld.";

	while (!WindowShouldClose())
	{

		deltaTime = GetFrameTime();
		//std::cout << deltaTime;

		int currentIntWait = static_cast<int>(waitTime);

		if (currentIntWait < prevIntWait) {
			std::cout << ".";
			prevIntWait = currentIntWait;
		}

		if (waitTime > 0) {
			waitTime -= deltaTime;
			//std::cout << waitTime;
		}

		if (waitTime <= 0) {
			battleActive = true;
		}

		while (battleActive) {

			battleSystem.battleState();

		}

		BeginDrawing();


		EndDrawing();



	}

	return 0;

	

}
	

	



