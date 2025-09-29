#include <iostream>
#include <raylib.h>
#include <unordered_map>


#include "Entity.h"
#include "HealthComponent.h"
#include "CombatStatsComponent.h"
#include "BattleSystem.h"
#include "Tags.h"
#include "nameComponent.h"
#include "GameStateManager.h"


/*
This is an rpg battle system programmed to explore ECS style of game development

an Entity is just an ID then you assign values to the entity using unordered maps. These maps
are then passed into Systems alongside the Entity ID. The idea is to prevent 100s of classes
for different enemy types.
*/



int main() {

	std::unordered_map<Entity, HealthComponent> healthStore;
	std::unordered_map<Entity, CombatStatsComponent> statsStore;
	std::unordered_map<Entity, NameComponent> nameStore;
	
	Entity player1 = createEntity();
	nameStore[player1] = { "Dave" };
	healthStore[player1] = { 200, 200 };
	statsStore[player1] = { 20, 5, 25 };
	
	Entity player2 = createEntity();
	nameStore[player2] = { "Steve" };
	healthStore[player2] = { 200, 200 };
	statsStore[player2] = { 20, 5, 11 };
	
	std::vector<Entity> players;
	players.push_back(player1);
	players.push_back(player2);


	/*
	Entity enemy1 = createEntity();
	nameStore[enemy1] = { "Goblin" };
	healthStore[enemy1] = { 100, 100 };
	statsStore[enemy1] = { 8, 2, 10 };

	Entity enemy2 = createEntity();
	nameStore[enemy2] = { "Goblin 2" };
	healthStore[enemy2] = { 100, 100 };
	statsStore[enemy2] = { 8, 2, 10 };
	
	
	std::vector<Entity>enemies;
	enemies.push_back(enemy1);
	enemies.push_back(enemy2);
	*/

	GameStateManager game(players, healthStore, statsStore, nameStore);


	const int windowWidth = 720;
	const int windowHeight = 480;

	float deltaTime{};

	
	SetRandomSeed(static_cast<unsigned int>(time(NULL)));

	InitWindow(windowWidth, windowHeight, "RPG Test");
	SetTargetFPS(60);


	std::cout << "Searching for a battle.";

	while (!WindowShouldClose())
	{

		deltaTime = GetFrameTime();


		game.update();



		BeginDrawing();


		EndDrawing();



	}

	return 0;

	

}