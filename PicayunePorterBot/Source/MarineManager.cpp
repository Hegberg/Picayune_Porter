#include "MarineManager.h"
#include "UnitUtil.h"

using namespace Picayune_Porter;

MarineManager::MarineManager()
{
}

void MarineManager::executeMicro(const BWAPI::Unitset & targets)
{
	const BWAPI::Unitset & marines = getUnits();

	BWAPI::Unitset bunkerTargets;
	for (auto & unit : BWAPI::Broodwar->self()->getUnits())
	{
		if (unit->getType() == BWAPI::UnitTypes::Terran_Bunker)
		{

			bunkerTargets.insert(unit);

		}
	}



	// figure out targets
	BWAPI::Unitset marineTargets;
	std::copy_if(targets.begin(), targets.end(), std::inserter(marineTargets, marineTargets.end()),
		[](BWAPI::Unit u){ return u->isVisible(); });

	int marineRange = BWAPI::UnitTypes::Terran_Marine.groundWeapon().maxRange() - 32;
	

	/// <summary>Orders the unit to load the target unit.</summary> Only works if this unit is a
	/// @Transport or @Bunker type.
	///
	/// <param name="target">
	///     The target unit to load into this @Transport or @Bunker.
	/// </param>
	/// <param name="shiftQueueCommand"> (optional)
	///     If this value is true, then the order will be queued instead of immediately executed.
	///     If this value is omitted, then the order will be executed immediately by default.
	/// </param>
	///
	/// @returns true if the command was passed to Broodwar, and false if BWAPI determined that 
	/// the command would fail.
	/// @note There is a small chance for a command to fail after it has been passed to Broodwar.
	///
	/// @see unload, unloadAll, getLoadedUnits, isLoaded
	//bool load(Unit target, bool shiftQueueCommand = false);
	for (auto & bunker : bunkerTargets)
			{
		for (auto & marine : marines)
		{
		
				if (!marine->isLoaded())
				{
					//marine->move(bunker->getPosition());
					//marine->rightClick(bunker, false);
					bunker->load(marine, false);
				}
			}
			
		}





	// for each zealot
	for (auto & marine : marines)
	{

		// if the order is to attack or defend
		if (order.getType() == SquadOrderTypes::Attack || order.getType() == SquadOrderTypes::Defend)
		{
			// if there are targets
			if (!marineTargets.empty())
			{
				// find the best target for this zealot
				BWAPI::Unit target = getTarget(marine, marineTargets);

				if (target && Config::Debug::DrawUnitTargetInfo)
				{
					BWAPI::Broodwar->drawLineMap(marine->getPosition(), marine->getTargetPosition(), BWAPI::Colors::Purple);
				}




				Micro::SmartKiteTarget(marine, target);
				
			}
			// if there are no targets
			else
			{
				// if we're not near the order position
				if (marine->getDistance(order.getPosition()) > 100)
				{
						// move to it
					Micro::SmartAttackMove(marine, order.getPosition());
				}
			}
		}
	}
}

// get a target for the zealot to attack
BWAPI::Unit MarineManager::getTarget(BWAPI::Unit marine, const BWAPI::Unitset & targets)
{
	int bestPriorityDistance = 1000000;
	int bestPriority = 0;

	double bestLTD = 0;

	BWAPI::Unit bestTargetThreatInRange = nullptr;
	double bestTargetThreatInRangeLTD = 0;

	int highPriority = 0;
	double closestDist = std::numeric_limits<double>::infinity();
	BWAPI::Unit closestTarget = nullptr;

	int marineRange = BWAPI::UnitTypes::Terran_Marine.groundWeapon().maxRange() - 32;
	BWAPI::Unitset targetsInMarineRange;
	for (auto & target : targets)
	{
		if (target->getDistance(marine) < marineRange && UnitUtil::CanAttack(marine, target))
		{
			targetsInMarineRange.insert(target);
		}
	}

	const BWAPI::Unitset & newTargets = targetsInMarineRange.empty() ? targets : targetsInMarineRange;

	// check first for units that are in range of our attack that can cause damage
	// choose the highest priority one from them at the lowest health
	for (const auto & target : newTargets)
	{
		if (!UnitUtil::CanAttack(marine, target))
		{
			continue;
		}

		double distance = marine->getDistance(target);
		double LTD = UnitUtil::CalculateLTD(target, marine);
		int priority = getAttackPriority(marine, target);
		bool targetIsThreat = LTD > 0;
		BWAPI::Broodwar->drawTextMap(target->getPosition(), "%d", priority);

		if (!closestTarget || (priority > highPriority) || (priority == highPriority && distance < closestDist))
		{
			closestDist = distance;
			highPriority = priority;
			closestTarget = target;
		}
	}

	if (bestTargetThreatInRange)
	{
		return bestTargetThreatInRange;
	}

	return closestTarget;
}

// get the attack priority of a type in relation to a zergling
int MarineManager::getAttackPriority(BWAPI::Unit rangedUnit, BWAPI::Unit target)
{
	BWAPI::UnitType rangedType = rangedUnit->getType();
	BWAPI::UnitType targetType = target->getType();

	bool isThreat = rangedType.isFlyer() ? targetType.airWeapon() != BWAPI::WeaponTypes::None : targetType.groundWeapon() != BWAPI::WeaponTypes::None;

	if (target->getType().isWorker())
	{
		isThreat = false;
	}

	if (target->getType() == BWAPI::UnitTypes::Zerg_Larva || target->getType() == BWAPI::UnitTypes::Zerg_Egg)
	{
		return 0;
	}

	// if the target is building something near our base something is fishy
	BWAPI::Position ourBasePosition = BWAPI::Position(BWAPI::Broodwar->self()->getStartLocation());
	if (target->getType().isWorker() && (target->isConstructing() || target->isRepairing()) && target->getDistance(ourBasePosition) < 1200)
	{
		return 100;
	}

	if (target->getType().isBuilding() && (target->isCompleted() || target->isBeingConstructed()) && target->getDistance(ourBasePosition) < 1200)
	{
		return 90;
	}

	// highest priority is something that can attack us or aid in combat
	if (targetType == BWAPI::UnitTypes::Terran_Bunker || isThreat)
	{
		return 11;
	}
	// next priority is worker
	else if (targetType.isWorker())
	{
		return 9;
	}
	// next is special buildings
	else if (targetType == BWAPI::UnitTypes::Zerg_Spawning_Pool)
	{
		return 5;
	}
	// next is special buildings
	else if (targetType == BWAPI::UnitTypes::Protoss_Pylon)
	{
		return 5;
	}
	// next is buildings that cost gas
	else if (targetType.gasPrice() > 0)
	{
		return 4;
	}
	else if (targetType.mineralPrice() > 0)
	{
		return 3;
	}
	// then everything else
	else
	{
		return 1;
	}
}

BWAPI::Unit MarineManager::closestrangedUnit(BWAPI::Unit target, std::set<BWAPI::Unit> & rangedUnitsToAssign)
{
	double minDistance = 0;
	BWAPI::Unit closest = nullptr;

	for (auto & rangedUnit : rangedUnitsToAssign)
	{
		double distance = rangedUnit->getDistance(target);
		if (!closest || distance < minDistance)
		{
			minDistance = distance;
			closest = rangedUnit;
		}
	}

	return closest;
}