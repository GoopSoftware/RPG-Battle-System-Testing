#pragma once
#include "ContentDatabase.h"
#include "EncounterTypes.h"
class EncounterGenerator
{
public:
	EncounterGenerator(const ContentDatabase& db);

	EncounterResult generate(const std::string& zoneId) const;

private:

	const ContentDatabase db;

	static int rollCount(int minCounter, int maxCount);
	static std::string pickWeightedTemplate(const ZoneEncounterTable& table);
	static std::string pickWeightedEnemy(const EncounterGroup& group);

};

