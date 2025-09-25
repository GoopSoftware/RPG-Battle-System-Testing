#pragma once

#include "Entity.h"
#include "HealthComponent.h"
#include "CombatStatsComponent.h"
#include <unordered_map>
#include <iostream>
#include <vector>
#include "nameComponent.h"


using Entity = std::uint32_t;


enum TurnOrder {
	PLAYER_TURN,
	ENEMY_TURN
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
	bool battleActive = false;

	void attack(Entity attacker, Entity defender);
	void defend(Entity defender);

	void calculateTurnOrder();
	void turnResolution();

	int calculateDamage(const CombatStatsComponent& attacker,
						const CombatStatsComponent& defender);

private:

	void handlePlayerTurn(int userInput);

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
		END
	};
	State state{ START };
	TurnOrder turn;


	std::vector<Entity> turnOrder;
	int currentTurnIndex = 0;
};

