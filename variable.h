#pragma once

#include <string>
#include <iostream>

namespace Guy {

class Variable
{
public:
	enum Type
	{
		Null = 0,
		Bool,
		Int,
		Double,
		Char,
		String
	};

	Variable();
	Variable(const Variable & other);
	Variable(bool dval);
	Variable(int dval);
	Variable(double dval);
	Variable(char dval);
	Variable(std::string dval);

	~Variable();

	const Variable &operator=(const Variable & other);

	void set(bool val);
	void set(int val);
	void set(double val);
	void set(char val);
	void set(std::string val);

	bool        toBool() const;
	int         toInt() const;
	double      toDouble() const;
	char        toChar() const;
	std::string toString() const;

	Type type() const {return m_type;}
	bool isNull() const { return m_type == Variable::Null; }

private:

	union Value
	{
		bool   asBool;
		int    asInt;
		double asDouble;
		char   asChar;
		char*  asString;
	};

	Type m_type;
	Value m_curVal;
};

std::ostream &operator <<(std::ostream &out, const Variable &var);
std::istream &operator >>(std::istream &in, Variable &var);

} //namespace Guy
