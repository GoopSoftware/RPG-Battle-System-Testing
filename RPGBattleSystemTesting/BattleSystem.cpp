#include "BattleSystem.h"
#include "raylib.h"
#include <algorithm>

BattleSystem::BattleSystem(Entity player, Entity enemy) :
	player(player), enemy(enemy)
{

}

BattleSystem::~BattleSystem() {

}

std::unordered_map<Entity, HealthComponent> healthStore;
std::unordered_map<Entity, CombatStatsComponent> statsStore;


void BattleSystem::calculateTurnOrder(Entity player, Entity enemy) {
	turnOrder.clear();
	turnOrder.push_back(player);
	turnOrder.push_back(enemy);

	std::sort(turnOrder.begin(), turnOrder.end(), [](Entity a, Entity b) {
		return statsStore[a].speed > statsStore[b].speed;
		});

	currentTurnIndex = 0;
	state = (turnOrder[currentTurnIndex] == player) ? PLAYERTURN : ENEMYTURN;
	std::cout << state << std::endl;
}


int BattleSystem::calculateDamage(const CombatStatsComponent& attacker,
	const CombatStatsComponent& defender)
{
	float damageBoost = GetRandomValue(1, attacker.attack * 1.5);
	float dmg = (attacker.attack * 1.5) - defender.defense + damageBoost;
	return (dmg > 0) ? dmg : 1;
}


void BattleSystem::turnResolution(Entity player, Entity enemy) {
	currentTurnIndex = (currentTurnIndex + 1) % turnOrder.size();
	state = (turnOrder[currentTurnIndex] == player) ? PLAYERTURN : ENEMYTURN;
}


void BattleSystem::attack(Entity attacker, Entity defender) {
	auto& attackerStats = statsStore[attacker];
	auto& defenderStats = statsStore[defender];
	auto& defenderHealth = healthStore[defender];

	int damage = calculateDamage(attackerStats, defenderStats);
	defenderHealth.hp -= damage;

	std::cout << "Entity " << attacker
		<< " hit Entity " << defender
		<< " for " << damage
		<< " damage! Remaining HP: " << defenderHealth.hp
		<< std::endl;

	if (defenderHealth.hp <= 0) {
		std::cout << "Entity " << defender << " has been defeated!" << std::endl;
	}
}

//void BattleSystem::enemyTurn


void BattleSystem::update() {

	switch (state) {
	case START:
		std::cout << "\n--------------------Battle--------------------" << std::endl;
		// This is where we can run speed calculations to see who goes first
		calculateTurnOrder(player, enemy);
		break;

	case PLAYERTURN:
		std::cout << "\n1. Attack\n" << "2. Defend\n" << "3. Run" << std::endl;
		std::cin >> userInput;
		state = ACTIONRESOLUTION;
		break;

	case ENEMYTURN:
		std::cout << "Enemy takes their turn" << std::endl;
		attack(enemy, player);
		state = CHECKBATTLESTATUS;
		break;

	case ACTIONRESOLUTION:
		std::cout << userInput << std::endl;
		if (userInput == 1) {
			attack(player, enemy);
		}
		else if (userInput == 2) {
			std::cout << "Your input was: " << userInput << std::endl;
		}
		else if (userInput == 3) {
			std::cout << "Your input was: " << userInput << std::endl;
			int runDecision = GetRandomValue(1, 3);
			if (runDecision == 1 || runDecision == 2) {
				std::cout << "You successfully ran.\n";
				// Running logic
				state = END;
			}
			else {
				std::cout << "You failed to run.\n";
			}
		}
		else {
			std::cout << "Invalid input. Please enter 1, 2, or 3" << std::endl;
		}

		if (state != END) {
			state = CHECKBATTLESTATUS;
		}
		break;


	case CHECKBATTLESTATUS:
	{
		auto& playerHealth = healthStore[player];
		auto& enemyHealth = healthStore[enemy];

		if (playerHealth.hp <= 0) state = DEFEAT;

		else if (enemyHealth.hp <= 0) state = VICTORY;

		else {

			turnOrder.erase(
				std::remove_if(turnOrder.begin(), turnOrder.end(),
					[](Entity e) { return healthStore[e].hp <= 0; }),
				turnOrder.end()
			);

			currentTurnIndex = (currentTurnIndex + 1) % turnOrder.size();


			if (currentTurnIndex >= turnOrder.size()) {
				currentTurnIndex = 0;
			}

			state = (turnOrder[currentTurnIndex] == player) ? PLAYERTURN : ENEMYTURN;
		}

		break;
	}
	case VICTORY:

		std::cout << "Congratulations you won!";
		state = END;
		break;

	case DEFEAT:

		break;

	case END:

		// END LOGIC BREAK OUT OF LOOP END PROGRAM
		battleActive = false;
		break;
	}


}