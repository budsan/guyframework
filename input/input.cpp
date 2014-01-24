#include "input.h"
#include "environment.h"

#include "debug.h"
#include <algorithm>

namespace Guy {

void Input::addFocusListener(FocusListener *listener)
{
	GUY_ASSERT(std::find(m_focusListeners.begin(),m_focusListeners.end(),listener) == m_focusListeners.end());
	m_focusListeners.insert(listener);
}

void Input::removeFocusListener(FocusListener *listener)
{
	GUY_ASSERT(find(m_focusListeners.begin(),m_focusListeners.end(),listener) != m_focusListeners.end());
	m_focusListeners.erase(listener);
}

void Input::addGamePadPlugListener(GamePadPlugListener *listener)
{
	GUY_ASSERT(std::find(m_gamepadListeners.begin(),m_gamepadListeners.end(),listener) == m_gamepadListeners.end());
	m_gamepadListeners.insert(listener);
}

void Input::removeGamePadPlugListener(GamePadPlugListener *listener)
{
	GUY_ASSERT(std::find(m_gamepadListeners.begin(),m_gamepadListeners.end(),listener) == m_gamepadListeners.end());
	m_gamepadListeners.insert(listener);
}

void Input::fireGamePadAdded(std::size_t id)
{
	std::set<GamePadPlugListener*>::iterator it = m_gamepadListeners.begin();
	for (;it != m_gamepadListeners.end(); ++it) {
		(*it)->onGamePadAdded(id);
	}
}

void Input::fireGamePadRemoved(std::size_t  id)
{
	std::set<GamePadPlugListener*>::iterator it = m_gamepadListeners.begin();
	for (;it != m_gamepadListeners.end(); ++it) {
		(*it)->onGamePadRemoved(id);
	}
}

} // namespace Guy

