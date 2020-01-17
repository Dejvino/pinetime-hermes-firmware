#include "cts_sync.h"
#include "cts.h"
#include <sys/printk.h>
#include "clock.h"

static void sync_cts_to_clock()
{
    printk("CTS sync to clock started.\n");
    
    cts_datetime_t cts_datetime;
    cts_get_datetime(&cts_datetime);

    struct tm clock_datetime;
    clock_datetime.tm_hour = cts_datetime.hours;
    clock_datetime.tm_min = cts_datetime.minutes;
    clock_datetime.tm_sec = cts_datetime.seconds;

    clock_set_datetime(&clock_datetime);

    printk("CTS sync to clock complete.\n");
}

static void cts_write_cb(int val)
{
    sync_cts_to_clock();
}

void cts_sync_init()
{
    cts_register_write_cb(cts_write_cb);
    sync_cts_to_clock();
}

void cts_sync_loop()
{
}
