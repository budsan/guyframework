#ifndef _LOG_INCLUDED_
#define _LOG_INCLUDED_

#include <cstdio>

#if defined(_DEBUG)
	#if defined(__GAME_FRAMEWORK_USE_LINUX)
		#include <typeinfo>
		#define _CLASS_NAME_ typeid(*this).name()
		#define dbgPrintLog(format, args...) printLog("%s " format, _CLASS_NAME_, ## args)
	#elif defined(__GAME_FRAMEWORK_USE_BBSDK)
		#include <typeinfo>
		#define _CLASS_NAME_ typeid(*this).name()
		#define dbgPrintLog(format, args...) printLog("%s " format, _CLASS_NAME_, ## args)
	#endif
#else
	#define dbgPrintLog(format, args...) do {} while(0);
#endif

void openLogFile();
void printLog(const char *fmt, ...);

#endif
