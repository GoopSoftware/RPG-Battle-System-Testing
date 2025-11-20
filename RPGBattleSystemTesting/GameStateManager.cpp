#include "GameStateManager.h"


/*
GameState owns the big picture of the program. It is the brain and glue of the operation
- Current State (OVERWORLD, BATTLE, MENU, etc
- Entity Component stores



update() - manages the state system

Facilitates transferring of data between other Systems unless unnecessary
Does:
	- Overworld.generateBattle() -> BattleSystem

Doesn't:
	- RenderSystem
	- DebugSystem
*/

GameStateManager::GameStateManager(
	float deltaTime,
	std::vector<Entity> players,
	TextureManager& textureManager,
	std::unordered_map<Entity, HealthComponent>& healthStore,
	std::unordered_map<Entity, CombatStatsComponent>& statsStore,
	std::unordered_map<Entity, NameComponent>& nameStore,
	std::unordered_map<Entity, SpriteComponent>& spriteStore,
	std::unordered_map<Entity, PositionComponent>& positionStore) :
		
		deltaTime(deltaTime),
		players(std::move(players)),
		textureManager(textureManager),
		overworld(textureManager),
		animationSystem(spriteStore),
		healthStore(healthStore),
		statsStore(statsStore),
		nameStore(nameStore),
		spriteStore(spriteStore),
		positionStore(positionStore)
{
	std::cout << "[GSM] TextureManager Goblin ID: "
		<< textureManager.goblinTexture.id << "\n";

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

	animationSystem.update(deltaTime);

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