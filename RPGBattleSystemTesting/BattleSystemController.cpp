#include "BattleSystemController.h"
#include "CombatSystem.h"

BattleSystemController::BattleSystemController(Entity player, Entity enemy) :
	player(player), enemy(enemy)
{
	
}

BattleSystemController::~BattleSystemController() {

}


void BattleSystemController::update() {

	switch (state) {
	case START:
		std::cout << "--------------------Battle--------------------" << std::endl;
		// This is where we can run speed calculations to see who goes first
		state = PLAYERTURN;
		break;

	case PLAYERTURN:
		std::cout << "\n1. Attack\n" << "2. Defend\n" << "3. Run" << std::endl;
		std::cin >> userInput;
		state = ACTIONRESOLUTION;
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

		state = CHECKBATTLESTATUS;
		break;

	case ENEMYTURN:
		break;
	case CHECKBATTLESTATUS:
		
		{
		auto& playerHealth = healthStore[player];
		auto& enemyHealth = healthStore[enemy];

		if (playerHealth.hp <= 0) state = DEFEAT;
		else if (enemyHealth.hp <= 0) state = VICTORY;
		else state = (state == PLAYERTURN) ? ENEMYTURN : PLAYERTURN;

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