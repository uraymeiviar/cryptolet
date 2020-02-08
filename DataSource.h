#ifndef DATASOURCE_H
#define DATASOURCE_H
#include "DataParser.h"

class DataSource {
    public:
        virtual std::string getName() const = 0;
        virtual std::string getLastData() const = 0;
        virtual float getLastValue() const = 0;
        virtual uint32_t getLastDataUpdateTime() const = 0;
        virtual bool update() = 0;
        virtual const std::string getPair(bool first = false) const;
        virtual void setPair(const std::string cur1, const std::string cur2);
        virtual DataParser* getParser() const;
        DataSource(const std::string cur1, const std::string cur2);
    protected :
        std::string firstCurrency;
        std::string secondCurrency;
};

#endif
