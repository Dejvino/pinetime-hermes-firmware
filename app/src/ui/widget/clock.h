#include <ui/win.h>

#define UIW_CLOCK_FORMAT_HMS 1
#define UIW_CLOCK_FORMAT_HM 2

/**
 * Convenience version, H:M:S clock in the middle of the display.
 */
void uiw_clock_draw(GFX* tft);

/**
 * Convenience version, H:M:S clock in the middle of the display
 * with adjustable Y position.
 */
void uiw_clock_draw(GFX* tft, u16_t y);


/**
 * Exposes all the options.
 */
void uiw_clock_draw(GFX* tft, u16_t x, u16_t y, u8_t text_size, u8_t format);