#include "persistencelayer.h"
#include "log.h"

namespace Guy {

static Variable guard;

bool PersistenceLayer::add(const std::string& name, const Variable &var)
{
	if (var.type() == Variable::Null)
		return false;

	std::map<std::string, Variable>::iterator it = m_vars.find(name);
	if (it != m_vars.end())
	{
		printLog("Var %s already exists.\n", name.c_str());
		return false;
	}
	else
	{
		m_vars[name] = var;
		return true;
	}
}

bool PersistenceLayer::replace(const std::string& name, const Variable &var)
{
	if (var.type() == Variable::Null)
		return false;

	std::map<std::string, Variable>::iterator it = m_vars.find(name);
	if (it != m_vars.end())
	{
		it->second = var;
		return true;
	}
	else
	{
		m_vars[name] = var;
		return false;
	}
}

bool PersistenceLayer::set(const std::string& name, const Variable &var)
{
	if (var.type() == Variable::Null)
		return false;

	std::map<std::string, Variable>::iterator it = m_vars.find(name);
	if (it != m_vars.end())
	{
		it->second = var;
		return true;
	}
	else {
		printLog("Var %s doesn't exist.\n", name.c_str());
		return false;
	}
}

const Variable &PersistenceLayer::get(const std::string &name)
{
	std::map<std::string, Variable>::iterator it = m_vars.find(name);
	if (it == m_vars.end()) return guard;
	return it->second;
}

} // namespace Guy
