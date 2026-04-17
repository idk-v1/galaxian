// Name: Ben Hamilton
// File: Laser.cpp

#include "Laser.h"

Laser::Laser(int x, int y, bool player, ObjectBase* owner)
    : ObjectBase(x, y, 2 * SCALE, 6 * SCALE), owner(owner)
{
    if (player)
        color = (Color){255, 255, 255};
    else
        color = (Color){255, 255, 0};
    fromPlayer = player;
    setVel(3.f);
}

void Laser::update(EventQueue& events)
{
    if (fromPlayer)
        moveVel(0, -1);
    else
        moveVel(0, 1);

    if (rect.y + rect.h < 0)
        events.kill(this);
    if (rect.y > SCREENH)
        events.kill(this);
}
