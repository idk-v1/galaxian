// Name: Ben Hamilton
// File: Spaceship.h

#ifndef SPACESHIP_H
#define SPACESHIP_H

#include <vector>

#include "ObjectBase.h"
#include "Constants.h"
#include "Events.h"

class Spaceship : public ObjectBase
{
public:
    Spaceship(int x, int y, int id) : id(id),
        ObjectBase(x, y, 16 * SCALE, 16 * SCALE)
    {
        setVel(2.f);
        tx = 144 + id * 32;
        ty = 64;
    }

    void respawn(int x, int y)
    {
        --lives;
        setPos(x, y);
        canShoot = true;
        alive = true;
        reqDel = false;
    }

    void shoot(EventQueue& events)
    {
        if (canShoot && shotsOut == 0)
        {
            events.spawnLaser(x - getWidth() / 4.f, y, true, this);
            events.spawnLaser(x + getWidth() / 4.f, y, true, this);
            shotsOut = 2;
        }
    }

    void update()
    {
        if (rect.x < 0)
            setPos(rect.w / 2.f, y);
        if (rect.x + rect.w >= SCREENW)
            setPos(SCREENW-1 - rect.w / 2.f, y);
    }

    void setCanShoot(bool value) {canShoot = value; shotsOut = 0;}
    void returnShot()
    {
        if (shotsOut > 0)
            --shotsOut;
    }

    void kill()
    {
        alive = false;
        reqDel = false;
    }
    bool isAlive() {return alive;}
    
    int getLives() {return lives;}
    void setLives(int count) {lives = count;}
    void resetLives() {lives = 5; extraLives = 0;}

    int getScore() {return score;}
    void setScore(int value) {score = value;}
    void addToScore(int value)
    {
        score += value;
        if (score / 5000 - extraLives > 0)
        {
            ++lives;
            ++extraLives;
        }
    }

private:
    int id;
    
    int score = 0;
    
    int lives = 4;
    int extraLives = 0;

    bool alive = true;
    
    bool canShoot = true;
    int shotsOut = 0;
};

#endif
