#include "panic.h"
#include "idt.h"                // enable()...

#include "libc.h"
#include "vid.h"

#include <stdarg.h>

void panic(const char *fmt, ...) {
//    disable();
    while (1) {
    home();
    setattr(BG_FG(BLACK, LRED));
    puts("Kernel panic: ");
    va_list ap;
    va_start(ap, fmt);
    static char buf[80];
    memset(buf, 'X', sizeof(buf));
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
#if 1
    puts(buf);
#else
    for (size_t i = 0; i < sizeof(buf); i++) {
        char c;
        c = buf[i] & 0xf;
        c = c > 9 ? 'a' + c - 10 : '0' + c;
        dputchar(c);
        c = buf[i] >> 4;
        c = c > 9 ? 'a' + c - 10 : '0' + c;
        dputchar(c);
    }
#endif
        halt();
    }
}
