#define INCLUDE_vTaskDelay 1
#define INCLUDE_uxTaskGetStackHighWaterMark 1
#define ESP32

#include <Arduino.h>
#include "Adafruit_GFX.h"    // Core graphics library by Adafruit
#include "Arduino_ST7789.h" // Hardware-specific library for ST7789 (with or without CS pin)
#include "SPIFFS.h"

// #define TFT_DC    8
// #define TFT_RST   9
// #define TFT_CS    10 // only for displays with CS pin
// #define TFT_MOSI  11   // for hardware SPI data pin (all of available pins)
// #define TFT_SCLK  13   // for hardware SPI sclk pin (all of available pins)

#define TFT_MISO -1
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   5  // Chip select control pin
#define TFT_DC   16  // Data Command control pin
#define TFT_RST   17  // Reset pin (could connect to RST pin)

//You can use different type of hardware initialization
//using hardware SPI (11, 13 on UNO; 51, 52 on MEGA; ICSP-4, ICSP-3 on DUE and etc)
//Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST); //for display without CS pin
//Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST, TFT_CS); //for display with CS pin
//or you can use software SPI on all available pins (slow)
//Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK); //for display without CS pin
Arduino_ST7789 tft = Arduino_ST7789(TFT_DC, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_CS); //for display with CS pin
// Arduino_ST7789 tft = Arduino_ST7789(-1, TFT_RST, TFT_MOSI, TFT_SCLK, TFT_CS); //for display with CS pin and DC via 9bit SPI


#define dispWidth   240
#define dispHeight  320

const char* ssid = "CXI28-LT1-N";
const char* password = "tasikmalaya10";



#include <ESPmDNS.h>
#include "FS.h"
#include "esp32-hal-cpu.h"
#include "HostTime.h"
#include "IndodaxDataSource.h"
#include "BfxDataSource.h"
#include "BinanceDataSource.h"
#include "PriceHistory.h"
#include "UIPriceTicker.h"
#include "WebServer.h"
#include "UIHeader.h"
#include "UIFooter.h"

UIHeader uiHeader(WiFi,tft,0,0,dispWidth, dispHeight);
UIFooter uiFooter(tft,0,0,dispWidth, dispHeight);
DataSource* dataSource[4];

#define priceHistoryMaxCount 30

PriceHistory priceHistory[] = {
    PriceHistory(priceHistoryMaxCount),
    PriceHistory(priceHistoryMaxCount),
    PriceHistory(priceHistoryMaxCount),
    PriceHistory(priceHistoryMaxCount),
};

UIPriceTicker uiPriceTicker[] = {
	UIPriceTicker(priceHistory[0], tft, 0,  16, dispWidth, 64),
	UIPriceTicker(priceHistory[1], tft, 0,  80, dispWidth, 64),
	UIPriceTicker(priceHistory[2], tft, 0, 144, dispWidth, 64),
	UIPriceTicker(priceHistory[3], tft, 0, 208, dispWidth, 64),
};

const size_t coinCount = sizeof(priceHistory)/sizeof(PriceHistory);

IPAddress connectToWifi(const char *ssid, const char *password)
{
	tft.printf("Connecting to: %s", ssid);

	WiFi.begin(ssid, password);
	int retryCount = 3;
	while (WiFi.status() != WL_CONNECTED)
	{
		tft.fillCircle((dispWidth / 2), (dispHeight / 2), 10, WHITE);
		vTaskDelay(500);
		tft.fillRect((dispWidth / 2) - 10, (dispHeight / 2) - 10, (dispWidth / 2) + 10, (dispHeight / 2) + 10, BLACK);
		retryCount--;
		if (retryCount == 0)
		{
			tft.println("** Scan Networks **");
			byte numSsid = WiFi.scanNetworks();

			tft.print("Number of available WiFi networks discovered:");
			tft.println(numSsid);
			retryCount = 3;
		}
	}

	IPAddress ip = WiFi.localIP();
	tft.print("Connected, IP:");
	tft.println(ip);

	long rssi = WiFi.RSSI();
	tft.print("RSSI:");
	tft.println(rssi);
	return ip;
}

int coinIndex = 0;
int dataUpdateCounter[coinCount];
int dataRenderCounter[coinCount];
SemaphoreHandle_t dataLock[coinCount];


