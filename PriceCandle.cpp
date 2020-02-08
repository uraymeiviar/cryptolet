#include "PriceCandle.h"
#include <Arduino.h>

PriceCandle::PriceCandle(size_t intervalSecs, size_t size)
    :dataCount(size)
{
    this->interval = intervalSecs;
    this->data = new PriceData[size];
    this->lastDataIndex = 0;
    this->writeDataIndex = 0;
    this->validDataCount = 0;
    this->minValue = 3.402823466E+38;
    this->maxValue  = 1.175494351E-38;
}

PriceCandle::~PriceCandle()
{
    if(this->data != nullptr)
    {
        delete[] this->data;
        this->data = nullptr;
    }
}

size_t PriceCandle::getValidDataCount() const 
{
    return this->validDataCount;
}

size_t PriceCandle::getMaxDataCount() const 
{
    return this->dataCount;
}

const PriceData& PriceCandle::getRecentData() const
{
    return this->data[this->lastDataIndex];
}

const PriceData& PriceCandle::getOldestData() const
{
    if(this->validDataCount < this->dataCount)
    {
        return this->data[0];
    }
    else{
        return *this->getData(this->writeDataIndex);
    }    
}

size_t PriceCandle::addNewData(const PriceData& newData)
{    
    bool recalcMinMax = false;
    if(this->data[this->writeDataIndex].timestamp > 0){
        if(this->minValue == this->data[this->writeDataIndex].lowPrice ||
           this->maxValue == this->data[this->writeDataIndex].highPrice )
        {
            recalcMinMax = true;
        }
    }

    this->data[this->writeDataIndex] = newData;

    this->lastDataIndex = this->writeDataIndex;
    this->validDataCount++;
    if(this->validDataCount > this->dataCount){
        this->validDataCount = this->dataCount;
    }

    const PriceData* prevCandle = this->getData(1);
    
    if(prevCandle){
        this->data[this->writeDataIndex].openPrice = prevCandle->closePrice;
    }
    else{
        this->data[this->writeDataIndex].openPrice = this->data[this->writeDataIndex].closePrice;
    }

    if(recalcMinMax)
    {
        this->minValue = 3.402823466E+38;
        this->maxValue  = 1.175494351E-38;
        for(size_t i=0 ; i<this->dataCount ; i++){
            const PriceData* p = this->getData(i);
            if(p != nullptr){
                this->minValue = p->getLowestThan(this->minValue);
                this->maxValue = p->getHighestThan(this->maxValue);
            }
        }
    }
    else
    {
        this->minValue = this->data[this->writeDataIndex].getLowestThan(this->minValue);
        this->maxValue = this->data[this->writeDataIndex].getHighestThan(this->maxValue);
    }

    this->writeDataIndex++;
    if(this->writeDataIndex >= this->dataCount){
        this->writeDataIndex = 0;
    }    

    return this->lastDataIndex;
}

size_t PriceCandle::addNewData(uint32_t timestamp, float closePrice)
{
    PriceData newData;
    newData.timestamp = timestamp;
    newData.closePrice = closePrice;
    if(this->validDataCount <= 0){
        newData.openPrice = newData.closePrice;
    }
    else{
        newData.openPrice = this->getRecentData().closePrice;
    }
    if(newData.isUp()){
        newData.lowPrice = newData.openPrice;
        newData.highPrice = newData.closePrice;
    }
    else{
        newData.lowPrice = newData.closePrice;
        newData.highPrice = newData.openPrice;
    }
    return this->addNewData(newData);
}

size_t PriceCandle::addData(const PriceData& newData)
{
    if(this->validDataCount <= 0){
        return this->addNewData(newData);
    }
    size_t recentDataIndex = this->getRecentData().timestamp/this->interval;
    size_t newDataIndex = newData.timestamp/this->interval;
    if(newDataIndex > recentDataIndex)
    {
        return this->addNewData(newData);
    }
    else{
        size_t indexOffset = recentDataIndex - newDataIndex;
        PriceData* oldData = (PriceData*)this->getData(indexOffset);
        if(oldData != nullptr){
            oldData->mergeData(newData);
            this->minValue = oldData->getLowestThan(this->minValue);
            this->maxValue = oldData->getHighestThan(this->maxValue);
            return indexOffset;
        }
        else{
            Serial.print(" merge target not exists, offset=");
            Serial.println(indexOffset);
            return -1;
        }
    }
}

size_t PriceCandle::addData(uint32_t timestamp, float closePrice)
{
    if(this->validDataCount <= 0){
        return this->addNewData(timestamp, closePrice);
    }
    size_t recentDataIndex = this->getRecentData().timestamp/this->interval;
    size_t newDataIndex = timestamp/this->interval;
    if(newDataIndex > recentDataIndex)
    {
        return this->addNewData(timestamp, closePrice);
    }
    else{        
        size_t indexOffset = recentDataIndex - newDataIndex;
        PriceData* oldData = (PriceData*)this->getData(indexOffset);
        if(oldData != nullptr){
            oldData->mergeData(timestamp, closePrice);
            this->minValue = oldData->getLowestThan(this->minValue);
            this->maxValue = oldData->getHighestThan(this->maxValue);            
            return indexOffset;
        }
        else{
            Serial.print(" merge target not exists, offset=");
            Serial.println(indexOffset);
            return -1;
        }
    }
}

const PriceData* PriceCandle::getData(size_t index) const 
{
    if(index >= this->dataCount || index >= this->validDataCount){
        return nullptr;
    }
    else{
        if(index > this->lastDataIndex){
            return &this->data[this->dataCount - (index - this->lastDataIndex)];
        }
        else{
            return &this->data[this->lastDataIndex - index];
        }
    }
}

float PriceCandle::getHistoryMaxValue() const
{
    return this->maxValue;
}

float PriceCandle::getHistoryMinValue() const
{
    return this->minValue;
}

float PriceCandle::getPeriodDiff() const{
    float currentPrice = this->getRecentData().closePrice;
    float oldestPrice = this->getOldestData().openPrice;
    return currentPrice - oldestPrice;
}

float PriceCandle::getLastUpdateDiff() const{
    if(this->validDataCount > 1){
        float currentPrice = this->getRecentData().closePrice;
        float lastPrice = this->getData(1)->openPrice;
        return currentPrice - lastPrice;
    }
    else{
        return 0.0f;
    }
}

uint32_t PriceCandle::getPeriodSecs() const
{
    float currentTImestamp = this->getRecentData().timestamp;
    float oldestTiemstamp = this->getOldestData().timestamp;
    return currentTImestamp - oldestTiemstamp;
}