#include <inttypes.h>
#include <string.h>

#define NO_ALIAS
#include "libc.h"

#include <stdio.h>

#include "expect.h"

void dputchar(char c) {
    putchar(c);
}

void dputs(char *s) {
    printf("%s", s);fflush(stdout);
}

void msnprintf(char *str, size_t size, const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    mvsnprintf(str, size, format, ap);
    va_end(ap);
}

#if 0
//#define ARGS "Kernel panic: [toto=%s dead=%" PRIx16 " 0xffff0=%p 0XAC1DC0D3=%#" PRIX32 " 1234=%d]\n", "toto", 0xdead, (void *)0xffff0, 0xac1dc0d3, 1234
#define ARGS "|%" PRIx16 " ", 0xdead
#define RES "|dead "
#else
#define ARGS "|%d ", 1234
#define RES "|1234 "
#endif
int main() {
//    printf(ARGS);
//    mprintf(ARGS);

    char buf[90];

    memset(buf, 'E', sizeof(buf));
    snprintf(buf, sizeof(buf), ARGS);
    EXPECT_STR_EQ(buf, RES);
//    printf("%s", buf);

    memset(buf, 'E', sizeof(buf));
    msnprintf(buf, sizeof(buf), ARGS);
    EXPECT_STR_EQ(buf, RES);
//    printf("%s", buf);

    return 0;
}
