#include <stddef.h>
#include <inttypes.h>
#include <time.h>
#include <sys/printk.h>
#include "clock.h"

/**
 * Local time maintained by the watch.
 */
static time_t local_time = 0;

void clock_init()
{
	local_time = 0;
}

void clock_loop()
{
	// TODO: use timer to increment
	local_time++;
}

time_t clock_get_ticks()
{
	return local_time;
}

struct tm* clock_get_datetime()
{
	return localtime(&local_time);
}

void clock_set_datetime(struct tm* datetime)
{
    local_time = mktime(datetime);
    printk("Local clock se to: %02d:%02d:%02d\n",
        datetime->tm_hour, datetime->tm_min, datetime->tm_sec);
}
