#pragma once
#include "persistencelayer.h"

#include <string>

class LinuxPersistenceLayer : public PersistenceLayer
{
public:
	virtual void persist();

	bool load(const char *filename);
	bool save();
private:
	std::map<std::string, Variable> m_vars;
	std::string m_filepath;
};
