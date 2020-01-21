#include <ui/win.h>

class UiwConsole {
public:
    UiwConsole(GFX* tft);

    void init();
    void draw();
    void append(char* text);
    void appendTimestamp();
    void print();
    void print(char* text);
    void println(char* text);
    void println();
    void printTimestamp();
    void printlnTimestamped(char* text);
    void clear();
    void deinit();

private:
    GFX* tft;
    u16_t basex = 2;
    u16_t basey = 40;
    u16_t h = 10;

    u16_t line;
    char line_text[64];
};
