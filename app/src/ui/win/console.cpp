#include "console.h"
#include <ui/widget/console.h>
#include <ui/widget/clock.h>
extern "C" {
#include <hw/clock.h>
#include <hw/display.h>
}

static GFX* tft;

static void init()
{
    tft = win_get_gfx();
    tft->fillScreen(ST77XX_BLACK);

    display_scroll_region(40, 200, 320 - 240);
}

static int x = 2;
static int h = 10;
void draw_line_text(char* line_text, u16_t line, bool highlight)
{
    int w = tft->width() - 2*x;
    int y = 40 + line * h;
	
	tft->openBuffer(x, y, w, h);
	tft->setCursor(x, y);
	tft->setTextSize(1);
    if (highlight) {
	    tft->setTextColor(ST77XX_RED);
	    tft->fillRect(x, y, w, h, ST77XX_WHITE);
    } else {
        tft->setTextColor(ST77XX_GREEN);
	    tft->fillRect(x, y, w, h, ST77XX_BLACK);
    }
	tft->print(line_text);
	tft->flushBuffer();
}

static char last_line_text[64];
static int line = 0;
static void loop()
{
    uiw_clock_draw(tft, 5);

    struct tm* tm_ptr = clock_get_datetime();
	uint8_t hours = tm_ptr->tm_hour;
	uint8_t minutes = tm_ptr->tm_min;
	uint8_t seconds = tm_ptr->tm_sec;

    static char line_text[64];
    memset(line_text, 0, sizeof(line_text));
	sprintf(line_text, "[%02d:%02d:%02d] Some random text. %d",
		hours, minutes, seconds, seconds * 11);

    display_scroll_offset(40 + line * h);
    draw_line_text(last_line_text, (line + 18) % 20, false);
    memcpy(last_line_text, line_text, sizeof(line_text));
    line = (line + 1) % 20;
    draw_line_text(line_text, (line + 18) % 20, true);
}

void deinit() {}

static win_t win = {
    .id = WIN_CONSOLE_ID,
    .init = init,
    .loop = loop,
    .deinit = deinit,
};

win_t* win_console_get()
{
    return &win;
}
