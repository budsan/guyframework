#include "setting.h"
#include <string.h>
#include <stdlib.h>

//Buffer for number conversions to ascii
char tmp[128];

char *allocStringCopy(const char* str, int len)
{
	char *ret = new char[len];
	memcpy(ret, str, len);
	return ret;
}

char *allocStringCopy(const std::string &str)
{
	int len = str.length()+1;
	return allocStringCopy(str.c_str(), len);
}

char *allocStringCopy(const char* str)
{

	int len = strlen(str)+1;
	return allocStringCopy(str, len);
}

bool toBool(bool val) {return val;}
bool toBool(int val) {return (bool)val;}
bool toBool(double val) {return (val!=0)?true:false;}
bool toBool(char val) {return (bool)val;}
bool toBool(const char* val) {return strcmp(val,"true") == 0;}

int toInt(bool val) {return val?1:0;}
int toInt(int val) {return val;}
int toInt(double val) {return (int)val;}
int toInt(char val) {return val;}
int toInt(const char* val) {return atoi(val);}

double toDouble(bool val) {return val?1:0;}
double toDouble(int val) {return (double)val;}
double toDouble(double val) {return val;}
double toDouble(char val) {return (double)val;}
double toDouble(const char* val) {return atof(val);}

char toChar(bool val) {return val?'1':'0';}
char toChar(int val) {return (char)val;}
char toChar(double val) {return (char)val;}
char toChar(char val) {return val;}
char toChar(const char* val) {return (char)atoi(val);}

char* toString(bool val)
{
	return allocStringCopy(val?"true":"false");
}

char* toString(int val)
{
	int len = sprintf(tmp,"%d",val)+1;
	return allocStringCopy(tmp, len);
}

char* toString(double val)
{
	int len = sprintf(tmp,"%f",val)+1;
	return allocStringCopy(tmp, len);
}

char* toString(char val)
{
	tmp[0] = val; tmp[1] = '\0';
	return allocStringCopy(tmp, 2);
}

char* toString(const char* val)
{
	return allocStringCopy(val);
}

Setting::Setting(std::string name, bool dval)
	: myName(name), myType(Setting::Bool)
{
	myDefVal.asBool = dval;
	myCurVal.asBool = dval;
}

Setting::Setting(std::string name, int dval)
	: myName(name), myType(Setting::Int)
{
	myDefVal.asInt = dval;
	myCurVal.asInt = dval;
}

Setting::Setting(std::string name, double dval)
	: myName(name), myType(Setting::Double)
{
	myDefVal.asDouble = dval;
	myCurVal.asDouble = dval;
}

Setting::Setting(std::string name, char dval)
	: myName(name), myType(Setting::Char)
{
	myDefVal.asChar = dval;
	myCurVal.asChar = dval;
}

Setting::Setting(std::string name, std::string dval)
	: myName(name), myType(Setting::String)
{
	myDefVal.asString = allocStringCopy(dval);
	myCurVal.asString = allocStringCopy(dval);
}

Setting::~Setting()
{
	if (myType == Setting::String)
	{
		delete myDefVal.asString;
		delete myCurVal.asString;
	}
}

void Setting::set(bool val)
{
	switch (myType)
	{
	case Setting::Bool: myCurVal.asBool = ::toBool(val); break;
	case Setting::Int: myCurVal.asInt = ::toInt(val); break;
	case Setting::Double: myCurVal.asDouble = ::toDouble(val); break;
	case Setting::Char: myCurVal.asChar = ::toChar(val); break;
	case Setting::String:
		if (myCurVal.asString != NULL) delete myCurVal.asString;
		myCurVal.asString = ::toString(val); break;
	}
}

void Setting::set(int val)
{
	switch (myType)
	{
	case Setting::Bool: myCurVal.asBool = ::toBool(val); break;
	case Setting::Int: myCurVal.asInt = ::toInt(val); break;
	case Setting::Double: myCurVal.asDouble = ::toDouble(val); break;
	case Setting::Char: myCurVal.asChar = ::toChar(val); break;
	case Setting::String:
		if (myCurVal.asString != NULL) delete myCurVal.asString;
		myCurVal.asString = ::toString(val); break;
	}
}

