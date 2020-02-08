#ifndef PRICEDATA_H
#define PRICEDATA_H
#include <stdint.h>
#include <string>

class PriceData {
    public :
        float openPrice;  
        float highPrice;  
        float lowPrice;
        float closePrice;
        uint32_t timestamp;

        PriceData();
        bool isUp() const;
        bool isDown() const;
        float diff() const;

        void operator=(const PriceData& rhs);
        void mergeData(const PriceData& newData);
        void mergeData(uint32_t timestamp, float closePrice);
        float getLowestThan(float refPrice) const;
        float getHighestThan(float refPrice) const;
}; 

#endif
