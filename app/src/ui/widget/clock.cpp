#include "clock.h"
extern "C" {
#include <hw/clock.h>
}

void uiw_clock_draw(GFX* tft, u16_t x, u16_t y, u8_t text_size, u8_t format)
{
	struct tm* tm_ptr = clock_get_datetime();
	uint8_t hours = tm_ptr->tm_hour;
	uint8_t minutes = tm_ptr->tm_min;
	uint8_t seconds = tm_ptr->tm_sec;
	char separator = seconds % 2 == 0 ? ':' : ' ';

	int w = tft->width() - x*2;
	int h = 8 * text_size;

	static char clockText[32];
	switch (format) {
		case UIW_CLOCK_FORMAT_HM:
			sprintf(clockText, "%02d%c%02d",
				hours, separator, minutes);
			break;
		case UIW_CLOCK_FORMAT_HMS:
		default:
			sprintf(clockText, "%02d:%02d:%02d",
				hours, minutes, seconds);
			break;
	}

	tft->setCursor(x, y);
	tft->setTextSize(text_size);
	tft->setTextColor(ST77XX_GREEN);
	tft->openBuffer(x, y, w, h);
	tft->fillRect(x, y, w, h, ST77XX_BLACK);
	tft->print(clockText);
	tft->flushBuffer();
}

void uiw_clock_draw(GFX* tft)
{
    uiw_clock_draw(tft, 100);
}

void uiw_clock_draw(GFX* tft, u16_t y)
{
    uiw_clock_draw(tft, 25, y, 4, UIW_CLOCK_FORMAT_HMS);
}
