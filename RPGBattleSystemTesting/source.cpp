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
#include "RenderSystem.h"
#include "TextureManager.h"

/*
This is an rpg battle system programmed to explore ECS style of game development

an Entity is just an ID then you assign values to the entity using unordered maps. These maps
are then passed into Systems alongside the Entity ID. The idea is to prevent 100s of classes
for different enemy types.
*/




int main() {

	// Initializing components
	std::unordered_map<Entity, HealthComponent> healthStore;
	std::unordered_map<Entity, CombatStatsComponent> statsStore;
	std::unordered_map<Entity, NameComponent> nameStore;
	std::unordered_map<Entity, SpriteComponent> spriteStore;
	std::unordered_map<Entity, PositionComponent> positionStore;
	
	Entity player1 = createEntity();
	nameStore[player1] = { "Dave" };
	healthStore[player1] = { 200, 200 };
	statsStore[player1] = { 20, 5, 25 };
	
	Entity player2 = createEntity();
	nameStore[player2] = { "Steve" };
	healthStore[player2] = { 200, 200 };
	statsStore[player2] = { 20, 5, 11 };

	Entity player3 = createEntity();
	nameStore[player3] = { "Frank" };
	healthStore[player3] = { 200, 200 };
	statsStore[player3] = { 20, 5, 11 };
	
	std::vector<Entity> players;
	players.push_back(player1);
	players.push_back(player2);
	players.push_back(player3);


	const int windowWidth = 1080;
	const int windowHeight = 720;
	float deltaTime{};

	Texture2D texture;
	// Vector purely for holding all textures to unload on game end
	std::vector<Texture2D> textures;
	TextureManager textureManager;
	GameStateManager game(deltaTime, players, textureManager, healthStore, statsStore, nameStore, spriteStore, positionStore);
	RenderSystem renderer(windowWidth, windowHeight, windowWidth, windowHeight);



	
	SetRandomSeed(static_cast<unsigned int>(time(NULL)));
	renderer.init();

	Texture2D testTex = LoadTexture("Assets/Orc-Idle.png");
	textures.push_back(testTex);
	textureManager.goblinTexture = testTex;
	textureManager.debugPrint();


	while (!WindowShouldClose())
	{

		deltaTime = GetFrameTime();

		game.update();
		renderer.begin();
		renderer.render(game);
		//DrawTexture(testTex, 100, 100, WHITE);
		renderer.end();

	}

	return 0;

	

}