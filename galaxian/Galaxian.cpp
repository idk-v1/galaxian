// Name: Ben Hamilton
// File: Galaxian.cpp

#include "Includes.h"
#include "Surface.h"
#include "Event.h"

#include "Game.h"

int main(int argc, char* argv[])
{
    Surface surface(SCREENW, SCREENH);
    SDL_WM_SetCaption("Galaxian", NULL);

    Game game;
    game.init();

    int lastTicks = getTicks();
    int deltaTime = 0;
    int ups = 60;
    
    bool running = true;
    while (running)
    {
        Event event;
        while (event.poll())
        {
            switch (event.type())
            {
                case QUIT: running = false; break;
            }
        }

        int ticks = getTicks();
        deltaTime += ticks - lastTicks;
        while (deltaTime >= 1000 / ups)
        {
            deltaTime -= 1000 / ups;

            game.update();
        }
        lastTicks = ticks;

        game.draw(surface);
    }

    game.quit();

    
    // Memory leaks from SDL are by design
    // "Nooo we can't shutdown dbus cuz other things in the program might use it."
    // "Add an option to shut it down then."
    // "No it's not needed. Just don't use a memory debugger and you can't even tell."

    /*
      ______________________________________________________________________________________________
     / slouken on Nov 8, 2023                                                     (Collaborator) ...\
     |----------------------------------------------------------------------------------------------|
     | Crashing the program is worse than leaking memory which will be cleaned up by the OS. :)     |
     |                                                                                              |
     | (:thumbsdown: 2)                                                                             |
     \______________________________________________________________________________________________/
         slouken closed this as not planned on Nov 8, 2023
    */
    
    return 0;
}
