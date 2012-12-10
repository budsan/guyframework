#pragma once
#include "persistencelayer.h"

#include <string>

namespace Guy {

class LinuxPersistenceLayer : public PersistenceLayer
{
public:
	virtual ~LinuxPersistenceLayer();

	virtual void persist();

	bool load(const char *filename);
	bool save();
private:
	std::map<std::string, Variable> m_vars;
	std::string m_filepath;
};

} // namespace Guy
