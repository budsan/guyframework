#pragma once

#include <map>
#include <string>

#include "variable.h"

class PersistenceLayer
{
public:
	virtual void persist() = 0;

	//create vars and sets value if it doesn't exists
	virtual void add(const Variable &var, bool overwrite = false, bool persist = false);

	//never create new vars, just sets value
	virtual void set(const Variable &var, bool persist = false);

	//get variable, returns void variable if not exists
	virtual Variable *get(const std::string& name);

private:
	std::map<std::string, Variable> m_vars;
};
