#include "GameStateManager.h"


GameStateManager::GameStateManager(
	std::vector<Entity> players,
	std::unordered_map<Entity, HealthComponent>& healthStore,
	std::unordered_map<Entity, CombatStatsComponent>& statsStore,
	std::unordered_map<Entity, NameComponent>& nameStore) :
	players(std::move(players)),
		healthStore(healthStore),
		statsStore(statsStore),
		nameStore(nameStore)
{

}

GameStateManager::~GameStateManager() {

}

void GameStateManager::triggerEncounter() {
	std::cout << "Encounter triggered\n";
	currentEncounter = overworld.generateEncounter(healthStore, statsStore, nameStore);

	battleSystem = std::make_unique<BattleSystem>(
		players,
		currentEncounter.enemies,
		healthStore,
		statsStore,
		nameStore
	);

}

void GameStateManager::update() {

	switch (currentState) {

	case GameState::OVERWORLD:
		overworld.update();

		if (overworld.getEncounter()) {
			triggerEncounter();
			overworld.clearEncounter();
			currentState = GameState::BATTLE;
		}
		break;

	case GameState::BATTLE:
		if (battleSystem) {

			battleSystem->update();

			if (!battleSystem->isActive()) {
				// TODO: Add victory/defeat logic here
				currentState = GameState::OVERWORLD;
				battleSystem.reset();
			}
		}

		break;

	case GameState::MENU:
		break;

	case GameState::GAME_OVER:
		break;
	}


}