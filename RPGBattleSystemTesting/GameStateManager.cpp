#include "GameStateManager.h"
#include "EnemyBlueprints.h"
#include "EnemyBlueprintsLoader.h"
#include "EncounterTemplates.h"
#include "ZoneEncounterTables.h"
#include "EncounterTemplatesLoader.h"
#include "ZoneEncounterTablesLoader.h"
#include "EncounterGenerator.h"
#include "EncounterSpawner.h"


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
	std::unordered_map<Entity, HealthComponent>& healthStore,
	std::unordered_map<Entity, CombatStatsComponent>& statsStore,
	std::unordered_map<Entity, NameComponent>& nameStore,
	std::unordered_map<Entity, SpriteComponent>& spriteStore,
	std::unordered_map<Entity, PositionComponent>& positionStore) :
		
		deltaTime(deltaTime),
		players(std::move(players)),
		animationSystem(spriteStore),
		healthStore(healthStore),
		statsStore(statsStore),
		nameStore(nameStore),
		spriteStore(spriteStore),
		positionStore(positionStore),
		overworld(healthStore, statsStore, nameStore, spriteStore, positionStore)

{


}


GameStateManager::~GameStateManager() {

}



void GameStateManager::init() {
	// This is the master initilization of the game. When the game loads this is the first
	// thing that is ran

	// Loads all textures
	// TODO: Make TextureLoader helper for TextureManager to load all textures in 1 one go
	auto& TM = TextureManager::Get();
	TM.Load("Goblin", "assets/Orc.png");
	TM.Load("Soldier", "assets/Soldier.png");
	TM.Load("BattleBG", "assets/battlebg.png");
	// TODO: This stays in init() for now then when we have multiple overworld maps to load from we develop a system to change based on location
	OverworldMapLoader::Load("assets/maps/TestMapjson.json", overworldMap);


	contentDb.enemyBlueprints = loadEnemyBlueprintsFromFile("content/data/enemy_blueprints.json");
	contentDb.encounterTemplates = loadEncounterTemplatesFromFile("content/data/encounter_templates.json");
	contentDb.zoneTables = loadZoneEncounterTablesFromFile("content/data/zone_encounters.json");

	std::cout << "Loaded enemies: " << contentDb.enemyBlueprints.byId.size() << "\n";
	std::cout << "Loaded templates: " << contentDb.encounterTemplates.byId.size() << "\n";
	std::cout << "Loaded zones: " << contentDb.zoneTables.byZoneId.size() << "\n";

	encounterGenerator = std::make_unique<EncounterGenerator>(contentDb);
	encounterSpawner = std::make_unique<EncounterSpawner>(
		contentDb, healthStore, statsStore, nameStore, spriteStore, positionStore
	);

	overworld.initializePlayer();
}

void GameStateManager::triggerEncounter() {

	EncounterResult r = encounterGenerator->generate("Forest"); // later: dynamic zone


	std::cout << "[Encounter] " << r.encounterName
		<< " template=" << r.templateId
		<< " enemies=" << r.enemyIds.size()
		<< " formation=" << r.formation << "\n";
	for (auto& id : r.enemyIds) {
		std::cout << "  enemyId: " << id << "\n";
	}

	currentEncounter = encounterSpawner->spawn(r);

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

		overworld.update(overworldMap);

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