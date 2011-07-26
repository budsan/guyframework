#include "settings.h"

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

Settings* Settings::m_pInstance = NULL;

Settings::Settings()
{
	//Default Settin-gs
	ScreenWidth  = 800;
	ScreenHeight = 600;
	ScreenBpp    = 32;
	Fullscreen   = false;
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
}

Settings::~Settings() { m_pInstance = NULL; }
Settings* Settings::pInstance()
{
	if(m_pInstance == NULL){
		m_pInstance = new Settings();
	}
	return m_pInstance;
}
bool Settings::bLoad(const char *filename)
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

bool Settings::bSave(const char *filename)
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

/*
bool CSettings::bLoad(char *filename)
{
	std::fstream fSettingsFile;
	fSettingsFile.open(filename, std::fstream::in | std::fstream::binary);
	if (fSettingsFile)
	{
		fSettingsFile.read((char*)m_pInstance, sizeof(CSettings));
		fSettingsFile.close();
	}
	else return false;

	return true;
}

bool CSettings::bSave(char *filename)
{
	std::fstream fSettingsFile;
	fSettingsFile.open(filename, std::fstream::out | std::fstream::binary | std::fstream::trunc);
	if (fSettingsFile)
	{
		fSettingsFile.write((char*)m_pInstance, sizeof(CSettings));
		fSettingsFile.close();
	}
	else return false;
	
	return true;
}
*/
