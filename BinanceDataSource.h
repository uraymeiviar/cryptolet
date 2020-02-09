#ifndef _BINANCEDATASOURCE_H_
#define _BINANCEDATASOURCE_H_

#include "HttpsDataSource.h"
#include "BinanceDataParser.h"

class BinanceDataSource : public HttpsDataSource{
    public:
        BinanceDataSource(const std::string cur1, const std::string cur2);
        virtual std::string getName() const override;
        DataParser* getParser() const override;
        virtual void setPair(const std::string cur1, const std::string cur2) override;
        virtual std::string getHostName() const override;
    protected:
        virtual std::string httpGetUrl() const override;
        BinanceDataParser parser;
        std::string name;
        std::string url;
};

#endif
