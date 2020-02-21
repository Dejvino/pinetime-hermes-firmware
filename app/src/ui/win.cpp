#include "win.h"
extern "C" {
#include <hw/display.h>
}

static GFX* tft;

static win_t* current_win;

void win_activate(win_t* win)
{
    if (current_win != NULL) {
        current_win->deinit();
    }
    current_win = win;
    if (current_win != NULL) {
        current_win->init();
    }
}

GFX* win_get_gfx()
{
    return tft;
}

void win_init()
{
    tft = createGFX(display_get_device());
    k_sleep(K_MSEC(100));
	tft->fillScreen(ST77XX_BLACK);
}

void win_loop()
{
    if (!current_win) {
        return;
    }
    current_win->loop();
}
