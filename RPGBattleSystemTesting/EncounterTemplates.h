#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "ContentIds.h"



struct WeightedEnemyChoice {
	std::string enemyId;
	int weight = 1;
};


struct EncounterGroup {
	int minCount = 1;
	int maxCount = 1;
	std::vector<WeightedEnemyChoice> choices;
};


struct EncounterTemplate {
	std::string id;
	std::string displayName;

	int difficulty = 1;
	std::string formation;

	std::vector<EncounterGroup> groups;
};

	
struct EncounterTemplatesDb {
	int schemaVersion = 1;
	std::unordered_map<std::string, EncounterTemplate> byId;
};


