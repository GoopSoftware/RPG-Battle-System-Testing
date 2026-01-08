#include "EncounterGenerator.h"
#include <raylib.h>


EncounterGenerator::EncounterGenerator(const ContentDatabase& db) : 
	db(db) {
	
}

int EncounterGenerator::rollCount(int minCount, int maxCount) {
	if (maxCount < minCount) maxCount = minCount;
	return GetRandomValue(minCount, maxCount);
}

std::string EncounterGenerator::pickWeightedTemplate(const ZoneEncounterTable& table) {

	int total = 0;
	for (const auto& enemy : table.encounters) {
		total += enemy.weight;
	}

	int roll = GetRandomValue(1, total);
	int running = 0;

	for (const auto& encounter : table.encounters) {
		running += encounter.weight;
		if (roll <= running) {
			return encounter.templateId;
		}
	}
	return table.encounters.back().templateId;

}

std::string EncounterGenerator::pickWeightedEnemy(const EncounterGroup& group) {
	int total = 0;

	for (const auto& choice : group.choices) {
		total += choice.weight;
	}

	int roll = GetRandomValue(1, total);
	int running = 0;

	for (const auto& choice : group.choices) {
		running += choice.weight;
		if (roll <= running) {
			return choice.enemyId;
		}
	}
	return group.choices.back().enemyId;
}

EncounterResult EncounterGenerator::generate(const std::string& zoneId) const {
	EncounterResult out;
	out.zoneId = zoneId;

	const ZoneEncounterTable& zoneTable = db.zoneTables.byZoneId.at(zoneId);
	out.templateId = pickWeightedTemplate(zoneTable);

	const EncounterTemplate& templ = db.encounterTemplates.byId.at(out.templateId);

	out.encounterName = templ.displayName;
	out.difficulty = templ.difficulty;
	out.formation = templ.formation;

	for (const auto& group : templ.groups) {
		int count = rollCount(group.minCount, group.maxCount);
		for (int i = 0; i < count; i++) {
			out.enemyIds.push_back(pickWeightedEnemy(group));
		}
	}
	return out;
}