#include "BattleSystem.h"
#include "RenderSystem.h"
#include "raylib.h"
#include <algorithm>
#include "SpriteComponent.h"
#include "PositionComponent.h"



/*
Battle System Code:

This code is the random encounter system for the RPG

It uses 5 vectors to work
	Players[]
	Enemies[]
	turnOrder[]
	livingPlayers[]
	livingEnemies[]
	Health, stats, name stores (reference)
	sprite, position (reference)

It first assigns turn order based on the speed value of the party vs the speed value of enemies populating turnOrder.
Then it changes to the next players turn through the turnResolution() function changing state respectively.
First index takes their turn then passes over the state to CHECKBATTLESTATUS.
CHECKBATTLESTATUS checks for dead players, removes from livingVector, checks victory/defeat condition
then passes to turnResolution.

If player runs it changes to RUN state


TODO:
	- Add magic
	- Add items



Flow:
Start -> PLAYERTURN/ENEMYTURN -> CHECKBATTLESTATUS -> PLAYERTURN/ENEMYTURN -> CHECKBATTLESTATUS -> ...

*/



BattleSystem::BattleSystem(std::vector<Entity> players,
							std::vector<Entity> enemies,
							std::unordered_map<Entity, HealthComponent>& healthStore,
							std::unordered_map<Entity, CombatStatsComponent>& statsStore,
							std::unordered_map<Entity, NameComponent>& nameStore,
							std::unordered_map<Entity, SpriteComponent>& spriteStore,
							std::unordered_map<Entity, PositionComponent>& positionStore

) :
	players(players),
	enemies(enemies),
	healthStore(healthStore),
	statsStore(statsStore),
	nameStore(nameStore),
	spriteStore(spriteStore),
	positionStore(positionStore)
{

}

BattleSystem::~BattleSystem() {

}


void BattleSystem::calculateTurnOrder() {
	// Runs calculations and populates the turnOrder vector
	// 
	// Calculates the initial turn order of the battle based on speed values
	turnOrder.clear();
	turnOrder.insert(turnOrder.end(), players.begin(), players.end());
	turnOrder.insert(turnOrder.end(), enemies.begin(), enemies.end());

	// Sorts beginning to end based on statsStore.speed or entities
	std::sort(turnOrder.begin(), turnOrder.end(),
		[this](Entity a, Entity b) {
			return statsStore[a].speed > statsStore[b].speed;
		});

	currentTurnIndex = -1;
}


int BattleSystem::calculateDamage(const CombatStatsComponent& attacker,
	// All calculations for damage between entities goes here
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

	buildLivingEnemies();
	buildLivingPlayers();

	// Decide if its player or enemy
	if (std::find(players.begin(), players.end(), currentEntity) != players.end()) {
		state = PLAYERTURN;
	}
	else {
		state = ENEMYTURN;
	}
}


void BattleSystem::attack(Entity attacker, Entity defender) {
	// All 
// ic of attacking goees here

	int damage = calculateDamage(statsStore[attacker], statsStore[defender]);
	healthStore[defender].hp -= damage;

	
	DebugSystem::log("BattleSystem", LogLevel::INFO, "[INFO]>>> " + nameStore[attacker].name + " attacks " + nameStore[defender].name + " for "
		+ std::to_string(damage) + " damage!" + "\n    " + nameStore[defender].name + " HP: "
		+ std::to_string(std::max(0, healthStore[defender].hp)));



	if (healthStore[defender].hp <= 0) {
		DebugSystem::log("BattleSystem", LogLevel::INFO, nameStore[defender].name + " has been defeated!");

	}
}

void BattleSystem::defend(Entity defender) {
	// Logic for defence. No currenty planned system so for now we just permenantly increase by 5
	statsStore[defender].defense += 5;

	DebugSystem::log("BattleSystem", LogLevel::INPUT, ">>> " + nameStore[defender].name + " defends and raises defense!");
}


void BattleSystem::handleAttackOption() {

	// Redundant but will show if for some reason game breaks
	if (livingEnemies.empty()) {
		DebugSystem::log("BattleSystem", LogLevel::DEBUG, "No enemies left to attack!\n>>> Something broke in the code to show this");

		validAction = true;
		return;
	}

	targetCandidates = livingEnemies;
	targetIndex = 0;

	DebugSystem::log("BattleSystem", LogLevel::INPUT, ">>> " + nameStore[currentEntity].name + " chose ATTACK.");
	DebugSystem::log("BattleSystem", LogLevel::INFO, "Choose your target:");
	
	for (size_t i = 0; i < targetCandidates.size(); ++i) {
		DebugSystem::log("BattleSystem", LogLevel::INFO, std::to_string((i + 1)) + ") " + nameStore[targetCandidates[i]].name + " (HP: " + std::to_string(healthStore[targetCandidates[i]].hp) + ")");
	}
	
	DebugSystem::log("BattleSystem", LogLevel::INFO, "Use W / S to select.Press ENTER to confirm, ESC to cancel.");

	playerPhase = PlayerPhase::TargetMenu;


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
		DebugSystem::log("BattleSystem", LogLevel::INFO, ">>> You successfully ran away!");

		state = RUN;
	}
	else {
		DebugSystem::log("BattleSystem", LogLevel::INFO, ">>> You tried to run, but failed!");

		state = CHECKBATTLESTATUS;
	}
	validAction = true;

}

