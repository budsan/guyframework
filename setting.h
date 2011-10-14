#ifndef SETTING_H
#define SETTING_H

#include <string>

class Setting
{
public:
	Setting(std::string myName, bool dval);
	Setting(std::string myName, int dval);
	Setting(std::string myName, double dval);
	Setting(std::string myName, char dval);
	Setting(std::string myName, std::string dval);

	~Setting();

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
private:

	enum Type
	{
		Bool = 0,
		Int,
		Double,
		Char,
		String
	};

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

#endif // SETTING_H
