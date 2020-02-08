#pragma once
#include "DataParser.h"
#include "ArduinoJson.h"

class BfxDataParser : DataParser
{
    public:
        virtual bool parseData(std::string body, float& out) override;
        BfxDataParser();
    protected:
        DynamicJsonDocument doc;
};