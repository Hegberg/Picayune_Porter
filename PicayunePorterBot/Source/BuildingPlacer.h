#pragma once

#include "Common.h"
#include "BuildingData.h"
#include "MetaType.h"
#include "InformationManager.h"

namespace Picayune_Porter
{


class BuildingPlacer
{
    BuildingPlacer();

    std::vector< std::vector<bool> > _reserveMap;

    int     _boxTop;
    int	    _boxBottom;
    int	    _boxLeft;
    int	    _boxRight;

    void    computeBuildableTileDistance(BWAPI::TilePosition tp);

public:

    static BuildingPlacer & Instance();

    // queries for various BuildingPlacer data
    bool					buildable(const Building & b,int x,int y) const;
    bool					isReserved(int x,int y) const;
    bool					isInResourceBox(int x,int y) const;
    bool					tileOverlapsBaseLocation(BWAPI::TilePosition tile,BWAPI::UnitType type) const;
    bool                    tileBlocksAddon(BWAPI::TilePosition position) const;

    BWAPI::TilePosition		GetBuildLocation(const Building & b,int padding) const;

    // determines whether we can build at a given location
    bool					canBuildHere(BWAPI::TilePosition position,const Building & b) const;
    bool					canBuildHereWithSpace(BWAPI::TilePosition position,const Building & b,int buildDist,bool horizontalOnly = false,bool verticalOnly = false) const;

    // returns a build location near a building's desired location
    BWAPI::TilePosition		getBuildLocationNear(const Building & b,int buildDist,bool horizontalOnly = false) const;

	// Created by Micheal Morris and D'Arcy Hamilton.   
	// Preferentially places Bunkers near chokepoints. Is not very clever.
	BWAPI::TilePosition     getBunkerBuildLocationNear(const Building & b, int buildDist, bool horizontalOnly) const;

	// Created by D'Arcy Hamilton.   
	// Spreads out Missile Turrets to get a full anti stealth coverage
	BWAPI::TilePosition     getTurretBuildLocationNear(const Building & b, int buildDist, bool horizontalOnly) const;
	// Created by D'Arcy Hamilton.  
	// checks if a tile is within sight of a Missile Turret, minus a set amount
	bool					isAreaMonitered(BWAPI::Position position, BWAPI::Unitset turrets, int bufferdist = 0) const;

    void					reserveTiles(BWAPI::TilePosition position,int width,int height);
    void					freeTiles(BWAPI::TilePosition position,int width,int height);

    void					drawReservedTiles();
    void					computeResourceBox();

    BWAPI::TilePosition		getRefineryPosition();

};
}
