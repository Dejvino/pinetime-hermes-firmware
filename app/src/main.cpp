#include <zephyr.h>
#include <sys/util.h>
#include <inttypes.h>
#include <time.h>
extern "C" {
#include "lib/log.h"
#include "hw/storage.h"
#include "hw/clock.h"
#include "hw/backlight.h"
#include "hw/display.h"
#include "hw/button.h"
#include "hw/battery.h"
#include "hw/bt.h"
#include "service/msg_store.h"
#include "service/powersave.h"
#include "service/cts.h"
#include "service/cts_sync.h"
#include "service/bt_notify_service.h"
}
#include "ui/win.h"
#include "ui/win/home.h"
#include "ui/win/console.h" // TESTING

void delay(int ms) {
	k_sleep(K_MSEC(ms));
}

void setup(void) {	

    settings_subsys_init();
	// HW
	LOG_INF("App setup: hardware");
	//storage_init();
	clock_init();
	backlight_init();
	button_init();
	display_init();
	battery_init();	
	bt_setup();

	// Services
	LOG_INF("App setup: services");
	msg_store_init();
	powersave_init();
	cts_init();
	cts_sync_init();
	bt_notify_service_init();

	win_init();

	win_activate(win_home_get());
	// TESTING:
	//win_activate(win_console_get());

	LOG_INF("App setup complete.");
}

inline void loop() {
	delay(990);

	// HW
	//storage_loop();
	clock_loop();
	battery_loop();
	bt_loop();

	// Services
	msg_store_loop();
	powersave_loop();
	cts_loop();
	cts_sync_loop();
	bt_notify_service_loop();

	win_loop();
}

void main(void)
{
	LOG_INF("PineTime Hermes starting.");

	setup();

	while (1) {
		loop();
	}
}
