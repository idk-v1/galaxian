// Name: Ben Hamilton
// File: Game.cpp

#include "Game.h"

#include <fstream>

void Game::init()
{
    srand(time(NULL));
    
    spritesheet = new Image("res/galaxianSpritesheet.png");
    
    for (int i = 0; i < 100; i++)
        stars.push_back(Star(false));

    for (int y = 0; y < GRIDH; ++y)
        for (int x = 0; x < GRIDW; ++x)
            alienGrid[y][x] = NULL;

    aliens.reserve(GRIDW * GRIDH);
    players.push_back(Spaceship(SCREENW * 0.66, PLAYER_Y));
    players.push_back(Spaceship(SCREENW * 0.33, PLAYER_Y));

    introWave();

    std::ifstream score("highscore.txt");
    if (score.is_open())
    {
        score >> highscore;
        
        score.close();
    }
    else
        highscore = 0;
}

void Game::quit()
{
    delete spritesheet;
    spritesheet = NULL;
    
    for (int i = 0; i < aliens.size(); ++i)
        delete aliens[i];
    aliens.clear();

    std::ofstream score("highscore.txt");
    if (score.is_open())
    {
        score << highscore;
        score.close();
    }
    else
        puts("ERROR: Unable to open highscore for writing");
}

Game::~Game()
{
    quit();
}


void Game::newWave()
{
    inIntro = false;
    
    ++waveCount;
    
    waitForPlayer = false;
    
    gridDir = true;
    gridOffset = 0;
    AlienBase::setGridOffsetX(gridOffset);
    AlienBase::setGridOffsetY(0);
    
    pLasers.clear();
    aLasers.clear();
    
    peaceTime = 60 * 4;
    timer = 0;
    
    //events.spawnEvents.clear();
    events.changeEvents.clear();
    events.killEvents.clear();
    
    for (int i = 0; i < players.size(); ++i)
        players[i].setCanShoot(true);

    /*
       0 1 2 3 4 5 6 7 8 9
       
       8 6 4 L 2 1 L 3 5 7
       x x G G G G G G x x
       x P P P P P P P P x
       B B B B B B B B B B
       B B B B B B B B B B
    */
    for (int y = 3; y < 6; ++y)
        for (int x = 0; x < GRIDW; ++x)
            events.spawnAlien(0, 0, x, y, true, TYPE_BLUE, x * 12);
    for (int x = 1; x < GRIDW - 1; ++x)
        events.spawnAlien(0, 0, x, 2, true, TYPE_PURPLE, x * 12);
    for (int x = 2; x < GRIDW - 2; ++x)
        events.spawnAlien(0, 0, x, 1, true, TYPE_GUARD, x * 12);
    events.spawnAlien(0, 0, 3, 0, true, TYPE_LEADER, 3 * 12);
    events.spawnAlien(0, 0, 6, 0, true, TYPE_LEADER, 6 * 12);

    // spawn leader alien for each escaped last round
    // try for middle first
    // its late at night
    // switch is good

    escaped = std::min(escaped, GRIDW - 2);
    switch (escaped)
    {
        case 0: break;
        case 8: events.spawnAlien(0, 0, 0, 0, true, TYPE_LEADER, 0 * 12);
        case 7: events.spawnAlien(0, 0, 9, 0, true, TYPE_LEADER, 9 * 12);
        case 6: events.spawnAlien(0, 0, 1, 0, true, TYPE_LEADER, 1 * 12);
        case 5: events.spawnAlien(0, 0, 8, 0, true, TYPE_LEADER, 8 * 12);
        case 4: events.spawnAlien(0, 0, 2, 0, true, TYPE_LEADER, 2 * 12);
        case 3: events.spawnAlien(0, 0, 7, 0, true, TYPE_LEADER, 7 * 12);
        case 2: events.spawnAlien(0, 0, 4, 0, true, TYPE_LEADER, 4 * 12);
        case 1: events.spawnAlien(0, 0, 5, 0, true, TYPE_LEADER, 5 * 12);
    }
    escaped = 0;
}


