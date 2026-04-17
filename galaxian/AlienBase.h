// Name: Ben Hamilton
// File: AlienBase.h

#ifndef ALIEN_BASE_H
#define ALIEN_BASE_H

#include "ObjectBase.h"

class AlienBase : public ObjectBase
{
public:
    AlienBase() = default;
    AlienBase(int x, int y, int gx, int gy, char type, bool onGrid) :
        ObjectBase(x, y, 16 * SCALE, 16 * SCALE), gridX(gx), gridY(gy), type(type), onGrid(onGrid)
    {
        if (onGrid)
        {
            returning = false;
            setPos(getCellX(), getCellY());
        }
        else
            returning = true;
    }

    void update(EventQueue& events, float px, int attackCount, int alienCount, bool canLeave);

    // stupid vtables messing up my pointer alignment
    // virtual is bad
    // im making my own
    int getScore()
    {
        switch (type)
        {
            case TYPE_LEADER:
                if (lostGuards) return 800;
                return attacking ? 300 : 60;
            case TYPE_GUARD:
                return attacking ? 100 : 50;
            case TYPE_PURPLE:
                return attacking ?  80 : 40;
            case TYPE_BLUE:
                return attacking ?  60 : 30;
            default:
                return 0;
        }
    }

    int getGridX() {return gridX;}
    int getGridY() {return gridY;}
    
    char getType() {return type;}

    bool isOnGrid() {return onGrid;}
    void setOnGrid(bool grid)
    {
        onGrid = grid;
        returning = false;
        attacking = !grid;
    }

    void setLeader(AlienBase* leader) {lead = leader;}
    AlienBase* getLeader() {return lead;}

    void setMovable(bool value) {canMove = value;}
    bool isMovable() {return canMove;}

    void setReturning() {returning = true;attacking = false;}
    bool isReturning() {return returning;}

    bool isAttacking() {return attacking;}

    float getCellX();
    float getCellY();

    static void setGridOffsetX(float offset) {gridOffsetX = offset;};
    static void setGridOffsetY(float offset) {gridOffsetY = offset;};
    
    void setGuardCount(int value)
    {
        guardCount = value;
        lostGuards = false;
    }
    void decGuardCount()
    {
        --guardCount;
        if (guardCount == 0)
            lostGuards = true;
    }
    
    bool hasLostGuards() {return lostGuards;}

    bool hadNoGuards() {return !lostGuards && guardCount == 0;}
    
protected:
    int gridX, gridY;

    int guardCount = 0;
    bool lostGuards = false;

    char type;

    // If onGrid, do nothing
    // If not onGrid and has leader, follow leader
    // If not onGrid and has no leader, normal dive attack
    bool onGrid;
    bool canMove = false;
    bool returning;
    bool attacking = false;

    bool flipSine = false;
    float lastSine = 0.f;
    float sineMul = 0.25f;
    float attackDist = 0.f;

    int cooldown = 0;

    AlienBase* lead = NULL;

    int animTimer = 0;

    static float gridOffsetX;
    static float gridOffsetY;
};

#endif
