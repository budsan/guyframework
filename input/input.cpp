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

Input* Input::myInstance = NULL;

Input::Input()
{
	myDoExit = false;
	mySettings = Settings::pInstance();
	myGlobalTime = 0;
	state.clear();
}

Input::~Input()
{
	myInstance = NULL;
}

Input& Input::Instance()
{
	return *pInstance();
}

Input* Input::pInstance()
{
	if(myInstance == NULL)
		myInstance = new Input();

	return myInstance;
}

void Input::Update()
{
	for (unsigned int j = 0; j < state.size(); j++)
	{
		state[j].ResetEvents();
	}

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		  case SDL_KEYDOWN:
			for (int i = 0; i < state.size(); i++)
			{
				const std::vector<Keybinds::Keybind> &keybinds
					= mySettings->getKeybinds()[i];

				for (unsigned int j = 0; j < keybinds.size(); j++)
				{
					if( event.key.keysym.sym == keybinds[j].getKey())
					{
						state[j].myKeyDown [i] = true;
						state[j].myKeyState[i] = true;
					}
				}
			}

			// ALT-F4 exit method :D
			if ( event.key.keysym.sym == SDLK_F4 &&
				 event.key.keysym.mod & KMOD_ALT)  {
				myDoExit = true;
			}
			break;
		  case SDL_KEYUP:
			for (int i = 0; i < state.size(); i++)
			{
				const std::vector<Keybinds::Keybind> &keybinds
					= mySettings->getKeybinds()[i];

				for (unsigned int j = 0; j < keybinds.size(); j++)
				{
					if( event.key.keysym.sym == keybinds[j].getKey())
					{
						state[j].myKeyUp   [i] = true;
						state[j].myKeyState[i] = false;
					}
				}
			}
			break;
		  case SDL_QUIT:
			myDoExit = true;
			break;
		  default:
			break;
		}
   	}

	myGlobalTime = ((float)SDL_GetTicks())/float(TICKS_PER_SECOND);
	
}

float Input::getGlobalTime()    { return myGlobalTime;}
float Input::getGlobalTimeRaw() { return ((float)SDL_GetTicks())/float(TICKS_PER_SECOND);}
bool  Input::Exit()             { return myDoExit; }

void Input::getKeyFromSettings()
{
	const Keybinds &kb = mySettings->getKeybinds();
	state.clear();
	state.resize(kb.getPlayerCount());
}

