obj-m := hello.o

.PHONY: build clean load unload

build:
	printf "#define LICENSE \""      > license.h
	shuf -n 1 LICENSES | tr -d "\n" >> license.h
	printf "\""                     >> license.h
	make -C /lib/modules/$(shell uname -r)/build modules M=$(PWD)

clean:
	rm -f license.h
	make -C /lib/modules/$(shell uname -r)/build clean M=$(PWD)

load:
	sudo insmod *.ko

unload:
	sudo rmmod $(patsubst %.ko, %, $(wildcard *.ko))
