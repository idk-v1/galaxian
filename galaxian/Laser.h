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
    Laser(int x, int y, bool player, ObjectBase* owner = NULL);

    void update(EventQueue& events);

    ObjectBase* getOwner() {return owner;}

private:
    bool fromPlayer;
    ObjectBase* owner;
};

#endif
