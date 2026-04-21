// Name: Ben Hamilton
// File: Laser.cpp

#include "Laser.h"

Laser::Laser(int x, int y, bool player, ObjectBase* owner, int type)
    : ObjectBase(x, y, 2 * SCALE, 6 * SCALE), owner(owner), projType(type)
{
    if (projType == 0)
    {
        tx = 208;
        if (player)
            ty = 64;
        else
            ty = 72;
        setVel(3.f);
    }
    else if (projType == 1)
    {
        tx = 64;
        ty = 96;
        rect.w = 32;
        rect.h = 32;
        setVel(1.f);
    }
    fromPlayer = player;
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

    if (projType == 1)
    {
        if (++timer > 60 * 3)
            events.kill(this);
        tx = 64 + (timer / 15 % 2) * 32;
    }
}
