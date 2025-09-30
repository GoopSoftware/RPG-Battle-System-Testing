#include "GameStateManager.h"


GameStateManager::GameStateManager
	(std::vector<Entity> players,
	std::unordered_map<Entity, HealthComponent>& healthStore,
	std::unordered_map<Entity, CombatStatsComponent>& statsStore,
	std::unordered_map<Entity, NameComponent>& nameStore) {

}

GameStateManager::~GameStateManager() {

}

void GameStateManager::triggerEncounter() {
	std::cout << "Encounter triggered\n";
		
}

void GameStateManager::update() {

	switch (currentState) {

	case GameState::OVERWORLD:
		overworld.update();
		if (overworld.getEncounter()) {
			currentState = GameState::BATTLE;
		}
		break;

	case GameState::BATTLE:
		std::cout << "In a battle!\n";
		break;

	case GameState::MENU:
		break;

	case GameState::GAME_OVER:
		break;
	}


}