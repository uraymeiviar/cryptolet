#include "UIRenderer.h"

UIRenderer::UIRenderer(Adafruit_GFX& screen, int offsetX, int offsetY, int width, int height)
    :screen(screen)
{
    this->screenOffsetX = offsetX;
    this->screenOffsetY = offsetY;
    this->screenWidth = width;
    this->screenHeight = height;
}