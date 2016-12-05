#include "Common.h"
#include "BuildingPlacer.h"
#include "MapGrid.h"

using namespace Picayune_Porter;

BuildingPlacer::BuildingPlacer()
    : _boxTop       (std::numeric_limits<int>::max())
    , _boxBottom    (std::numeric_limits<int>::lowest())
    , _boxLeft      (std::numeric_limits<int>::max())
    , _boxRight     (std::numeric_limits<int>::lowest())
{
    _reserveMap = std::vector< std::vector<bool> >(BWAPI::Broodwar->mapWidth(),std::vector<bool>(BWAPI::Broodwar->mapHeight(),false));

    computeResourceBox();
}

BuildingPlacer & BuildingPlacer::Instance() 
{
    static BuildingPlacer instance;
    return instance;
}

bool BuildingPlacer::isInResourceBox(int x, int y) const
{
    int posX(x * 32);
    int posY(y * 32);

    return (posX >= _boxLeft) && (posX < _boxRight) && (posY >= _boxTop) && (posY < _boxBottom);
}

void BuildingPlacer::computeResourceBox()
{
    BWAPI::Position start(BWAPI::Broodwar->self()->getStartLocation());
    BWAPI::Unitset unitsAroundNexus;

    for (auto & unit : BWAPI::Broodwar->getAllUnits())
    {
        // if the units are less than 400 away add them if they are resources
        if (unit->getDistance(start) < 300 && unit->getType().isMineralField())
        {
            unitsAroundNexus.insert(unit);
        }
    }

    for (auto & unit : unitsAroundNexus)
    {
        int x = unit->getPosition().x;
        int y = unit->getPosition().y;

        int left = x - unit->getType().dimensionLeft();
        int right = x + unit->getType().dimensionRight() + 1;
        int top = y - unit->getType().dimensionUp();
        int bottom = y + unit->getType().dimensionDown() + 1;

        _boxTop     = top < _boxTop       ? top    : _boxTop;
        _boxBottom  = bottom > _boxBottom ? bottom : _boxBottom;
        _boxLeft    = left < _boxLeft     ? left   : _boxLeft;
        _boxRight   = right > _boxRight   ? right  : _boxRight;
    }

    //BWAPI::Broodwar->printf("%d %d %d %d", boxTop, boxBottom, boxLeft, boxRight);
}

// makes final checks to see if a building can be built at a certain location
bool BuildingPlacer::canBuildHere(BWAPI::TilePosition position,const Building & b) const
{
    /*if (!b.type.isRefinery() && !InformationManager::Instance().tileContainsUnit(position))
    {
    return false;
    }*/

    //returns true if we can build this type of unit here. Takes into account reserved tiles.
    if (!BWAPI::Broodwar->canBuildHere(position,b.type,b.builderUnit))
    {
        return false;
    }

    // check the reserve map
    for (int x = position.x; x < position.x + b.type.tileWidth(); x++)
    {
        for (int y = position.y; y < position.y + b.type.tileHeight(); y++)
        {
            if (_reserveMap[x][y])
            {
                return false;
            }
        }
    }

    // if it overlaps a base location return false
    if (tileOverlapsBaseLocation(position,b.type))
    {
        return false;
    }

    return true;
}

bool BuildingPlacer::tileBlocksAddon(BWAPI::TilePosition position) const
{

    for (int i=0; i<=3; ++i)
    {
        for (auto & unit : BWAPI::Broodwar->getUnitsOnTile(position.x - i,position.y))
        {
            if (unit->getType() == BWAPI::UnitTypes::Terran_Command_Center ||
                unit->getType() == BWAPI::UnitTypes::Terran_Factory ||
                unit->getType() == BWAPI::UnitTypes::Terran_Starport ||
                unit->getType() == BWAPI::UnitTypes::Terran_Science_Facility)
            {
                return true;
            }
        }
    }

    return false;
}

