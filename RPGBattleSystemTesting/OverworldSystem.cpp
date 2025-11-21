#include "OverworldSystem.h"
#include "RenderSystem.h"


/*
Handles all overworld logic of the program

- WASD movement
- Rolls random encounters
- Created the random encounter information and sends to BattleSystem through an Encounter.h struct

*/



OverworldSystem::OverworldSystem()
{

}

OverworldSystem::~OverworldSystem() {

}



void OverworldSystem::update() {
	if (IsKeyPressed(KEY_W)) {
		DebugSystem::log("OverworldSystem", LogLevel::INPUT, "Moved Up.");
		partyPosition.y -= 1;
		std::cout << "X: " << partyPosition.x << "Y: " << partyPosition.y << std::endl;
		encounterCheck();
	}
	if (IsKeyPressed(KEY_S)) {
		partyPosition.y += 1;
		DebugSystem::log("OverworldSystem", LogLevel::INPUT, "Moved Down.");
		std::cout << "X: " << partyPosition.x << "Y: " << partyPosition.y << std::endl;
		encounterCheck();
	}
	if (IsKeyPressed(KEY_A)) {
		partyPosition.x -= 1;
		DebugSystem::log("OverworldSystem", LogLevel::INPUT, "Moved Left.");
		std::cout << "X: " << partyPosition.x << "Y: " << partyPosition.y << std::endl;
		encounterCheck();
	}
	if (IsKeyPressed(KEY_D)) {
		partyPosition.x += 1;
		DebugSystem::log("OverworldSystem", LogLevel::INPUT, "Moved Right.");
		std::cout << "X: " << partyPosition.x << "Y: " << partyPosition.y << std::endl;
		encounterCheck();
	}

	if (IsKeyPressed(KEY_B)) {
		encounter = true;
	}
}

void OverworldSystem::encounterCheck() {
	// Checks 1/10 chance for a battle to happen then hands off to GameStateManager
	// GameState sees the flag inside its update()
	int encounterRoll = GetRandomValue(1, 10);
	if (encounterRoll == 1) { 
		encounter = true;
		std::cout << "Encountered a Battle!\n";
	}
}

void OverworldSystem::initializePlayer(std::unordered_map<Entity, PositionComponent>& positionStore,
									   std::unordered_map<Entity, SpriteComponent>& spriteStore) {

	overworldPlayer = createEntity();
	positionStore[overworldPlayer] = { 320.f, 320.f };

	SpriteComponent overworldSprite;
	overworldSprite.texture = TextureManager::Get().Get("Goblin");
	overworldSprite.columns = 8;
	overworldSprite.rows = 6;
	overworldSprite.frameWidth = overworldSprite.texture.width / overworldSprite.columns;
	overworldSprite.frameHeight = overworldSprite.texture.height / overworldSprite.rows;
	overworldSprite.animations = {
								{ AnimationState::Idle,     { 0, 5, 0.15f } },
								{ AnimationState::Walk,     { 8, 15, 0.15f } },
								{ AnimationState::Attack,   { 16, 21, 0.15f } },
								{ AnimationState::Attack2,  { 24, 29, 0.15f } },
								{ AnimationState::Hurt,     { 32, 36, 0.15f } },
								{ AnimationState::Dead,     { 40, 44, 0.15f } }
	};
	overworldSprite.scale = 3.0f;
	spriteStore[overworldPlayer] = overworldSprite;

	std::cout << "[Overworld] Player initialized (entity " << overworldPlayer << ")\n";

}


Encounter OverworldSystem::generateEncounter(std::unordered_map<Entity, HealthComponent>& healthStore,
											 std::unordered_map<Entity, CombatStatsComponent>& statsStore,
											 std::unordered_map<Entity, NameComponent>& nameStore,
											 std::unordered_map<Entity, SpriteComponent>& spriteStore,
											 std::unordered_map<Entity, PositionComponent>& positionStore
) 
{
	Encounter encounter;
	encounter.difficulty = 1;
	encounter.zone = "Forest";
	encounter.encounterName = "Test Battle";
	int enemyCount = 4;
	
	std::vector<Vector2> positions = calculateEnemyPosition(enemyCount, GetScreenWidth(), GetScreenHeight());

	for (int i = 0; i < enemyCount; i++) {
		
		Entity enemy = createEntity();

		healthStore[enemy] = { 30, 30 };
		statsStore[enemy] = { 5 + GetRandomValue(0, 3), 2, 5 };
		nameStore[enemy] = { "Goblin_" + std::to_string(i + 1) };

		// TODO Make this scalable
		SpriteComponent sprite;
		sprite.texture = TextureManager::Get().Get("Goblin");
		sprite.columns = 8;
		sprite.rows = 6;
		sprite.frameWidth = sprite.texture.width / sprite.columns;
		sprite.frameHeight = sprite.texture.height / sprite.rows;
		sprite.animations = {
			{ AnimationState::Idle,     { 0, 5, 0.15f } },
			{ AnimationState::Walk,     { 8, 15, 0.15f } },
			{ AnimationState::Attack,   { 16, 21, 0.15f } },
			{ AnimationState::Attack2,  { 24, 29, 0.15f } },
			{ AnimationState::Hurt,     { 32, 36, 0.15f } },
			{ AnimationState::Dead,     { 40, 44, 0.15f } }
		};
		sprite.scale = 3.0f;

		spriteStore[enemy] = sprite;

		PositionComponent pos;
		pos.x = positions[i].x;
		pos.y = positions[i].y;
		positionStore[enemy] = pos;
		std::cout << "PositionsX: " << positions[i].x << std::endl;
		std::cout << "PosX: " << pos.x << std::endl;

		auto& stats = statsStore[enemy];
		auto& health = healthStore[enemy];
		std::cout << "Created " << nameStore[enemy].name
			<< " (Health: " << health.hp
			<< " (Atk: " << stats.attack
			<< ", Def: " << stats.defense
			<< ", Spd: " << stats.speed << ")\n";

		encounter.enemies.push_back(enemy);
	}

	return encounter;
}

std::vector<Vector2> OverworldSystem::calculateEnemyPosition(int total, float screenWidth, float screenHeight) {
	std::vector<Vector2> positions;
	positions.reserve(total);

	float centerX = screenWidth * .5f;
	float baseY = screenHeight * 0.58f;

	float clusterWidth = 0.0f;

	switch (total) {
	case 1: 
		clusterWidth = 0.0f; 
		break;
	case 2:
		clusterWidth = screenWidth * .25f;
		break;
	case 3:
		clusterWidth = screenWidth * .6f;;
		break;
	case 4:
		clusterWidth = screenWidth * .7f;
		break;
	default:
		clusterWidth = screenWidth * .7f;
		break;
	}

	if (total == 1) {
		positions.push_back({ centerX, baseY });
		return positions;
	}

	float startX = centerX - (clusterWidth / 2.f);

	float spacing = clusterWidth / (total - 1);


	for (int i = 0; i < total; i++) {
		float x = startX + spacing * i;
		float y = baseY;
		positions.push_back({ x, y });
	}

	return positions;
}

void OverworldSystem::draw(RenderSystem& renderer) const {

}
