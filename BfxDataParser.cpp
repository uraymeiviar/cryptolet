#include "BfxDataParser.h"

BfxDataParser::BfxDataParser()
    :doc(512)
{

}

bool BfxDataParser::parseData(std::string body, float& out)
{
    int startIndex = body.find_first_of('[');
    int endIndex = body.find_last_of(']');
    std::string data = body.substr(startIndex,endIndex+1);
    
    //Serial.print("bitfinex parsing data: ");
    //Serial.println(data);

    this->doc.clear();
    DeserializationError parseResult = deserializeJson(this->doc,data);
    if( parseResult != DeserializationError::Ok){
      Serial.println("! parse json doc failed");
      Serial.println(parseResult.c_str());
      return false;
    }
    else{
      JsonArray root = this->doc.as<JsonArray>();

      out = root[6].as<float>();

    //   out.closePrice = root[6].as<float>();
    //   out.highPrice = root[8].as<float>();
    //   out.lowPrice = root[9].as<float>();
    //   out.timestamp = 0;
      return true;
    }
}
