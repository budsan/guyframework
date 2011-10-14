#include "setting.h"

Setting::Setting(std::string name, bool dval)
	: name(name), type(Setting::Bool)
{
	default_value.bool_value = dval;
	current_value.bool_value = dval;
}

Setting::Setting(std::string name, int dval)
	: name(name), type(Setting::Int)
{
	default_value.int_value = dval;
	current_value.int_value = dval;
}

Setting::Setting(std::string name, double dval)
	: name(name), type(Setting::Double)
{
	default_value.double_value = dval;
	current_value.double_value = dval;
}

Setting::Setting(std::string name, char dval)
	: name(name), type(Setting::Char)
{
	default_value.char_value = dval;
	current_value.char_value = dval;
}

Setting::Setting(std::string name, std::string dval)
	: name(name), type(Setting::String)
{

}

void Setting::set(bool val)
{

}

void Setting::set(int val)
{

}

void Setting::set(double val)
{

}

void Setting::set(char val)
{

}

void Setting::set(std::string val)
{

}

bool Setting::toBool()
{

}

int Setting::toInt()
{

}

double Setting::toDouble()
{

}

char Setting::toChar()
{

}

std::string Setting::toString()
{

}
