// Name: Ben Hamilton
// File: AlienLeader.cpp

#include "AlienLeader.h"

AlienLeader::AlienLeader(int x, int y, int gx, int gy, bool onGrid) :
    AlienBase(x, y, gx, gy, TYPE_LEADER, onGrid)
{
    color = (Color){255, 255, 0};
    setVel(0.9f);

    tx = 0;
    ty = 48;
}
