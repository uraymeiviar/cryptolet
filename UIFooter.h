#ifndef _UIFOOTER_H_
#define _UIFOOTER_H_

#include "UIRenderer.h"

class UIFooter : UIRenderer
{
    public:
        UIFooter(Adafruit_GFX& screen, int offsetX, int offsetY, int width, int height);
        virtual void draw() override;
};

#endif
