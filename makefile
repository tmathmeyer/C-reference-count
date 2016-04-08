CFLAGS = -std=c11 -pedantic-errors -rdynamic -g -D_GNU_SOURCE -Wall -Wextra

all: ref.o types.o list.o
	@echo "eventually build lib file;"
	@echo "for now, please run 'make tests'"
#gcc runrefs.c ref.o types.o list.o -o runrefs -g -pedantic

.c.o: config.mk
	@echo CC -c $<
	@${CC} -c $< ${CFLAGS}


tests: tests.c all
	@./scut.sh tests.c > /dev/null
	@mv tests.c.o tests.o > /dev/null
	@gcc ref.o types.o tests.o list.o -o tests -g -pedantic > /dev/null
	@./tests

clean:
	rm reftest *.o