void Game::introWave()
{
    for (int i = 0; i < players.size(); ++i)
    {
        players[i].kill();
        players[i].setLives(0);
    }
    
    inIntro = true;
    
    AlienBase::setGridOffsetX(4 * 8);
    //AlienBase::setGridOffsetY(SCREENH / 5.f * 3.f);

    // wait 1 sec
    //   WE ARE THE GALAXIANS
    // wait 1 sec
    // MISSION: DESTROY ALIENS

    // wait 1 sec
    //  - SCORE ADVANCE TABLE -
    // wait 1 sec
    //      CONVOY  CHARGER
    // wait 1 sec
    // fly in

    events.spawnAlien(SCREENW, SCREENH / 10.f * 6.0f, 0, 0, false, TYPE_LEADER, 60 *  5);
    events.spawnAlien(SCREENW, SCREENH / 10.f * 6.5f, 0, 0, false, TYPE_GUARD,  60 *  8);
    events.spawnAlien(SCREENW, SCREENH / 10.f * 7.0f, 0, 0, false, TYPE_PURPLE, 60 * 11);
    events.spawnAlien(SCREENW, SCREENH / 10.f * 7.5f, 0, 0, false, TYPE_BLUE,   60 * 14);
}



void Game::updatePlayers()
{
    Uint8* kbd = SDL_GetKeyState(NULL);
    if (players.size() >= 1)
    {
        if (players[0].isAlive())
        {
            int move = 0;
            if (kbd[SDLK_LEFT]) move -= 1;
            if (kbd[SDLK_RIGHT]) move += 1;
            players[0].moveVel(move, 0);
            if (!peaceTime)
                if (kbd[SDLK_UP])
                    players[0].shoot(events);
        }
    }
    if (players.size() >= 2)
    {
        if (players[1].isAlive())
        {
            int move = 0;
            if (kbd[SDLK_a]) move -= 1;
            if (kbd[SDLK_d]) move += 1;
            players[1].moveVel(move, 0);
            if (!peaceTime)
                if (kbd[SDLK_w])
                    players[1].shoot(events);
        }
    }

    playersDead = true;
    for (int i = 0; i < players.size(); ++i)
    {
        if (players[i].isAlive())
        {
            playersDead = false;
            players[i].update();
            
            for (int a = 0; a < aliens.size(); ++a)
            {
                if (players[i].collides(aliens[a]))
                {
                    events.kill(&players[i], 2);
                    events.kill(aliens[a], 1);
                    players[i].addToScore(aliens[a]->getScore());
                }
            }
        }
        else if (players[i].getLives() > 0)
            playersDead = false;
    }
}

void Game::updateAliens()
{
    if (!inIntro)
    {
        // bounce on edge of alien grid hitting window edge
        int gridLeft = GRIDW - 1;
        int gridRight = 0;
        for (int i = 0; i < aliens.size(); ++i)
        {
            int x = aliens[i]->getGridX();
            if (x < gridLeft)
                gridLeft = x;
            if (x > gridRight)
                gridRight = x;
        }
        
        gridOffset += 0.3f * (gridDir ? 1 : -1);
        
        AlienBase temp(0, 0, gridLeft, 0, 0, false);
        if (temp.getCellX() <= 0 + temp.getWidth())
            gridDir = true;
        temp = AlienBase(0, 0, gridRight, 0, 0, false);
        if (temp.getCellX() >= SCREENW - temp.getWidth())
            gridDir = false;
    
        AlienBase::setGridOffsetX(gridOffset);
    }
    
    bool noLeaders = true;
    for (int i = 0; i < aliens.size(); ++i)
    {
        if (aliens[i]->getType() == TYPE_LEADER) noLeaders = false;

        // get closest player to target
        float px = SCREENW / 2.f;
        float bestDist = 100000.f;
        for (int p = 0; p < players.size(); ++p)
        {
            if (players[p].isAlive())
            {
                float dist = std::abs(players[p].getX() - aliens[i]->getX());
                if (dist < bestDist)
                {
                    px = players[p].getX();
                    bestDist = dist;
                }
            }
        }
        aliens[i]->update(events, px, attacking, aliens.size(), !waitForPlayer);
        aliens[i]->setMovable(false);
    }

    if (!peaceTime)
        for (int y = 0; y < GRIDH; ++y)
        {
            // first blues can attack, then leaders
            // purples are last
            // guards are under control most of the time
            bool isTime = false;
            switch (y)
            {
                case 0: case 1: if (timer >= 10 * 60) isTime = true; break;
                case 2: if (timer >= 30 * 60) isTime = true; break;
                case 3: case 4: isTime = true; break;
            }
            if (isTime)
            {
                AlienBase* first = NULL;
                AlienBase* last = NULL;
                for (int x = 0; x < GRIDW; ++x)
                {
                    if (alienGrid[y][x])
                    {
                        if (!first) first = alienGrid[y][x];
                        last = alienGrid[y][x];
                    }
                }
                // guards cannot leave until all guards are gone
                if (y != 1 || noLeaders)
                {
                    if (first) first->setMovable(true);
                    if (last) last->setMovable(true);
                }
            }
        }
}

