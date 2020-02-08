#ifndef PRICEHISTORY_H
#define PRICEHISTORY_H
#include "PriceCandle.h"

enum PriceCandlePeriod : uint8_t {
    smallest  = 0,
    minutely  = 1,
    hourly    = 2,
    daily     = 3 
};

class PriceHistory : public PriceCandle{
    public :
        PriceHistory(size_t size = 60);        
        virtual ~PriceHistory();
        size_t virtual addData(const PriceData& newData) override;
        size_t virtual addData(uint32_t timestamp, float closePrice) override;

        const PriceData& getRecentDataInPeriod(PriceCandlePeriod period) const;
        const PriceData* getDataInPeriod(PriceCandlePeriod period, size_t index = 0) const;
        size_t getValidDataCountInPeriod(PriceCandlePeriod period) const;
        size_t getMaxDataCountInPeriod(PriceCandlePeriod period) const;
        float getHistoryMaxValueInPeriod(PriceCandlePeriod period) const;
        float getHistoryMinValueInPeriod(PriceCandlePeriod period) const;

        const PriceData& getOldestDataInPeriod(PriceCandlePeriod period) const;
        float getPeriodDiffInPeriod(PriceCandlePeriod period) const;
        float getLastUpdateDiffInPeriod(PriceCandlePeriod period) const;
        uint32_t getPeriodSecsInPeriod(PriceCandlePeriod period) const;
    protected:
        PriceCandle* candles;
};

#endif
