#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include "backlight.h"

#define LED_PORT        DT_ALIAS_LED1_GPIOS_CONTROLLER
#define LED             DT_ALIAS_LED1_GPIOS_PIN

struct device* backlight_dev;

void backlight_enable(bool enable) {
	gpio_pin_write(backlight_dev, LED, enable ? 0 : 1);
}

void backlight_init(void)
{
    printk("Backlight starting...");
	backlight_dev = device_get_binding(LED_PORT);
	gpio_pin_configure(backlight_dev, LED, GPIO_DIR_OUT);
	backlight_enable(true);
	printk("Backlight inited.\n");
}