void Game::updateLasers()
{
    for (int i = 0; i < pLasers.size(); ++i)
    {
        pLasers[i].update(events);

        for (int a = 0; a < aliens.size(); ++a)
            if (pLasers[i].collides(aliens[a]))
            {
                events.kill(aliens[a], 1);
                events.kill(&pLasers[i]);
                ((Spaceship*)pLasers[i].getOwner())->addToScore(aliens[a]->getScore());
            }
    }

    for (int i = 0; i < aLasers.size(); ++i)
    {
        aLasers[i].update(events);

        for (int p = 0; p < players.size(); ++p)
            if (players[p].isAlive())
            {
                if (aLasers[i].collides(&players[p]))
                {
                    events.kill(&players[p], 2);
                    events.kill(&aLasers[i]);
                }
            }
    }
}

void Game::updateStars()
{
    if (rand() % 5 == 0)
        stars.push_back(Star());
    
    for (int i = 0; i < stars.size(); i++)
    {
        stars[i].update();
        if (stars[i].y - 2 * stars[i].r > SCREENH)
        {
            stars[i] = stars.back();
            stars.pop_back();
        }
    }
}

void Game::draw(Surface& surface)
{
    surface.lock();
    surface.fill(BLACK);

    for (int i = 0; i < stars.size(); ++i)
        stars[i].draw(surface);

    for (int i = 0; i < explosions.size(); ++i)
        explosions[i].draw(surface, spritesheet);
    
    for (int i = 0; i < pLasers.size(); ++i)
        pLasers[i].draw(surface);

    for (int i = 0; i < aLasers.size(); ++i)
        aLasers[i].draw(surface);

    for (int i = 0; i < aliens.size(); ++i)
    {
        aliens[i]->draw(surface, spritesheet);
    }

    for (int i = 0; i < players.size(); ++i)
        if (players[i].isAlive())
            players[i].draw(surface, spritesheet);

    // draw lives
    for (int i = 0; i < players.size(); ++i)
        for (int l = 0; l < players[i].getLives(); ++l)
        {
            int pos = 0;
            if (i == 0)
                pos = SCREENW-1 - (l + 1) * 16;
            else if (i == 1)
                pos = l * 16;
            drawImage(surface, spritesheet, Rect(pos, 16, 16, 16), 16, 64);
        }

    // draw scores
    std::string p0Score = std::to_string(players[0].getScore());
    std::string p1Score = std::to_string(players[1].getScore());
    drawText(surface, spritesheet, p0Score, SCREENW-1 - (p0Score.size() * 8), 0);
    drawText(surface, spritesheet, p1Score, 0, 0);

    if (playersDead)
        drawText(surface, spritesheet, "GAME  OVER",
            SCREENW/2 - 4 * 8, SCREENH / 5 * 3, 1);

    std::string high = std::to_string(highscore);
    drawText(surface, spritesheet, "HIGH SCORE", SCREENW/2 - 5 * 8, 0);
    drawText(surface, spritesheet, high, SCREENW/2 - high.size() * 4, 10);
    
    int smallCount = waveCount % 10;
    int largeCount = waveCount / 10;
    int flagOffset = (smallCount * 8 + largeCount * 16) / 2;
    // small flag
    for (int i = 0; i < smallCount; ++i)
    {
        drawImage(surface, spritesheet,
            Rect(SCREENW/2 - flagOffset + i * 8,
                 SCREENH-16, 8, 16), 40, 64);
    }
    // large flag
    for (int i = 0; i < largeCount; ++i)
    {
        drawImage(surface, spritesheet,
            Rect(SCREENW/2 - flagOffset + i * 16 + smallCount * 8,
                 SCREENH-16, 16, 16), 48, 64);
    }

    
    // debug stuff
    if (0)
    {
        drawText(surface, spritesheet, std::to_string(timer), SCREENW/2, 0);
        drawText(surface, spritesheet, std::to_string(attacking), SCREENW/2, 10);
        drawText(surface, spritesheet, std::to_string(events.spawnEvents.size()), SCREENW/2, 20);
    }

    if (inIntro)
        drawIntro(surface);
    
    surface.unlock();
    surface.flip();
}

