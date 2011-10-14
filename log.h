#ifndef _LOG_INCLUDED_
#define _LOG_INCLUDED_

#include <iostream>
#include <typeinfo>

#define _CLASS_NAME_ typeid(*this).name()
#define LOG Log()<<_CLASS_NAME_<<" "

std::ostream & Log();

#endif
