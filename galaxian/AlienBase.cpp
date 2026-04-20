// Name: Ben Hamilton
// File: AlienBase.cpp

#include "AlienBase.h"

float AlienBase::gridOffsetX = 0.f;
float AlienBase::gridOffsetY = 0.f;

float AlienBase::getCellX()
{
    return (SCREENW / 2.f) + (rect.w + 6) * (gridX - GRIDW / 2.f + 0.5f)
        + gridOffsetX;
}

float AlienBase::getCellY()
{
    return (SCREENH / 5.f) + (rect.w + 6) * (gridY - GRIDH / 2.f + 0.5f)
        + gridOffsetY;
}

void AlienBase::update(EventQueue& events, float px, int attackCount, int alienCount, bool canLeave)
{
    ++animTimer;
    if (animTimer >= 15 * 3)
        animTimer = 0;
    tx = getWidth() * (animTimer / 15);
    
    if (cooldown) --cooldown;
    
    attacking = false;
    float cx = getCellX();
    float cy = getCellY();
    
    if (onGrid)
    {
        setPos(cx, cy);
        if (canLeave && canMove && attackCount < 4 &&
            (rand() % 400 == 0 ||
             (alienCount < 5 && rand() % 100 == 0))
        )
        {
            if (type == TYPE_LEADER)
                events.requestGuards(this, gridX, gridY);
                
            events.leaveGrid(this);

            attackDist = 0.f;
            flipSine = (cx > SCREENW / 2);
            lastSine = 0.f;
        }
    }
    else
    {
        if (returning)
        {
            // wait some time to let player spawn
            // maybe help with attack counter messing up
            // it was the instantly rejoining battle if only a few were left
            
            lead = NULL;
            
            if (x < cx)
            {
                move(1.25, 0);
                if (x > cx) setPos(cx, y);
            }
            if (x > cx)
            {
                move(-1.25, 0);
                if (x < cx) setPos(cx, y);
            }
            if (y < cy)
            {
                move(0, 1.0f);
                if (y > cy) setPos(x, cy);
            }

            if (x == cx && y == cy)
                events.enterGrid(this);
        }
        else
        {
            attacking = true;

            float sine = sin(attackDist) * (SCREENW * sineMul);
            if (flipSine) sine = -sine;

            if (rand() % 300 == 0 && !cooldown)
            {
                cooldown = 60;
                events.spawnLaser(x - getWidth() / 4, y + getHeight(), false);
                events.spawnLaser(x + getWidth() / 4, y + getHeight(), false);
            }
            
            if (!lead)
            {
                move(-lastSine, 0);
                if (x < px)
                    moveVel(1, 0);
                else if (x > px)
                    moveVel(-1, 0);
                
                move(sine, 0.5f);
            }
            else
            {
                float offx = getCellX() - lead->getCellX();
                float offy = getCellY() - lead->getCellY();
                setPos(lead->getX() + offx, lead->getY() + offy);
                flipSine = lead->flipSine;
            }
            lastSine = sine;
            attackDist += 0.025f;
            
            if (y - getHeight() * 2 > SCREENH)
            {
                events.returnToTop(this);
            }
        }
    }
}
