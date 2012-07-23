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

} // namespace Guy

