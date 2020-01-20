#include "zephyr.h"
#include <hw/backlight.h>
#include <hw/button.h>
#include <hw/clock.h>
#include "powersave.h"

time_t last_event = 0;
time_t next_sleep = 0;
const uint8_t backlight_timeout = 3; // sec

void powersave_init()
{
    backlight_enable(false);
}

void powersave_loop()
{
	if (isButtonPressed()) {
        powersave_wakeup(backlight_timeout);
    }

    if (!powersave_is_awake() && backlight_is_enabled()) {
        backlight_enable(false);
    }
}

void powersave_wakeup(uint8_t awake_timeout)
{
    last_event = clock_get_ticks();
    next_sleep = last_event + awake_timeout;

    backlight_enable(true);
}

bool powersave_is_awake()
{
    return next_sleep > clock_get_ticks();
}
