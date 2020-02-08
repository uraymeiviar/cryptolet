#include "IndodaxDataSource.h"
#include <algorithm>

IndodaxDataSource::IndodaxDataSource(const std::string cur1, const std::string cur2)
    : HttpsDataSource(cur1, cur2)
{
    this->setPair(cur1, cur2);
}

void IndodaxDataSource::setPair(const std::string cur1, const std::string cur2)
{
    std::string upCur1 = cur1;
    std::transform(upCur1.begin(), upCur1.end(), upCur1.begin(), ::toupper);

    std::string upCur2 = cur2;
    std::transform(upCur2.begin(), upCur2.end(), upCur2.begin(), ::toupper);

    std::string loCur1 = cur1;
    std::transform(loCur1.begin(), loCur1.end(), loCur1.begin(), ::tolower);

    std::string loCur2 = cur2;
    std::transform(loCur2.begin(), loCur2.end(), loCur2.begin(), ::tolower);

    this->name = "Indodax "+upCur1+"/"+upCur2;
    this->url = "https://vip.bitcoin.co.id/api/"+loCur1+"_"+loCur2+"/ticker/";

    this->firstCurrency = loCur1;
    this->secondCurrency = loCur2;
}

std::string IndodaxDataSource::getName() const
{
    return this->name;
}

std::string IndodaxDataSource::httpGetUrl() const
{
    return this->url;
}

DataParser* IndodaxDataSource::getParser() const
{
    return (DataParser*)&this->parser;
}

std::string IndodaxDataSource::getHostName() const
{
    return "vip.bitcoin.co.id";
}