#pragma once
#include "OverworldSystem.h"
#include <memory>
#include "BattleSystem.h"

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



private:

	GameState currentState = GameState::OVERWORLD;
	OverworldSystem overworld;
	std::unique_ptr<BattleSystem> battle;
	// MenuSystem menu;                                   ----- For when MenuSystem is done -----
};

