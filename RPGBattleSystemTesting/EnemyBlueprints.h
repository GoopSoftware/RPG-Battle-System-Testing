#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "ContentIds.h"


struct HealthData {
	int maxHp = 1;
};


struct CombatStatsData {
	int attack = 1;
	int defense = 0;
	int speed = 1;
};

	
struct AnimationClipData {
	std::string state;
	int startFrame = 0;
	int endFrame = 0;
	float frameTime = 0.15f;
};


struct SpriteBlueprintData {
	std::string textureKey;

	int columns = 1;
	int rows = 1;

	float scale = 1.0f;

	std::vector<AnimationClipData> animations;
};


struct EnemyBlueprint {
	std::string id;

	std::string displayName;
	std::string baseName;
	std::vector<std::string> tags;

	HealthData health;
	CombatStatsData stats;
	SpriteBlueprintData sprite;
};


struct EnemyBlueprintsDb {
	int schemaVersion = 1;
	std::unordered_map<std::string, EnemyBlueprint> byId;
};
