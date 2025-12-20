#pragma once
#include <string>
#include <vector>
#include "ContentIds.h"


struct EncounterResult {
    std::string zoneId;
    std::string templateId;
    std::string encounterName;
    int difficulty = 1;

    std::string formation;
    std::vector<std::string> enemyIds;
};
