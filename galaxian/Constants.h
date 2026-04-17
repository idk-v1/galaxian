// Name: Ben Hamilton
// File: Constants.h

#ifndef CONSTANTS_H
#define CONSTANTS_H

const float SCALE = 1.f;
const int SCREENW = 224 * 2.f;
const int SCREENH = 256 * 2.f;
const float PLAYER_Y = SCREENH * 0.9f;

const int GRIDW = 10;
const int GRIDH = 6;


enum
{
    TYPE_PLAYER,
    TYPE_BLUE,
    TYPE_PURPLE,
    TYPE_GUARD,
    TYPE_LEADER,
    TYPE_LASER,
};

enum
{
    ACTION_RET_TOP,
    ACTION_LEAVE_GRID,
    ACTION_ENTER_GRID,
    ACTION_REQ_GUARDS,
};

#endif
