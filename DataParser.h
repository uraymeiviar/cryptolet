#ifndef DATAPARSER_H
#define DATAPARSER_H
#include <string>

class DataParser {
    public:
        virtual bool parseData(std::string body, float& out) = 0;
};

#endif
