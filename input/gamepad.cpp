#include "gamepad.h"

#include <algorithm>

#include "debug.h"

#include "environment.h"
#include "gamepadlistener.h"

namespace Guy {

GamePad::GamePad(std::size_t _id)
{
	m_id = _id;
	for (std::size_t i = 0; i < Button_Count; i++)
	{
		m_isButtonDown[i] = false;
	}
}

GamePad::~GamePad()
{
}

void GamePad::addListener(GamePadListener *_listener)
{
	GUY_ASSERT(find(m_listeners.begin(),m_listeners.end(),_listener) == m_listeners.end());
	m_listeners.push_back(_listener);
}

void GamePad::removeListener(GamePadListener *_listener)
{
	GUY_ASSERT(find(m_listeners.begin(),m_listeners.end(),_listener) != m_listeners.end());
	m_listeners.erase(find(m_listeners.begin(),m_listeners.end(),_listener));
}

void GamePad::fireDownEvent(Button _button)
{
	m_isButtonDown[_button] = true;
	if (m_listeners.size()>0 && isEnabled())
	{
		int numListeners = (int)m_listeners.size();
		for (int i=0 ; i<numListeners ; i++)
		{
			m_listeners[i]->onGamePadButtonDown(this,_button);
		}
	}
}

void GamePad::fireUpEvent(Button _button)
{
	m_isButtonDown[_button] = false;
	if (m_listeners.size()>0 && isEnabled())
	{
		int numListeners = (int)m_listeners.size();
		for (int i=0 ; i<numListeners ; i++)
		{
			m_listeners[i]->onGamePadButtonUp(this,_button);
		}
	}
}

void GamePad::setEnabled(bool _enabled)
{
	Controller::setEnabled(_enabled);
}

void GamePad::setConnected(bool _connected)
{
	// if there's no change, return:
	if (_connected == isConnected())
		return;

	// connect the controller:
	Controller::setConnected(_connected);

	// fire notification:
	if (_connected)
		Environment::instance().input().fireGamePadAdded(m_id);
	else
		Environment::instance().input().fireGamePadRemoved(m_id);
}

float GamePad::triggerL()
{
	return m_triggerL;
}

float GamePad::triggerR()
{
	return m_triggerR;
}

void GamePad::setTriggers(float _l, float _r)
{
	m_triggerL = _l;
	m_triggerR = _r;
}

void GamePad::setButtonDown(Button _button, bool _down)
{
	if (m_isButtonDown[_button]==_down)
	{
		return;
	}

	m_isButtonDown[_button] = _down;
	if (_down)
	{
		fireDownEvent(_button);
	}
	else
	{
		fireUpEvent(_button);
	}
}

bool GamePad::isButtonDown(Button _button)
{
	GUY_ASSERT(_button<Button_Count);
	return m_isButtonDown[_button];
}

const math::vec2f &GamePad::analogL()
{
	return m_analogL;
}

const math::vec2f &GamePad::analogR()
{
	return m_analogR;
}

void GamePad::setAnalogL(float _x, float _y)
{
	m_analogL.x = _x;
	m_analogL.y = _y;
}

void GamePad::setAnalogR(float _x, float _y)
{
	m_analogR.x = _x;
	m_analogR.y = _y;
}

} // namespace Guy
