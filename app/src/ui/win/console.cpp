#include <ui/win.h>
#include <ui/widget/console.h>
#include <ui/widget/clock.h>
#include "console.h"

static GFX* tft;
static UiwConsole* console;

static void init()
{
    tft = win_get_gfx();
    tft->fillScreen(ST77XX_BLACK);

    console = new UiwConsole(tft);
    console->init();
}

static void loop()
{
    uiw_clock_draw(tft, 5);

    char line[64];
    sprintf(line, "Random text every second. %d", z_tick_get() % 99);
    console->println();
    console->appendTimestamp();
    console->print(line);
    console->draw();
}

void deinit()
{
    console->deinit();
    delete console;
    console = NULL;
}

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
