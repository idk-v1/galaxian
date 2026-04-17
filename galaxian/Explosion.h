// Name: Ben Hamilton
// File: Explosion.h

#ifndef EXPLOSION_H
#define EXPLOSION_H

#include "ObjectBase.h"
#include "Events.h"

class Explosion : public ObjectBase
{
public:
    Explosion(int x, int y, int size);

    void update(EventQueue& events);

private:
    int size;

    int timer = 0;
};

#endif
