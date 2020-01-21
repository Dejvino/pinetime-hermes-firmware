#include "clock.h"
extern "C" {
#include <hw/clock.h>
}

void uiw_clock_draw(GFX* tft, u16_t x, u16_t y)
{
	struct tm* tm_ptr = clock_get_datetime();
	uint8_t hours = tm_ptr->tm_hour;
	uint8_t minutes = tm_ptr->tm_min;
	uint8_t seconds = tm_ptr->tm_sec;

	static char clockText[32];
	sprintf(clockText, "%02d:%02d:%02d",
		hours, minutes, seconds);

	int w = tft->width() - x*2;
	int h = 30;
	tft->openBuffer(x, y, w, h);
	tft->setCursor(x, y);
	tft->setTextSize(4);
	tft->setTextColor(ST77XX_GREEN);
	tft->fillRect(x, y, w, h, ST77XX_BLACK);
	tft->print(clockText);
	tft->flushBuffer();
}


void uiw_clock_draw(GFX* tft, u16_t y)
{
    uiw_clock_draw(tft, 25, y);
}

void uiw_clock_draw(GFX* tft)
{
    uiw_clock_draw(tft, 25, 100);
}
