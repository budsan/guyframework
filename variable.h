#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>

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
	Variable(std::string myName, bool dval);
	Variable(std::string myName, int dval);
	Variable(std::string myName, double dval);
	Variable(std::string myName, char dval);
	Variable(std::string myName, std::string dval);

	~Variable();

	const Variable &operator=(const Variable & other);

	void set(bool val);
	void set(int val);
	void set(double val);
	void set(char val);
	void set(std::string val);
	void setDefault();

	bool        toBool();
	int         toInt();
	double      toDouble();
	char        toChar();
	std::string toString();

	const std::string &name() const {return myName;}
	Type type() const {return myType;}

private:

	union Value
	{
		bool   asBool;
		int    asInt;
		double asDouble;
		char   asChar;
		char*  asString;
	};

	std::string myName;
	Type myType;
	Value myDefVal;
	Value myCurVal;
};

#endif // VARIABLE_H
