.PHONY: test clean

all: test

test: test.c
	clang -Weverything -Wno-unused-macros -Wno-declaration-after-statement -std=c11 test.c -o test

clean:
	rm test