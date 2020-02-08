#include "UIPriceTicker.h"

UIPriceTicker::UIPriceTicker(const PriceHistory& history, Adafruit_GFX& screen, int offsetX, int offsetY, int width, int height)
    : UIRenderer(screen, offsetX, offsetY, width, height), priceHistory(history)
{
    this->period = PriceCandlePeriod::minutely;
    this->label = "PriceTicker";
}

void UIPriceTicker::draw()
{
    drawTicker();
    drawChart();
}

void UIPriceTicker::drawTicker()
{
	const PriceData &data = this->priceHistory.getRecentDataInPeriod(this->period);
	this->screen.setTextSize(1);
	this->screen.fillRect(this->screenOffsetX, this->screenOffsetY, this->screenWidth, this->screenHeight, 0x0000);
	this->screen.drawFastHLine(this->screenOffsetX,this->screenOffsetY+this->screenHeight-4,this->screenWidth,0x8410);

	int16_t x = 0;
	int16_t y = this->screenOffsetY;
	int16_t tx, ty;
	uint16_t tw, th;

	this->screen.setCursor(x, y);

	this->screen.setTextSize(1);
	this->screen.setTextColor(0xFFFF);
	this->screen.print(this->label.c_str());

	String LHPrice = "  L " + formatThousands(data.lowPrice) + " H " + formatThousands(data.highPrice);
	this->screen.getTextBounds(LHPrice, 0, 0, &tx, &ty, &tw, &th);
	this->screen.setCursor(this->screenWidth - (tw + 2), y);
	this->screen.println(LHPrice);

	x = this->screen.getCursorX();
	y = this->screen.getCursorY();

	String closePriceStr = this->formatThousands(data.closePrice);

	this->screen.setTextSize(2);
	this->screen.getTextBounds(closePriceStr, 0, 0, &tx, &ty, &tw, &th);

	this->screen.setCursor(this->screenWidth - (tw + 2), y + 4);
	if(this->priceHistory.getLastUpdateDiffInPeriod(this->period) > 0.0f){
		this->screen.setTextColor(0x7FE0);
	}
	else{
		this->screen.setTextColor(0xFB40);
	}
	this->screen.println(closePriceStr);

	float periodDiff = this->priceHistory.getPeriodDiffInPeriod(period);
	uint32_t periodSecs = this->priceHistory.getPeriodSecsInPeriod(period);
	String periodDiffStr;
	if( periodDiff >= 0.0f){
		this->screen.setTextColor(0x7FE0);
		periodDiffStr = "+"+String(periodDiff);
	}
	else{
		this->screen.setTextColor(0xFB40);
		periodDiffStr = String(periodDiff);
	}

	if(periodSecs >= 60*60*24){
		periodDiffStr += " "+ String(periodSecs/(60*60*24))+"d";
	}
	else if(periodSecs >= 60*60){
		periodDiffStr += " "+ String(periodSecs/(60*60))+"h";
	}
	else if(periodSecs >= 60){
		periodDiffStr += " "+ String(periodSecs/60)+"m";
	}
	else {
		periodDiffStr += " "+ String(periodSecs)+"s";
	}

	this->screen.getTextBounds(periodDiffStr, 0, 0, &tx, &ty, &tw, &th);
	this->screen.setCursor(this->screenWidth - (tw + 2), y+24);
	this->screen.print(periodDiffStr);
}

void UIPriceTicker::drawChart()
{
	int candleCount = ((this->screenWidth / 2) - 4) / 4;
	if(candleCount > this->priceHistory.getValidDataCountInPeriod(this->period)){
		candleCount = this->priceHistory.getValidDataCountInPeriod(this->period);
	}

	int x = (this->screenWidth / 2) - 4;
	int y = this->screenOffsetY + 14;
	int h = this->screenHeight - 22;
	size_t historyIndex = 0;
	const PriceData* data = this->priceHistory.getDataInPeriod(this->period, historyIndex);

	while (candleCount > 0 && data != nullptr)
	{
		this->drawCandle(data, x, y, this->priceHistory.getHistoryMinValueInPeriod(this->period), this->priceHistory.getHistoryMaxValueInPeriod(period), h);
		candleCount--;
		x -= 4;
		historyIndex++;
		data = this->priceHistory.getDataInPeriod(this->period, historyIndex);
	}
}


void UIPriceTicker::drawCandle(const PriceData* data, int posX, int posY, float minV, float maxV, int height)
{
	float span = maxV - minV;
	if(span > 0.0f){
		float pxPerPrice = (float)height / span;
		int lineStartY = posY + (int)((maxV - data->highPrice) * pxPerPrice);
		int lineHeight = (int)((data->highPrice - data->lowPrice) * pxPerPrice);

		if (data->isDown())
		{
			this->screen.drawFastVLine(posX + 1, lineStartY, lineHeight, 0xFAE0);
			int barStartY = posY + (int)((maxV - data->openPrice) * pxPerPrice);
			int barHeight = 1 + (int)(data->diff() * pxPerPrice);

			this->screen.fillRect(posX, barStartY, 3, barHeight, 0xFAE0);
		}
		else
		{
			this->screen.drawFastVLine(posX + 1, lineStartY, lineHeight, 0x97E0);
			int barStartY = posY + (int)((maxV - data->closePrice) * pxPerPrice);
			int barHeight = 1 + (int)(data->diff() * pxPerPrice);

			this->screen.fillRect(posX, barStartY, 3, barHeight, 0x97E0);
		}
	}
}

String UIPriceTicker::formatThousands(float value)
{
	String source = String(value);
	int decimalPos = source.indexOf('.');
	while (decimalPos - 3 > 0)
	{
		source = source.substring(0, decimalPos - 3) + "," + source.substring(decimalPos - 3);
		decimalPos = decimalPos - 3;
	}
	return source;
}