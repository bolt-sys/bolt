//
// Created by Emil Glenn Karpawitz on 8/26/23.
//
#include "unistd.h"
#include "x86_64/io.h"

static const unsigned short fd_0 = 0x3f8; /* FD 0 */

void write(int fd, const char *buf, size_t count)
{
	if (fd == 0)
	{
		outsb(fd_0, buf, count);
	}
}