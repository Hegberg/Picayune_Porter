#include "ComsatManager.h"

using namespace Picayune_Porter;

ComsatManager::ComsatManager() : unitClosestToEnemy(nullptr) { }

void ComsatManager::executeMicro(const BWAPI::Unitset & targets)
{
	const BWAPI::Unitset & comsatUnits = getUnits();

	if (comsatUnits.empty())
	{
		BWAPI::Broodwar->printf("Comsat don't exist");
		return;
	}

	for (size_t i(0); i<targets.size(); ++i)
	{
		// do something here if there's targets
	}

	cloakedUnitMap.clear();
	BWAPI::Unitset cloakedUnits;
	// figure out targets
	for (auto & unit : BWAPI::Broodwar->enemy()->getUnits())
	{
		// conditions for targeting
		if (unit->getType() == BWAPI::UnitTypes::Zerg_Lurker ||
			unit->getType() == BWAPI::UnitTypes::Protoss_Dark_Templar ||
			unit->getType() == BWAPI::UnitTypes::Terran_Wraith ||
			unit->getType() == BWAPI::UnitTypes::Terran_Ghost ||
			unit->getType() == BWAPI::UnitTypes::Protoss_Observer ||
			unit->getType() == BWAPI::UnitTypes::Terran_Vulture_Spider_Mine)
		{
			cloakedUnits.insert(unit);
			cloakedUnitMap[unit] = false;
		}
	}

	for (auto & cloaked : cloakedUnits)
	{
		BWAPI::Broodwar->printf("cloaked %s at %d, %d  Cloaked: %d", cloaked->getType().toString(), cloaked->getPosition().x, cloaked->getPosition().y, cloaked->isCloaked());
	}
	// for each detectorUnit
	for (auto & detectorUnit : comsatUnits)
	{
		unitClosestToEnemy = closestCloakedUnit(cloakedUnits, detectorUnit);
		//BWAPI::Position explorePosition = MapGrid::Instance().getLeastExplored();
		if (detectorUnit->getEnergy() == 50){
			detectorUnit->useTech(BWAPI::TechTypes::Scanner_Sweep, unitClosestToEnemy->getPosition());
			BWAPI::Broodwar->drawCircle(BWAPI::CoordinateType::Map, unitClosestToEnemy->getPosition().x, unitClosestToEnemy->getPosition().y, 2000, BWAPI::Colors::Blue, true);
		}
		
		/*
		// if we need to regroup, move the detectorUnit to that location
		if (!detectorUnitInBattle && unitClosestToEnemy && unitClosestToEnemy->getPosition().isValid())
		{
		Micro::SmartMove(detectorUnit, unitClosestToEnemy->getPosition());
		detectorUnitInBattle = true;
		}
		// otherwise there is no battle or no closest to enemy so we don't want our detectorUnit to die
		// send him to scout around the map
		else
		{
		BWAPI::Position explorePosition = MapGrid::Instance().getLeastExplored();
		Micro::SmartMove(detectorUnit, explorePosition);
		}
		*/
	}
}


BWAPI::Unit ComsatManager::closestCloakedUnit(const BWAPI::Unitset & cloakedUnits, BWAPI::Unit detectorUnit)
{
	BWAPI::Unit closestCloaked = nullptr;
	double closestDist = 100000;

	for (auto & unit : cloakedUnits)
	{
		// if we haven't already assigned an detectorUnit to this cloaked unit
		if (!cloakedUnitMap[unit])
		{
			double dist = unit->getDistance(detectorUnit);

			if (!closestCloaked || (dist < closestDist))
			{
				closestCloaked = unit;
				closestDist = dist;
			}
		}
	}

	return closestCloaked;
}