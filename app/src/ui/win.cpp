#include "win.h"
extern "C" {
#include <hw/display.h>
}

static GFX* tft;

static win_t* current_win;

void win_activate(win_t* win)
{
    bool win_change = (win && !current_win) || (win->id != current_win->id);
    if (current_win && win_change) {
        current_win->deinit();
    }
    current_win = win;
    if (win_change) {
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
	tft->fillScreen(ST77XX_BLACK);
}

void win_loop()
{
    if (!current_win) {
        return;
    }
    current_win->loop();
}
