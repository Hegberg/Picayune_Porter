#include "DetectorManager.h"

//Implemented by D'Arcy Hamilton

using namespace Picayune_Porter;

DetectorManager::DetectorManager() : unitClosestToEnemy(nullptr) { }

void DetectorManager::executeMicro(const BWAPI::Unitset & targets) 
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



	bool detectorUnitInBattle = false;

	// for each detectorUnit
	for (auto & detectorUnit : detectorUnits)
	{
		
		// if we need to regroup, move the detectorUnit to that location
		if (!detectorUnitInBattle && unitClosestToEnemy && unitClosestToEnemy->getPosition().isValid())
		{

			BWAPI::UnitCommand currentCommand(detectorUnit->getLastCommand());

			// if we've already told this unit to move to this position, ignore this command
			if (detectorUnit->getEnergy() > 100)
			{
				if (detectorUnit->canUseTech(BWAPI::TechTypes::EMP_Shockwave, findClosestEMPTarget(targets, detectorUnit)))
				{
					if (!(currentCommand.getType() == BWAPI::UnitCommandTypes::Use_Tech_Unit))
					{
						detectorUnit->useTech(BWAPI::TechTypes::EMP_Shockwave, findClosestEMPTarget(targets, detectorUnit));
					}
				}
			}
			else if (detectorUnit->getEnergy() > 100)
			{
				if (detectorUnit->canUseTech(BWAPI::TechTypes::Irradiate, findClosestIrradiateTarget(targets, detectorUnit)))
				{
					if (!(currentCommand.getType() == BWAPI::UnitCommandTypes::Use_Tech_Unit))
					{
						detectorUnit->useTech(BWAPI::TechTypes::Irradiate, findClosestIrradiateTarget(targets, detectorUnit));
					}
				}
			}
			else if (detectorUnit->getEnergy() >= 200)
			{
				if (detectorUnit->canUseTech(BWAPI::TechTypes::Defensive_Matrix, unitClosestToEnemy))
				{
					if (!(currentCommand.getType() == BWAPI::UnitCommandTypes::Use_Tech_Unit))
					{
						detectorUnit->useTech(BWAPI::TechTypes::Defensive_Matrix, unitClosestToEnemy);
					}
				}
			}

			else
			{
				Micro::SmartMove(detectorUnit, unitClosestToEnemy->getPosition());
			}


			detectorUnitInBattle = true;
		}
		// otherwise there is no battle or no closest to enemy so we don't want our detectorUnit to die
		// send him to scout around the map
		else
		{
			//BWAPI::Position explorePosition = MapGrid::Instance().getLeastExplored();
			//Micro::SmartMove(detectorUnit, explorePosition);

		}
		
	}
}


BWAPI::Unit DetectorManager::closestCloakedUnit(const BWAPI::Unitset & cloakedUnits, BWAPI::Unit detectorUnit)
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


BWAPI::Unit  DetectorManager::findClosestEMPTarget(const BWAPI::Unitset & targets, BWAPI::Unit detectorUnit)
{
	int bestPriorityDistance = 1000000;
	int bestPriority = 0;

	double bestLTD = 0;

	int highPriority = 0;
	double closestDist = std::numeric_limits<double>::infinity();
	BWAPI::Unit closestTarget = nullptr;

	for (const auto & target : targets)
	{
		if (target->getType().getRace() == BWAPI::Races::Protoss)
		{
			continue;
		}

		double distance = detectorUnit->getDistance(target);
		int priority = target->getShields();

		if (!closestTarget || (priority > highPriority) || (priority == highPriority && distance < closestDist))
		{
			closestDist = distance;
			highPriority = priority;
			closestTarget = target;
		}
	}

	return closestTarget;
}

BWAPI::Unit  DetectorManager::findClosestIrradiateTarget(const BWAPI::Unitset & targets, BWAPI::Unit detectorUnit)
{
	int bestPriorityDistance = 1000000;
	int bestPriority = 0;

	double bestLTD = 0;

	int highPriority = 0;
	double closestDist = std::numeric_limits<double>::infinity();
	BWAPI::Unit closestTarget = nullptr;

	for (const auto & target : targets)
	{
		if (!target->getType().isOrganic())
		{
			continue;
		}

		double distance = detectorUnit->getDistance(target);
		int priority = target->getHitPoints();

		if (!closestTarget || (priority > highPriority) || (priority == highPriority && distance < closestDist))
		{
			closestDist = distance;
			highPriority = priority;
			closestTarget = target;
		}
	}

	return closestTarget;
}