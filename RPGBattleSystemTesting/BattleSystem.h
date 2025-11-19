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
#include "RenderSystem.h"


class RenderSystem;

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
		std::unordered_map<Entity, NameComponent>& nameStore,
		std::unordered_map<Entity, SpriteComponent>& spriteStore,
		std::unordered_map<Entity, PositionComponent>& positionStore);
	~BattleSystem();

	void update();
	bool isActive() const { return battleActive; }

	void attack(Entity attacker, Entity defender);
	void defend(Entity defender);

	bool isTargeting() const { return playerPhase == PlayerPhase::TargetMenu; }
	int getTargetIndex() const { return targetIndex; }

	//result = (condition) ? value_if_true : value_if_false;

	const std::vector<Entity>& getEnemies() const { return enemies; }
	const std::vector<Entity>& getLivingEnemies() const { return livingEnemies; }

	BattleResult getResult() const { return result; }
	void calculateTurnOrder();
	void turnResolution();

	void populateEnemyTargets();
	int calculateDamage(const CombatStatsComponent& attacker,
		const CombatStatsComponent& defender);
	void draw(RenderSystem& renderer) const;


private:

	void handleAttackOption();
	void handleDefendOption();
	void handleRunOption();
	void handleInvalidOption();

	void buildLivingPlayers();
	void buildLivingEnemies();

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
	std::unordered_map<Entity, SpriteComponent>& spriteStore;
	std::unordered_map<Entity, PositionComponent>& positionStore;

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
	bool targeting = false;
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
	DebugSystem debug;

};