unsigned long lastRenderDataTaskExec;
TaskHandle_t renderTask;
int renderCoinIndex = 0;
void renderDataTask( void* param)
{		
	Serial.print("render data task started on core ");
	Serial.println(xPortGetCoreID());	
	uiFooter.draw();
	uiHeader.draw();
	//drawHeader(HostTime::getCurrentTime());

	vTaskDelay(1000);
	while(true)
	{
		//drawHeader(HostTime::getCurrentTime());
		uiHeader.draw();
		if (WiFi.status() == WL_CONNECTED)
		{			
			lastRenderDataTaskExec = HostTime::getMillis();

			renderCoinIndex = coinIndex - 1;
			if (renderCoinIndex < 0)
			{
				renderCoinIndex = coinCount - 1;
			}

			if(xSemaphoreTake( dataLock[renderCoinIndex], 0 ) == pdTRUE)
			{
				if(dataRenderCounter[renderCoinIndex] != dataUpdateCounter[renderCoinIndex])
				{
					dataRenderCounter[renderCoinIndex] = dataUpdateCounter[renderCoinIndex];
					uiPriceTicker[renderCoinIndex].draw();			
				}
				xSemaphoreGive( dataLock[renderCoinIndex] );
				taskYIELD();
			}
			// else{
			// 	Serial.print("mutex timeout skip render on coin ");
			// 	Serial.println(renderCoinIndex);
			// }

			unsigned long elapsed = (HostTime::getMillis() - lastRenderDataTaskExec);
		
			if (elapsed < 1000)
			{
				vTaskDelay(1000 - elapsed);
			}
		}
		else
		{
			vTaskDelay(1000);
		}

		// UBaseType_t stackSize = uxTaskGetStackHighWaterMark(renderTask);
		// Serial.printf("renderTask stack %d\n", stackSize);
	}
}

WebServer* webserver = nullptr;