void BattleSystem::handleInvalidOption() {
	DebugSystem::log("BattleSystem", LogLevel::INFO, "Invalid choice. Please try again.");

}


void BattleSystem::buildLivingPlayers() {
	livingPlayers.clear();
	for (auto p : players) {
		if (healthStore[p].hp > 0) {
			livingPlayers.push_back(p);
		}
	}
}

void BattleSystem::buildLivingEnemies() {
	livingEnemies.clear();
	for (auto e : enemies) {
		if (healthStore[e].hp > 0) {
			livingEnemies.push_back(e);
		}
	}
}



void BattleSystem::printTurnOptions() {
	DebugSystem::log("BattleSystem", LogLevel::INFO, nameStore[currentEntity].name + "'s turn");
	DebugSystem::log("BattleSystem", LogLevel::INFO, "1. Attack");
	DebugSystem::log("BattleSystem", LogLevel::INFO, "2. Defend");
	DebugSystem::log("BattleSystem", LogLevel::INFO, "3. Run");

}

void BattleSystem::printInitialTurnOrder() {
	DebugSystem::log("BattleSystem", LogLevel::INFO, "-- Turn Order --");

	for (size_t i = 0; i < turnOrder.size(); i++) {
		Entity e = turnOrder[i];
		DebugSystem::log("BattleSystem", LogLevel::INFO, " " + std::to_string((i + 1)) + ") " + nameStore[e].name + " (Speed: " + std::to_string(statsStore[e].speed) + ")");
	}
	DebugSystem::log("BattleSystem", LogLevel::INFO, "----------------");
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

void BattleSystem::removeDefeatedFromTurnOrder() {
	// Removes dead entities from turnOrder vector
	turnOrder.erase(
		std::remove_if(turnOrder.begin(), turnOrder.end(),
			[this](Entity e) {return healthStore[e].hp <= 0; }),
		turnOrder.end()
	);
}


bool BattleSystem::checkDefeatCondition() {
	// Checks the player vector if std::all_of() players are dead
	return std::all_of(players.begin(), players.end(),
		[this](Entity e) {return healthStore[e].hp <= 0; });
}

bool BattleSystem::checkVictoryCondition() {
	// Checks the enemy vector if std::all_of() players are dead
	return std::all_of(enemies.begin(), enemies.end(),
		[this](Entity e) {return healthStore[e].hp <= 0; });
}


void BattleSystem::updatePlayerTurn() {


	switch (playerPhase) {

		case PlayerPhase::ActionMenu: {

			if (!printedActionMenu) {
				actionIndex = 0;
				printTurnOptions();
				printedActionMenu = true;
				DebugSystem::log("BattleSystem", LogLevel::INPUT, "Currently highlighting : " + actionOptions[actionIndex]);
				DebugSystem::log("BattleSystem", LogLevel::TOOLTIP, "Use W/S to select. Press ENTER to confirm, ESC to cancel.");
			}

			if (IsKeyPressed(KEY_W)) {
				actionIndex = (actionIndex + actionOptions.size() - 1) % actionOptions.size();
				DebugSystem::log("BattleSystem", LogLevel::INPUT, "UP now highlighting: " + actionOptions[actionIndex]);
			}
			if (IsKeyPressed(KEY_S)) {
				actionIndex = (actionIndex + 1) % actionOptions.size();
				DebugSystem::log("BattleSystem", LogLevel::INPUT, "DOWN now highlighting: " + actionOptions[actionIndex]);
			}

			if (IsKeyPressed(KEY_ENTER)) {
				DebugSystem::log("BattleSystem", LogLevel::INPUT, "Enter pressed: " + actionOptions[actionIndex]);

				switch (actionIndex) {
				case 0:
					handleAttackOption();
					break;

				case 1:
					handleDefendOption();
					playerPhase = PlayerPhase::Done;
					break;

				case 2:
					handleRunOption();
					playerPhase = PlayerPhase::Done;
					break;
				}
			}

			break;

		case PlayerPhase::TargetMenu:
			if (targetCandidates.empty()) {
				DebugSystem::log("BattleSystem", LogLevel::INFO, ">>> No valid targets. Returning to ActionMenu.");
				playerPhase = PlayerPhase::Done;
				break;
			}

			// So we can press numbers to select an index
			//if (IsKeyPressed(KEY_ONE)) targetIndex = 0;
			//if (IsKeyPressed(KEY_TWO) && targetCandidates.size() > 1) targetIndex = 1;
			//if (IsKeyPressed(KEY_THREE) && targetCandidates.size() > 2) targetIndex = 2;

			if (IsKeyPressed(KEY_A)) {
				targetIndex = (targetIndex + targetCandidates.size() - 1) % targetCandidates.size();
				DebugSystem::log("BattleSystem", LogLevel::INPUT, "UP now highlighting : " + nameStore[targetCandidates[targetIndex]].name);
			}
			if (IsKeyPressed(KEY_D)) {
				targetIndex = (targetIndex + 1) % targetCandidates.size();
				DebugSystem::log("BattleSystem", LogLevel::INPUT, "DOWN now highlighting : " + nameStore[targetCandidates[targetIndex]].name);
			}

			// select option
			if (IsKeyPressed(KEY_ENTER)) {
				DebugSystem::log("BattleSystem", LogLevel::INPUT, "[CONFIRM] Enter pressed attacking " + nameStore[targetCandidates[targetIndex]].name);
				attack(currentEntity, targetCandidates[targetIndex]);
				playerPhase = PlayerPhase::Done;
			}

			// Cancel back to main action menu
			if (IsKeyPressed(KEY_Q) || IsKeyPressed(KEY_BACKSPACE)) {
				DebugSystem::log("BattleSystem", LogLevel::INPUT, "[CANCEL] Returning to turn decision");

				playerPhase = PlayerPhase::ActionMenu;
				printedActionMenu = false;

				break;
			}

		case PlayerPhase::Done:
			break;


		default:
			break;
		}
	}
}


void BattleSystem::update() {

	switch (state) {
	case START:
		DebugSystem::log("BattleSystem", LogLevel::INFO, "==================== BATTLE START ====================");
		
		// Runs calculations and populates the turnOrder vector
		calculateTurnOrder();
		//printInitialTurnOrder(); // Debug

		// Moves to next players turn
		turnResolution();

		break;

	case PLAYERTURN:

		updatePlayerTurn();

		if (playerPhase == PlayerPhase::Done) {
			// This is super jank and will probably cause issues later but idk how what to do to make it better
			// This is a second check to make sure we can move into state RUN without overwriting with CHECKBATTLESTATUS
			// causing the system to break.
			if (state != RUN) {
				state = CHECKBATTLESTATUS;
			}

			playerPhase = PlayerPhase::ActionMenu;
			targetCandidates.clear();
			printedActionMenu = false;
		}


		break;

	case ENEMYTURN:
		// Placeholder enemy logic
		// In future add enemy ai logic to decide healing/spells/attack/run

		// Runs a check for alive players from players vector and assigns them to livingPLayers vector
		populateEnemyTargets();

		// if livingPlayers isnt empty Picks a random living player and attacks
		if (!livingPlayers.empty()) {
			Entity target = livingPlayers[GetRandomValue(0, livingPlayers.size() - 1)];
			attack(currentEntity, target);
		}

		state = CHECKBATTLESTATUS;
		break;

	case CHECKBATTLESTATUS: {

		// updates turn order 
		removeDefeatedFromTurnOrder();

		if (checkVictoryCondition()) {
			state = VICTORY;
		}
		else if (checkDefeatCondition()) {
			state = DEFEAT;
		}
		else {
			turnResolution();
		}
		break;

	}
	case VICTORY:

		DebugSystem::log("BattleSystem", LogLevel::INFO, "Victory! All enemies defeated!");

		result = BattleResult::VICTORY;

		state = END;
		break;

	case DEFEAT:
		DebugSystem::log("BattleSystem", LogLevel::INFO, "Defeat... Your party has fallen.");

		result = BattleResult::DEFEAT;

		state = END;
		break;

	case RUN:

		result = BattleResult::RUN;


		state = END;
		break;

	case END:

		// set battleActive to false and leave the combat system
		battleActive = false;
		break;
	}

}

void BattleSystem::draw(RenderSystem& renderer) const {
	// Handle all of the drawing of the battles, 
	// it will call methods from RenderSystem to display 
	// Render system calls draw
	// Prevents us from having to transfer tons of information to the render system for displaying

	int textSize = 20;

	int playersXPos = 200;
	int playersYPos = 600;

	int actionsXPos = 75;
	int actionsYPos = 575;

	int enemyXPos = 200;
	int enemyYPos = 200;

	for (int i = 0; i < players.size(); i++) {
		Entity p = players[i];
		std::string name = nameStore.at(p).name;
		std::string health = std::to_string((healthStore).at(p).hp);
		std::string maxHealth = std::to_string((healthStore).at(p).maxHp);
		std::string healthDisplay = name + ": " + health + "/" + maxHealth;
		DrawText(healthDisplay.c_str(), playersXPos, playersYPos, textSize, BLACK);
		playersYPos += 25;
	}

	for (int i = 0; i < actionOptions.size(); i++) {
		if (i == actionIndex) {
			DrawText(">", actionsXPos - 20, actionsYPos, textSize, RED);
		}
		DrawText(actionOptions[i].c_str(), actionsXPos, actionsYPos, textSize, BLACK);
		actionsYPos += 25;
	}

	if (!livingEnemies.empty()) {
		for (int i = 0; i < livingEnemies.size(); i++) {

			Entity e = livingEnemies[i];
			int eX = positionStore.at(e).x;
			int eY = positionStore.at(e).y;
			std::string name = nameStore.at(e).name;
			//DrawText(name.c_str(), enemyXPos, enemyYPos, textSize, BLACK);
			renderer.drawSprite(e, spriteStore.at(e), positionStore.at(e));

		}
	}

}
