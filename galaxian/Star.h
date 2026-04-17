// Name: Ben Hamilton
// File: Star.h

#include "Includes.h"
#include "Surface.h"

#include "Constants.h"

class Star
{
public:
    Star(bool atTop = true);

    void draw(Surface& surface);

    void update();
    
    float x, y, r;
    Color color;
};

