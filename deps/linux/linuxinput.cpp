#include <SDL/SDL.h>

#include "log.h"
#include "deps/linux/linuxinput.h"

//---------------------------------------------------------------------------//

LinuxInput::LinuxInput() :
    m_doExit(false),
    m_focusState(0)
{
	m_state.clear();
}

bool LinuxInput::init()
{
	return true;
}

void LinuxInput::update()
{
	for (unsigned int j = 0; j < m_state.size(); j++) {
		m_state[j].resetKeys();
	}

	pollEvents();
}

void LinuxInput::pollEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) handleEvent(event);
}

void LinuxInput::waitEvent()
{
	SDL_Event event;
	if (SDL_WaitEvent(&event)) handleEvent(event);
	dbgPrintLog("Error waiting event\n");
}

void LinuxInput::handleEvent(const SDL_Event &event)
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
	case SDL_ACTIVEEVENT:
	{
		if (event.active.gain) {
			if (event.active.state & SDL_APPMOUSEFOCUS) {
				m_focusState |= MOUSE;
				std::set<FocusListener*>::iterator it = m_FocusListeners.begin();
				for (;it != m_FocusListeners.end(); ++it) {
					(*it)->onGainMouseFocus();
				}
			}
			if (event.active.state & SDL_APPINPUTFOCUS) {
				m_focusState |= INPUT;
				std::set<FocusListener*>::iterator it = m_FocusListeners.begin();
				for (;it != m_FocusListeners.end(); ++it) {
					(*it)->onGainInputFocus();
				}
			}
			if (event.active.state & SDL_APPACTIVE) {
				m_focusState |= DRAW;
				std::set<FocusListener*>::iterator it = m_FocusListeners.begin();
				for (;it != m_FocusListeners.end(); ++it) {
					(*it)->onGainDrawFocus();
				}
			}
		}
		else {
			if (event.active.state & SDL_APPMOUSEFOCUS) {
				m_focusState &= ~MOUSE;
				std::set<FocusListener*>::iterator it = m_FocusListeners.begin();
				for (;it != m_FocusListeners.end(); ++it) {
					(*it)->onLoseMouseFocus();
				}
			}
			if (event.active.state & SDL_APPINPUTFOCUS) {
				m_focusState &= ~INPUT;
				std::set<FocusListener*>::iterator it = m_FocusListeners.begin();
				for (;it != m_FocusListeners.end(); ++it) {
					(*it)->onLoseInputFocus();
				}
			}
			if (event.active.state & SDL_APPACTIVE) {
				m_focusState &= ~DRAW;
				std::set<FocusListener*>::iterator it = m_FocusListeners.begin();
				for (;it != m_FocusListeners.end(); ++it) {
					(*it)->onLoseDrawFocus();
				}
			}
		}

		break;
	}
	default:
		break;
	}
}

