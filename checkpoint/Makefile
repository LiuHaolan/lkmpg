obj-m += checkpoint.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
	gcc test.c -o test
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
test:
	make all
	sudo dmesg -C
	sudo insmod lab3.ko
	sudo rmmod lab3.ko
	dmesg
