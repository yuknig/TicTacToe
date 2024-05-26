#pragma once
#include "GeomTypes.h"

class IPlayerController
{
public:
    virtual ~IPlayerController() = default;

    virtual bool MoveReady() = 0;
    virtual Point GetMove() = 0;
};
