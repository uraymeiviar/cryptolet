#include "HostTime.h"

//SemaphoreHandle_t HostTime::timerUpdateEvent;

HostTime* HostTime::getInstance()
{
    static HostTime* instance = nullptr;
    if(instance == nullptr)
    {
        instance = new HostTime();
    }
    return instance;
}

void HostTime::onTimer(){
    HostTime* hostTime = HostTime::getInstance();
    hostTime->timerCounter++;
    hostTime->currentTime++;
    // xSemaphoreGiveFromISR(hostTime->timerUpdateEvent, NULL);
    // vTaskDelay(100);
    // xSemaphoreTakeFromISR(hostTime->timerUpdateEvent, NULL);
}

HostTime::HostTime()
 : timeJsonDoc(512)
{
    this->timerCounter = 0;
    // this->timerUpdateEvent = xSemaphoreCreateMutex();
    // xSemaphoreTakeFromISR(this->timerUpdateEvent, NULL);

    this->hwTimer = timerBegin(0, 80, true); // timer_id = 0; divider=80; countUp = true;
	timerAttachInterrupt(hwTimer, &this->onTimer, true); // edge = true
	timerAlarmWrite(hwTimer, 1000000, true);  //1000 ms
	timerAlarmEnable(hwTimer);	    
    this->lastGetMillis = this->timerCounter;
}

uint32_t HostTime::getLastUpdateMillis()
{
    HostTime* hostTime = HostTime::getInstance();
    return hostTime->lastGetMillis;
}

uint32_t HostTime::getMillis()
{
    HostTime* hostTime = HostTime::getInstance();
    return hostTime->timerCounter;
}

uint32_t HostTime::getCurrentTime()
{
    HostTime* hostTime = HostTime::getInstance();
    return hostTime->currentTime;
}

uint32_t HostTime::syncCurrentTime()
{	
    HostTime* hostTime = HostTime::getInstance();
    if(!hostTime->client.connected())
    {
        //Serial.println("HostTime sync connecting to worldtimeapi.org");
        if (!hostTime->client.connect("worldtimeapi.org", 80))
        {
            Serial.println("Hostime sync connect failed to worldtimeapi.org");
        }
        else{
            unsigned long startSendMillis = hostTime->timerCounter;
            bool dataReceived = false;
            String body;

            hostTime->httpClient.begin(hostTime->client, "http://worldtimeapi.org/api/ip");

            int httpCode = hostTime->httpClient.GET();
            if(httpCode > 0) {
                if(httpCode == HTTP_CODE_OK) {
                    body = hostTime->httpClient.getString();
                    dataReceived = true;
                }
            } 

            if(dataReceived)
            {
                int startIndex = body.indexOf('{');
                int endIndex = body.lastIndexOf('}');
                String data = body.substring(startIndex, endIndex + 1);

                //Serial.print("timesync parsing data: ");
                //Serial.println(data);

                hostTime->timeJsonDoc.clear();

                DeserializationError parseResult = deserializeJson(hostTime->timeJsonDoc, data);
                if (parseResult != DeserializationError::Ok)
                {
                    Serial.println("! parse json doc failed");
                    Serial.println(parseResult.c_str());
                }
                else
                {
                    JsonObject root = hostTime->timeJsonDoc.as<JsonObject>();
                    JsonVariant unixTime = root["unixtime"];
                    JsonVariant unixTimeOffset = root["raw_offset"];
                    if (!unixTime.isNull())
                    {
                        hostTime->currentTime = unixTime.as<uint32_t>() + unixTimeOffset.as<uint32_t>();
                        
                        //Serial.print("sync time = ");
                        //Serial.println(hostTime->currentTime);
                    }
                    else
                    {
                        Serial.println("! parse json root failed");
                    }
                }                
            }     
            else{
                Serial.println("Hostime sync receive failed from worldtimeapi.org");
            }            	
        }      
        //Serial.println("HostTime sync done");
        hostTime->client.stop();   
    }
    return hostTime->currentTime;
}