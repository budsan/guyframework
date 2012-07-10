#pragma once

#include <vector>

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

class Keybinds
{
public:
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
