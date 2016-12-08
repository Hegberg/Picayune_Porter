#pragma once

#include <Common.h>
#include <BWAPI.h>

namespace Picayune_Porter
{
namespace Micro
{      
    void SmartAttackUnit(BWAPI::Unit attacker, BWAPI::Unit target);
    void SmartAttackMove(BWAPI::Unit attacker, const BWAPI::Position & targetPosition);
    void SmartMove(BWAPI::Unit attacker, const BWAPI::Position & targetPosition);
    void SmartRightClick(BWAPI::Unit unit, BWAPI::Unit target);
    void SmartLaySpiderMine(BWAPI::Unit unit, BWAPI::Position pos, int mines);
	void SmartEMPShockwave(BWAPI::Unit unit, BWAPI::Unit enemy, int energy);
	void SmartIrradiate(BWAPI::Unit unit, BWAPI::Unit enemy, int energy);
	void SmartDefensiveMatrix(BWAPI::Unit unit, BWAPI::Unit ally, int energy);
    void SmartRepair(BWAPI::Unit unit, BWAPI::Unit target);
    void SmartKiteTarget(BWAPI::Unit rangedUnit, BWAPI::Unit target);
    void MutaDanceTarget(BWAPI::Unit muta, BWAPI::Unit target);
    BWAPI::Position GetKiteVector(BWAPI::Unit unit, BWAPI::Unit target);

    void Rotate(double &x, double &y, double angle);
    void Normalize(double &x, double &y);

    void drawAPM(int x, int y);
};
}