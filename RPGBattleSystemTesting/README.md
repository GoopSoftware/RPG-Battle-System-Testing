This project is an olschool RPG game made in C++ using the Raylib library

We use ECS (Entity Compenet Systems) to handle the architecture of the game

Systems
GameStateManager
	- Always runs, 
	- handles the overworld, battle system, menu, and game over states of the game
	- Handles transitions between systems

OverworldSystem
	- Always live but only updates when GameStatemanager is in overworld state
	- Handles:
		- Player Movement
		- Random encounter rolls
		- Random encounter creation
		- Holds Environmnent context to influence the rest of the game
			- E.g. Water temple, Fire temple, Ocean, Forest, etc
	- Returns an encounter struct to GameStateManager
		- Contains:
			- Entity IDs

BattleSystem
	- Pointer created when encounter begins and destroyed when ends
	- Constructed with
		- Players
		- OverworldSystem struct
		- Component stores
			- Player health, player stats, player names, etc
	- Runs battle loop
	- Returns flag Victory or Defeat to GameStateManager


Flow Example
	1. OVERWORLD
		- Player moves
		- rolls n% encoutenr chance
		- Triggers Encounter
		- Feeds info to GameStateManager for battle
		- GameStateManager switches to battle
	2. Battle
		- Construct BattleSystem with info from Overworld
		- Battle runs
		- Ends with victory -> Switches back to GameState OVERWORLD
		- Ends with Defeat -> Switches to GameState GAME_OVER
	3. GAME_OVER
		- Display "You Died"
		- Restart game logic_