#pragma once

#include "Common.h"

namespace Picayune_Porter
{

class AutoObserver
{
    int                         _cameraLastMoved;
    int                         _unitFollowFrames;
    BWAPI::Unit      _observerFollowingUnit;

public:

    AutoObserver();
    void onFrame();
};

}