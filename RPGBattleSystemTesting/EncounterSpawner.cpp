#include "EncounterSpawner.h"

#include <cctype>
#include <algorithm>
#include <stdexcept>

#include "TextureManager.h"
#include "raylib.h"


EncounterSpawner::EncounterSpawner(
    const ContentDatabase& contentDb,
    std::unordered_map<Entity, HealthComponent>& healthStore,
    std::unordered_map<Entity, CombatStatsComponent>& statsStore,
    std::unordered_map<Entity, NameComponent>& nameStore,
    std::unordered_map<Entity, SpriteComponent>& spriteStore,
    std::unordered_map<Entity, PositionComponent>& positionStore
)
    : contentDb(contentDb),
    healthStore(healthStore),
    statsStore(statsStore),
    nameStore(nameStore),
    spriteStore(spriteStore),
    positionStore(positionStore)
{
}


static std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
        [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return s;
}

AnimationState EncounterSpawner::toAnimationState(const std::string& stateStr) {
    // Match your enum names: Idle, Walk, Attack, Attack2, Attack3, Hurt, Dead
    const std::string s = toLower(stateStr);

    if (s == "idle") return AnimationState::Idle;
    if (s == "walk") return AnimationState::Walk;
    if (s == "attack") return AnimationState::Attack;
    if (s == "attack2") return AnimationState::Attack2;
    if (s == "attack3") return AnimationState::Attack3;
    if (s == "hurt") return AnimationState::Hurt;
    if (s == "dead") return AnimationState::Dead;

    // If unknown, default to Idle (or throw if you prefer)
    return AnimationState::Idle;
}

Encounter EncounterSpawner::spawn(const EncounterResult& result) {
    Encounter encounter;
    encounter.zone = result.zoneId;
    encounter.difficulty = result.difficulty;
    encounter.encounterName = result.encounterName;

    const int enemyCount = static_cast<int>(result.enemyIds.size());
    const auto positions = calculateEnemyEncounterPosition(enemyCount, (float)GetScreenWidth(), (float)GetScreenHeight());

    for (int i = 0; i < enemyCount; ++i) {
        const std::string& enemyId = result.enemyIds[i];
        const EnemyBlueprint& bp = contentDb.enemyBlueprints.byId.at(enemyId);

        Entity enemy = createEntity();

        // Health
        healthStore[enemy] = { bp.health.maxHp, bp.health.maxHp };

        // Stats
        statsStore[enemy] = { bp.stats.attack, bp.stats.defense, bp.stats.speed };

        // Name (Goblin_1 etc). Use baseName from blueprint.
        nameStore[enemy] = { bp.baseName + "_" + std::to_string(i + 1) };

        // Sprite
        SpriteComponent sprite;
        sprite.texture = TextureManager::Get().Get(bp.sprite.textureKey);
        sprite.columns = bp.sprite.columns;
        sprite.rows = bp.sprite.rows;

        sprite.frameWidth = sprite.texture.width / sprite.columns;
        sprite.frameHeight = sprite.texture.height / sprite.rows;

        sprite.scale = bp.sprite.scale;

        // Animations: convert strings -> enum
        sprite.animations.clear();
        for (const auto& clip : bp.sprite.animations) {
            AnimationState st = toAnimationState(clip.state);
            sprite.animations[st] = { clip.startFrame, clip.endFrame, clip.frameTime };
        }

        spriteStore[enemy] = sprite;

        // Position
        PositionComponent pos;
        pos.x = positions[i].x;
        pos.y = positions[i].y;
        positionStore[enemy] = pos;

        encounter.enemies.push_back(enemy);
    }

    return encounter;
}




std::vector<Vector2> EncounterSpawner::calculateEnemyEncounterPosition(int totalEnemies, float screenWidth, float screenHeight) {
    std::vector<Vector2> positions;
    positions.reserve(totalEnemies);

    float centerX = screenWidth * 0.5f;
    float baseY = screenHeight * 0.58f;

    float clusterWidth = 0.0f;

    switch (totalEnemies) {
    case 1: clusterWidth = 0.0f; break;
    case 2: clusterWidth = screenWidth * 0.25f; break;
    case 3: clusterWidth = screenWidth * 0.6f; break;
    case 4: clusterWidth = screenWidth * 0.7f; break;
    default: clusterWidth = screenWidth * 0.7f; break;
    }

    if (totalEnemies <= 1) {
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