#include "keybind.h"

Keybind::Keybind() : m_def(0), m_key(0) {}
void Keybind::setDefault(unsigned short k)
{
	m_def = k; if (m_key == 0) m_key = k;
}

unsigned short Keybind::getDefault() const
{
	return m_def;
}

void Keybind::setKey(unsigned short k)
{
	m_key = k;
}

unsigned short Keybind::getKey() const
{
	return m_key;
}

const Keybind &Keybind::operator=(const Keybind& other)
{
	m_key = other.m_key; m_def = other.m_def;
	return *this;
}

Keybinds::Keybinds() {}

Keybinds::Keybinds(unsigned int players, unsigned int actions)
{
	m_playerKeybinds.clear();
	m_playerKeybinds.resize(players);
	for (unsigned int i = 0 ; i < players; i ++)
	{
		std::vector<Keybind> &keybinds = m_playerKeybinds.at(i);
		keybinds.clear();
		keybinds.resize(actions);
	}
}

Keybinds::Keybinds(const Keybinds& other)
{
	this->operator =(other);
}

std::vector<Keybind> &Keybinds::operator[](unsigned int player)
{
	return m_playerKeybinds.at(player);
}

const std::vector<Keybind> &Keybinds::operator[](unsigned int player) const
{
	return m_playerKeybinds.at(player);
}

const Keybinds &Keybinds::operator=(const Keybinds& other)
{
	m_playerKeybinds = other.m_playerKeybinds;
	return *this;
}

unsigned int Keybinds::getPlayerCount() const
{
	return (unsigned int) m_playerKeybinds.size();
}

unsigned int Keybinds::getAccionCount() const
{
	if (m_playerKeybinds.empty()) return 0;
	return (unsigned int) m_playerKeybinds[0].size();
}
