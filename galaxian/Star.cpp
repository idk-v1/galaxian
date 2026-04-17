// Name: Ben Hamilton
// File: Star.cpp

#include "Star.h"

Color colorLerp(Color x, Color y, float v)
{
    Color out;
    out.r = x.r * v + y.r * (1.f - v);
    out.g = x.g * v + y.g * (1.f - v);
    out.b = x.b * v + y.b * (1.f - v);
    return out;
}

Color colorLerp2(Color x, Color y, Color z, float v)
{
    if (v < 0.f)
        return colorLerp(x, y, -v);
    return colorLerp(y, z, v);
}

Star::Star(bool atTop)
{
    r = (rand() % 20) / 10.f + 1;
    if (atTop)
        y = -r * 2;
    else
        y = rand() % SCREENH;
    x = rand() % SCREENW;

    float heat = (rand() % 21 - 10) / 10.f;

    Color hotColor =  {0xBF, 0xDF, 0xFF};
    Color midColor =  {0xFF, 0xFF, 0xFF};
    Color coolColor = {0xFF, 0xDF, 0xBF};

    color = colorLerp2(hotColor, midColor, coolColor, heat);
}

void Star::draw(Surface& surface)
{
    surface.put_circle(x - r, y - r, r, color);
}

void Star::update()
{
    y += r / 2.f;
}