void Game::drawIntro(Surface& surface)
{
    if (timer >= 60 * 1.25)
        drawText(surface, spritesheet, "  WE ARE THE GALAXIANS",
            SCREENW/2 - 11 * 8, SCREENH/4, 1);
    if (timer >= 60 * 2.50)
        drawText(surface, spritesheet, "MISSION: DESTROY ALIENS",
            SCREENW/2 - 11 * 8, SCREENH/4 + 16, 1);
    if (timer >= 60 * 3.75)
        drawText(surface, spritesheet, " - SCORE ADVANCE TABLE -",
            SCREENW/2 - 11 * 8, SCREENH/2, 0);
    if (timer >= 60 * 5.00)
        drawText(surface, spritesheet, "     CONVOY  CHARGER",
            SCREENW/2 - 11 * 8, SCREENH/2 + 16, 2);

    for (int i = 0; i < aliens.size(); ++i)
    {
        drawText(surface, spritesheet,
            std::to_string(aliens[i]->getScore()) + "      " +
            (aliens[i]->getScore()*2 >= 100 ? "" : " ") + 
            std::to_string(aliens[i]->getScore()*2),
            aliens[i]->getX() + 4 * 8, aliens[i]->getY() - 4, 2);

        drawImage(surface, spritesheet,
            Rect(aliens[i]->getX() + 17 * 8, aliens[i]->getY() - 4, 16, 8),
                  128, 64);
    }

    if (timer >= 60 * 18)
        if (timer / 30 % 2)
            drawText(surface, spritesheet, "PRESS SPACE TO START",
                     SCREENW/2 - 9 * 8, SCREENH / 10 * 9, 2);
}


void Game::update()
{
    if (inIntro)
    {
        updateIntro();
        return;
    }

    if (playersDead)
    {
        Uint8* kbd = SDL_GetKeyState(NULL);
        if (kbd[SDLK_SPACE])
        {
            for (int i = 0; i < aliens.size(); ++i)
                delete aliens[i];
            aliens.clear();
            events.spawnEvents.clear();
            events.changeEvents.clear();
            events.killEvents.clear();
            escaped = 0;
            waveCount = -1;
            waitForPlayer = false;
            attacking = 0;
            aLasers.clear();
            pLasers.clear();
            explosions.clear();
            
            for (int i = 0; i < players.size(); ++i)
            {
                highscore = std::max(highscore, players[i].getScore());
                players[i].setScore(0);
                players[i].setLives(5);
                events.spawnPlayer(SCREENW / ((float)players.size()+1) * (players.size()-i),
                    PLAYER_Y, i, 60 * 1);
            }
            
            newWave();
        }
    }
    
    // it was really annoying to wait 10 seconds for aliens at the top to fall
    // speed up the whole game while they are waiting on players
    // make sure nothing is happening
    bool speedup = explosions.empty() && pLasers.empty() && timer > 60 * 5;
    for (int i = 0; i < players.size(); ++i)
        if (players[i].isAlive())
            speedup = false;
    
    for (int t = 0; t <= speedup; ++t)
    {
        ++timer;
        if (peaceTime)
            --peaceTime;
        else if (aliens.empty())
            newWave();
        
        for (int i = 0; i < explosions.size(); ++i)
            explosions[i].update(events);
        
        updateStars();
        
        updatePlayers();
        updateAliens();
        updateLasers();

        processChangeEvents();
        processKillEvents();
        processSpawnEvents();
    }
}


void Game::updateIntro()
{
    ++timer;
    
    updateStars();

    updateAliens();

    
    processChangeEvents();
    processKillEvents();
    processSpawnEvents();

    Uint8* kbd = SDL_GetKeyState(NULL);
    if (kbd[SDLK_SPACE])
    {
        for (int i = 0; i < aliens.size(); ++i)
            delete aliens[i];
        aliens.clear();
        events.spawnEvents.clear();

        for (int i = 0; i < players.size(); ++i)
        {
            players[i].setLives(5);
            events.spawnPlayer(SCREENW / ((float)players.size()+1) * (players.size()-i),
                PLAYER_Y, i, 60 * 1);
        }
        
        newWave();
    }
}


