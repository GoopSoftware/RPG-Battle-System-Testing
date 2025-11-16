#include "OverworldSystem.h"


OverworldSystem::OverworldSystem()
	
{

}

OverworldSystem::~OverworldSystem() {

}



void OverworldSystem::update() {
	if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
		DebugSystem::log("OverworldSystem", LogLevel::INPUT, "Moved Up");
		encounterCheck();
	}
	if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
		DebugSystem::log("OverworldSystem", LogLevel::INPUT, "Moved Down");
		encounterCheck();
	}
	if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) {
		DebugSystem::log("OverworldSystem", LogLevel::INPUT, "Moved Left");
		encounterCheck();
	}
	if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) {
		DebugSystem::log("OverworldSystem", LogLevel::INPUT, "Moved Right");
		encounterCheck();
	}
}

void OverworldSystem::encounterCheck() {
	// Checks 1/10 chance for a battle to happen then hands off to GameStateManager
	// GameState sees the flag inside its update()
	int encounterRoll = GetRandomValue(1, 10);
	if (encounterRoll == 1) { 
		encounter = true;
		DebugSystem::log("OverworldSystem", LogLevel::INFO, "Encountered a Battle!");
	}
}


Encounter OverworldSystem::generateEncounter(std::unordered_map<Entity, HealthComponent>& healthStore,
											 std::unordered_map<Entity, CombatStatsComponent>& statsStore,
											 std::unordered_map<Entity, NameComponent>& nameStore) 
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
		auto& stats = statsStore[enemy];
		auto& health = healthStore[enemy];

		DebugSystem::log("OverworldSystem", LogLevel::INFO, "Created " + nameStore[enemy].name
			+ " (Health: " + std::to_string(health.hp)
			+ " (Atk: " + std::to_string(stats.attack)
			+ ", Def: " + std::to_string(stats.defense)
			+ ", Spd: " + std::to_string(stats.speed) + ")");
		encounter.enemies.push_back(enemy);
	}

	return encounter;
}

