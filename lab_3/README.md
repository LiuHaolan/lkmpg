## Lab3: Kernel Threads and Synchronization

# usage
make all
insmod/rmmod

# test
We traverse the list to test for data structure integrity
make test

# explanation
This is a basic linux kernel module. code in lab3.c
It's mainly about experimenting on manipulating kernel thread.
Note that you must be very careful with the cleaning up function. stop the kernel thread requires the task struct should be valid when we doing that.
