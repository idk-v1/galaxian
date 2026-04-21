// Name: Ben Hamilton
// File: Events.h

#ifndef EVENTS_H
#define EVENTS_H

#include "Constants.h"
#include <vector>


struct SpawnEvent
{
    int x, y;
    int gx, gy;
    // 0 - Player
    // 1 - Blue Alien
    // 2 - Purple Alien
    // 3 - Guard Alien
    // 4 - Leader Alien
    // 5 - Laser (player)
    // 6 - Laser (alien)
    char type;
    bool onGrid;
    int delay;
    void* owner;
};

struct ChangeEvent
{
    void* target; // AlienBase
    int gx, gy;
    // Return to top (or escape round for leaders)
    // Leave grid
    // Enter grid
    // Request guards
    char action;
    int delay;
};

struct KillEvent
{
    void* target; // ObjectBase
    int explode;
    int delay;
};


class EventQueue
{
public:
    std::vector<SpawnEvent> spawnEvents;
    std::vector<ChangeEvent> changeEvents;
    std::vector<KillEvent> killEvents;

    void returnToTop(void* target, int delay = 0)
    {
        ChangeEvent change;
        change.target = target;
        change.action = ACTION_RET_TOP;
        change.gx = 0;
        change.gy = 0;
        change.delay = delay;
        changeEvents.push_back(change);
    }

    void leaveGrid(void* target, int delay = 0)
    {
        ChangeEvent change;
        change.target = target;
        change.action = ACTION_LEAVE_GRID;
        change.gx = 0;
        change.gy = 0;
        change.delay = delay;
        changeEvents.push_back(change);
    }

    void enterGrid(void* target, int delay = 0)
    {
        ChangeEvent change;
        change.target = target;
        change.action = ACTION_ENTER_GRID;
        change.gx = 0;
        change.gy = 0;
        change.delay = delay;
        changeEvents.push_back(change);
    }

    void requestGuards(void* target, int gx, int gy, int delay = 0)
    {
        ChangeEvent change;
        change.target = target;
        change.action = ACTION_REQ_GUARDS;
        change.gx = gx;
        change.gy = gy;
        change.delay = delay;
        changeEvents.push_back(change);
    }

    inline void spawnPlayer(int x, int y, int index, int delay = 0)
    {
        SpawnEvent spawn;
        spawn.x = x;
        spawn.y = y;
        spawn.gx = index;
        spawn.gy = 0;
        spawn.type = TYPE_PLAYER;
        spawn.onGrid = false;
        spawn.owner = NULL;
        spawn.delay = delay;
        spawnEvents.push_back(spawn);
    }

    inline void spawnAlien(int x, int y, int gx,int gy, bool onGrid, char type, int delay = 0)
    {
        SpawnEvent spawn;
        spawn.x = x;
        spawn.y = y;
        spawn.gx = gx;
        spawn.gy = gy;
        spawn.type = type;
        spawn.onGrid = onGrid;
        spawn.owner = NULL;
        spawn.delay = delay;
        spawnEvents.push_back(spawn);
    }
    
    void spawnLaser(int x, int y, bool player, void* owner = NULL, int type = 0, int delay = 0)
    {
        SpawnEvent spawn;
        spawn.x = x;
        spawn.y = y;
        spawn.gx = type;
        spawn.gy = 0;
        spawn.type = TYPE_LASER + player;
        spawn.owner = owner;
        spawn.onGrid = false;
        spawn.delay = delay;
        spawnEvents.push_back(spawn);
    }
    
    inline void kill(void* target, int explode = 0, int delay = 0)
    {
        KillEvent kill;
        kill.target = target;
        kill.explode = explode;
        kill.delay = delay;
        killEvents.push_back(kill);
    }
};

#endif
