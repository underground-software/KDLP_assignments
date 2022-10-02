obj-m += hello.o

CONFIG_MODULE_SIG=n

.PHONY: build clean load unload

build:
	make -C /lib/modules/$(shell uname -r)/build modules M=$(PWD)

clean:
	make -C /lib/modules/$(shell uname -r)/build clean M=$(PWD)

load:
	sudo insmod hello.ko

unload:
	-sudo rmmod hello
