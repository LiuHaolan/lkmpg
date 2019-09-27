obj-m += hello.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
test:
	make all
	sudo dmesg -C
	sudo insmod hello.ko
	cat /proc/wuklab
	echo "test_string" >/proc/wuklab
	sudo rmmod hello.ko
	dmesg
