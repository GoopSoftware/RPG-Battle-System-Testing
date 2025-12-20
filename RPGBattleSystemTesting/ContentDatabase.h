#pragma once
#include "EnemyBlueprints.h"
#include "EncounterTemplates.h"
#include "ZoneEncounterTables.h"


struct ContentDatabase {
    EnemyBlueprintsDb enemyBlueprints;
    EncounterTemplatesDb encounterTemplates;
    ZoneEncounterTablesDb zoneTables;
};
