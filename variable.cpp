#include "variable.h"

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

Variable::Variable()
	: m_name(""), m_type(Variable::Invalid)
{
	m_defVal.asInt = 0;
	m_curVal.asInt = 0;
}

Variable::Variable(const Variable & other)
	: m_name(other.m_name), m_type(other.m_type)
{
	if (m_type == Variable::String)
	{
		m_defVal.asString = allocStringCopy(other.m_defVal.asString);
		m_curVal.asString = allocStringCopy(other.m_curVal.asString);
	}
	else
	{
		m_defVal.asInt = m_defVal.asInt;
		m_curVal.asInt = m_curVal.asInt;
	}
}

Variable::Variable(std::string name, bool dval)
	: m_name(name), m_type(Variable::Bool)
{
	m_defVal.asBool = dval;
	m_curVal.asBool = dval;
}

Variable::Variable(std::string name, int dval)
	: m_name(name), m_type(Variable::Int)
{
	m_defVal.asInt = dval;
	m_curVal.asInt = dval;
}

Variable::Variable(std::string name, double dval)
	: m_name(name), m_type(Variable::Double)
{
	m_defVal.asDouble = dval;
	m_curVal.asDouble = dval;
}

Variable::Variable(std::string name, char dval)
	: m_name(name), m_type(Variable::Char)
{
	m_defVal.asChar = dval;
	m_curVal.asChar = dval;
}

Variable::Variable(std::string name, std::string dval)
	: m_name(name), m_type(Variable::String)
{
	m_defVal.asString = allocStringCopy(dval);
	m_curVal.asString = allocStringCopy(dval);
}

Variable::~Variable()
{
	if (m_type == Variable::String)
	{
		delete m_defVal.asString;
		delete m_curVal.asString;
	}
}

const Variable &Variable::operator=(const Variable & other)
{
	m_name = other.m_name;
	m_type = other.m_type;

	if (m_type == Variable::String)
	{
		m_defVal.asString = allocStringCopy(other.m_defVal.asString);
		m_curVal.asString = allocStringCopy(other.m_curVal.asString);
	}
	else
	{
		m_defVal.asInt = m_defVal.asInt;
		m_curVal.asInt = m_curVal.asInt;
	}

	return *this;
}

void Variable::set(bool val)
{
	switch (m_type)
	{
	case Variable::Invalid: break;
	case Variable::Bool: m_curVal.asBool = ::toBool(val); break;
	case Variable::Int: m_curVal.asInt = ::toInt(val); break;
	case Variable::Double: m_curVal.asDouble = ::toDouble(val); break;
	case Variable::Char: m_curVal.asChar = ::toChar(val); break;
	case Variable::String:
		delete m_curVal.asString;
		m_curVal.asString = ::toString(val); break;
	}
}

void Variable::set(int val)
{
	switch (m_type)
	{
	case Variable::Invalid: break;
	case Variable::Bool: m_curVal.asBool = ::toBool(val); break;
	case Variable::Int: m_curVal.asInt = ::toInt(val); break;
	case Variable::Double: m_curVal.asDouble = ::toDouble(val); break;
	case Variable::Char: m_curVal.asChar = ::toChar(val); break;
	case Variable::String:
		delete m_curVal.asString;
		m_curVal.asString = ::toString(val); break;
	}
}

void Variable::set(double val)
{
	switch (m_type)
	{
	case Variable::Invalid: break;
	case Variable::Bool: m_curVal.asBool = ::toBool(val); break;
	case Variable::Int: m_curVal.asInt = ::toInt(val); break;
	case Variable::Double: m_curVal.asDouble = ::toDouble(val); break;
	case Variable::Char: m_curVal.asChar = ::toChar(val); break;
	case Variable::String:
		delete m_curVal.asString;
		m_curVal.asString = ::toString(val); break;
	}
}

