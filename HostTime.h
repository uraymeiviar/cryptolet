#ifndef HOSTTIME_H
#define HOSTTIME_H
#include "ArduinoJson.h"
#include "WiFiClient.h"
#include "HTTPClient.h"
#include "esp32-hal-timer.h"

class HostTime{
    public:
        static uint32_t getCurrentTime();
        static uint32_t syncCurrentTime();
        static uint32_t getLastUpdateMillis();
        static uint32_t getMillis();
        static HostTime* getInstance(); 
        //static SemaphoreHandle_t timerUpdateEvent;      
    protected:
        HostTime();
        static void onTimer();
        unsigned long lastGetMillis;
        unsigned long timerCounter;
        uint32_t currentTime;    
        DynamicJsonDocument timeJsonDoc;
        WiFiClient client;
        HTTPClient httpClient;
        hw_timer_t* hwTimer;
};

#endif
