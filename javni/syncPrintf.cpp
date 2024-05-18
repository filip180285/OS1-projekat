#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>
#include "Init.h"

int syncPrintf(const char *format, ...)
{
	int res;
	va_list args;
#ifndef BCC_BLOCK_IGNORE
	lock
#endif
	va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
#ifndef BCC_BLOCK_IGNORE
	unlock
#endif
		return res;
}
