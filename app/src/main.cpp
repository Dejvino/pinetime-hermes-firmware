#include <logging/log.h>
LOG_MODULE_REGISTER(app, LOG_LEVEL_INF);

#include <zephyr.h>
#include <device.h>
#include <display.h>
#include <drivers/gpio.h>
#include <sys/util.h>
#include <inttypes.h>
#include <time.h>
extern "C" {
// HW
#include "clock.h"
#include "backlight.h"
#include "display.h"
#include "button.h"
#include "bt.h"
// Services
#include "powersave.h"
#include "cts_sync.h"
}

#include "GFX.h"
GFX* tft;

void delay(int ms) {
	k_sleep(K_MSEC(ms));
}

void draw_clock() {
	struct tm* tm_ptr = clock_get_datetime();
	uint8_t hours = tm_ptr->tm_hour;
	uint8_t minutes = tm_ptr->tm_min;
	uint8_t seconds = tm_ptr->tm_sec;

	static char clockText[32];
	sprintf(clockText, "%02d:%02d:%02d",
		hours, minutes, seconds);

	int x = 25;
	int y = 100;
	int w = tft->width() - x*2;
	int h = tft->height() - y*2;
	tft->openBuffer(x, y, w, h);
	tft->setCursor(x, y);
	tft->setTextSize(4);
	tft->setTextColor(ST77XX_GREEN);
	tft->fillRect(x, y, w, h, ST77XX_BLACK);
	tft->print(clockText);
	tft->flushBuffer();
}

void setup(void) {
	// HW
	clock_init();
	backlight_init();
	button_init();
	display_init();	
	bt_setup();
	// Services
	powersave_init();
	cts_sync_init();

	tft = createGFX(display_get_device());
	tft->fillScreen(ST77XX_BLACK);

	printk("Setup complete.\n");
}

inline void loop() {
	draw_clock();

	delay(999);

	// HW
	clock_loop();
	bt_loop();

	// Services
	powersave_loop();
	cts_sync_loop();
}

void main(void)
{
	LOG_INF("PineTime Hermes starting.");

	setup();

	while (1) {
		loop();
	}
}

