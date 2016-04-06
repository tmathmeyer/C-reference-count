CFLAGS = -std=c11 -pedantic -rdynamic -g -D_GNU_SOURCE

all: ref.o types.o
	gcc runrefs.c ref.o types.o -o runrefs -g -pedantic

.c.o: config.mk
	@echo CC -c $<
	@${CC} -c $< ${CFLAGS}


tests: tests.c all
	@./scut.sh tests.c > /dev/null
	@mv tests.c.o tests.o > /dev/null
	@gcc ref.o types.o tests.o -o tests -g -pedantic > /dev/null
	@./tests

clean:
	rm reftest *.o