void Game::processSpawnEvents()
{
    for (int i = events.spawnEvents.size() - 1; i >= 0; --i)
    {       
        SpawnEvent& spawn = events.spawnEvents[i];
        bool keep = false;
        
        if (spawn.delay)
        {
            --spawn.delay;
            continue;
        }

        switch (spawn.type)
        {
            case TYPE_PLAYER:
            {
                if (attacking)
                    keep = true;
                else
                {
                    if (!players[spawn.gx].isAlive() && players[spawn.gx].getLives())
                        players[spawn.gx].respawn(spawn.x, spawn.y);

                    // wait until players have respawned to set aliens free again
                    // players can't respawn until aliens have stopped attacking
                    // out of lives players don't count as dead
                    int deadPlayers = 0;
                    for (int i = 0; i < players.size(); ++i)
                        if (!players[i].isAlive() && players[i].getLives())
                            ++deadPlayers;
                    if (deadPlayers == 0)
                        waitForPlayer = false;
                }
                break;
            }

            case TYPE_LASER:
                aLasers.push_back(Laser(spawn.x, spawn.y, false, NULL));
                break;

            case TYPE_LASER + 1: // player laser
                pLasers.push_back(Laser(spawn.x, spawn.y, true,
                                        (Spaceship*)spawn.owner));
                break;

            case TYPE_BLUE:
                aliens.push_back(
                    new AlienBlue(spawn.x, spawn.y, spawn.gx, spawn.gy, spawn.onGrid));
                if (spawn.onGrid)
                    alienGrid[spawn.gy][spawn.gx] = aliens.back();
                break;
                    
            case TYPE_PURPLE:
                aliens.push_back(
                    new AlienPurple(spawn.x, spawn.y, spawn.gx, spawn.gy, spawn.onGrid));
                if (spawn.onGrid)
                    alienGrid[spawn.gy][spawn.gx] = aliens.back();
                break;
                
            case TYPE_GUARD:
                aliens.push_back(
                    new AlienGuard(spawn.x, spawn.y, spawn.gx, spawn.gy, spawn.onGrid));
                if (spawn.onGrid)
                    alienGrid[spawn.gy][spawn.gx] = aliens.back();
                break;
                
            case TYPE_LEADER:
                aliens.push_back(
                    new AlienLeader(spawn.x, spawn.y, spawn.gx, spawn.gy, spawn.onGrid));
                if (spawn.onGrid)
                    alienGrid[spawn.gy][spawn.gx] = aliens.back();
                break;
        }

        if (!keep)
        {
            std::swap(events.spawnEvents[i], events.spawnEvents.back());
            events.spawnEvents.pop_back();
        }
    }
}

void Game::processChangeEvents()
{
    for (int i = events.changeEvents.size() - 1; i >= 0; --i)
    {
        ChangeEvent change = events.changeEvents[i];
        
        AlienBase* alien = (AlienBase*)change.target;
        if (change.delay)
        {
            --events.changeEvents[i].delay;
            continue;
        }

        // change cannot be a reference
        // requesting guards adds to changeEvents,
        // invalidating the reference sometimes
        std::swap(events.changeEvents[i], events.changeEvents.back());
        events.changeEvents.pop_back();

        switch (change.action)
        {
            case ACTION_RET_TOP:
                // kill already takes away attacking count
                // leader alien that made it to end without any guards at start escapes
                if (alien->getType() == TYPE_LEADER && alien->hadNoGuards())
                {
                    ++escaped;
                    events.kill(alien);
                }
                else
                {
                    // should be obvious if it requested to return to top
                    // AFTER ATTACKING that it is IN FACT ATTACKING
                    // but instead fires 20 returns messing up the game

                    // return to top fucks up after a while
                    if (alien->isAttacking())
                        --attacking;
                    alien->setReturning();
                    alien->setPos(SCREENW / 2, 0);
                    alien->setLeader(NULL);
                }
                break;

            case ACTION_LEAVE_GRID:
                if (!peaceTime)
                {
                    // sanity check
                    if (!alien->isAttacking())
                        ++attacking;
                    alien->setOnGrid(false);
                    alienGrid[alien->getGridY()][alien->getGridX()]
                        = NULL;
                }
                break;

            case ACTION_ENTER_GRID:
                alien->setOnGrid(true);
                alienGrid[alien->getGridY()][alien->getGridX()]
                    = alien;
                break;

            case ACTION_REQ_GUARDS:
                if (!peaceTime)
                {
                    int y = std::min(change.gy + 1, GRIDH-1);
                    int endX = std::min(change.gx + 1, GRIDW);
                    int startX = std::max(change.gx - 1, 0);
                    int count = 0;
                    // the actual game seemed random which guards it chose
                    // randomly starts from left or right
                    if (rand() % 2)
                    {
                        for (int x = startX; x <= endX && count < 2; ++x)
                            if (alienGrid[y][x] && alienGrid[y][x]->getType() == TYPE_GUARD)
                            {
                                events.leaveGrid(alienGrid[y][x]);
                                alienGrid[y][x]->setLeader(alien);
                                ++count;
                            }
                    }
                    else
                    {
                        for (int x = endX; x >= startX && count < 2; --x)
                            if (alienGrid[y][x] && alienGrid[y][x]->getType() == TYPE_GUARD)
                            {
                                events.leaveGrid(alienGrid[y][x]);
                                alienGrid[y][x]->setLeader(alien);
                                ++count;
                            }
                    }

                    alien->setGuardCount(count);
                }
                break;
        }
    }
}

