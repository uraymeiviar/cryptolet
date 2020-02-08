#include "PriceHistory.h"

PriceHistory::PriceHistory(size_t size)
    : PriceCandle(1,size)
{
    this->candles = new PriceCandle[3]{
        {60*5, size},
        {60*60, size},
        {60*60*24, size}
    };
}

PriceHistory::~PriceHistory()
{
    if(this->candles != nullptr)
    {
        delete[] this->candles;
        this->candles = nullptr;
    }
}

size_t PriceHistory::addData(const PriceData& newData)
{    
    this->candles[PriceCandlePeriod::minutely-1].addData(newData);
    this->candles[PriceCandlePeriod::hourly  -1].addData(newData);
    this->candles[PriceCandlePeriod::daily   -1].addData(newData);
    return PriceCandle::addData(newData);
}

size_t PriceHistory::addData(uint32_t timestamp, float closePrice)
{
    this->candles[PriceCandlePeriod::minutely-1].addData(timestamp, closePrice);
    this->candles[PriceCandlePeriod::hourly  -1].addData(timestamp, closePrice);
    this->candles[PriceCandlePeriod::daily   -1].addData(timestamp, closePrice);
    return PriceCandle::addData(timestamp, closePrice);
}

const PriceData& PriceHistory::getRecentDataInPeriod(PriceCandlePeriod period) const
{
    if(period == PriceCandlePeriod::smallest){
        return this->getRecentData();
    }
    else{
        return this->candles[period-1].getRecentData();
    }
}

const PriceData* PriceHistory::getDataInPeriod(PriceCandlePeriod period, size_t index) const
{
    if(period == PriceCandlePeriod::smallest){
        return this->getData(index);
    }
    else{
        const PriceCandle& candle = this->candles[period-1];
        return candle.getData(index);
    }
}

size_t PriceHistory::getValidDataCountInPeriod(PriceCandlePeriod period) const
{
    if(period == PriceCandlePeriod::smallest){
        return this->getValidDataCount();
    }
    else{
        return this->candles[period-1].getValidDataCount();
    }
}

size_t PriceHistory::getMaxDataCountInPeriod(PriceCandlePeriod period) const
{
    if(period == PriceCandlePeriod::smallest){
        return this->getMaxDataCount();
    }
    else{
        return this->candles[period-1].getMaxDataCount();
    }
}

float PriceHistory::getHistoryMaxValueInPeriod(PriceCandlePeriod period) const
{
    if(period == PriceCandlePeriod::smallest){
        return this->getHistoryMaxValue();
    }
    else{
        return this->candles[period-1].getHistoryMaxValue();
    }
}

float PriceHistory::getHistoryMinValueInPeriod(PriceCandlePeriod period) const
{
    if(period == PriceCandlePeriod::smallest){
        return this->getHistoryMinValue();
    }
    else{
        return this->candles[period-1].getHistoryMinValue();
    }
}

const PriceData& PriceHistory::getOldestDataInPeriod(PriceCandlePeriod period) const
{
    if(period == PriceCandlePeriod::smallest){
        return this->getOldestData();
    }
    else{
        return this->candles[period-1].getOldestData();
    }
}

float PriceHistory::getPeriodDiffInPeriod(PriceCandlePeriod period) const
{
    if(period == PriceCandlePeriod::smallest){
        return this->getPeriodDiff();
    }
    else{
        return this->candles[period-1].getPeriodDiff();
    }
}

float PriceHistory::getLastUpdateDiffInPeriod(PriceCandlePeriod period) const
{
    if(period == PriceCandlePeriod::smallest){
        return this->getLastUpdateDiff();
    }
    else{
        return this->candles[period-1].getLastUpdateDiff();
    }
}

uint32_t PriceHistory::getPeriodSecsInPeriod(PriceCandlePeriod period) const
{
    if(period == PriceCandlePeriod::smallest){
        return this->getPeriodSecs();
    }
    else{
        return this->candles[period-1].getPeriodSecs();
    }
}
