#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>

#include "query_ioctl.h"

int main(){
	int fd = open("/dev/mynull", O_RDWR);
	int *buffer = malloc(sizeof(int));
	if(ioctl(fd, CMD_PROCESS_ID, buffer) < 0)
	{
		printf("ioctl wrong opened\n");
	}
	close(fd);
}