void setup(void)
{
	Serial.begin(115200);
	Serial.println(" ");
	Serial.printf("CPU clock %dMhz\n",getCpuFrequencyMhz());

	char _hostname[20];
	uint8_t mac[6];
	WiFi.macAddress(mac);
	sprintf(_hostname, "cryptolet%02x%02x%02x%02x", mac[2], mac[3], mac[4], mac[5]);
	Serial.printf("generated hostname %s\n", _hostname);

	Serial.println("init LCD");
	tft.init(dispWidth, dispHeight); // initialize a ST7789 chip, 240x240 pixels
	tft.setRotation(2);
	tft.invertDisplay(false);
	tft.fillScreen(BLACK);
	tft.setTextSize(1);
	tft.setCursor(0, 0);	
	Serial.println("Initialized");

	Serial.println("init data");
	dataSource[0] = new IndodaxDataSource("BTC", "IDR");
	dataSource[1] = new BfxDataSource("BTC", "USD");
	dataSource[2] = new IndodaxDataSource("ETH", "IDR");
	dataSource[3] = new BinanceDataSource("ETH", "USDT");

	for(int i=0 ; i<coinCount ; i++){
		dataUpdateCounter[i] = 0;
		dataRenderCounter[i] = 0;
		dataLock[i] = xSemaphoreCreateMutex();
		if(dataLock[i] == NULL){
			Serial.println("! mutex creation failed");
		}
		else{
			xSemaphoreGive( dataLock[renderCoinIndex] );
		}
		uiPriceTicker[i].label = dataSource[i]->getName();
	}
	Serial.println("init data done");		

	Serial.println("init SPIFFS..");
	bool spiffsMounted = false;
	if(!SPIFFS.begin(true)){
		Serial.println("An Error has occurred while mounting SPIFFS\n");
		vTaskDelay(500);
		Serial.println("retrying for second time could be because of not formatted yet");
		if(!SPIFFS.begin(true)){
			Serial.println("(still) An Error has occurred while mounting SPIFFS\n");			
		}
		else{
			spiffsMounted = true;
		}
	}
	else{
		spiffsMounted = true;
	}

	String hostname = String(_hostname);

	if(spiffsMounted)
	{
		Serial.printf("SPI FFS %'d of %'d\n",SPIFFS.usedBytes(),SPIFFS.totalBytes());
		fs::File root = SPIFFS.open("/");
		if(root)
		{
			fs:File file = root.openNextFile();
			while(file){

				Serial.printf("SPI FFS File: %s\n", file.name());		
				file = root.openNextFile();
			}
		}
		Serial.println("init SPIFFS done.");

		if(SPIFFS.exists("/conf/hostname.txt"))
		{
			fs::File hostnameFile = SPIFFS.open("/conf/hostname.txt", FILE_READ);
	
			if(!hostnameFile){
				Serial.println("! failed to open /conf/hostname.txt\n");
			}
			else{
				size_t fileSize = hostnameFile.size();
				char* fileContent = new char[fileSize+1];
				if(hostnameFile.readBytes(fileContent, fileSize) >= fileSize)
				{
					fileContent[fileSize] = NULL;
					hostname = String(fileContent);
					Serial.printf("configured hostname %s\n", fileContent);
				}
				delete[] fileContent;
				hostnameFile.close();
			}
		}
		else{
			Serial.println("/conf/hostname.txt not found\n");
			fs::File hostnameFile = SPIFFS.open("/conf/hostname.txt", FILE_WRITE);
			if(!hostnameFile){
				Serial.println("! failed to open /conf/hostname.txt for write\n");
			}
			else{
				if(hostnameFile.write((const uint8_t*)hostname.c_str(),hostname.length()) >= hostname.length())
				{
					Serial.printf("hostname %s written to /conf/hostname.txt\n", hostname.c_str());
				}
				else{
					Serial.println("! failed to write to /conf/hostname.txt\n");
				}
				hostnameFile.close();
			}
		}
	}

	
	Serial.printf("setting hostname = %s\n", hostname.c_str());
	WiFi.setHostname(hostname.c_str());

	Serial.printf("Connecting to Wifi.. (ssid=%s)\n",ssid);
	connectToWifi(ssid, password);
	Serial.println("Connected");

	if (!MDNS.begin(hostname.c_str())) {
        Serial.println("Error setting up MDNS responder!\n");
    }

	Serial.println("syncing time..");
	HostTime::syncCurrentTime();
	Serial.println("syncing time, done.");

	if(spiffsMounted)
	{
		Serial.println("init WebServer..");
		webserver = new WebServer(80);
		webserver->begin();
		Serial.println("init WebServer, done.");

		MDNS.addService("http","tcp",80);
	}

	tft.fillScreen(BLACK);
	Serial.println("starting render thread..");
	xTaskCreatePinnedToCore(
		renderDataTask,         /* pvTaskCode */
		"render",            	/* pcName */
		4096,                    /* usStackDepth */
		NULL,                   /* pvParameters */
		2 | portPRIVILEGE_BIT,                      /* uxPriority */
		&renderTask,            /* pxCreatedTask */
		0
	);                    		/* xCoreID */	
}

unsigned long lastFetchDataTaskExec;
int fetchDataTaskCounter = 0;
void loop()
{
	if (WiFi.status() != WL_CONNECTED)
	{
		Serial.println("! Wifi Reconnecting...");
		connectToWifi(ssid, password);
	}
	else{			
		lastFetchDataTaskExec = HostTime::getMillis();
		fetchDataTaskCounter++;

		if( xSemaphoreTake( dataLock[coinIndex], 1000 ) == pdTRUE)
		{
			if(dataSource[coinIndex]->update())
			{
				float newValue = dataSource[coinIndex]->getLastValue();
				uint32_t newValueTimestamp = dataSource[coinIndex]->getLastDataUpdateTime();
				priceHistory[coinIndex].addData(newValueTimestamp, newValue);
				dataUpdateCounter[coinIndex]++;
			}
			xSemaphoreGive( dataLock[coinIndex] );
			taskYIELD();
		}
		else{
			Serial.print("mutex timeout skip fetch on coin ");
			Serial.println(coinIndex);
		}

		if (coinIndex+1 >= coinCount)
		{
			coinIndex = 0;
		}	
		else{
			coinIndex = coinIndex + 1;
		}

		if (fetchDataTaskCounter % 60*15 == 0)
		{
			HostTime::syncCurrentTime();
		}

		unsigned long elapsed = (HostTime::getMillis() - lastFetchDataTaskExec);
		
		if (elapsed < 1000)
		{
			vTaskDelay(1000 - elapsed);
		}
	}
	// UBaseType_t stackSize = uxTaskGetStackHighWaterMark(fetchTask);
	// Serial.printf("fetchTask counter = %d stack %d\n", fetchDataTaskCounter, stackSize);
}
