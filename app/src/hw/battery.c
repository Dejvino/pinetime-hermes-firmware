#include "zephyr.h"
#include <device.h>
#include <drivers/gpio.h>
#include "battery.h"

uint16_t battery_voltage = 0;
uint8_t battery_percent = 0;

void battery_init()
{

}

void battery_loop()
{

}

uint8_t battery_get_percent()
{
    return battery_percent;
}

uint16_t battery_get_voltage()
{
    return battery_voltage;
}
