#include "log.h"

#include <cstdarg>
#include <cstdlib>
#include <time.h>

namespace Guy {

#define LOG_FILE "debug.txt"
static FILE* sLogFile = NULL;

void openLogFile()
{
	FILE* fLog;
	fLog = fopen(LOG_FILE, "wb");
	if(fLog != NULL) sLogFile = fLog;
}

void printLog(const char *fmt, ...)
{
	if (fmt == NULL) return;
	time_t t;
	time(&t);

	struct tm * ptm;
	ptm = gmtime (&t);

#ifdef _DEBUG
	fprintf(stderr, "[%2d:%02d:%02d] ", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
#endif

	if (sLogFile != NULL)
	{
		fprintf(sLogFile, "[%2d:%02d:%02d] ", ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
		va_list ap2;
		va_start(ap2, fmt);
		vfprintf(sLogFile, fmt, ap2);
		va_end(ap2);
		fflush(sLogFile);
	}
}

}


