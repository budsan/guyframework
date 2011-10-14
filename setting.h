#ifndef SETTING_H
#define SETTING_H

#include <string>

class Setting
{
public:
	Setting(std::string name, bool dval);
	Setting(std::string name, int dval);
	Setting(std::string name, double dval);
	Setting(std::string name, char dval);
	Setting(std::string name, std::string dval);

	void set(bool val);
	void set(int val);
	void set(double val);
	void set(char val);
	void set(std::string val);

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
		bool   bool_value;
		int    int_value;
		double double_value;
		char   char_value;
		char*  string_value;
	};

	std::string name;
	Type type;
	Value default_value;
	Value current_value;
};

#endif // SETTING_H
