#include <logging/log.h>
LOG_MODULE_REGISTER(app_display, LOG_LEVEL_INF);

#include <zephyr.h>
#include <device.h>
#include <display.h>
#include <drivers/gpio.h>
#include <sys/util.h>
#include <inttypes.h>
#include "display.h"

struct device* display_dev;

void display_scroll_region(u16_t top, u16_t middle, u16_t bottom)
{
	display_set_scroll_region(display_dev, top, middle, bottom);
}

void display_scroll_offset(u16_t offset)
{
	display_set_scroll_offset(display_dev, offset);
}

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

	printk("Display inited.\n");
}

struct device* display_get_device()
{
    return display_dev;
}
