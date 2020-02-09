#include "UIFooter.h"

UIFooter::UIFooter(Adafruit_GFX& screen, int offsetX, int offsetY, int width, int height)
    :UIRenderer(screen, offsetX, offsetY, width, height)
{

}

void UIFooter::draw()
{
	this->screen.setTextSize(1);
	this->screen.setTextColor(0x00);

	this->screen.fillRect(1, this->screenHeight - 15, this->screenWidth / 4 - 2, 15, 0x9FE0);
	this->screen.setCursor(4, this->screenHeight - 13);
	this->screen.print("MENU");

	this->screen.fillRect(1 + this->screenWidth / 4, this->screenHeight - 15, this->screenWidth / 4 - 2, 15, 0x07FF);
	this->screen.setCursor(4 + this->screenWidth / 4, this->screenHeight - 13);
	this->screen.print(" UP ");

	this->screen.fillRect(1 + 2 * (this->screenWidth / 4), this->screenHeight - 15, this->screenWidth / 4 - 2, 15, 0xF01F);
	this->screen.setCursor(4 + 2 * (this->screenWidth / 4), this->screenHeight - 13);
	this->screen.print("DOWN");

	this->screen.fillRect(1 + 3 * (this->screenWidth / 4), this->screenHeight - 15, this->screenWidth / 4 - 2, 15, 0xF800);
	this->screen.setCursor(4 + 3 * (this->screenWidth / 4), this->screenHeight - 13);
	this->screen.print(" RST");
}
