#ifndef PRICECANDLE_H
#define PRICECANDLE_H

#include "PriceData.h"
#include <stddef.h>

class PriceCandle {
    public:
        const PriceData& getRecentData() const;
        const PriceData& getOldestData() const;
        const PriceData* getData(size_t index = 0) const;
        size_t virtual addData(const PriceData& newData);
        size_t virtual addData(uint32_t timestamp, float closePrice);
        size_t getValidDataCount() const;
        size_t getMaxDataCount() const;
        float getHistoryMaxValue() const;
        float getHistoryMinValue() const;
        PriceCandle(size_t intervalSecs, size_t size = 60);
        virtual ~PriceCandle();
        float getPeriodDiff() const;
        float getLastUpdateDiff() const;
        uint32_t getPeriodSecs() const;

    protected :
        float minValue;
        float maxValue;
        size_t virtual addNewData(const PriceData& newData);
        size_t virtual addNewData(uint32_t timestamp, float closePrice);
        size_t interval;
        PriceData* data;

        const size_t dataCount;
        size_t lastDataIndex;
        size_t writeDataIndex;
        size_t validDataCount;
};

#endif