//returns true if we can build this type of unit here with the specified amount of space.
//space value is stored in this->buildDistance.
bool BuildingPlacer::canBuildHereWithSpace(BWAPI::TilePosition position,const Building & b,int buildDist, bool horizontalOnly, bool verticalOnly) const
{
    BWAPI::UnitType type = b.type;

    //if we can't build here, we of course can't build here with space
    if (!canBuildHere(position,b))
    {
        return false;
    }

    // height and width of the building
    int width(b.type.tileWidth());
    int height(b.type.tileHeight());

    //make sure we leave space for add-ons. These types of units can have addons:
    if (b.type==BWAPI::UnitTypes::Terran_Command_Center ||
        b.type==BWAPI::UnitTypes::Terran_Factory ||
        b.type==BWAPI::UnitTypes::Terran_Starport ||
        b.type==BWAPI::UnitTypes::Terran_Science_Facility)
    {
        width += 3;
    }

    // define the rectangle of the building spot
    int startx = position.x - buildDist;
    int starty = position.y - buildDist;
    int endx   = position.x + width + buildDist;
    int endy   = position.y + height + buildDist;

    if (b.type.isAddon())
    {
        const BWAPI::UnitType builderType = type.whatBuilds().first;

        BWAPI::TilePosition builderTile(position.x - builderType.tileWidth(),position.y + 2 - builderType.tileHeight());

        startx = builderTile.x - buildDist;
        starty = builderTile.y - buildDist;
        endx = position.x + width + buildDist;
        endy = position.y + height + buildDist;
    }

    if (horizontalOnly) {
        starty += buildDist;
        endy -= buildDist;
    }

	if (verticalOnly) {
		startx += buildDist;
		endx -= buildDist;
	}

    // if this rectangle doesn't fit on the map we can't build here
    if (startx < 0 || starty < 0 || endx > BWAPI::Broodwar->mapWidth() || endx < position.x + width || endy > BWAPI::Broodwar->mapHeight())
    {
        return false;
    }

    // if we can't build here, or space is reserved, or it's in the resource box, we can't build here
    for (int x = startx; x < endx; x++)
    {
        for (int y = starty; y < endy; y++)
        {
            if (!b.type.isRefinery())
            {
                if (!buildable(b,x,y) || _reserveMap[x][y] || ((b.type != BWAPI::UnitTypes::Protoss_Photon_Cannon) && isInResourceBox(x,y)))
                {
                    return false;
                }
            }
        }
    }

    return true;
}

BWAPI::TilePosition BuildingPlacer::GetBuildLocation(const Building & b,int padding) const
{
    return BWAPI::TilePosition(0,0);
}

BWAPI::TilePosition BuildingPlacer::getBuildLocationNear(const Building & b, int buildDist, bool horizontalOnly) const
{
	SparCraft::Timer t;
	t.start();

	// get the precomputed vector of tile positions which are sorted closes to this location
	const std::vector<BWAPI::TilePosition> & closestToBuilding = MapTools::Instance().getClosestTilesTo(BWAPI::Position(b.desiredPosition));

	double ms1 = t.getElapsedTimeInMilliSec();

	// special easy case of having no pylons
	int numPylons = BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Pylon);
	if (b.type.requiresPsi() && numPylons == 0) {
		return BWAPI::TilePositions::None;
	}

	// Bunker requires different placement logic.

		// iterate through the list until we've found a suitable location
		for (size_t i(0); i < closestToBuilding.size(); ++i)
		{
			if (canBuildHereWithSpace(closestToBuilding[i], b, buildDist, horizontalOnly))
			{
				double ms = t.getElapsedTimeInMilliSec();
				//BWAPI::Broodwar->printf("Building Placer Took %d iterations, lasting %lf ms @ %lf iterations/ms, %lf setup ms", i, ms, (i / ms), ms1);

				return closestToBuilding[i];
			}
		}
		double ms = t.getElapsedTimeInMilliSec();
		//BWAPI::Broodwar->printf("Turret Placer Took %lf ms", ms);

		//  Todo: the BuildingManager can't handle a position of none, and will lock up. 
		//  Thankfully its virtually impossible to get to this point
		return  BWAPI::TilePositions::None;
}

