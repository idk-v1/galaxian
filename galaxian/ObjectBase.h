// Name: Ben Hamilton
// File: ObjectBase.h

#ifndef OBJECT_BASE_H
#define OBJECT_BASE_H

#include "Includes.h"
#include "Surface.h"
#include "Constants.h"
#include "Events.h"
#include "DrawImage.h"

class ObjectBase
{
public:
    ObjectBase() {}
    ObjectBase(int x, int y, int w, int h) :
        rect(x - w / 2, y - h / 2, w, h), x(x), y(y) {}

    void draw(Surface& surface)
    {
        surface.put_rect(rect, color);
    }
    
    void draw(Surface& surface, Image* spritesheet)
    {
        drawImage(surface, spritesheet, rect, tx, ty);
    }

    bool collides(const ObjectBase* other)
    {
        if (rect.x + rect.w < other->rect.x)
            return false;
        if (rect.x > other->rect.x + other->rect.w)
            return false;
        if (rect.y + rect.h < other->rect.y)
            return false;
        if (rect.y > other->rect.y + other->rect.h)
            return false;
        return true;
    }

    void move(float x, float y)
    {
        this->x += x;
        this->y += y;
        rect.x = this->x - rect.w / 2.f;
        rect.y = this->y - rect.h / 2.f;
    }

    void moveVel(float x, float y)
    {
        move(x * vel, y * vel);
    }

    void setPos(float x, float y)
    {
        rect.x = x - rect.w / 2.f;
        rect.y = y - rect.h / 2.f;
        this->x = x;
        this->y = y;
    }

    void markDelete()
    {
        reqDel = true;
    }

    bool shouldDelete()
    {
        return reqDel;
    }

    float getX() {return x;}
    float getY() {return y;}

    float getWidth() {return rect.w;}
    float getHeight() {return rect.h;}
    
    void setVel(float v)
    {
        vel = v;
    }

protected:
    Rect rect;
    float x, y;
    float vel = 1.f;
    int tx = -1, ty = -1;
    bool reqDel = false;
    Color color = {255, 255, 255};
};

#endif
