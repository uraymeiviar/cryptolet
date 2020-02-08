#include "BfxDataSource.h"

BfxDataSource::BfxDataSource(const std::string cur1, const std::string cur2)
    : HttpsDataSource(cur1, cur2)
{
    this->setPair(cur1, cur2);
}


void BfxDataSource::setPair(const std::string cur1, const std::string cur2)
{
    std::string upCur1 = cur1;
    std::transform(upCur1.begin(), upCur1.end(), upCur1.begin(), ::toupper);

    std::string upCur2 = cur2;
    std::transform(upCur2.begin(), upCur2.end(), upCur2.begin(), ::toupper);

    std::string loCur1 = cur1;
    std::transform(loCur1.begin(), loCur1.end(), loCur1.begin(), ::tolower);

    std::string loCur2 = cur2;
    std::transform(loCur2.begin(), loCur2.end(), loCur2.begin(), ::tolower);

    this->name = "BFX "+upCur1+"/"+upCur2;
    this->url = "https://api-pub.bitfinex.com/v2/ticker/t" + upCur1 + upCur2 + "/";

    this->firstCurrency = loCur1;
    this->secondCurrency = loCur2;
}

std::string BfxDataSource::getName() const
{
    return this->name;
}

std::string BfxDataSource::httpGetUrl() const
{
    return this->url;
}

DataParser* BfxDataSource::getParser() const
{
    return (DataParser*)&this->parser;
}

std::string BfxDataSource::getHostName() const
{
    return "api-pub.bitfinex.com";
}