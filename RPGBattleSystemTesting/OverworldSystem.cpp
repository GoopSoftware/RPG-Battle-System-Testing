#include "OverworldSystem.h"
#include "RenderSystem.h"


/*
Handles all overworld logic of the program

- WASD movement
- Rolls random encounters
- Created the random encounter information and sends to BattleSystem through an Encounter.h struct

*/



OverworldSystem::OverworldSystem(std::unordered_map<Entity, HealthComponent>& healthStore,
								std::unordered_map<Entity, CombatStatsComponent>& statsStore,
								std::unordered_map<Entity, NameComponent>& nameStore,
								std::unordered_map<Entity, SpriteComponent>& spriteStore,
								std::unordered_map<Entity, PositionComponent>& positionStore) :
	healthStore(healthStore),
	statsStore(statsStore),
	nameStore(nameStore),
	spriteStore(spriteStore),
	positionStore(positionStore)

{

}

OverworldSystem::~OverworldSystem() {

}



void OverworldSystem::update(const OverworldMap& map) {

	int speed = 5;
	PositionComponent& pos = positionStore[overworldPlayer];

	moveDirection = MoveDirection::None;
	bool moved = false;

	float newX = pos.x;
	float newY = pos.y;

	if (IsKeyDown(KEY_W)) {
		newY -= speed;
		moveDirection = MoveDirection::Up;
		moved = true;
	}
	if (IsKeyDown(KEY_S)) {
		newY += speed;
		moveDirection = MoveDirection::Down;
		moved = true;
	}
	if (IsKeyDown(KEY_A)) {
		newX -= speed;
		moveDirection = MoveDirection::Left;
		moved = true;
	}
	if (IsKeyDown(KEY_D)) {
		newX += speed;
		moveDirection = MoveDirection::Right;
		moved = true;
	}

	if (!isBlocked(map, newX, newY)) {
		pos.x = newX;
		pos.y = newY;
	}

	if (moved) { encounterCheck(); }

	// Debug instant battle
	if (IsKeyPressed(KEY_B)) { encounter = true; }
}

bool OverworldSystem::isBlocked(const OverworldMap& map, float x, float y) const {
	int tileX = static_cast<int>(x) / map.tileWidth;
	int tileY = static_cast<int>(y) / map.tileHeight;

	// Safety check
	if (tileX < 0 || tileY < 0 || tileX >= map.width || tileY >= map.height)
		return true; // treat outside map as blocked

	return map.collision[tileY * map.width + tileX] != 0;
}

void OverworldSystem::encounterCheck() {
	// Checks 1/10 chance for a battle to happen then hands off to GameStateManager
	// GameState sees the flag inside its update()
	int encounterRoll = GetRandomValue(1, 10000);
	if (encounterRoll == 1) { 
		encounter = true;
		std::cout << "Encountered a Battle!\n";
	}
}

void OverworldSystem::initializePlayer() {

	overworldPlayer = createEntity();
	positionStore[overworldPlayer] = { 320.f, 320.f };

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


std::vector<Vector2> OverworldSystem::calculateEnemyEncounterPosition(int totalEnemies, float screenWidth, float screenHeight) {
	std::vector<Vector2> positions;
	positions.reserve(totalEnemies);

	float centerX = screenWidth * .5f;
	float baseY = screenHeight * 0.58f;

	float clusterWidth = 0.0f;

	switch (totalEnemies) {
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

	if (totalEnemies == 1) {
		positions.push_back({ centerX, baseY });
		return positions;
	}

	float startX = centerX - (clusterWidth / 2.f);

	float spacing = clusterWidth / (totalEnemies - 1);


	for (int i = 0; i < totalEnemies; i++) {
		float x = startX + spacing * i;
		float y = baseY;
		positions.push_back({ x, y });
	}

	return positions;
}

void OverworldSystem::draw(RenderSystem& renderer) const {

}
