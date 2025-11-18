#include "GameStateManager.h"


GameStateManager::GameStateManager(
	std::vector<Entity> players,
	std::unordered_map<Entity, HealthComponent>& healthStore,
	std::unordered_map<Entity, CombatStatsComponent>& statsStore,
	std::unordered_map<Entity, NameComponent>& nameStore,
	std::unordered_map<Entity, SpriteComponent>& spriteStore,
	std::unordered_map<Entity, PositionComponent>& positionStore) :
	players(std::move(players)),
		healthStore(healthStore),
		statsStore(statsStore),
		nameStore(nameStore),
		spriteStore(spriteStore),
		positionStore(positionStore)
{

}

GameStateManager::~GameStateManager() {

}

void GameStateManager::triggerEncounter() {
	// This functions creates the actual battle using the generated values from generateEncounter()
	// Creates a unique pointer of a BattleSystem
	currentEncounter = overworld.generateEncounter( healthStore, 
													statsStore, 
													nameStore,
													spriteStore,
													positionStore
	);

	for (int i = 0; i < players.size(); i++) {
		std::cout << players[i];
	}
	battleSystem = std::make_unique<BattleSystem>(
		players,
		currentEncounter.enemies,
		healthStore,
		statsStore,
		nameStore,
		spriteStore,
		positionStore
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

				BattleResult result = battleSystem->getResult();

				if (result == BattleResult::VICTORY) {
					std::cout << "Your have won the battle!\n";
					// TODO: Give out xp and loot etc when RewardSystem is done
					currentState = GameState::OVERWORLD;
				}
				else if (result == BattleResult::DEFEAT) {
					std::cout << "Darn you lost GG noob\n";
					currentState = GameState::GAME_OVER;
				}
				else if (result == BattleResult::RUN) {
					std::cout << "Your Escaped!\n";
					currentState = GameState::OVERWORLD;
				}
				else {
					std::cout << "***BUG*** No valid BattleResult Inside GameStateManager.update()";
				}

				//currentState = GameState::OVERWORLD;
				battleSystem.reset();
			}
		}

		break;

	case GameState::MENU:
		break;

	case GameState::GAME_OVER:
		// TODO: Create DefeatSystem and hook in here
		break;
	}


}