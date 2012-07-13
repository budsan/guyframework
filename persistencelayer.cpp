#include "persistencelayer.h"
#include "log.h"

namespace Guy {

void PersistenceLayer::add(const Variable &var, bool overwrite, bool persist)
{
	if (var.type() == Variable::Invalid) return;
	std::map<std::string, Variable>::iterator it = m_vars.find(var.name());
	if (it != m_vars.end())
	{
		if (overwrite) {
			it->second = var;
			if (persist) this->persist();
		}
		else printLog("Var %s already exists.\n", var.name().c_str());
	}
	else
	{
		m_vars[var.name()] = var;
		if (persist) this->persist();
	}
}

void PersistenceLayer::set(const Variable &var, bool persist)
{
	if (var.type() == Variable::Invalid) return;
	std::map<std::string, Variable>::iterator it = m_vars.find(var.name());
	if (it != m_vars.end())
	{
		it->second = var;
		if (persist) this->persist();
	}
	else {
		printLog("Var %s doesn't exist.\n", var.name().c_str());
	}
}

Variable *PersistenceLayer::get(const std::string &name)
{
	std::map<std::string, Variable>::iterator it = m_vars.find(name);
	if (it == m_vars.end()) return NULL;
	Variable *v = &(it->second);
	return v;
}

} // namespace Guy
