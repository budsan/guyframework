#pragma once
#include "persistencelayer.h"

#include <string>

class LinuxPersistenceLayer : public PersistenceLayer
{
public:
	virtual void persist();

	//create vars and sets value if it doesn't exists
	void add(const Variable &set, bool overwrite = false);

	//never create new vars, just sets value
	void set(const Variable &set);

	//get variable, returns void variable if not exists
	Variable *get(std::string name);

	void setKeybinds(const Keybinds &keys);
	const Keybinds &getKeybinds() const;

	bool load(const char *filename);
	bool save();
private:
	std::map<std::string, Variable> m_vars;
	Keybinds m_keybinds;

	std::string m_filepath;
};
