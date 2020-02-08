#include "PriceData.h"

PriceData::PriceData()
{
    this->timestamp  = 0;
    this->openPrice  = 0.0f;
    this->highPrice  = 0.0f;
    this->lowPrice   = 0.0f;
    this->closePrice = 0.0f;
}

void PriceData::operator=(const PriceData& rhs)
{
    this->timestamp  = rhs.timestamp;     
    this->openPrice  = rhs.openPrice;  
    this->highPrice  = rhs.highPrice;  
    this->lowPrice   = rhs.lowPrice;   
    this->closePrice = rhs.closePrice; 
}

bool PriceData::isUp() const
{
    return this->closePrice > this->openPrice;
}

bool PriceData::isDown() const
{
    return this->openPrice > this->closePrice;
}

float PriceData::diff() const
{
    if(this->openPrice > this->closePrice){
        return this->openPrice - this->closePrice;
    }
    else{
        return this->closePrice - this->openPrice;
    }
}

void PriceData::mergeData(const PriceData& newData)
{
    if(this->timestamp < newData.timestamp)
    {
        this->closePrice = newData.closePrice;
        this->timestamp = newData.timestamp;
    }
    else{
        this->openPrice = newData.openPrice;
    }

    if(this->closePrice > this->highPrice){
        this->highPrice = this->closePrice;
    }

    if(this->closePrice < this->lowPrice){
        this->lowPrice = this->closePrice;
    }
}

void PriceData::mergeData(uint32_t timestamp, float closePrice)
{
    if(this->timestamp < timestamp)
    {
        this->closePrice = closePrice;
        this->timestamp = timestamp;
        if(this->closePrice > this->highPrice){
            this->highPrice = this->closePrice;
        }

        if(this->closePrice < this->lowPrice){
            this->lowPrice = this->closePrice;
        }
    }
}

float PriceData::getLowestThan(float refPrice) const
{
    float result = refPrice;
    if(this->openPrice < result){
        result = this->openPrice;
    }
    if(this->highPrice < result){
        result = this->highPrice;
    }
    if(this->lowPrice < result){
        result = this->lowPrice;
    }
    if(this->closePrice < result){
        result = this->closePrice;
    }
    return result;
}

float PriceData::getHighestThan(float refPrice) const
{
    float result = refPrice;
    if(this->openPrice > result){
        result = this->openPrice;
    }
    if(this->highPrice > result){
        result = this->highPrice;
    }
    if(this->lowPrice > result){
        result = this->lowPrice;
    }
    if(this->closePrice > result){
        result = this->closePrice;
    }
    return result;
}