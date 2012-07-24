#include "linuxpersistencelayer.h"

#include <fstream>

namespace Guy {

LinuxPersistenceLayer::~LinuxPersistenceLayer()
{

}

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

	do
	{
		std::string name;
		char c[2] = { '\0', '\0'};
		while(file.read(c, sizeof(char)))
		{
			if( c != '\0') name.append(c);
			else break;
		}

		Variable var;
		if (file >> var)
		{
			this->add(name, var);
		}
	}
	while(file);

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
		const std::string &s = it->first;
		file.write(s.c_str(),s.length()+1);
		file << it->second;
	}

	return true;
}

} // namespace Guy
