#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>
#include <iostream>

class Variable
{
public:
	enum Type
	{
		Invalid = 0,
		Bool,
		Int,
		Double,
		Char,
		String
	};

	Variable();
	Variable(const Variable & other);
	Variable(std::string _name, bool dval);
	Variable(std::string _name, int dval);
	Variable(std::string _name, double dval);
	Variable(std::string _name, char dval);
	Variable(std::string _name, std::string dval);

	~Variable();

	const Variable &operator=(const Variable & other);

	void set(bool val);
	void set(int val);
	void set(double val);
	void set(char val);
	void set(std::string val);
	void setDefault();

	bool        toBool() const;
	int         toInt() const;
	double      toDouble() const;
	char        toChar() const;
	std::string toString() const;

	const std::string &name() const {return m_name;}
	Type type() const {return m_type;}

private:

	union Value
	{
		bool   asBool;
		int    asInt;
		double asDouble;
		char   asChar;
		char*  asString;
	};

	std::string m_name;
	Type m_type;
	Value m_defVal;
	Value m_curVal;
};

std::ostream &operator <<(std::ostream &out, const Variable &var);
std::istream &operator >>(std::istream &in, Variable &var);

#endif // VARIABLE_H
