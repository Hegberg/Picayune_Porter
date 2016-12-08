# Picayune_Porter
CMPUT350: Starcraft Broodwar AI Final Project
D'Arcy Hamilt, Chris Hegberg Hegberg, Michael Morris
University of Alberta, 2016
-------------------------------------------------------------------------------------------------------------------

NOTE: This solution is made specifically to support development and deployment on the Windows platform only.

-------------------------------------------------------------------------------------------------------------------
Installation and Compilation Instructions
-------------------------------------------------------------------------------------------------------------------
1. Unzip all files into your BWAPI folder.  This should create the following directory structure:
.../BWAPI/Picayune_Porter/PicayunePorterBot/Picayune_Porter_Config.txt

2. Import project into Visual Studio 2013 or later
3. If your version of Visual Studio is later than 2013, you must set each project to use build platform v120 (VS 2013).
4. Compile solution
4. a. If linker errors are encountered, set "Whole Program Optimization" to "No Whole Program Optimization" for each project.

-------------------------------------------------------------------------------------------------------------------
Added and Changed Files and Functions
-------------------------------------------------------------------------------------------------------------------

All references of "UAlbertaBot" in all files renamed to "Picayune_Porter".

-------------------------------------------------------------------------------------------------------------------
New Files
-------------------------------------------------------------------------------------------------------------------
ComsatManager.cpp / ComsatManager.h
DetectorManager.cpp / DetectorManager.h
MarineManager.cpp / MarineManager.h
VultureManager.cpp / VultureManager.h

-------------------------------------------------------------------------------------------------------------------
Changed Files
-------------------------------------------------------------------------------------------------------------------
BuildingData.h
Config.cpp

-------------------------------------------------------------------------------------------------------------------
BuildingManager.cpp / BuildingManager.h
-------------------------------------------------------------------------------------------------------------------
-void BuildingManager::assignWorkersToUnassignedBuildings()
-void BuildingManager::checkForDeadTerranBuilders() 
-BWAPI::TilePosition BuildingManager::getBuildingLocation(const Building & b)
-void BuildingManager::checkForStartedConstruction()
-std::vector<BWAPI::UnitType> BuildingManager::buildingsQueued()

-------------------------------------------------------------------------------------------------------------------
BuildingPlacer.cpp / BuildingPlacer.h
-------------------------------------------------------------------------------------------------------------------
-bool BuildingPlacer::tileBlocksAddon(BWAPI::TilePosition position) const
-bool BuildingPlacer::canBuildHereWithSpace(BWAPI::TilePosition position,const Building & b,int buildDist, bool horizontalOnly, bool verticalOnly) const
-BWAPI::TilePosition BuildingPlacer::getBuildLocationNear(const Building & b, int buildDist, bool horizontalOnly) const
-BWAPI::TilePosition BuildingPlacer::getBunkerBuildLocationNear(const Building & b,int buildDist,bool horizontalOnly) const
-BWAPI::TilePosition BuildingPlacer::getTurretBuildLocationNear(const Building & b, int buildDist, bool horizontalOnly) const
-bool BuildingPlacer::isAreaMonitered(BWAPI::Position position, BWAPI::Unitset turrets,  int bufferdist) const

-------------------------------------------------------------------------------------------------------------------
GameState.cpp / GameState.h
-------------------------------------------------------------------------------------------------------------------
-GameState::GameState(BWAPI::GameWrapper & game, BWAPI::PlayerInterface * self, c

-------------------------------------------------------------------------------------------------------------------
InformationManager.cpp / InformationManager.h
-------------------------------------------------------------------------------------------------------------------
-bool InformationManager::isCombatUnit(BWAPI::UnitType type) const
-bool InformationManager::hasMobileDetector(BWAPI::Player player) const
-void InformationManager::getNearbyForce(std::vector<UnitInfo> & unitInfo, BWAPI::Position p, BWAPI::Player player, int radius) 

-------------------------------------------------------------------------------------------------------------------
MapTools.cpp / MapTools.h
-------------------------------------------------------------------------------------------------------------------
-int MapTools::getGroundDistance(BWAPI::Position origin,BWAPI::Position destination...

-------------------------------------------------------------------------------------------------------------------
Micro.cpp / Micro.h
-------------------------------------------------------------------------------------------------------------------
-void Micro::SmartRightClick(BWAPI::Unit unit, BWAPI::Unit target)
-void Micro::SmartLaySpiderMine(BWAPI::Unit unit, BWAPI::Position pos)
-void Micro::SmartAttackUnit(BWAPI::Unit attacker, BWAPI::Unit target)

-------------------------------------------------------------------------------------------------------------------
ProductionManager.cpp / ProductionManager.h
-------------------------------------------------------------------------------------------------------------------
-void ProductionManager::update()
-void ProductionManager::manageBuildOrderQueue()
-BWAPI::Unit ProductionManager::getProducer(MetaType t, BWAPI::Position closestTo

-------------------------------------------------------------------------------------------------------------------
Squad.cpp / Squad.h
-------------------------------------------------------------------------------------------------------------------
-void Squad::update()
-BWAPI::Unit Squad::unitClosestToEnemy()
-void Squad::addUnitsToMicroManagers()
-bool Squad::needsToRegroup()
-void Squad::setAllUnits()

-------------------------------------------------------------------------------------------------------------------
StrategyManager.cpp / StrategyManager.h
-------------------------------------------------------------------------------------------------------------------
-const int StrategyManager::shouldExpandNow() const
-const MetaPairVector StrategyManager::getTerranBuildOrderGoal() const

-------------------------------------------------------------------------------------------------------------------
UnitUtil.cpp / UnitUtil.h
-------------------------------------------------------------------------------------------------------------------
-bool UnitUtil::IsCombatUnit(BWAPI::Unit unit)

-------------------------------------------------------------------------------------------------------------------
DFBB_BuildOrderStackSearch.cpp / DFBB_BuildOrderStackSearch.h
-------------------------------------------------------------------------------------------------------------------
-void DFBB_BuildOrderStackSearch::DFBB()