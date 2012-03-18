#include <SDL/SDL.h>

/*
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
*/

#include "input.h"
#include "settings.h"

//---------------------------------------------------------------------------//

Input* Input::m_instance = NULL;

Input::Input()
{
	m_doExit = false;
	m_settings = Settings::pInstance();
	m_time = 0;
	m_state.clear();
}

Input::~Input()
{
	m_instance = NULL;
}

Input& Input::Instance()
{
	return *ptrInstance();
}

Input* Input::ptrInstance()
{
	if(m_instance == NULL)
		m_instance = new Input();

	return m_instance;
}

void Input::update()
{
	for (unsigned int j = 0; j < m_state.size(); j++)
	{
		m_state[j].resetEvents();
	}

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		  case SDL_KEYDOWN:
			for (unsigned int i = 0; i < m_state.size(); i++)
			{
				const std::vector<Keybinds::Keybind> &keybinds
					= m_settings->getKeybinds()[i];

				for (unsigned int j = 0; j < keybinds.size(); j++)
				{
					if( event.key.keysym.sym == keybinds[j].getKey())
					{
						m_state[i].m_keyDown [j] = true;
						m_state[i].m_keyState[j] = true;
					}
				}
			}

			// ALT-F4 exit method :D
			if ( event.key.keysym.sym == SDLK_F4 &&
				 event.key.keysym.mod & KMOD_ALT)  {
				m_doExit = true;
			}
			break;
		  case SDL_KEYUP:
			for (unsigned int i = 0; i < m_state.size(); i++)
			{
				const std::vector<Keybinds::Keybind> &keybinds
					= m_settings->getKeybinds()[i];

				for (unsigned int j = 0; j < keybinds.size(); j++)
				{
					if( event.key.keysym.sym == keybinds[j].getKey())
					{
						m_state[i].m_keyUp   [j] = true;
						m_state[i].m_keyState[j] = false;
					}
				}
			}
			break;
		  case SDL_QUIT:
			m_doExit = true;
			break;
		  default:
			break;
		}
   	}

	m_time = ((float)SDL_GetTicks())/float(TICKS_PER_SECOND);
	
}

float Input::getTime()    { return m_time;}
float Input::getTimeRaw() { return ((float)SDL_GetTicks())/float(TICKS_PER_SECOND);}
bool  Input::exit()             { return m_doExit; }

void Input::getKeyFromSettings()
{
	const Keybinds &kb = m_settings->getKeybinds();
	m_state.clear();
	m_state.resize(kb.getPlayerCount());
}