void Setting::set(double val)
{
	switch (myType)
	{
	case Setting::Bool: myCurVal.asBool = ::toBool(val); break;
	case Setting::Int: myCurVal.asInt = ::toInt(val); break;
	case Setting::Double: myCurVal.asDouble = ::toDouble(val); break;
	case Setting::Char: myCurVal.asChar = ::toChar(val); break;
	case Setting::String:
		if (myCurVal.asString != NULL) delete myCurVal.asString;
		myCurVal.asString = ::toString(val); break;
	}
}

void Setting::set(char val)
{
	switch (myType)
	{
	case Setting::Bool: myCurVal.asBool = ::toBool(val); break;
	case Setting::Int: myCurVal.asInt = ::toInt(val); break;
	case Setting::Double: myCurVal.asDouble = ::toDouble(val); break;
	case Setting::Char: myCurVal.asChar = ::toChar(val); break;
	case Setting::String:
		if (myCurVal.asString != NULL) delete myCurVal.asString;
		myCurVal.asString = ::toString(val); break;
	}
}

void Setting::set(std::string _val)
{
	const char* val = _val.c_str();
	switch (myType)
	{
	case Setting::Bool: myCurVal.asBool = ::toBool(val); break;
	case Setting::Int: myCurVal.asInt = ::toInt(val); break;
	case Setting::Double: myCurVal.asDouble = ::toDouble(val); break;
	case Setting::Char: myCurVal.asChar = ::toChar(val); break;
	case Setting::String:
		if (myCurVal.asString != NULL) delete myCurVal.asString;
		myCurVal.asString = ::toString(val); break;
	}
}

void Setting::setDefault()
{
	switch (myType)
	{
	case Setting::Bool: myCurVal.asBool = myDefVal.asBool; break;
	case Setting::Int: myCurVal.asInt = myDefVal.asInt; break;
	case Setting::Double: myCurVal.asDouble = myDefVal.asDouble; break;
	case Setting::Char: myCurVal.asChar = myDefVal.asChar; break;
	case Setting::String:
		if (myCurVal.asString != NULL) delete myCurVal.asString;
		myCurVal.asString = allocStringCopy(myDefVal.asString); break;
	}
}

bool Setting::toBool()
{
	switch (myType)
	{
	case Setting::Bool: return ::toBool(myCurVal.asBool);
	case Setting::Int: return ::toBool(myCurVal.asInt);
	case Setting::Double: return ::toBool(myCurVal.asDouble);
	case Setting::Char: return ::toBool(myCurVal.asChar);
	case Setting::String: return ::toBool(myCurVal.asString);
	}
}

int Setting::toInt()
{
	switch (myType)
	{
	case Setting::Bool: return ::toInt(myCurVal.asBool);
	case Setting::Int: return ::toInt(myCurVal.asInt);
	case Setting::Double: return ::toInt(myCurVal.asDouble);
	case Setting::Char: return ::toInt(myCurVal.asChar);
	case Setting::String: return ::toInt(myCurVal.asString);
	}
}

double Setting::toDouble()
{
	switch (myType)
	{
	case Setting::Bool: return ::toDouble(myCurVal.asBool);
	case Setting::Int: return ::toDouble(myCurVal.asInt);
	case Setting::Double: return ::toDouble(myCurVal.asDouble);
	case Setting::Char: return ::toDouble(myCurVal.asChar);
	case Setting::String: return ::toDouble(myCurVal.asString);
	}
}

char Setting::toChar()
{
	switch (myType)
	{
	case Setting::Bool: return ::toBool(myCurVal.asBool);
	case Setting::Int: return ::toBool(myCurVal.asInt);
	case Setting::Double: return ::toBool(myCurVal.asDouble);
	case Setting::Char: return ::toBool(myCurVal.asChar);
	case Setting::String: return ::toBool(myCurVal.asString);
	}
}

std::string Setting::toString()
{
	char *c_ret = NULL;
	switch (myType)
	{
	case Setting::Bool: c_ret = ::toString(myCurVal.asBool); break;
	case Setting::Int: c_ret = ::toString(myCurVal.asInt); break;
	case Setting::Double: c_ret = ::toString(myCurVal.asDouble); break;
	case Setting::Char: c_ret = ::toString(myCurVal.asChar); break;
	case Setting::String: c_ret = ::toString(myCurVal.asString); break;
	}

	std::string ret(c_ret); delete c_ret;
	return ret;
}
