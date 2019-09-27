# Lab2: Adding a new /proc file

## usage
make all
insmod/rmmod

## test
sudo make test

## explanation
This is a basic linux kernel module. code in hello.c

It's mainly registering the wuklab proc device with read and write handler.

Read handler is procfile_read function

Write handler is procfile_write function

One tricky thing to note is that the read/write mechanisms.

the user space buffer ask for a segment of data, the offset argument is updated and from which new read/write will begin

return the number of bytes that have been transferred

