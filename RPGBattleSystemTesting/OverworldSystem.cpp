#include "OverworldSystem.h"
#include "RenderSystem.h"


/*
Handles all overworld logic of the program

- WASD movement
- Rolls random encounters
- Created the random encounter information and sends to BattleSystem through an Encounter.h struct

*/



OverworldSystem::OverworldSystem(TextureManager& textureManager) :
	textureManager(textureManager)
{
	std::cout << "[Overworld] GoblinTexture ID at construction: "
		<< textureManager.goblinTexture.id << "\n";
}

OverworldSystem::~OverworldSystem() {

}



void OverworldSystem::update() {
	if (IsKeyPressed(KEY_W)) {
		debug.log("OverworldSystem", LogLevel::INPUT, "Moved Up.");
		partyPosition.y -= 1;
		std::cout << "X: " << partyPosition.x << "Y: " << partyPosition.y << std::endl;
		encounterCheck();
	}
	if (IsKeyPressed(KEY_S)) {
		partyPosition.y += 1;
		debug.log("OverworldSystem", LogLevel::INPUT, "Moved Down.");
		std::cout << "X: " << partyPosition.x << "Y: " << partyPosition.y << std::endl;
		encounterCheck();
	}
	if (IsKeyPressed(KEY_A)) {
		partyPosition.x -= 1;
		debug.log("OverworldSystem", LogLevel::INPUT, "Moved Left.");
		std::cout << "X: " << partyPosition.x << "Y: " << partyPosition.y << std::endl;
		encounterCheck();
	}
	if (IsKeyPressed(KEY_D)) {
		partyPosition.x += 1;
		debug.log("OverworldSystem", LogLevel::INPUT, "Moved Right.");
		std::cout << "X: " << partyPosition.x << "Y: " << partyPosition.y << std::endl;
		encounterCheck();
	}

	if (IsKeyPressed(KEY_B)) {
		encounter = true;
	}
}

void OverworldSystem::encounterCheck() {
	// Checks 1/10 chance for a battle to happen then hands off to GameStateManager
	// GameState sees the flag inside its update()
	int encounterRoll = GetRandomValue(1, 10);
	if (encounterRoll == 1) { 
		encounter = true;
		std::cout << "Encountered a Battle!\n";
	}
}


Encounter OverworldSystem::generateEncounter(std::unordered_map<Entity, HealthComponent>& healthStore,
											 std::unordered_map<Entity, CombatStatsComponent>& statsStore,
											 std::unordered_map<Entity, NameComponent>& nameStore,
											 std::unordered_map<Entity, SpriteComponent>& spriteStore,
											 std::unordered_map<Entity, PositionComponent>& positionStore
) 
{
	Encounter encounter;
	encounter.difficulty = 1;
	encounter.zone = "Forest";
	encounter.encounterName = "Test Battle";

	for (int i = 0; i < 3; i++) {
		Entity enemy = createEntity();

		healthStore[enemy] = { 30, 30 };
		statsStore[enemy] = { 5 + GetRandomValue(0, 3), 2, 5 };
		nameStore[enemy] = { "Goblin_" + std::to_string(i + 1) };

		SpriteComponent sprite;
		sprite.texture = textureManager.goblinTexture;
		sprite.maxFrames = 6;
		sprite.frameWidth = sprite.texture.width / sprite.maxFrames;
		sprite.frameHeight = sprite.texture.height;
		sprite.scale = 3.0f;

		spriteStore[enemy] = sprite;

		PositionComponent pos;
		pos.x = 100 + (i * 250);              
		pos.y = 100;    
		positionStore[enemy] = pos;

		auto& stats = statsStore[enemy];
		auto& health = healthStore[enemy];
		std::cout << "Created " << nameStore[enemy].name
			<< " (Health: " << health.hp
			<< " (Atk: " << stats.attack
			<< ", Def: " << stats.defense
			<< ", Spd: " << stats.speed << ")\n";
		encounter.enemies.push_back(enemy);
	}

	return encounter;
}

void OverworldSystem::draw(RenderSystem& renderer) const {

}
