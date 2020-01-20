#include <zephyr.h>
#include <zephyr/types.h>
#ifndef GFX_H
#define GFX_H
#include "lib/GFX.h"
#endif

#ifndef WIN_H
#define WIN_H
typedef void (*win_init_cb_t)();
typedef void (*win_loop_cb_t)();
typedef void (*win_deinit_cb_t)();

typedef struct {
    u16_t id;
    win_init_cb_t init;
    win_loop_cb_t loop;
    win_deinit_cb_t deinit;
} win_t;

void win_init();
void win_loop();

void win_activate(win_t* win);
GFX* win_get_gfx();
#endif // WIN_H