#include "ComsatManager.h"

using namespace Picayune_Porter;

ComsatManager::ComsatManager() : unitClosestToEnemy(nullptr) { }

void ComsatManager::executeMicro(const BWAPI::Unitset & targets)
{
	const BWAPI::Unitset & detectorUnits = getUnits();

	if (detectorUnits.empty())
	{
		return;
	}

	for (size_t i(0); i<targets.size(); ++i)
	{
		// do something here if there's targets
	}

	cloakedUnitMap.clear();
	BWAPI::Unitset cloakedUnits;


	// for each detectorUnit
	for (auto & detectorUnit : detectorUnits)
	{

		BWAPI::Position explorePosition = MapGrid::Instance().getLeastExplored();
		if (detectorUnit->getEnergy > 150){
			detectorUnit->useTech(BWAPI::TechTypes::Scanner_Sweep, explorePosition);
		}
		if(detectorUnit->getEnergy > 50){
			detectorUnit->useTech(BWAPI::TechTypes::Scanner_Sweep, explorePosition);
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