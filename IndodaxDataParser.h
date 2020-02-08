#ifndef INDODAXDATAPARSER_H
#define INDODAXDATAPARSER_H
#include "DataParser.h"
#include "ArduinoJson.h"

class IndodaxDataParser : DataParser
{
    public:
        virtual bool parseData(std::string body, float& out) override;
        IndodaxDataParser();
    protected:
        DynamicJsonDocument doc;
};

#endif
