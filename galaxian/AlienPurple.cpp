// Name: Ben Hamilton
// File: AlienPurple.cpp

#include "AlienPurple.h"

AlienPurple::AlienPurple(int x, int y, int gx, int gy, bool onGrid) :
    AlienBase(x, y, gx, gy, TYPE_PURPLE, onGrid)
{
    color = (Color){127, 0, 255};
    setVel(0.9f);

    tx = 0;
    ty = 16;
}
