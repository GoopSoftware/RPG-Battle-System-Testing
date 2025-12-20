#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "ContentIds.h"


struct WeightedTemplateChoice {
    std::string templateId;
    int weight = 1;
};

struct ZoneEncounterTable {
    std::string id;
    std::vector<WeightedTemplateChoice> encounters;
};

struct ZoneEncounterTablesDb {
    int schemaVersion = 1;
    std::unordered_map<std::string, ZoneEncounterTable> byZoneId;
};
