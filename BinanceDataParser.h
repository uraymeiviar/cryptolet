#ifndef _BINANCEDATAPARSER_H_
#define _BINANCEDATAPARSER_H_
#include "DataParser.h"
#include "ArduinoJson.h"

class BinanceDataParser : DataParser
{
    public:
        virtual bool parseData(std::string body, float& out) override;
        BinanceDataParser();
    protected:
        DynamicJsonDocument doc;
};

#endif
