#include "DataSource.h"

DataSource::DataSource(const std::string cur1, const std::string cur2)
    :firstCurrency(cur1), secondCurrency(cur2)
{

}

const std::string DataSource::getPair(bool first) const
{
    if(first){
        return this->firstCurrency;
    }
    else{
        return this->secondCurrency;
    }
}

void DataSource::setPair(const std::string cur1, const std::string cur2)
{
    this->firstCurrency = cur1;
    this->secondCurrency = cur2;
}


DataParser* DataSource::getParser() const
{
    return nullptr;
}