#pragma once
#include <cstdio>

#ifdef _DEBUG
	#if defined(GUY_USE_LINUX)
		#include <typeinfo>
		#define _CLASS_NAME_ typeid(*this).name()
		#define dbgPrintLog(format, args...) printLog("%s " format, _CLASS_NAME_, ## args)
	#elif defined(GUY_USE_BB10)
		#include <typeinfo>
		#define _CLASS_NAME_ typeid(*this).name()
		#define dbgPrintLog(format, args...) printLog("%s " format, _CLASS_NAME_, ## args)
	#else
		#error "dbgPrintLog not defined for your platform"
	#endif
#else
	#define dbgPrintLog(format, args...) do {} while(0);
#endif

namespace Guy {

void openLogFile();
void printLog(const char *fmt, ...);

} //namespace Guy
