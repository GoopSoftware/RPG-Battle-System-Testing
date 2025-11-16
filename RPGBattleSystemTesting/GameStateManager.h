#pragma once
#include <memory>

#include "OverworldSystem.h"
#include "BattleSystem.h"
#include "Encounter.h"

enum class GameState {
	OVERWORLD,
	BATTLE,
	MENU,
	GAME_OVER
};

class GameStateManager
{
public:
	GameStateManager(std::vector<Entity> players,
					 std::unordered_map<Entity, HealthComponent>& healthStore,
					 std::unordered_map<Entity, CombatStatsComponent>& statsStore,
					 std::unordered_map<Entity, NameComponent>& nameStore);
	~GameStateManager();

	void update();
	void triggerEncounter();
	GameState getCurrentState() const { return currentState; }
	
	//OverworldSystem& getOverworld() { return overworld; }
	const OverworldSystem& getOverworld() const { return overworld; }

	//BattleSystem* getBattleSystem() { return battleSystem.get(); }
	const BattleSystem* getBattleSystem() const { return battleSystem.get(); }


private:

	GameState currentState = GameState::OVERWORLD;
	OverworldSystem overworld;
	// MenuSystem menu;  ----- For when MenuSystem is done -----


	std::vector<Entity> players;
	std::unordered_map<Entity, HealthComponent>& healthStore;
	std::unordered_map<Entity, CombatStatsComponent>& statsStore;
	std::unordered_map<Entity, NameComponent>& nameStore;

	Encounter currentEncounter;
	std::unique_ptr<BattleSystem> battleSystem; // ptr created when battle triggered

};

