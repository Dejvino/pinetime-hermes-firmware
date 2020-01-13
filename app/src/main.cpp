#include <logging/log.h>
LOG_MODULE_REGISTER(app, LOG_LEVEL_INF);

#include <zephyr.h>
#include <device.h>
#include <display.h>
#include <drivers/gpio.h>

#include "GFX.h"
GFX* tft;

#define LED_PORT        DT_ALIAS_LED1_GPIOS_CONTROLLER
#define LED             DT_ALIAS_LED1_GPIOS_PIN

struct device *display_dev;

static void backlight_init(void)
{
	struct device *dev;

	dev = device_get_binding(LED_PORT);
    /* If you have a backlight, set it up and turn it on here */
	gpio_pin_configure(dev, LED, GPIO_DIR_OUT);
	gpio_pin_write(dev, LED, 0);
}

void delay(int ms) {
	k_sleep(K_MSEC(ms));
}

uint16_t millis() { return 0; }

void setup(void) {
	backlight_init();
	
	display_dev = device_get_binding(DT_INST_0_SITRONIX_ST7789V_LABEL);

	if (display_dev == NULL) {
		LOG_ERR("Device not found. Aborting test.");
		return;
	}

	display_blanking_off(display_dev);

	tft = createGFX(display_dev);

	tft->fillScreen(ST77XX_BLACK);
}

u16_t clock = 0;
inline void loop() {
	char clockText[16];
	int ts = clock % 60;
	int tm = (clock / 60) % 60;
	int th = (clock / 60 / 60) % 24;
	sprintf(clockText, "%02d:%02d:%02d", th, tm, ts);

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

	delay(100);

	clock++;
}

void main(void)
{
	LOG_INF("PineTime Hermes starting.");

#ifdef CONFIG_ST7789V_RGB565
	//const size_t rgb_size = 2;
#else
	//const size_t rgb_size = 3;
	LOG_ERR("RGB888 mode not supported. Aborting test.");
	return;
#endif

	setup();

	while (1) {
		loop();
	}
}

