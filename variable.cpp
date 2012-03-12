#include "variable.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

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

Variable::Variable()
	: myName(""), myType(Variable::Invalid)
{
	myDefVal.asInt = 0;
	myCurVal.asInt = 0;
}

Variable::Variable(const Variable & other)
	: myName(other.myName), myType(other.myType)
{
	if (myType == Variable::String)
	{
		myDefVal.asString = allocStringCopy(other.myDefVal.asString);
		myCurVal.asString = allocStringCopy(other.myCurVal.asString);
	}
	else
	{
		myDefVal.asInt = myDefVal.asInt;
		myCurVal.asInt = myCurVal.asInt;
	}
}

Variable::Variable(std::string name, bool dval)
	: myName(name), myType(Variable::Bool)
{
	myDefVal.asBool = dval;
	myCurVal.asBool = dval;
}

Variable::Variable(std::string name, int dval)
	: myName(name), myType(Variable::Int)
{
	myDefVal.asInt = dval;
	myCurVal.asInt = dval;
}

Variable::Variable(std::string name, double dval)
	: myName(name), myType(Variable::Double)
{
	myDefVal.asDouble = dval;
	myCurVal.asDouble = dval;
}

Variable::Variable(std::string name, char dval)
	: myName(name), myType(Variable::Char)
{
	myDefVal.asChar = dval;
	myCurVal.asChar = dval;
}

Variable::Variable(std::string name, std::string dval)
	: myName(name), myType(Variable::String)
{
	myDefVal.asString = allocStringCopy(dval);
	myCurVal.asString = allocStringCopy(dval);
}

Variable::~Variable()
{
	if (myType == Variable::String)
	{
		delete myDefVal.asString;
		delete myCurVal.asString;
	}
}

const Variable &Variable::operator=(const Variable & other)
{
	myName = other.myName;
	myType = other.myType;

	if (myType == Variable::String)
	{
		myDefVal.asString = allocStringCopy(other.myDefVal.asString);
		myCurVal.asString = allocStringCopy(other.myCurVal.asString);
	}
	else
	{
		myDefVal.asInt = myDefVal.asInt;
		myCurVal.asInt = myCurVal.asInt;
	}

	return *this;
}

void Variable::set(bool val)
{
	switch (myType)
	{
	case Variable::Invalid: break;
	case Variable::Bool: myCurVal.asBool = ::toBool(val); break;
	case Variable::Int: myCurVal.asInt = ::toInt(val); break;
	case Variable::Double: myCurVal.asDouble = ::toDouble(val); break;
	case Variable::Char: myCurVal.asChar = ::toChar(val); break;
	case Variable::String:
		delete myCurVal.asString;
		myCurVal.asString = ::toString(val); break;
	}
}

void Variable::set(int val)
{
	switch (myType)
	{
	case Variable::Invalid: break;
	case Variable::Bool: myCurVal.asBool = ::toBool(val); break;
	case Variable::Int: myCurVal.asInt = ::toInt(val); break;
	case Variable::Double: myCurVal.asDouble = ::toDouble(val); break;
	case Variable::Char: myCurVal.asChar = ::toChar(val); break;
	case Variable::String:
		delete myCurVal.asString;
		myCurVal.asString = ::toString(val); break;
	}
}

void Variable::set(double val)
{
	switch (myType)
	{
	case Variable::Invalid: break;
	case Variable::Bool: myCurVal.asBool = ::toBool(val); break;
	case Variable::Int: myCurVal.asInt = ::toInt(val); break;
	case Variable::Double: myCurVal.asDouble = ::toDouble(val); break;
	case Variable::Char: myCurVal.asChar = ::toChar(val); break;
	case Variable::String:
		delete myCurVal.asString;
		myCurVal.asString = ::toString(val); break;
	}
}

void Variable::set(char val)
{
	switch (myType)
	{
	case Variable::Invalid: break;
	case Variable::Bool: myCurVal.asBool = ::toBool(val); break;
	case Variable::Int: myCurVal.asInt = ::toInt(val); break;
	case Variable::Double: myCurVal.asDouble = ::toDouble(val); break;
	case Variable::Char: myCurVal.asChar = ::toChar(val); break;
	case Variable::String:
		delete myCurVal.asString;
		myCurVal.asString = ::toString(val); break;
	}
}

