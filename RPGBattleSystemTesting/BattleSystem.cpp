#include "BattleSystem.h"
#include "raylib.h"
#include <algorithm>

BattleSystem::BattleSystem( std::vector<Entity> players,
							std::vector<Entity> enemies,
							std::unordered_map<Entity, HealthComponent>& healthStore,
							std::unordered_map<Entity, CombatStatsComponent>& statsStore,
							std::unordered_map<Entity, NameComponent>& nameStore

	) :
	players(players),
	enemies(enemies), 
	healthStore(healthStore), 
	statsStore(statsStore),
	nameStore(nameStore)
{

}

BattleSystem::~BattleSystem() {

}


void BattleSystem::calculateTurnOrder() {

	// Calculates the initial turn order of the battle based on speed values
	turnOrder.clear();
	turnOrder.insert(turnOrder.end(), players.begin(), players.end());
	turnOrder.insert(turnOrder.end(), enemies.begin(), enemies.end());

	std::sort(turnOrder.begin(), turnOrder.end(), 
		[this](Entity a, Entity b) {
		return statsStore[a].speed > statsStore[b].speed;
		});

	currentTurnIndex = -1;
}


int BattleSystem::calculateDamage(const CombatStatsComponent& attacker,
	const CombatStatsComponent& defender)
{
	float damageBoost = GetRandomValue(1, static_cast<int>(attacker.attack * 1.5));
	float dmg = (attacker.attack * 1.5) - defender.defense + damageBoost;
	return (dmg > 0) ? dmg : 1;
}


void BattleSystem::turnResolution() {
	// Handles moving to the next turn

	// Advance turn index
	currentTurnIndex = (currentTurnIndex + 1) % turnOrder.size();
	currentEntity = turnOrder[currentTurnIndex];


	// Decide if its player or enemy
	if (std::find(players.begin(), players.end(), currentEntity) != players.end()) {
		state = PLAYERTURN;
	}
	else {
		state = ENEMYTURN;
	}
}


void BattleSystem::attack(Entity attacker, Entity defender) {

	int damage = calculateDamage(statsStore[attacker], statsStore[defender]);
	healthStore[defender].hp -= damage;


    std::cout << "\n>>> " << nameStore[attacker].name
              << " attacks " << nameStore[defender].name
              << " for " << damage << " damage!"
              << "\n    " << nameStore[defender].name
              << " HP: " << std::max(0, healthStore[defender].hp) << "\n";


	if (healthStore[defender].hp <= 0) {
		std::cout << "     " << nameStore[defender].name
			<< " has been defeated!\n";
	}
}

void BattleSystem::defend(Entity defender) {
	// Logic for defence. No currenty planned system so for now we just permenantly increase by 5
	statsStore[defender].defense += 5;
	std::cout << "\n>>> " << nameStore[defender].name
			  << " defends and raises defense!\n";
}

void BattleSystem::handlePlayerTurn(int userInput) {
	if (userInput == 1) {

		livingEnemies.clear();
		for (auto e : enemies) {
			if (healthStore[e].hp > 0) {
				livingEnemies.push_back(e);
				std::cout << "   " << livingEnemies.size() 
                          << ") " << nameStore[e].name
                          << " (HP: " << healthStore[e].hp << ")\n";
			}
		}

		if (livingEnemies.empty()) {
			std::cout << "\n>>> No enemies left to attack!\n";
			validAction = true;
		}
		else {
			int choice = 0;
			while (choice < 1 || choice > static_cast<int>(livingEnemies.size())) {
				std::cout << "\nChoose a target (1-" << livingEnemies.size() << "): ";
				std::cin >> choice;
			}
			attack(currentEntity, livingEnemies[choice - 1]);
			validAction = true;
		}

	}
	else if (userInput == 2) {
		defend(currentEntity);
		validAction = true;
	}
	else if (userInput == 3) {
		int runDecision = GetRandomValue(1, 3);
		if (runDecision <= 2) {
			std::cout << "\n>>> You successfully ran away!\n";
			// Running logic
			state = END;
		}
		else {
			std::cout << "\n>>> You tried to run, but failed!\n";
		}
		validAction = true;
	}
	else {
		std::cout << "\nInvalid choice. Please try again.\n";
	}
}

void BattleSystem::update() {

	switch (state) {
	case START:
		std::cout << "\n==================== BATTLE START ====================\n";
		// This is where we can run speed calculations to see who goes first
		calculateTurnOrder();
		turnResolution();
		break;

	case PLAYERTURN:

		std::cout << "\n1. Attack\n" << "2. Defend\n" << "3. Run" << std::endl;
		std::cin >> userInput;

		handlePlayerTurn(userInput);
		

		if (state != END) {
			state = CHECKBATTLESTATUS;
		}

		break;

	case ENEMYTURN:
		// Placeholder enemy logic


		// Picks a random living player
		livingPlayers.clear();
		for (auto p : players) {
			if (healthStore[p].hp > 0) {
				livingPlayers.push_back(p);
			}
		}
		// if above vector isnt empty attack its current player
		if (!livingPlayers.empty()) {
			Entity target = livingPlayers[GetRandomValue(0, livingPlayers.size() - 1)];
			attack(currentEntity, target);
		}

		state = CHECKBATTLESTATUS;

		break;


	case CHECKBATTLESTATUS:
	{
		// Removes dead entities from turnOrder
		turnOrder.erase(
			std::remove_if(turnOrder.begin(), turnOrder.end(),
				[this](Entity e) {return healthStore[e].hp <= 0; }),
			turnOrder.end()
		);

		// Check if all players are dead
		bool allPlayersDead = std::all_of(players.begin(), players.end(),
			[this](Entity e) {return healthStore[e].hp <= 0; });
		
		// Check if all enemies are dead
		bool allEnemiesDead = std::all_of(enemies.begin(), enemies.end(),
			[this](Entity e) {return healthStore[e].hp <= 0; });

		if (allPlayersDead) {
			state = DEFEAT;
		}

		else if (allEnemiesDead) {
			state = VICTORY;
		}

		else {
			turnResolution();
		}

		break;
	}
	case VICTORY:
		std::cout << "\n Victory! All enemies defeated!\n";
		state = END;
		break;

	case DEFEAT:
		std::cout << "\n Defeat... Your party has fallen.\n";
		state = END;
		break;

	case END:

		// END LOGIC BREAK OUT OF LOOP END PROGRAM
		battleActive = false;
		break;
	}


}