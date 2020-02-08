#include "HttpsDataSource.h"
#include "HostTime.h"

HttpsDataSource::HttpsDataSource(const std::string cur1, const std::string cur2)
    : DataSource(cur1,cur2), lastValueUpdate(0)
{
}

bool HttpsDataSource::update()
{
    if(this->wifiClient.connected()){
        return false;
    }
    else{
        this->wifiClient.setTimeout(5000);
        if (!this->wifiClient.connect(this->getHostName().c_str(), 443))
		{
            Serial.printf("! %s connect failed to %s\n", this->getName().c_str(), this->httpGetUrl().c_str());
		}
        else{
            unsigned long startSendMillis = millis();
            bool dataReceived = false;

            this->httpClient.begin(this->wifiClient, this->httpGetUrl().c_str());

            int httpCode = this->httpClient.GET();
            if(httpCode > 0) {
                if(httpCode == HTTP_CODE_OK) {
                    this->lastData = std::string(this->httpClient.getString().c_str());
                    this->lastValueUpdate = HostTime::getCurrentTime();
                    dataReceived = true;
                }
            }             

			if (dataReceived)
			{
                DataParser* parser = this->getParser();
                if(parser != nullptr)
                {
                    float newValue = this->lastValue;
                    if( parser->parseData(this->lastData, newValue) )
                    {
                        this->lastValue = newValue;
                        Serial.printf("%s updated = %f\n", this->getName().c_str(), this->lastValue);
                    }
                }	                
			}
            else{
                Serial.printf("! %s HTTP GET failed to %s\n", this->getName().c_str(), this->httpGetUrl().c_str());
            }

            this->wifiClient.stop();

            return dataReceived;
        }
    }
}

std::string HttpsDataSource::getLastData() const
{
    return this->lastData;
}

float HttpsDataSource::getLastValue() const
{
    return this->lastValue;
}

uint32_t HttpsDataSource::getLastDataUpdateTime() const
{
    return this->lastValueUpdate;
}