#include <ui/win.h>
#include <ui/widget/console.h>
#include <ui/widget/clock.h>
extern "C" {
#include <service/msg_store.h>
}
#include "console.h"

static GFX* tft;
static UiwConsole* console;

static void console_log(char* line)
{
    console->println();
    console->appendTimestamp();
    console->print(line);
}

static void msg_store_listener(char* text, u16_t len)
{
    console->println();
    console_log(text);
}

static void init()
{
    tft = win_get_gfx();
    tft->fillScreen(ST77XX_BLACK);

    console = new UiwConsole(tft);
    console->init();

    msg_store_register_listener(msg_store_listener);
}

static void loop()
{
    uiw_clock_draw(tft, 5);

    char line[64];
    sprintf(line, "Random text every second. %d", z_tick_get() % 99);
    console_log(line);
    
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
