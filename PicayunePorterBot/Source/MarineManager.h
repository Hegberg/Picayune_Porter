#pragma once;

#include <Common.h>
#include "MicroManager.h"

namespace Picayune_Porter
{
	class MarineManager : public MicroManager
	{
	public:

		MarineManager();
		void executeMicro(const BWAPI::Unitset & targets);

		BWAPI::Unit chooseTarget(BWAPI::Unit rangedUnit, const BWAPI::Unitset & targets, std::map<BWAPI::Unit, int> & numTargeting);
		BWAPI::Unit closestrangedUnit(BWAPI::Unit target, std::set<BWAPI::Unit> & rangedUnitsToAssign);

		int getAttackPriority(BWAPI::Unit rangedUnit, BWAPI::Unit target);
		BWAPI::Unit getTarget(BWAPI::Unit rangedUnit, const BWAPI::Unitset & targets);
	};
}