void Variable::set(std::string _val)
{
	const char* val = _val.c_str();
	switch (myType)
	{
	case Variable::Invalid: break;
	case Variable::Bool: myCurVal.asBool = ::toBool(val); break;
	case Variable::Int: myCurVal.asInt = ::toInt(val); break;
	case Variable::Double: myCurVal.asDouble = ::toDouble(val); break;
	case Variable::Char: myCurVal.asChar = ::toChar(val); break;
	case Variable::String:
		delete myCurVal.asString;
		myCurVal.asString = ::toString(val); break;
	}
}

void Variable::setDefault()
{
	switch (myType)
	{
	case Variable::Invalid: break;
	case Variable::Bool: myCurVal.asBool = myDefVal.asBool; break;
	case Variable::Int: myCurVal.asInt = myDefVal.asInt; break;
	case Variable::Double: myCurVal.asDouble = myDefVal.asDouble; break;
	case Variable::Char: myCurVal.asChar = myDefVal.asChar; break;
	case Variable::String:
		delete myCurVal.asString;
		myCurVal.asString = allocStringCopy(myDefVal.asString); break;
	}
}

bool Variable::toBool()
{
	switch (myType)
	{
	case Variable::Invalid: return false;
	case Variable::Bool: return ::toBool(myCurVal.asBool);
	case Variable::Int: return ::toBool(myCurVal.asInt);
	case Variable::Double: return ::toBool(myCurVal.asDouble);
	case Variable::Char: return ::toBool(myCurVal.asChar);
	case Variable::String: return ::toBool(myCurVal.asString);
	}

	return false;
}

int Variable::toInt()
{
	switch (myType)
	{
	case Variable::Invalid: return 0;
	case Variable::Bool: return ::toInt(myCurVal.asBool);
	case Variable::Int: return ::toInt(myCurVal.asInt);
	case Variable::Double: return ::toInt(myCurVal.asDouble);
	case Variable::Char: return ::toInt(myCurVal.asChar);
	case Variable::String: return ::toInt(myCurVal.asString);
	}

	return 0;
}

double Variable::toDouble()
{
	switch (myType)
	{
	case Variable::Invalid: return 0;
	case Variable::Bool: return ::toDouble(myCurVal.asBool);
	case Variable::Int: return ::toDouble(myCurVal.asInt);
	case Variable::Double: return ::toDouble(myCurVal.asDouble);
	case Variable::Char: return ::toDouble(myCurVal.asChar);
	case Variable::String: return ::toDouble(myCurVal.asString);
	}

	return 0;
}

char Variable::toChar()
{
	switch (myType)
	{
	case Variable::Invalid: return 0;
	case Variable::Bool: return ::toBool(myCurVal.asBool);
	case Variable::Int: return ::toBool(myCurVal.asInt);
	case Variable::Double: return ::toBool(myCurVal.asDouble);
	case Variable::Char: return ::toBool(myCurVal.asChar);
	case Variable::String: return ::toBool(myCurVal.asString);
	}

	return 0;
}

std::string Variable::toString()
{
	char *c_ret = NULL;
	switch (myType)
	{
	case Variable::Invalid: std::string();
	case Variable::Bool: c_ret = ::toString(myCurVal.asBool); break;
	case Variable::Int: c_ret = ::toString(myCurVal.asInt); break;
	case Variable::Double: c_ret = ::toString(myCurVal.asDouble); break;
	case Variable::Char: c_ret = ::toString(myCurVal.asChar); break;
	case Variable::String: c_ret = ::toString(myCurVal.asString); break;
	default: c_ret = ::toString("");
	}

	std::string ret(c_ret); delete c_ret;
	return ret;
}

std::ostream &operator <<(std::ostream &out, const Variable &var)
{
	/*
	Variable::Type type = var.type();
	out.write((const char*)&type, sizeof(Variable::Type));
	if(var.type() == Variable::Invalid) return out;

	out.write(var.name().c_str(), var.name().length()+1);
	switch (var.type())
	{
	case Variable::Invalid: break;
	case Variable::Bool: out.write(&var.toBool(),sizeof(bool)); break;
	case Variable::Int: out.write(&var.toInt(),sizeof(int)); break;
	case Variable::Double: out.write(&var.toDouble(),sizeof(double)); break;
	case Variable::Char: out.write(&var.toChar(),sizeof(char)); break;
	case Variable::String: {
			std::string s = var.toString();
			out.write(s.c_str(),s.length()+1);
		} break;
	}
	*/
	return out;
}

std::istream &operator >>(std::istream &in, Variable &var)
{
	return in;
}
