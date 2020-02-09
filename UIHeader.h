#ifndef _UIHEADER_H_
#define _UIHEADER_H_

#include <WiFi.h>
#include "UIRenderer.h"

class UIHeader : UIRenderer
{
    public:
        UIHeader(WiFiClass& wifi, Adafruit_GFX& screen, int offsetX, int offsetY, int width, int height);
        virtual void draw() override;
    protected:
        WiFiClass& wifi;
};

#endif
