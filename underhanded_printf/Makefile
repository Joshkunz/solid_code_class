.PHONY: clean spoil
CC = clang
CFLAGS = -O0 -g -Wall -Wextra -pedantic
LDLIBS = -lm

objects = xprintf.o test.o
bin = test
spoilers = spoilers.text

run-tests: $(bin)
	./test

$(bin): $(objects)

xprintf.o: xprintf.h
test.o: xprintf.h

rot128:

spoil: rot128
	@./$^ < $(spoilers)
	@rm -f $^ $^.o

clean:
	-rm -f $(objects) $(bin)
