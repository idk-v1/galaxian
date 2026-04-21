// Name: Ben Hamilton
// File: Laser.h

#ifndef LASER_H
#define LASER_H

#include "Includes.h"
#include "ObjectBase.h"
#include "Events.h"

class Laser : public ObjectBase
{
public:
    Laser() = default;
    Laser(int x, int y, bool player, ObjectBase* owner = NULL, int type = 0);

    void update(EventQueue& events);

    ObjectBase* getOwner() {return owner;}

    int getProjType() {return projType;}

private:
    bool fromPlayer;
    int projType;
    int timer = 0;
    ObjectBase* owner;
};

#endif
