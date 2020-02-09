#include "UIHeader.h"
#include "HostTime.h"

#include <TimeLib.h>

UIHeader::UIHeader(WiFiClass& wifi, Adafruit_GFX& screen, int offsetX, int offsetY, int width, int height)
    :UIRenderer(screen, offsetX, offsetY, width, height), wifi(wifi)
{

}

void UIHeader::draw()
{
    String ssid = this->wifi.SSID();
	float rssi = (float)this->wifi.RSSI();

	this->screen.setTextSize(1);
	this->screen.fillRect(this->screenOffsetX, this->screenOffsetY, this->screenWidth, 13, 0xC7E0);
	this->screen.setCursor(4, 3);
	this->screen.setTextColor(0x0000);

    uint32_t currentTime = HostTime::getCurrentTime();

	this->screen.printf("%02d/%02d %02d:%02d:%02d",
		month(currentTime),
		day(currentTime),
		hour(currentTime),
		minute(currentTime),
		second(currentTime));

	int16_t tx, ty;
	uint16_t tw, th;
	this->screen.getTextBounds(ssid, 0, 0, &tx, &ty, &tw, &th);
	this->screen.setCursor(this->screenWidth - (tw + 10), 3);
	this->screen.print(ssid);

	int16_t rssiHeight = 1+(int16_t)(6.0f*((60.0f+(rssi+30.0f))/60.0f));
	this->screen.drawRect(this->screenWidth-7,1,6,11, 0x0261);
	this->screen.fillRect(this->screenWidth-5,11-rssiHeight,2,rssiHeight, 0x0000);
}