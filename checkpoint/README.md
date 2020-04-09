# Lab3: Kernel Threads and Synchronization

## usage
make all

insmod/rmmod

## test
We traverse the list to test for data structure integrity

make test

## explanation
This is a basic linux kernel module. code in lab3.c

It's mainly about experimenting on manipulating kernel thread.

Note that you must be very careful with the cleaning up function. stop the kernel thread requires the task struct should be valid when we doing that.

Kernel header:
We use list.h as double linked list data structure to avoid reinventing the wheel

Function:
thread_init: init 4 thread with configurable list appending operation
thread_destroy: to cleanup kernel thread
add_item, add_monkey: warpper function for kthread to run, using synchronizing spin lock in add_monkey.

