#include "log.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <time.h>

namespace Guy {

void openLogFile()
{

}

void printLog(const char *fmt, ...)
{
	if (fmt == NULL) return;
	time_t t;
	time(&t);

	struct tm * ptm;
	ptm = gmtime (&t);

	fprintf(stderr, "[%2d:%02d:%02d] ", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
}

}


