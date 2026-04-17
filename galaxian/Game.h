// Name: Ben Hamilton
// File: Game.h

#ifndef GAME_H
#define GAME_H

#include <vector>

#include "Spaceship.h"
#include "Laser.h"
#include "AlienBase.h"
#include "AlienLeader.h"
#include "AlienGuard.h"
#include "AlienPurple.h"
#include "AlienBlue.h"
#include "Star.h"
#include "Explosion.h"
#include "Constants.h"
#include "Events.h"
#include "DrawImage.h"

class Game
{
public:
    Game() = default;
    ~Game();
    void init();
    void quit();
    void update();
    void draw(Surface& surface);
    
private:
    void newWave();
    void introWave();

    void drawIntro(Surface& surface);
    void updateIntro();
    
    void processSpawnEvents();
    void processChangeEvents();
    void processKillEvents();

    void updatePlayers();
    void updateAliens();
    void updateLasers();
    
    void updateStars();

    AlienBase* alienGrid[GRIDH][GRIDW];
    
    std::vector<Spaceship> players;
    std::vector<Laser> pLasers;
    
    std::vector<AlienBase*> aliens;
    int escaped = 0;
    int attacking = 0;
    std::vector<Laser> aLasers;

    std::vector<Explosion> explosions;
    
    std::vector<Star> stars;

    float gridOffset = 0;
    bool gridDir = true;

    bool inIntro = false;

    bool playersDead = false;
    bool waitForPlayer = false;
    int peaceTime = 0;
    int timer = 0;
    EventQueue events;

    int waveCount = -1;

    int highscore = 0;

    Image* spritesheet;
};

#endif
