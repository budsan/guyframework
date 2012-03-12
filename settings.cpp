#include "settings.h"
#include "log.h"

#ifdef _WINDOWS
#include <windows.h>
#include <SDL/SDL.h>
#else
#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <fstream>

#include <iostream>

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

std::vector<Keybinds::Keybind> &Keybinds::operator[](unsigned int player)
{
	return m_playerKeybinds.at(player);
}

const std::vector<Keybinds::Keybind> &Keybinds::operator[](unsigned int player) const
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
	return m_playerKeybinds.size();
}

unsigned int Keybinds::getAccionCount() const
{
	if (m_playerKeybinds.empty()) return 0;
	return m_playerKeybinds[0].size();
}

Keybinds::Keybind::Keybind() : m_def(0), m_key(0) {}
void Keybinds::Keybind::setDefault(unsigned short k)
{
	m_def = k; if (m_key == 0) m_key = k;
}

unsigned short Keybinds::Keybind::getDefault() const
{
	return m_def;
}

void Keybinds::Keybind::setKey(unsigned short k)
{
	m_key = k;
}

unsigned short Keybinds::Keybind::getKey() const
{
	return m_key;
}

const Keybinds::Keybind &Keybinds::Keybind::operator=(const Keybinds::Keybind& other)
{
	m_key = other.m_key; m_def = other.m_def;
	return *this;
}

//-SETTINGS------------------------------------------------------------------//

Settings* Settings::m_instance = NULL;

Settings::Settings()
{
	//Video Configurations
	add(Variable("ScreenWidth",(int) 0));
	add(Variable("ScreenHeight",(int) 0));
	add(Variable("ScreenBpp",(int) 32));
	add(Variable("Fullscreen",(bool) true));

/*
	Keyboard[0][K_UP    ] = SDLK_q;
	Keyboard[0][K_DOWN  ] = SDLK_a;
	Keyboard[0][K_WORKER] = SDLK_r;
	Keyboard[0][K_UNIT1 ] = SDLK_w;
	Keyboard[0][K_UNIT2 ] = SDLK_e;
	Keyboard[0][K_UNIT3 ] = SDLK_s;
	Keyboard[0][K_UNIT4 ] = SDLK_d;

	Keyboard[1][K_UP    ] = SDLK_u;
	Keyboard[1][K_DOWN  ] = SDLK_j;
	Keyboard[1][K_WORKER] = SDLK_p;
	Keyboard[1][K_UNIT1 ] = SDLK_i;
	Keyboard[1][K_UNIT2 ] = SDLK_o;
	Keyboard[1][K_UNIT3 ] = SDLK_k;
	Keyboard[1][K_UNIT4 ] = SDLK_l;
	*/
}

Settings::~Settings() { m_instance = NULL; }

void Settings::add(const Variable &set)
{
	if (set.type() == Variable::Invalid) return;
	std::map<std::string, Variable>::iterator it = m_settings.find(set.name());
	if (it != m_settings.end())
	{
		LOG << "Setting " << set.name() << " already exists.";
		return;
	}

	m_settings[set.name()] = set;
}

Variable *Settings::get(std::string name)
{
	std::map<std::string, Variable>::iterator it = m_settings.find(name);
	if (it == m_settings.end()) return NULL;
	Variable *v = &(it->second);
	return v;
}

void Settings::setKeybinds(const Keybinds &keys)
{
	m_keybinds = keys;
}

const Keybinds &Settings::getKeybinds() const
{
	return m_keybinds;
}

Settings* Settings::pInstance()
{
	if(m_instance == NULL){
		m_instance = new Settings();
	}
	return m_instance;
}

#ifdef UNUSED
bool Settings::load(const char *filename)
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

bool Settings::Save(const char *filename)
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


bool Settings::load(const char *filename)
{
	std::fstream fSettingsFile;
	fSettingsFile.open(filename, std::fstream::in | std::fstream::binary);
	/*
	if (fSettingsFile)
	{
		fSettingsFile.read((char*)m_pInstance, sizeof(CSettings));
		fSettingsFile.close();
	}
	else return false;
*/
	return true;
}

bool Settings::Save(const char *filename)
{
	std::fstream fSettingsFile;
	fSettingsFile.open(filename, std::fstream::out | std::fstream::binary | std::fstream::trunc);
	/*
	if (fSettingsFile)
	{
		fSettingsFile.write((char*)m_pInstance, sizeof(CSettings));
		fSettingsFile.close();
	}
	else return false;
	*/
	return true;
}