// Created by Micheal Morris and D'Arcy Hamilton
BWAPI::TilePosition BuildingPlacer::getTurretBuildLocationNear(const Building & b,int buildDist,bool horizontalOnly) const
{
    SparCraft::Timer t;
    t.start();

    // get the precomputed vector of tile positions which are sorted closes to this location

 

    // special easy case of having no pylons
    int numPylons = BWAPI::Broodwar->self()->completedUnitCount(BWAPI::UnitTypes::Protoss_Pylon);
    if (b.type.requiresPsi() && numPylons == 0) {
        return BWAPI::TilePositions::None;
    }

		int numTurrets = BWAPI::Broodwar->self()->completedUnitCount(b.type);
		BWAPI::Position home(BWAPI::Broodwar->self()->getStartLocation());
		BWAPI::Position closest;
		BWAPI::Position dest;

		double closestDist = 999999;

		// Get closest Chokepoint to AI base.
		for (std::set<BWTA::Region*>::const_iterator r = BWTA::getRegions().begin(); r != BWTA::getRegions().end(); r++)
		{
			for (std::set<BWTA::Chokepoint*>::const_iterator c = (*r)->getChokepoints().begin(); c != (*r)->getChokepoints().end(); c++) {
				if (numTurrets > 2) {
					return BuildingPlacer::Instance().getBuildLocationNear(b, buildDist, false);
				}
				else {
					dest = (*c)->getSides().second;
				}
				
				double distance = abs( home.getDistance(dest));
				if (distance < closestDist) {
					closestDist = distance;
					closest = dest;
					/*
					// This can give numbers that are not multiples of 32, which isn't useful at all.
					// Get the nearest valid multiple of 64 (2 tiles)
					int xRemainder = closest.x % 64;
					if (xRemainder != 0) {
						int signMod = 1;

						closest.x += signMod * (64 - xRemainder);
					}

					int yRemainder = closest.y % 64;
					if (yRemainder != 0) {
						

						closest.y += signMod * (64 - yRemainder);
					}

					int signMod = 1;
					// Then home is smaller than closest, meaning home is at top of map.
					if ((home.y - closest.y) < 0) {
						signMod = -1;
					}

					closest.y += signMod * 96;
					*/
				}

			}
		}

		//this map has no known chokepoints
		if (closestDist == 999999)
		{
			return BuildingPlacer::Instance().getBuildLocationNear(b, buildDist, false);
		}
		// Closest chokepoint now set.
		//BWAPI::Broodwar->printf("closest (%i, %i) = %i ", closest.x, closest.y, closest.isValid());

		// we want to build turrets at the top of the ramp, so build sightly closer to the base.
		closest.x = (int)(4 * closest.x + BWAPI::Broodwar->self()->getStartLocation().x*32) / 5;
		closest.y = (int)(4 * closest.y + BWAPI::Broodwar->self()->getStartLocation().y*32) / 5;

		// iterate through the list until we've found a suitable location
		const std::vector<BWAPI::TilePosition> & closestToChoke = MapTools::Instance().getClosestTilesTo(closest);
		double ms1 = t.getElapsedTimeInMilliSec();
		//BWAPI::Broodwar->printf("ClosestToChoke size: %i", closestToChoke.size());
		for (size_t i(0); i < closestToChoke.size(); ++i) {
			//BWAPI::Broodwar->printf("Closest: %i, %i", closest.x, closest.y);
			if (canBuildHereWithSpace(closestToChoke[i], b, buildDist, false, true))
			{
				double ms = t.getElapsedTimeInMilliSec();
				//BWAPI::Broodwar->printf("Building Placer Took %d iterations, lasting %lf ms @ %lf iterations/ms, %lf setup ms", i, ms, (i / ms), ms1);
				//BWAPI::Broodwar->printf("Built turret at %i, %i", closestToChoke[i].x, closestToChoke[i].y);
				return closestToChoke[i];
			}
		

	}

    double ms = t.getElapsedTimeInMilliSec();
    //BWAPI::Broodwar->printf("Turret Placer Took %lf ms", ms);

	//  Todo: the BuildingManager can't handle a position of none, and will lock up. 
	//  Thankfully its virtually impossible to get to this point
    return  BWAPI::TilePositions::None;
}

bool BuildingPlacer::tileOverlapsBaseLocation(BWAPI::TilePosition tile,BWAPI::UnitType type) const
{
    // if it's a resource depot we don't care if it overlaps
    if (type.isResourceDepot())
    {
        return false;
    }

    // dimensions of the proposed location
    int tx1 = tile.x;
    int ty1 = tile.y;
    int tx2 = tx1 + type.tileWidth();
    int ty2 = ty1 + type.tileHeight();

    // for each base location
    for (BWTA::BaseLocation * base : BWTA::getBaseLocations())
    {
        // dimensions of the base location
        int bx1 = base->getTilePosition().x;
        int by1 = base->getTilePosition().y;
        int bx2 = bx1 + BWAPI::Broodwar->self()->getRace().getCenter().tileWidth();
        int by2 = by1 + BWAPI::Broodwar->self()->getRace().getCenter().tileHeight();

        // conditions for non-overlap are easy
        bool noOverlap = (tx2 < bx1) || (tx1 > bx2) || (ty2 < by1) || (ty1 > by2);

        // if the reverse is true, return true
        if (!noOverlap)
        {
            return true;
        }
    }

    // otherwise there is no overlap
    return false;
}

