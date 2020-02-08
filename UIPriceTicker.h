#ifndef UIPRICETICKER_H
#define UIPRICETICKER_H
#include "UIRenderer.h"
#include "PriceHistory.h"

class UIPriceTicker : public UIRenderer
{
    public:
        UIPriceTicker(const PriceHistory& history, Adafruit_GFX& screen, int offsetX, int offsetY, int width, int height);
        PriceCandlePeriod period;
        std::string label;
        virtual void draw() override;
    protected:        
        const PriceHistory& priceHistory;        
        void drawTicker();
        void drawChart();
        void drawCandle(const PriceData* data, int posX, int posY, float minV, float maxV, int height);
        String formatThousands(float value);
};

#endif
