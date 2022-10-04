#define NO_ALIAS
#include "libc.h"

#include "test/test.h"

static void msnprintf(char *str, size_t size, const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    mvsnprintf(str, size, format, ap);
    va_end(ap);
}

#define ARGS "|%d ", 1234
#define RES "|1234 "
TEST_F(libc, msnprintf) {
    char buf[90];

    memset(buf, 'E', sizeof(buf));
    snprintf(buf, sizeof(buf), ARGS);
    EXPECT_STREQ(buf, RES);

    memset(buf, 'E', sizeof(buf));
    msnprintf(buf, sizeof(buf), ARGS);
    EXPECT_STREQ(buf, RES);
}
