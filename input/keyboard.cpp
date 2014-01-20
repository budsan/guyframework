#include "keyboard.h"
#include "keyboardlistener.h"

#include <algorithm>

#include "debug.h"

namespace Guy {

Keyboard::Keyboard()
{
}

Keyboard::~Keyboard()
{
}

void Keyboard::addListener(KeyboardListener *listener)
{
	GUY_ASSERT(find(m_listeners.begin(),m_listeners.end(),listener) == m_listeners.end());
	m_listeners.push_back(listener);
}

void Keyboard::removeListener(KeyboardListener *listener)
{
	GUY_ASSERT(find(m_listeners.begin(),m_listeners.end(),listener) != m_listeners.end());
	m_listeners.erase(find(m_listeners.begin(),m_listeners.end(),listener));
}

void Keyboard::fireKeyDownEvent(wchar_t unicode, Key key, Mod mods)
{
//	dbgPrintLog("Keyboard::fireKeyDownEvent(): unicode=%c key=0x%02x\n",unicode,key);
	if (m_listeners.size()>0 && isEnabled())
	{
		int numListeners = (int)m_listeners.size();
		for (int i=0 ; i<numListeners ; i++)
		{
			m_listeners[i]->onKeyDown(unicode, key, mods);
		}
	}
}

void Keyboard::fireKeyUpEvent(wchar_t unicode, Key key, Mod mods)
{
//	dbgPrintLog("Keyboard::fireKeyUpEvent(): unicode=%c key=0x%02x\n",unicode,key);
	if (m_listeners.size()>0 && isEnabled())
	{
		int numListeners = (int)m_listeners.size();
		for (int i=0 ; i<numListeners ; i++)
		{
			m_listeners[i]->onKeyUp(unicode, key, mods);
		}
	}
}

} // namespace Guy
