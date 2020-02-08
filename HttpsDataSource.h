#pragma once
#include "WiFiClientSecure.h"
#include "DataSource.h"
#include <string>
#include <HTTPClient.h>

class HttpsDataSource : public DataSource{
    public:
        virtual std::string getLastData() const override;
        virtual float getLastValue() const override;
        virtual bool update() override;

        HttpsDataSource(const std::string cur1, const std::string cur2);
        virtual std::string getHostName() const = 0;
        virtual uint32_t getLastDataUpdateTime() const override;

    protected:
        WiFiClientSecure wifiClient;
        HTTPClient httpClient;
        virtual std::string httpGetUrl() const = 0;
        std::string lastData;
        float lastValue;
        uint32_t lastValueUpdate;
};