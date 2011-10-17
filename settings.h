#pragma once

#include <map>
#include <string>

#include "variable.h"

enum { K_UP, K_DOWN, K_WORKER, K_UNIT1, K_UNIT2, K_UNIT3, K_UNIT4, K_SIZE };
enum { NUMPLAYERS = 2 };

class Settings
{
public:
	~Settings();
	static Settings* pInstance();

	bool bLoad(const char *filename);
	bool bSave(const char *filename);

	void add(const Variable &set);
	Variable *get(std::string name);

	//Video Configurations
	//unsigned int  ScreenWidth;
	//unsigned int  ScreenHeight;
	//unsigned int  ScreenBpp;
	//bool Fullscreen;

	//Input Configs
	unsigned short Keyboard[NUMPLAYERS][K_SIZE];

private:
	std::map<std::string, Variable> settings;

	Settings();
	static Settings *m_pInstance;
};
