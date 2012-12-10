#include "qnxpersistencelayer.h"

#include <fstream>

namespace Guy {

QNXPersistenceLayer::~QNXPersistenceLayer()
{

}

void QNXPersistenceLayer::persist()
{
	this->save();
}

bool QNXPersistenceLayer::load(const char *filename)
{
	std::fstream file;
	file.open(filename, std::fstream::in | std::fstream::binary);

	if (!file.is_open()) return false;

	Variable var;
	while(file >> var)
	{
		this->add(var);
	}

	m_filepath = std::string(filename);
	return true;
}

bool QNXPersistenceLayer::save()
{
	if (m_filepath.empty()) return false;

	std::fstream file;
	file.open(m_filepath.c_str(), std::fstream::out | std::fstream::binary | std::fstream::trunc);

	std::map<std::string, Variable>::iterator it = m_vars.begin();
	for(;it != m_vars.end(); ++it)
	{
		file << it->second;
	}

	return true;
}

} // namespace Guy
