// Name: Ben Hamilton
// File: AlienBlue.cpp

#include "AlienBlue.h"

AlienBlue::AlienBlue(int x, int y, int gx, int gy, bool onGrid) :
    AlienBase(x, y, gx, gy, TYPE_BLUE, onGrid)
{
    color = (Color){0, 127, 255};
    setVel(0.8f);
    
    tx = 0;
    ty = 32;
}