bool BuildingPlacer::buildable(const Building & b,int x,int y) const
{
    BWAPI::TilePosition tp(x,y);

    //returns true if this tile is currently buildable, takes into account units on tile
	//D'Arcy Hamilton - units do not block the building of Addons, except they do
    if (!BWAPI::Broodwar->isBuildable(x,y))
    {
		if (b.type.isAddon()){
			BWAPI::Broodwar->printf("addon builder1");
		}
        return false;
    }

    if ((BWAPI::Broodwar->self()->getRace() == BWAPI::Races::Terran) && tileBlocksAddon(BWAPI::TilePosition(x,y)))
    {
		if (b.type.isAddon()){
			BWAPI::Broodwar->printf("addon builder2");
		}
        return false;
    }

    for (auto & unit : BWAPI::Broodwar->getUnitsOnTile(x,y))
    {
		if ((b.builderUnit != nullptr) && ((unit != b.builderUnit) && !b.type.isAddon()))
        {
			if (b.type.isAddon()){
				BWAPI::Broodwar->printf("addon builder3");
			}
            return false;
        }
    }

    if (!tp.isValid())
    {
		if (b.type.isAddon()){
			BWAPI::Broodwar->printf("addon builder4");
		}
        return false;
    }

    return true;
}

void BuildingPlacer::reserveTiles(BWAPI::TilePosition position,int width,int height)
{
    int rwidth = _reserveMap.size();
    int rheight = _reserveMap[0].size();
    for (int x = position.x; x < position.x + width && x < rwidth; x++)
    {
        for (int y = position.y; y < position.y + height && y < rheight; y++)
        {
            _reserveMap[x][y] = true;
        }
    }
}

void BuildingPlacer::drawReservedTiles()
{
    if (!Config::Debug::DrawReservedBuildingTiles)
    {
        return;
    }

    int rwidth = _reserveMap.size();
    int rheight = _reserveMap[0].size();

    for (int x = 0; x < rwidth; ++x)
    {
        for (int y = 0; y < rheight; ++y)
        {
            if (_reserveMap[x][y] || isInResourceBox(x,y))
            {
                int x1 = x*32 + 8;
                int y1 = y*32 + 8;
                int x2 = (x+1)*32 - 8;
                int y2 = (y+1)*32 - 8;

                BWAPI::Broodwar->drawBoxMap(x1,y1,x2,y2,BWAPI::Colors::Yellow,false);
            }
        }
    }
}

void BuildingPlacer::freeTiles(BWAPI::TilePosition position, int width, int height)
{
    int rwidth = _reserveMap.size();
    int rheight = _reserveMap[0].size();

    for (int x = position.x; x < position.x + width && x < rwidth; x++)
    {
        for (int y = position.y; y < position.y + height && y < rheight; y++)
        {
            _reserveMap[x][y] = false;
        }
    }
}

BWAPI::TilePosition BuildingPlacer::getRefineryPosition()
{
    BWAPI::TilePosition closestGeyser = BWAPI::TilePositions::None;
    double minGeyserDistanceFromHome = std::numeric_limits<double>::max();
    BWAPI::Position homePosition = BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation());

    // for each geyser
    for (auto & geyser : BWAPI::Broodwar->getStaticGeysers())
    {
        if (geyser->getType() != BWAPI::UnitTypes::Resource_Vespene_Geyser)
        {
            continue;
        }

        BWAPI::Position geyserPos = geyser->getInitialPosition();
        BWAPI::TilePosition geyserTilePos = geyser->getInitialTilePosition();

        // check to see if it's next to one of our depots
        bool nearDepot = false;
        for (auto & unit : BWAPI::Broodwar->self()->getUnits())
        {
            if (unit->getType().isResourceDepot() && unit->getDistance(geyserPos) < 300)
            {
                nearDepot = true;
            }
        }

        if (nearDepot)
        {
            double homeDistance = geyser->getDistance(homePosition);

            if (homeDistance < minGeyserDistanceFromHome)
            {
                minGeyserDistanceFromHome = homeDistance;
                closestGeyser = geyser->getInitialTilePosition();
            }
        }
    }
    
    return closestGeyser;
}

bool BuildingPlacer::isReserved(int x, int y) const
{
    int rwidth = _reserveMap.size();
    int rheight = _reserveMap[0].size();
    if (x < 0 || y < 0 || x >= rwidth || y >= rheight)
    {
        return false;
    }

    return _reserveMap[x][y];
}

