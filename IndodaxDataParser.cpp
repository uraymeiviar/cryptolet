#include "IndodaxDataParser.h"

IndodaxDataParser::IndodaxDataParser()
    :doc(512)
{

}

bool IndodaxDataParser::parseData(std::string body, float& out)
{
    int startIndex = body.find_first_of('{');
    int endIndex = body.find_last_of('}');
    std::string data = body.substr(startIndex,endIndex+1);

    //Serial.print("indodax parsing data: ");
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
      JsonVariant ticker = root["ticker"];
      if(!ticker.isNull()){
        JsonObject tickerObj = ticker.as<JsonObject>();
        out = tickerObj["last"].as<float>()/1000000.0f;

        // out.closePrice = tickerObj["last"].as<float>()/1000000.0f;
        // out.highPrice = tickerObj["high"].as<float>()/1000000.0f;
        // out.lowPrice = tickerObj["low"].as<float>()/1000000.0f;
        // out.timestamp = tickerObj["server_time"].as<uint32_t>();
      
        return true;
      }
      else{
        Serial.println("! parse json root failed");
        return false;
      }
    }
}