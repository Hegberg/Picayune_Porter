#pragma once;

#include <Common.h>
#include "MicroManager.h"

namespace Picayune_Porter
{
	class ComsatManager : public MicroManager
	{

		std::map<BWAPI::Unit, bool>	cloakedUnitMap;

		bool isAssigned(BWAPI::Unit unit);
		bool explore = true;

		BWAPI::Unit unitClosestToEnemy;

	public:

		ComsatManager();
		~ComsatManager() {}

		void setUnitClosestToEnemy(BWAPI::Unit unit) { unitClosestToEnemy = unit; }
		void executeMicro(const BWAPI::Unitset & targets);

		BWAPI::Unit closestCloakedUnit(const BWAPI::Unitset & cloakedUnits, BWAPI::Unit detectorUnit);
	};
}