#include "input.h"
#include "environment.h"

#include <cassert>
#include <algorithm>

namespace Guy {

void Input::addFocusListener(FocusListener *listener)
{
	assert(std::find(m_FocusListeners.begin(),m_FocusListeners.end(),listener) == m_FocusListeners.end());
	m_FocusListeners.insert(listener);
}

void Input::removeFocusListener(FocusListener *listener)
{
	assert(find(m_FocusListeners.begin(),m_FocusListeners.end(),listener) != m_FocusListeners.end());
	m_FocusListeners.erase(listener);
}

} // namespace Guy

