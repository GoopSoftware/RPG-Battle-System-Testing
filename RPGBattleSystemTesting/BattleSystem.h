#pragma once

#include "Entity.h"
#include "HealthComponent.h"
#include "CombatStatsComponent.h"
#include <unordered_map>
#include <iostream>


extern std::unordered_map<Entity, HealthComponent> healthStore;
extern std::unordered_map<Entity, CombatStatsComponent> statsStore;


using Entity = std::uint32_t;


enum TurnOrder {
	PLAYER_TURN,
	ENEMY_TURN
};

class BattleSystem
{
public:
	BattleSystem(Entity player, Entity enemy);
	~BattleSystem();

	void update();
	bool isActive() const { return battleActive; }
	bool battleActive = false;


	void attack(Entity attacker, Entity defender);
	void defend(Entity attacker, Entity defender);
	void calculateTurnOrder(Entity player, Entity enemy);

	int calculateDamage(const CombatStatsComponent& attacker,
						const CombatStatsComponent& defender);
	void turnResolution(Entity player, Entity enemy);

private:

	Entity player;
	Entity enemy;

	int userInput{};

	enum State {
		START,
		PLAYERTURN,
		ACTIONRESOLUTION,
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

