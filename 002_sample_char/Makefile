obj-m += simple_char.o
KERNEL_SOURCE := /lib/modules/$(shell uname -r)/build

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
