#ifndef INDODAXDATASOURCE_H
#define INDODAXDATASOURCE_H

#include "HttpsDataSource.h"
#include "IndodaxDataParser.h"

class IndodaxDataSource : public HttpsDataSource{
    public:
        IndodaxDataSource(const std::string cur1, const std::string cur2);
        virtual std::string getName() const override;
        DataParser* getParser() const override;
        virtual void setPair(const std::string cur1, const std::string cur2) override;
        virtual std::string getHostName() const override;
    protected:
        virtual std::string httpGetUrl() const override;
        IndodaxDataParser parser;
        std::string name;
        std::string url;
};

#endif
