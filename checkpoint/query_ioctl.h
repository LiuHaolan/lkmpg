#ifndef QUERY_IOCTL_H
#define QUERY_IOCTL_H

#include <asm/ioctl.h>

typedef struct
{
	int status, dignity, ego;
} query_arg_t;

#define CMD_PROCESS_ID _IOC(_IOC_NONE,'k',0, 0)

#endif
