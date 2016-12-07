# Picayune_Porter
Starcraft Broodwar AI

This solution is made specifically to support development and deployment on the Windows platform only.

Installation and Compilation Instructions

1. Unzip all files into your BWAPI folder.  This should create the following directory structure:
.../BWAPI/Picayune_Porter/PicayunePorterBot/Picayune_Porter_Config.txt

2. Import project into Visual Studio 2013 or later
3. If your version of Visual Studio is later than 2013, you must set each project to use build platform v120 (VS 2013).
4. Compile solution
4. a. If linker errors are encountered, set "Whole Program Optimization" to "No Whole Program Optimization" for each project.

FIXME: A list of modules or functions you changed or added (filenames, function names). To evaluate your code we will only look at the listed code segments

Changed/added filenames and function declarations

BuildingPlacer.cpp
-bool BuildingPlacer::tileBlocksAddon(BWAPI::TilePosition position) const
-bool BuildingPlacer::canBuildHereWithSpace(BWAPI::TilePosition position,const Building & b,int buildDist, bool horizontalOnly, bool verticalOnly) const
-BWAPI::TilePosition BuildingPlacer::getBuildLocationNear(const Building & b, int buildDist, bool horizontalOnly) const
-BWAPI::TilePosition BuildingPlacer::getBunkerBuildLocationNear(const Building & b,int buildDist,bool horizontalOnly) const
-BWAPI::TilePosition BuildingPlacer::getTurretBuildLocationNear(const Building & b, int buildDist, bool horizontalOnly) const
-bool BuildingPlacer::isAreaMonitered(BWAPI::Position position, BWAPI::Unitset turrets,  int bufferdist) const

BuildingPlacer.h
ComsatManager.cpp
ComsatManager.h
MarineManager.cpp
MarineManager.h
VultureManager.cpp
VultureManager.h




