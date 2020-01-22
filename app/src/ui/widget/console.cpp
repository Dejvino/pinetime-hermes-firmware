#include <common.h>
extern "C" {
#include <hw/clock.h>
#include <hw/display.h>
}
#include "console.h"

static int console_w = 39;

void draw_line_text(GFX* tft, u16_t basex, u16_t basey, u16_t h, char* line_text,
        u16_t line, bool highlight)
{
    int x = basex;
    int w = tft->width() - 2*x;
    int y = basey + line * h;
	
	tft->openBuffer(x, y, w, h);
	tft->setCursor(x, y + 1);
	tft->setTextSize(1);
    if (highlight) {
	    tft->setTextColor(ST77XX_RED);
	    tft->fillRect(x, y, w, h, ST77XX_WHITE);
    } else {
        tft->setTextColor(ST77XX_GREEN);
	    tft->fillRect(x, y, w, h, ST77XX_BLACK);
    }
	tft->print(line_text);
	tft->flushBuffer();
}

UiwConsole::UiwConsole(GFX* tft)
{
    this->tft = tft;
}

void UiwConsole::init()
{
    display_scroll_region(40, 200, 320 - 240);
}

void UiwConsole::draw()
{
}

void UiwConsole::append(char* text)
{
    strcat(this->line_text, text);
}

void UiwConsole::print()
{
    char* subline = this->line_text;
    char console_line[console_w + 1];
    while (strlen(subline) > console_w) {
        memset(console_line, 0, sizeof(console_line));
        memcpy(console_line, subline, console_w);
        this->draw_history_line(console_line);
        this->draw_newline();
        subline = subline + console_w;
    }
    strcpy(console_line, subline);
    memset(this->line_text, 0, sizeof(this->line_text));
    strcpy(this->line_text, console_line);

    this->draw_current_line(this->line_text);
}

void UiwConsole::print(char* text)
{
    this->append(text);
    this->print();
}

void UiwConsole::println(char* text)
{
    this->append(text);
    this->println();
}

void UiwConsole::println()
{
    this->draw_history_line(this->line_text);
    this->draw_newline();
    memset(this->line_text, 0, sizeof(this->line_text));
}

void UiwConsole::appendTimestamp()
{
    struct tm* tm_ptr = clock_get_datetime();
	uint8_t hours = tm_ptr->tm_hour;
	uint8_t minutes = tm_ptr->tm_min;
	uint8_t seconds = tm_ptr->tm_sec;

    static char timestamp[32];
    memset(timestamp, 0, sizeof(timestamp));
	sprintf(timestamp, "[%02d:%02d:%02d] ", hours, minutes, seconds);

    this->append(timestamp);
}

void UiwConsole::printTimestamp()
{
    this->appendTimestamp();
    this->print();
}

void UiwConsole::printlnTimestamped(char* text)
{
    this->appendTimestamp();
    this->println(text);
}

void UiwConsole::clear()
{}

void UiwConsole::deinit()
{}

void UiwConsole::draw_history_line(char* text)
{
    draw_line_text(this->tft, this->basex, this->basey, this->h,
        text, (this->line + 18) % 20, false);
}

void UiwConsole::draw_current_line(char* text)
{
    draw_line_text(this->tft, this->basex, this->basey, this->h,
        text, (this->line + 18) % 20, true);
}

void UiwConsole::draw_newline()
{
    display_scroll_offset(40 + this->line * this->h);
    this->line = (this->line + 1) % 20;
}