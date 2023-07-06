CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -g

MODULE_NAME = asciiart
TEST_PROGRAM_NAME = test

KERNEL_BUILD_PATH = /lib/modules/$(shell uname -r)/build
CURRENT_PATH = $(shell pwd)

.PHONY: build test load unload clean all

# List of object files that the module depends on
obj-m += $(MODULE_NAME).o

all: build test

# Target to build the module
build: $(MODULE_NAME).c $(MODULE_NAME).h
	make -C $(KERNEL_BUILD_PATH) modules M=$(CURRENT_PATH)

# Target to build the test program
test: $(TEST_PROGRAM_NAME).c $(MODULE_NAME).h
	$(CC) $(CFLAGS) -o $(TEST_PROGRAM_NAME) $(TEST_PROGRAM_NAME).c

# Target to load the module into the kernel
load:
	sudo insmod $(MODULE_NAME).ko

# Target to unload the module from the kernel
unload:
	sudo rmmod $(MODULE_NAME)

# Target to clean up all generated files
clean:
	make -C $(KERNEL_BUILD_PATH) clean M=$(CURRENT_PATH)
	@rm -vf $(TEST_PROGRAM_NAME)
