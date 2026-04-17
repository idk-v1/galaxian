// Name: Ben Hamilton
// File: AlienGuard.h

#ifndef ALIEN_GUARD_H
#define ALIEN_GUARD_H

#include "AlienBase.h"

class AlienGuard : public AlienBase
{
public:
    AlienGuard(int x, int y, int gx, int gy, bool onGrid);
};

#endif
