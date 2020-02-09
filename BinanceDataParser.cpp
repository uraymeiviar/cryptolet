#include "BinanceDataParser.h"

BinanceDataParser::BinanceDataParser()
    :doc(512)
{

}

bool BinanceDataParser::parseData(std::string body, float& out)
{
    int startIndex = body.find_first_of('{');
    int endIndex = body.find_last_of('}');
    std::string data = body.substr(startIndex,endIndex+1);

    //Serial.print("binance parsing data: ");
    //Serial.println(data);
  
    this->doc.clear();
    DeserializationError parseResult = deserializeJson(this->doc,data);
    if( parseResult != DeserializationError::Ok){
      Serial.println("! parse json doc failed");
      Serial.println(parseResult.c_str());
      return false;
    }
    else{
      JsonObject root = this->doc.as<JsonObject>();
      if(!root.isNull()){
        out = root["price"].as<float>();
      
        return true;
      }
      else{
        Serial.println("! parse json root failed");
        return false;
      }
    }
}