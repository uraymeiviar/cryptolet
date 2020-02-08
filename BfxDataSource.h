#ifndef BFXDATASOURCE_H
#define BFXDATASOURCE_H

#include "HttpsDataSource.h"
#include "BfxDataParser.h"

class BfxDataSource : public HttpsDataSource{
    public:
        BfxDataSource(const std::string cur1, const std::string cur2);
        virtual std::string getName() const override;
        DataParser* getParser() const override;
        virtual void setPair(const std::string cur1, const std::string cur2) override;
        virtual std::string getHostName() const override;
    protected:
        virtual std::string httpGetUrl() const override;
        BfxDataParser parser;
        std::string name;
        std::string url;
};

#endif
