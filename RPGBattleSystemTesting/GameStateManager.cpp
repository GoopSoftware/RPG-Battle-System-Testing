#include "GameStateManager.h"


GameStateManager::GameStateManager
	(std::vector<Entity> players,
	std::unordered_map<Entity, HealthComponent>& healthStore,
	std::unordered_map<Entity, CombatStatsComponent>& statsStore,
	std::unordered_map<Entity, NameComponent>& nameStore) {

}

GameStateManager::~GameStateManager() {

}


void GameStateManager::update() {

	switch (currentState) {

	case GameState::OVERWORLD:
		overworld.update();
		break;

	case GameState::BATTLE:
		break;

	case GameState::MENU:
		break;

	case GameState::GAME_OVER:
		break;
	}


}