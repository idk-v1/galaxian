// Name: Ben Hamilton
// File: Explosion.cpp

#include "Explosion.h"

Explosion::Explosion(int x, int y, int size) :
    ObjectBase(x, y, size * 16 * SCALE, size * 16 * SCALE), size(size)
{
    color = (Color){255, 127, 0};
    timer = 15 * 4;

    tx = 0;
    if (size == 1)
        ty = 80;
    else if (size == 2)
        ty = 96;
}

void Explosion::update(EventQueue& events)
{
    if (timer)
    {
        --timer;
        tx = getWidth() * (3 - timer / 15);
    }
    else
        events.kill(this);
}
