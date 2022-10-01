
TARGET:=test_libc

all: $(TARGET)

check check32: test_libc
	./test_libc

CFLAGS+=-Wall -Wextra -Werror -g -O0
CFLAGS+=-I../kernel
test_libc: test_libc.o libc.o
	$(CC) -o $@ $^ -I.

clean:
	$(RM) *.o $(TARGET)

clobber: clean
