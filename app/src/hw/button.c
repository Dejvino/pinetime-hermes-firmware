#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include "backlight.h"
#include "button.h"

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
    // TODO: callback!
	bool b = isButtonPressed();
	printk("Button pressed: %d\n", b);
	if (b) {
        backlight_enable(b);
    }
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
