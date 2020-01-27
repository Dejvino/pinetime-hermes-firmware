#include "home.h"
extern "C" {
#include <hw/battery.h>
#include <hw/bt.h>
#include <service/msg_store.h>
}
#include <ui/widget/clock.h>

static GFX* tft;

char notify[MSG_STORE_TEXT_LIMIT];
u8_t notify_redraw = 0;

static void msg_store_listener(char* text, u16_t len)
{
    memcpy(notify, text, len);
	notify_redraw = 1;
}

void draw_notify() {
	if (notify_redraw == 0) {
		return;
	}

	int w = 240;
	int h = 80;
	int x = 0;
	int y = 240 - h;
	tft->setCursor(x, y);
	tft->setTextSize(2);
	tft->setTextWrap(true);
	tft->setTextColor(ST77XX_GREEN);
	tft->fillRect(x, y, w, h, ST77XX_BLACK);
	tft->print(notify);

	notify_redraw = 0;
}

void draw_battery() {
	uint8_t battery_percent = battery_get_percent();

	static char text[16];
	sprintf(text, "%02d%s", battery_percent, "%");

	int w = 40;
	int h = 15;
	int x = tft->width() - w;
	int y = 0;
	tft->openBuffer(x, y, w, h);
	tft->setCursor(x, y);
	tft->setTextSize(2);
	tft->setTextColor(ST77XX_GREEN);
	tft->fillRect(x, y, w, h, ST77XX_BLACK);
	tft->print(text);
	tft->flushBuffer();
}

void draw_bt_status() {
	uint8_t bt_status = bt_get_connection_status();

	uint16_t color;
	switch (bt_status) {
		case BT_STATUS_DISCONNECTED:
			color = ST77XX_RED;
			break;
		case BT_STATUS_CONNECTING:
			color = ST77XX_ORANGE;
			break;
		case BT_STATUS_CONNECTED:
			color = ST77XX_GREEN;
			break;
		default:
			color = ST77XX_MAGENTA;
	}
	int w = 20;
	int h = 15;
	int x = 0;
	int y = 0;
	//tft->openBuffer(x, y, w, h);
	tft->setCursor(x, y);
	tft->setTextSize(2);
	tft->setTextColor(color);
	//tft->fillRect(x, y, w, h, ST77XX_BLACK); // no need for now
	tft->print("BT");
	//tft->flushBuffer();
}


void home_init()
{
    tft = win_get_gfx();
    tft->fillScreen(ST77XX_BLACK);

	msg_store_register_listener(msg_store_listener);
}

void home_loop()
{
    uiw_clock_draw(tft, 46, 90, 5, UIW_CLOCK_FORMAT_HM);
	draw_battery();
	draw_bt_status();
	draw_notify();
}

void home_deinit() {}

static win_t win_home = {
    .id = WIN_HOME_ID,
    .init = home_init,
    .loop = home_loop,
    .deinit = home_deinit,
};

win_t* win_home_get()
{
    return &win_home;
}
