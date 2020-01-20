#include "console.h"
#include <ui/widget/console.h>
#include <ui/widget/clock.h>

static GFX* tft;

static void init()
{
    tft = win_get_gfx();
    tft->fillScreen(ST77XX_BLACK);
}

static void loop()
{
    uiw_clock_draw(tft);
}

void deinit() {}

static win_t win = {
    .id = WIN_CONSOLE_ID,
    .init = init,
    .loop = loop,
    .deinit = deinit,
};

win_t* win_console_get()
{
    return &win;
}
