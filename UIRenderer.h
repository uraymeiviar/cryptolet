#ifndef UIRENDERER_H
#define UIRENDERER_H

#include "Adafruit_GFX.h"

class UIRenderer
{
    public:
        UIRenderer(Adafruit_GFX& screen, int offsetX, int offsetY, int width, int height);
        virtual void draw() = 0;
    protected:
        Adafruit_GFX& screen;
        int screenOffsetX;
        int screenOffsetY;
        int screenWidth;
        int screenHeight;
};

#endif

