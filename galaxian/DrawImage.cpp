// Name: Ben Hamilton
// File: DrawImage.cpp

#include "DrawImage.h"


void drawImage(Surface& surface, Image* image,
    Rect rect, int tx, int ty)
{
    Rect src(tx, ty, rect.w, rect.h);
    surface.put_image(*image, src, rect);
}

void drawText(Surface& surface, Image* image,
    const std::string& str, int x, int y, int color)
{
    Rect charRect(x, y, 8, 8);
    for (int i = 0; i < str.size(); ++i)
    {
        char c = str[i];

        int pos = -1;

        if (c == '\n')
        {
            charRect.x = x;
            charRect.y += charRect.h;
        }
        else if (c >= 'a' && c <= 'z')
            pos = c - 'a' + 10;
        else if (c >= 'A' && c <= 'Z')
            pos = c - 'A' + 10;
        else if (c >= '0' && c <= '9')
            pos = c - '0';
        else if (c == '-')
            pos = 10 + 26;
        else if (c == ':')
            pos = 10 + 26 + 1;
        else if (c == ' ')
            charRect.x += charRect.w;

        if (pos != -1)
        {
            drawImage(surface, image, charRect, pos * charRect.w,
                128 + color * 8);
            charRect.x += charRect.w;
        }
    }
}

