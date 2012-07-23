#include "variable.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

namespace Guy {

//Buffer for number conversions to ascii
char tmp[128];

char *allocStringCopy(const char* str, unsigned int len)
{
	char *ret = new char[len];
	memcpy(ret, str, len);
	return ret;
}

char *allocStringCopy(const std::string &str)
{
	unsigned int len = (unsigned int) str.length()+1;
	return allocStringCopy(str.c_str(), len);
}

char *allocStringCopy(const char* str)
{

	unsigned int len = (unsigned int) strlen(str)+1;
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
	: m_type(Variable::Null)
{
	m_curVal.asInt = 0;
}

Variable::Variable(const Variable & other)
	: m_type(other.m_type)
{
	if (m_type == Variable::String)
	{
		m_curVal.asString = allocStringCopy(other.m_curVal.asString);
	}
	else
	{
		m_curVal.asInt = other.m_curVal.asInt;
	}
}

Variable::Variable(bool dval)
	: m_type(Variable::Bool)
{
	m_curVal.asBool = dval;
}

Variable::Variable(int dval)
	: m_type(Variable::Int)
{
	m_curVal.asInt = dval;
}

Variable::Variable(double dval)
	: m_type(Variable::Double)
{
	m_curVal.asDouble = dval;
}

Variable::Variable(char dval)
	: m_type(Variable::Char)
{
	m_curVal.asChar = dval;
}

Variable::Variable(std::string dval)
	: m_type(Variable::String)
{
	m_curVal.asString = allocStringCopy(dval);
}

Variable::~Variable()
{
	if (m_type == Variable::String)
	{
		delete m_curVal.asString;
	}
}

const Variable &Variable::operator=(const Variable & other)
{
	m_type = other.m_type;
	if (m_type == Variable::String)
		m_curVal.asString = allocStringCopy(other.m_curVal.asString);

	else
		m_curVal.asInt = other.m_curVal.asInt;

	return *this;
}

void Variable::set(bool val)
{
	switch (m_type)
	{
	case Variable::Null: break;
	case Variable::Bool: m_curVal.asBool = Guy::toBool(val); break;
	case Variable::Int: m_curVal.asInt = Guy::toInt(val); break;
	case Variable::Double: m_curVal.asDouble = Guy::toDouble(val); break;
	case Variable::Char: m_curVal.asChar = Guy::toChar(val); break;
	case Variable::String:
		delete m_curVal.asString;
		m_curVal.asString = Guy::toString(val); break;
	}
}

void Variable::set(int val)
{
	switch (m_type)
	{
	case Variable::Null: break;
	case Variable::Bool: m_curVal.asBool = Guy::toBool(val); break;
	case Variable::Int: m_curVal.asInt = Guy::toInt(val); break;
	case Variable::Double: m_curVal.asDouble = Guy::toDouble(val); break;
	case Variable::Char: m_curVal.asChar = Guy::toChar(val); break;
	case Variable::String:
		delete m_curVal.asString;
		m_curVal.asString = Guy::toString(val); break;
	}
}

void Variable::set(double val)
{
	switch (m_type)
	{
	case Variable::Null: break;
	case Variable::Bool: m_curVal.asBool = Guy::toBool(val); break;
	case Variable::Int: m_curVal.asInt = Guy::toInt(val); break;
	case Variable::Double: m_curVal.asDouble = Guy::toDouble(val); break;
	case Variable::Char: m_curVal.asChar = Guy::toChar(val); break;
	case Variable::String:
		delete m_curVal.asString;
		m_curVal.asString = Guy::toString(val); break;
	}
}

void Variable::set(char val)
{
	switch (m_type)
	{
	case Variable::Null: break;
	case Variable::Bool: m_curVal.asBool = Guy::toBool(val); break;
	case Variable::Int: m_curVal.asInt = Guy::toInt(val); break;
	case Variable::Double: m_curVal.asDouble = Guy::toDouble(val); break;
	case Variable::Char: m_curVal.asChar = Guy::toChar(val); break;
	case Variable::String:
		delete m_curVal.asString;
		m_curVal.asString = Guy::toString(val); break;
	}
}

void Variable::set(std::string _val)
{
	const char* val = _val.c_str();
	switch (m_type)
	{
	case Variable::Null: break;
	case Variable::Bool: m_curVal.asBool = Guy::toBool(val); break;
	case Variable::Int: m_curVal.asInt = Guy::toInt(val); break;
	case Variable::Double: m_curVal.asDouble = Guy::toDouble(val); break;
	case Variable::Char: m_curVal.asChar = Guy::toChar(val); break;
	case Variable::String:
		delete m_curVal.asString;
		m_curVal.asString = Guy::toString(val); break;
	}
}

bool Variable::toBool() const
{
	switch (m_type)
	{
	case Variable::Null: return false;
	case Variable::Bool: return Guy::toBool(m_curVal.asBool);
	case Variable::Int: return Guy::toBool(m_curVal.asInt);
	case Variable::Double: return Guy::toBool(m_curVal.asDouble);
	case Variable::Char: return Guy::toBool(m_curVal.asChar);
	case Variable::String: return Guy::toBool(m_curVal.asString);
	}

	return false;
}

int Variable::toInt() const
{
	switch (m_type)
	{
	case Variable::Null: return 0;
	case Variable::Bool: return Guy::toInt(m_curVal.asBool);
	case Variable::Int: return Guy::toInt(m_curVal.asInt);
	case Variable::Double: return Guy::toInt(m_curVal.asDouble);
	case Variable::Char: return Guy::toInt(m_curVal.asChar);
	case Variable::String: return Guy::toInt(m_curVal.asString);
	}

	return 0;
}

double Variable::toDouble() const
{
	switch (m_type)
	{
	case Variable::Null: return 0;
	case Variable::Bool: return Guy::toDouble(m_curVal.asBool);
	case Variable::Int: return Guy::toDouble(m_curVal.asInt);
	case Variable::Double: return Guy::toDouble(m_curVal.asDouble);
	case Variable::Char: return Guy::toDouble(m_curVal.asChar);
	case Variable::String: return Guy::toDouble(m_curVal.asString);
	}

	return 0;
}

char Variable::toChar() const
{
	switch (m_type)
	{
	case Variable::Null: return 0;
	case Variable::Bool: return Guy::toBool(m_curVal.asBool);
	case Variable::Int: return Guy::toBool(m_curVal.asInt);
	case Variable::Double: return Guy::toBool(m_curVal.asDouble);
	case Variable::Char: return Guy::toBool(m_curVal.asChar);
	case Variable::String: return Guy::toBool(m_curVal.asString);
	}

	return 0;
}

std::string Variable::toString() const
{
	char *c_ret = NULL;
	switch (m_type)
	{
	case Variable::Null: std::string(); break;
	case Variable::Bool: c_ret = Guy::toString(m_curVal.asBool); break;
	case Variable::Int: c_ret = Guy::toString(m_curVal.asInt); break;
	case Variable::Double: c_ret = Guy::toString(m_curVal.asDouble); break;
	case Variable::Char: c_ret = Guy::toString(m_curVal.asChar); break;
	case Variable::String: c_ret = Guy::toString(m_curVal.asString); break;
	default: c_ret = Guy::toString(""); break;
	}

	std::string ret(c_ret); delete c_ret;
	return ret;
}

std::ostream &operator <<(std::ostream &out, const Variable &var)
{
	Variable::Type type = var.type();
	out.write((const char *)&type, sizeof(Variable::Type));
	if(var.type() == Variable::Null) return out;

	out.write(var.name().c_str(), var.name().length()+1);
	switch (var.type())
	{
	case Variable::Null: break;
	case Variable::Bool: {
		bool value = var.toBool();
		out.write((const char *)&value, sizeof(bool)); break;
	}
	case Variable::Int: {
		int value = var.toInt();
		out.write((const char *)&value, sizeof(int)); break;
	}
	case Variable::Double: {
		double value = var.toDouble();
		out.write((const char *)&value, sizeof(double)); break;
	}
	case Variable::Char: {
		char value = var.toChar();
		out.write((const char *)&value, sizeof(char)); break;
	}
	case Variable::String: {
			std::string s = var.toString();
			out.write(s.c_str(),s.length()+1);
		} break;
	}

	return out;
}

std::string variableReadString(std::istream &in)
{
	std::string result;
	char c[2] = { '\0', '\0'};
	while(in.read(c, sizeof(char))) {
		if( c != '\0') result.append(c);
		else break;
	}

	return result;
}

std::istream &operator >>(std::istream &in, Variable &var)
{
	Variable::Type type = Variable::Null;
	in.read((char *)&type, sizeof(Variable::Type));

	if(type == Variable::Null)
	{
		var = Variable();
		return in;
	}

	std::string name = variableReadString(in);
	switch (type)
	{
	case Variable::Null: break;
	case Variable::Bool: {
		bool value;
		in.read((char *)&value, sizeof(bool)); break;
		var = Variable(name, value);
	} break;
	case Variable::Int: {
		int value;
		in.read((char *)&value, sizeof(int)); break;
		var = Variable(name, value);
	} break;
	case Variable::Double: {
		double value;
		in.read((char *)&value, sizeof(double)); break;
		var = Variable(name, value);
	} break;
	case Variable::Char: {
		char value;
		in.read((char *)&value, sizeof(char)); break;
		var = Variable(name, value);
	} break;
	case Variable::String: {
			var = Variable(name, variableReadString(in));
		} break;
	}

	return in;
}

} // namespace Guy
