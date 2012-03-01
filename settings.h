#pragma once

#include <map>
#include <string>
#include <vector>

#include "variable.h"

class Keybinds
{
public:
	class Keybind
	{
	public:
		Keybind();

		void setDefault(unsigned short key);
		unsigned short getDefault() const;

		void setKey(unsigned short key);
		unsigned short getKey() const;

		const Keybind &operator=(const Keybind& other);
	private:

		unsigned short m_def;
		unsigned short m_key;
	};

	Keybinds();
	Keybinds(unsigned int players, unsigned int actions);
	Keybinds(const Keybinds& other);

	      std::vector<Keybind> &operator[](unsigned int player);
	const std::vector<Keybind> &operator[](unsigned int player) const;
	const Keybinds &operator=(const Keybinds& other);

	unsigned int getPlayerCount() const;
	unsigned int getAccionCount() const;
private:
	std::vector< std::vector <Keybind> > m_playerKeybinds;
};

class Settings
{
public:
	~Settings();
	static Settings* ptrInstance();

	bool load(const char *filename);
	bool Save(const char *filename);

	void add(const Variable &set);
	Variable *get(std::string name);

	void setKeybinds(const Keybinds &keys);
	const Keybinds &getKeybinds() const;
private:
	std::map<std::string, Variable> m_settings;
	Keybinds m_keybinds;

	Settings();
	static Settings *m_instance;
};
