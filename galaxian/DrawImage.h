// Name: Ben Hamilton
// File: DrawImage.h

#ifndef DRAW_IMAGE_H
#define DRAW_IMAGE_H

#include "Includes.h"
#include "Surface.h"
#include "Constants.h"

#include <string>

void drawImage(Surface& surface, Image* image,
    Rect rect, int tx, int ty);

void drawText(Surface& surface, Image* image,
    const std::string& str, int x, int y, int color = 0);

#endif
