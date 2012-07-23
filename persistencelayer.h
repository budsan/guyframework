#pragma once

#include <map>
#include <string>

#include "variable.h"

namespace Guy {

class PersistenceLayer
{
public:
	virtual void persist() = 0;

	//create vars and sets value if it doesn't exists
	virtual bool add(const std::string& name, const Variable &var);
	virtual bool replace(const std::string& name, const Variable &var);

	//never create new vars, just sets value
	virtual bool set(const std::string& name, const Variable &var);

	//get variable, returns void variable if not exists
	virtual const Variable &get(const std::string& name);

private:
	std::map<std::string, Variable> m_vars;
};

} //namespace Guy