void Variable::set(char val)
{
	switch (m_type)
	{
	case Variable::Invalid: break;
	case Variable::Bool: m_curVal.asBool = ::toBool(val); break;
	case Variable::Int: m_curVal.asInt = ::toInt(val); break;
	case Variable::Double: m_curVal.asDouble = ::toDouble(val); break;
	case Variable::Char: m_curVal.asChar = ::toChar(val); break;
	case Variable::String:
		delete m_curVal.asString;
		m_curVal.asString = ::toString(val); break;
	}
}

void Variable::set(std::string _val)
{
	const char* val = _val.c_str();
	switch (m_type)
	{
	case Variable::Invalid: break;
	case Variable::Bool: m_curVal.asBool = ::toBool(val); break;
	case Variable::Int: m_curVal.asInt = ::toInt(val); break;
	case Variable::Double: m_curVal.asDouble = ::toDouble(val); break;
	case Variable::Char: m_curVal.asChar = ::toChar(val); break;
	case Variable::String:
		delete m_curVal.asString;
		m_curVal.asString = ::toString(val); break;
	}
}

void Variable::setDefault()
{
	switch (m_type)
	{
	case Variable::Invalid: break;
	case Variable::Bool: m_curVal.asBool = m_defVal.asBool; break;
	case Variable::Int: m_curVal.asInt = m_defVal.asInt; break;
	case Variable::Double: m_curVal.asDouble = m_defVal.asDouble; break;
	case Variable::Char: m_curVal.asChar = m_defVal.asChar; break;
	case Variable::String:
		delete m_curVal.asString;
		m_curVal.asString = allocStringCopy(m_defVal.asString); break;
	}
}

bool Variable::toBool()
{
	switch (m_type)
	{
	case Variable::Invalid: return false;
	case Variable::Bool: return ::toBool(m_curVal.asBool);
	case Variable::Int: return ::toBool(m_curVal.asInt);
	case Variable::Double: return ::toBool(m_curVal.asDouble);
	case Variable::Char: return ::toBool(m_curVal.asChar);
	case Variable::String: return ::toBool(m_curVal.asString);
	}

	return false;
}

int Variable::toInt()
{
	switch (m_type)
	{
	case Variable::Invalid: return 0;
	case Variable::Bool: return ::toInt(m_curVal.asBool);
	case Variable::Int: return ::toInt(m_curVal.asInt);
	case Variable::Double: return ::toInt(m_curVal.asDouble);
	case Variable::Char: return ::toInt(m_curVal.asChar);
	case Variable::String: return ::toInt(m_curVal.asString);
	}

	return 0;
}

double Variable::toDouble()
{
	switch (m_type)
	{
	case Variable::Invalid: return 0;
	case Variable::Bool: return ::toDouble(m_curVal.asBool);
	case Variable::Int: return ::toDouble(m_curVal.asInt);
	case Variable::Double: return ::toDouble(m_curVal.asDouble);
	case Variable::Char: return ::toDouble(m_curVal.asChar);
	case Variable::String: return ::toDouble(m_curVal.asString);
	}

	return 0;
}

char Variable::toChar()
{
	switch (m_type)
	{
	case Variable::Invalid: return 0;
	case Variable::Bool: return ::toBool(m_curVal.asBool);
	case Variable::Int: return ::toBool(m_curVal.asInt);
	case Variable::Double: return ::toBool(m_curVal.asDouble);
	case Variable::Char: return ::toBool(m_curVal.asChar);
	case Variable::String: return ::toBool(m_curVal.asString);
	}

	return 0;
}

std::string Variable::toString()
{
	char *c_ret = nullptr;
	switch (m_type)
	{
	case Variable::Invalid: std::string();
	case Variable::Bool: c_ret = ::toString(m_curVal.asBool); break;
	case Variable::Int: c_ret = ::toString(m_curVal.asInt); break;
	case Variable::Double: c_ret = ::toString(m_curVal.asDouble); break;
	case Variable::Char: c_ret = ::toString(m_curVal.asChar); break;
	case Variable::String: c_ret = ::toString(m_curVal.asString); break;
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
