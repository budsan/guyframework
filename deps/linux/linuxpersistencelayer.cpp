#include "linuxpersistencelayer.h"

#include <fstream>

namespace Guy {

void LinuxPersistenceLayer::persist()
{
	this->save();
}

#ifdef UNUSED
bool PersistenceLayer::load(const char *filename)
{
	FILE* fSettingsFile;
	fSettingsFile = fopen(filename, "rb");
	if(fSettingsFile != NULL)
	{
		fread((void*)m_pInstance,1,sizeof(Settings),fSettingsFile);
		fclose(fSettingsFile);
	}
	else return false;

	return true;
}

bool PersistenceLayer::Save(const char *filename)
{
	FILE* fSettingsFile;
	fSettingsFile = fopen(filename, "wb");
	if(fSettingsFile != NULL)
	{
		fwrite((void*)m_pInstance,1,sizeof(Settings),fSettingsFile);
		fclose(fSettingsFile);
	}
	else return false;

	return true;
}
#endif


bool LinuxPersistenceLayer::load(const char *filename)
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

bool LinuxPersistenceLayer::save()
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
