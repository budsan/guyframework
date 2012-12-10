#pragma once
#include "persistencelayer.h"

#include <string>

namespace Guy {

class QNXPersistenceLayer : public PersistenceLayer
{
public:
	virtual ~QNXPersistenceLayer();

	virtual void persist();

	bool load(const char *filename);
	bool save();
private:
	std::map<std::string, Variable> m_vars;
	std::string m_filepath;
};

} // namespace Guy
