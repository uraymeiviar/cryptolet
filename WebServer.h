#ifndef _WEBSERVER_H_
#define _WEBSERVER_H_

#include "ESPAsyncWebServer.h"
#include "FS.h"

class WebServerDefaultHandler : public AsyncWebHandler
{
    public :
        WebServerDefaultHandler(const fs::FS& fs, const String& username=String(), const String& password=String());
        virtual bool canHandle(AsyncWebServerRequest *request) override;
        virtual void handleRequest(AsyncWebServerRequest *request) override;
        virtual void handleUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) override;
        virtual bool isRequestHandlerTrivial() override;
        protected:
    private:
        fs::FS _fs;
        String _username;
        String _password; 
        bool _authenticated;
        uint32_t _startTime;
};

class WebServer {
    public:
        WebServer(uint16_t listenPort, const String& username=String(), const String& password=String());
        void begin();
        void end();
    protected:
        AsyncWebServer server;
        WebServerDefaultHandler requestHandler;
};

#endif
