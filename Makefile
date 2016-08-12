PROJECT := cref

SRC_DFS := -D__NO_FLAGS__
binary  := build/$(PROJECT)
library := build/lib$(PROJECT).a
CFLAGS  := -pedantic -g -rdynamic

src_source_files := $(wildcard src/C/*.c)
src_object_files := $(patsubst src/C/%.c, \
	build/obj/%.o, $(src_source_files))

lib_source_files := $(wildcard src/lib/C/*.c)
lib_object_files := $(patsubst src/lib/C/%.c, \
	build/lib/%.o, $(lib_source_files))
lib_header_files := $(wildcard src/lib/C/*.h)

.PHONY: all clean

all: library

clean:
	@rm -fr build

library: $(lib_object_files) headers
	ar -cq $(library) $(lib_object_files)

build/lib/%.o: src/lib/C/%.c
	@mkdir -p $(shell dirname $@)
	gcc $(CFLAGS) -c $< -o $@ $(SRC_DFS)

build/obj/%.o: src/C/%.c
	@mkdir -p $(shell dirname $@)
	gcc $(CFLAGS) -c $< -o $@ $(SRC_DFS)

headers:
	@mkdir -p build/headers/$(PROJECT)
	@cp $(lib_header_files) build/headers/$(PROJECT)

test: headers library
	@mkdir -p build/test/$(PROJECT)
	@cp $(lib_header_files) build/test/$(PROJECT)
	@cp -r test/C/* build/test
	gcc -c build/test/test.c
	gcc build/test/test.c build/lib$(PROJECT).a -o build/test.out
	@./build/test.out
	
