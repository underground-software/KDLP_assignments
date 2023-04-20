CC = gcc
WARN = -Wall -Wextra -Wpedantic
TST_EXE = test

all: test

test: test.c
	$(CC) $(WARN) -o $(TST_EXE) test.c
clean:
	rm -f $(TST_EXE) *~
