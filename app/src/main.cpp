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
#include "clock.h"
#include "bt.h"
#include "cts_sync.h"
}
#include "GFX.h"
GFX* tft;

#define LED_PORT        DT_ALIAS_LED1_GPIOS_CONTROLLER
#define LED             DT_ALIAS_LED1_GPIOS_PIN

struct device* display_dev;

void display_init() {
#ifdef CONFIG_ST7789V_RGB565
	//const size_t rgb_size = 2;
#else
	//const size_t rgb_size = 3;
	LOG_ERR("RGB888 mode not supported. Aborting test.");
	return;
#endif

	display_dev = device_get_binding(DT_INST_0_SITRONIX_ST7789V_LABEL);

	if (display_dev == NULL) {
		LOG_ERR("Device not found. Aborting test.");
		return;
	}

	display_blanking_off(display_dev);

	tft = createGFX(display_dev);

	tft->fillScreen(ST77XX_BLACK);
	printk("Display inited.\n");
}

struct device* backlight_dev;

static void backlight_enable(bool enable) {
	gpio_pin_write(backlight_dev, LED, enable ? 0 : 1);
}

static void backlight_init(void)
{
	backlight_dev = device_get_binding(LED_PORT);
    /* If you have a backlight, set it up and turn it on here */
	gpio_pin_configure(backlight_dev, LED, GPIO_DIR_OUT);
	backlight_enable(true);
	printk("Backlight inited.\n");
}

void delay(int ms) {
	k_sleep(K_MSEC(ms));
}

/* change this to use another GPIO port */
#ifndef DT_ALIAS_SW0_GPIOS_CONTROLLER
#ifdef SW0_GPIO_NAME
#define DT_ALIAS_SW0_GPIOS_CONTROLLER SW0_GPIO_NAME
#else
#error SW0_GPIO_NAME or DT_ALIAS_SW0_GPIOS_CONTROLLER needs to be set in board.h
#endif
#endif
#define PORT	DT_ALIAS_SW0_GPIOS_CONTROLLER

/* change this to use another GPIO pin */
#ifdef DT_ALIAS_SW0_GPIOS_PIN
#define PIN     DT_ALIAS_SW0_GPIOS_PIN
#else
#error DT_ALIAS_SW0_GPIOS_PIN needs to be set in board.h
#endif

#define EDGE    (GPIO_INT_EDGE | GPIO_INT_DOUBLE_EDGE)

/* change this to enable pull-up/pull-down */
#ifndef DT_ALIAS_SW0_GPIOS_FLAGS
#ifdef DT_ALIAS_SW0_GPIOS_PIN_PUD
#define DT_ALIAS_SW0_GPIOS_FLAGS DT_ALIAS_SW0_GPIOS_PIN_PUD
#else
#define DT_ALIAS_SW0_GPIOS_FLAGS 0
#endif
#endif
#define PULL_UP DT_ALIAS_SW0_GPIOS_FLAGS

struct device* button_dev;

bool isButtonPressed() {
	u32_t val = 0U;
	gpio_pin_read(button_dev, PIN, &val);
	return val == 1U;
}

void button_pressed(struct device *gpiob, struct gpio_callback *cb,
		u32_t pins)
{
	bool b = isButtonPressed();
	printk("Button pressed: %d\n", b);
	backlight_enable(b);
}

static struct gpio_callback gpio_cb;

void button_init()
{
	u32_t button_out = 1U;

	button_dev = device_get_binding(PORT);
	if (!button_dev) {
		printk("error\n");
		return;
	}

	gpio_pin_configure(button_dev, PIN,
			GPIO_DIR_IN | GPIO_INT |  PULL_UP | EDGE);

	gpio_init_callback(&gpio_cb, button_pressed, BIT(PIN));
	gpio_add_callback(button_dev, &gpio_cb);
	gpio_pin_enable_callback(button_dev, PIN);

//port 15 has to be high in order for the button to work
	gpio_pin_configure(button_dev, 15,GPIO_DIR_OUT); //push button out
	gpio_pin_write(button_dev, 15, button_out); //set port high
	printk("Button inited.\n");
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
	clock_init();
	backlight_init();
	button_init();
	display_init();	
	bt_setup();
	cts_sync_init();

	printk("Setup complete.\n");
}

inline void loop() {
	draw_clock();
	backlight_enable(isButtonPressed());

	delay(999);

	clock_loop();
	bt_loop();
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

