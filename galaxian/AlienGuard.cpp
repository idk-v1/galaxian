// Name: Ben Hamilton
// File: AlienGuard.cpp

#include "AlienGuard.h"

AlienGuard::AlienGuard(int x, int y, int gx, int gy, bool onGrid) :
    AlienBase(x, y, gx, gy, TYPE_GUARD, onGrid)
{
    color = (Color){255, 0, 0};
    setVel(0.9f);

    tx = 0;
    ty = 0;
}
