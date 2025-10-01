#include "OverworldSystem.h"



OverworldSystem::OverworldSystem()
	
{

}

OverworldSystem::~OverworldSystem() {

}



void OverworldSystem::update() {
	if (IsKeyPressed(KEY_W)) {
		std::cout << "Moved Up\n";
		encounterCheck();
	}
	if (IsKeyPressed(KEY_S)) {
		std::cout << "Moved Down\n";
		encounterCheck();
	}
	if (IsKeyPressed(KEY_A)) {
		std::cout << "Moved Left\n";
		encounterCheck();
	}
	if (IsKeyPressed(KEY_D)) {
		std::cout << "Moved Right\n";
		encounterCheck();
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
		std::cout << "Created " << nameStore[enemy].name
			<< " (Health: " << health.hp
			<< " (Atk: " << stats.attack
			<< ", Def: " << stats.defense
			<< ", Spd: " << stats.speed << ")\n";
		encounter.enemies.push_back(enemy);
	}

	return encounter;
}

