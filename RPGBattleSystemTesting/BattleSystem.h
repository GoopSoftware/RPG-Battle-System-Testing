#pragma once

#include <unordered_map>
#include <iostream>
#include <vector>
#include <deque>
#include <string>

#include "Entity.h"
#include "HealthComponent.h"
#include "CombatStatsComponent.h"
#include "nameComponent.h"
#include "DebugSystem.h"




using Entity = std::uint32_t;

enum class BattleResult {
	NONE,
	VICTORY,
	DEFEAT,
	RUN
};

enum TurnOrder {
	PLAYER_TURN,
	ENEMY_TURN
};

enum class PlayerPhase {
	ActionMenu,
	TargetMenu,
	Done
};

class BattleSystem
{
public:
	BattleSystem(std::vector<Entity> players,
		std::vector<Entity> enemies,
		std::unordered_map<Entity, HealthComponent>& healthStore,
		std::unordered_map<Entity, CombatStatsComponent>& statsStore,
		std::unordered_map<Entity, NameComponent>& nameStore

	);
	~BattleSystem();

	void update();
	bool isActive() const { return battleActive; }

	void attack(Entity attacker, Entity defender);
	void defend(Entity defender);

	BattleResult getResult() const { return result; }
	void calculateTurnOrder();
	void turnResolution();

	void populateEnemyTargets();
	int calculateDamage(const CombatStatsComponent& attacker,
		const CombatStatsComponent& defender);



private:

	//void handlePlayerTurn();
	void handleAttackOption();
	void handleDefendOption();
	void handleRunOption();
	void handleInvalidOption();


	void removeDefeatedFromTurnOrder();
	bool checkDefeatCondition();
	bool checkVictoryCondition();

	// actor currently making their move
	Entity currentEntity{};

	std::vector<Entity> players;
	std::vector<Entity> livingPlayers;

	std::vector<Entity> enemies;
	std::vector<Entity> livingEnemies;


	std::unordered_map<Entity, HealthComponent>& healthStore;
	std::unordered_map<Entity, CombatStatsComponent>& statsStore;
	std::unordered_map<Entity, NameComponent>& nameStore;

	int userInput{};
	bool validAction = false;

	enum State {
		START,
		PLAYERTURN,
		ENEMYTURN,
		CHECKBATTLESTATUS,
		VICTORY,
		DEFEAT,
		RUN,
		END
	};
	State state{ START };
	TurnOrder turn;

	bool battleActive = true;

	BattleResult result = BattleResult::NONE;
	PlayerPhase playerPhase = PlayerPhase::ActionMenu;

	// These will be fed into RenderSystem 
	int actionIndex = 0;
	std::vector<std::string> actionOptions{ "Attack", "Defend", "Run" };
	int targetIndex = 0;
	std::vector<Entity> targetCandidates;
	// End RenderSystem


	void updatePlayerTurn();

	std::vector<Entity> turnOrder;
	int currentTurnIndex = 0;

	// CONSOLE DEBUG
	bool printedActionMenu;
	void printTurnOptions();
	void printInitialTurnOrder();


	// FUTURE BATTLE LOGGING
	std::deque<std::string> battleLog;
	void logBattle(const std::string& msg);
	bool debugPrint = true;
	DebugSystem debug;

};