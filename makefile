CFLAGS = -std=c11 -pedantic-errors -rdynamic -g -D_GNU_SOURCE -Wall -Wextra

all: ref.o types.o list.o
	@echo "eventually build lib file;"
	@echo "for now, please run 'make tests'"
#gcc runrefs.c ref.o types.o list.o -o runrefs -g -pedantic

.c.o: config.mk
	@echo CC -c $<
	@${CC} -c $< ${CFLAGS}

linkable: ref.o types.o list.o
	@ld -r $+ -o cref.o

tests: tests.c linkable
	@gcc cref.o tests.c -o tests -g -pedantic > /dev/null
	@./tests

clean:
	@rm -rf *.o tests html/ latex/
