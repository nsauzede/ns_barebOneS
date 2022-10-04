#include "panic.h"
#include "idt.h"                // enable()...

#include "libc.h"
#include "vid.h"

#include <stdarg.h>

void panic(const char *fmt, ...) {
    disable();
    home();
    setattr(BG_FG(BLACK, LRED));
    dputs("Kernel panic: ");
    va_list ap;
    va_start(ap, fmt);
    static char buf[80];
    memset(buf, 'X', sizeof(buf));
    vsnprintf(buf, sizeof(buf), fmt, ap);
    va_end(ap);
    dputs(buf);
    while (1) {
        halt();
    }
}
