#include "input.h"
#include "environment.h"

#include <cassert>
#include <algorithm>

namespace Guy {

void Input::addFocusListener(FocusListener *listener)
{
	assert(std::find(m_focusListeners.begin(),m_focusListeners.end(),listener) == m_focusListeners.end());
	m_focusListeners.insert(listener);
}

void Input::removeFocusListener(FocusListener *listener)
{
	assert(find(m_focusListeners.begin(),m_focusListeners.end(),listener) != m_focusListeners.end());
	m_focusListeners.erase(listener);
}

void Input::addGamePadPlugListener(GamePadPlugListener *listener)
{
	assert(std::find(m_gamepadListeners.begin(),m_gamepadListeners.end(),listener) == m_gamepadListeners.end());
	m_gamepadListeners.insert(listener);
}

void Input::removeGamePadPlugListener(GamePadPlugListener *listener)
{
	assert(std::find(m_gamepadListeners.begin(),m_gamepadListeners.end(),listener) == m_gamepadListeners.end());
	m_gamepadListeners.insert(listener);
}

void Input::fireGamePadAdded(int id)
{
	std::set<GamePadPlugListener*>::iterator it = m_gamepadListeners.begin();
	for (;it != m_gamepadListeners.end(); ++it) {
		(*it)->onGamePadAdded(id);
	}
}

void Input::fireGamePadRemoved(int id)
{
	std::set<GamePadPlugListener*>::iterator it = m_gamepadListeners.begin();
	for (;it != m_gamepadListeners.end(); ++it) {
		(*it)->onGamePadRemoved(id);
	}
}

} // namespace Guy

