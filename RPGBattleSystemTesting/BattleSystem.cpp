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


void BattleSystem::handleAttackOption() {

	// Clear livingEnemies vector in case an enemy died
	livingEnemies.clear();
	// loop through enemeis and assign if hp > 0
	for (auto e : enemies) {
		if (healthStore[e].hp > 0) {
			livingEnemies.push_back(e);

			// Print out living enemies if user selected attack
			std::cout << "   " << livingEnemies.size()
				<< ") " << nameStore[e].name
				<< " (HP: " << healthStore[e].hp << ")\n";
		}
	}

	// Redundant but will show if for some reason game breaks
	if (livingEnemies.empty()) {
		std::cout << "\n>>> No enemies left to attack!\n";
		std::cout << "\n>>> Something broke in the code to show this\n";
		validAction = true;
		return;
	}

	// seitialize choice to 0 and ask user for input on which enemy to attack
	int choice = 0;
	while (choice < 1 || choice > static_cast<int>(livingEnemies.size())) {
		std::cout << "\nChoose a target (1-" << livingEnemies.size() << "): ";
		std::cin >> choice;
	}
	// TODO: In future add option to cancel attack and go back a screen


	attack(currentEntity, livingEnemies[choice - 1]);
	validAction = true;

}


void BattleSystem::handleDefendOption() {
	defend(currentEntity);
	validAction = true;
}

void BattleSystem::handleRunOption() {

	// 66% chance to run. This will change in the future
	// TODO: Add calculation based on average speed values of enemies and players
	int runDecision = GetRandomValue(1, 3);
	if (runDecision <= 2) {
		std::cout << "\n>>> You successfully ran away!\n";
		state = END;
	}
	else {
		std::cout << "\n>>> You tried to run, but failed!\n";
	}
	validAction = true;

}

void BattleSystem::handleInvalidOption() {
	std::cout << "\nInvalid choice. Please try again.\n";
}


void BattleSystem::handlePlayerTurn(int userInput) {
	std::cout << "\n" << nameStore[currentEntity].name << "'s turn\n";
	std::cout << "1. Attack\n" << "2. Defend\n" << "3. Run" << std::endl;
	std::cin >> userInput;

	switch (userInput) {

	case 1:
		handleAttackOption();
		break;

	case 2:
		handleDefendOption();
		break;

	case 3:
		handleRunOption();
		break;

	default:
		handleInvalidOption();
		break;
	}
}

void BattleSystem::printTurnOrder() {
	std::cout << "\n-- Turn Order --\n";
	for (size_t i = 0; i < turnOrder.size(); i++) {
		Entity e = turnOrder[i];
		std::cout << " " << (i + 1) << ") " << nameStore[e].name
			<< " (Speed: " << statsStore[e].speed << ")\n";
	}
	std::cout << "----------------\n";
}

void BattleSystem::populateEnemyTargets() {
	// Clears livingPlayers vector in case players died, 
	// checks players vector, if hp > 0 add to livingPlayers vector

	livingPlayers.clear();
	for (auto p : players) {
		if (healthStore[p].hp > 0) {
			livingPlayers.push_back(p);
		}
	}
}

void BattleSystem::update() {

	switch (state) {
	case START:
		std::cout << "\n==================== BATTLE START ====================\n";
		// This is where we can run speed calculations to see who goes first
		calculateTurnOrder();
		// printTurnOrder();           This function is debug to ensure correct calculations
		turnResolution();

		break;

	case PLAYERTURN:

		handlePlayerTurn(userInput);
		
		// Pretty sure this is redundant
		if (state != END) {
			state = CHECKBATTLESTATUS;
		}

		break;

	case ENEMYTURN:
		// Placeholder enemy logic
		// In future add enemy ai logic to decide healing/spells/attack/run

		populateEnemyTargets();

		// if livingPlayers isnt empty Picks a random living player and attacks
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
		// TODO: Create victory boolean for future logic
		state = END;
		break;

	case DEFEAT:
		std::cout << "\n Defeat... Your party has fallen.\n";
		// TODO: Create defeat boolean for future logic
		state = END;
		break;

	case END:

		// set battleActive to false and leave the combat system
		battleActive = false;
		break;
	}

}