void Game::processKillEvents()
{
    for (int i = events.killEvents.size() - 1; i >= 0; --i)
    {
        KillEvent& kill = events.killEvents[i];
        ObjectBase* object = (ObjectBase*)kill.target;
        if (kill.delay)
        {
            --kill.delay;
            continue;
        }

        // pass to mark objects as deleted, without messing up indexes
        object->markDelete();
        if (kill.explode)
        {
            explosions.push_back(Explosion(
                object->getX(), object->getY(), kill.explode));
        }
        
        std::swap(kill, events.killEvents.back());
        events.killEvents.pop_back();
    }

    for (int i = players.size() - 1; i >= 0; --i)
        if (players[i].shouldDelete())
        {
            events.spawnPlayer(SCREENW / ((float)players.size()+1) * (players.size()-i),
                PLAYER_Y, i, 60 * 5);
            players[i].kill();
            waitForPlayer = true;
        }

    for (int i = pLasers.size() - 1; i >= 0; --i)
        if (pLasers[i].shouldDelete())
        {
            if (pLasers[i].getOwner())
                ((Spaceship*)pLasers[i].getOwner())->setCanShoot(true);
            std::swap(pLasers[i], pLasers.back());
            pLasers.pop_back();
        }

    for (int i = aliens.size() - 1; i >= 0; --i)
        if (aliens[i]->shouldDelete())
        {
            // remove any events referencing this alien
            for (int ii = events.changeEvents.size() - 1; ii >= 0; --ii)
                if (events.changeEvents[ii].target == aliens[i])
                {
                    std::swap(events.changeEvents[ii], events.changeEvents.back());
                    events.changeEvents.pop_back();
                }

            if (!aliens[i]->isOnGrid())
            {
                // remove leader pointer from all guards
                if (aliens[i]->getType() == TYPE_LEADER)
                {
                    for (int ii = 0; ii < aliens.size(); ++ii)
                        if (aliens[ii]->getLeader() == aliens[i])
                            aliens[ii]->setLeader(NULL);
                }
                // decrease guards of leader
                else if (aliens[i]->getType() == TYPE_GUARD)
                {
                    for (int ii = 0; ii < aliens.size(); ++ii)
                        if (aliens[ii] == aliens[i]->getLeader())
                            aliens[ii]->decGuardCount();
                }
            }
                
            if (aliens[i]->isAttacking())
                --attacking;
            if (aliens[i]->isOnGrid())
                alienGrid[aliens[i]->getGridY()][aliens[i]->getGridX()] = NULL;
            std::swap(aliens[i], aliens.back());
            delete aliens.back();
            aliens.pop_back();
        }
    
    for (int i = aLasers.size() - 1; i >= 0; --i)
        if (aLasers[i].shouldDelete())
        {
            std::swap(aLasers[i], aLasers.back());
            aLasers.pop_back();
        }

    for (int i = explosions.size() - 1; i >= 0; --i)
        if (explosions[i].shouldDelete())
        {
            std::swap(explosions[i], explosions.back());
            explosions.pop_back();
        }
}
