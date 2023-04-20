.PHONY: all clean
all: clean # hope you installed ccache
	mkdir -p build
	clang -Wall -Wextra -Wpedantic -Weverything -Werror -Wno-declaration-after-statement -g -Og --std=gnu17 main.c -o build/test

clean:
	rm -rf build
