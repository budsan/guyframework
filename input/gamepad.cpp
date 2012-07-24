#include "gamepad.h"

#include <algorithm>
#include <cassert>

#include "environment.h"
#include "gamepadlistener.h"

namespace Guy {

GamePad::GamePad(int id)
{
	m_id = id;
	for (int i=0 ; i<Button_Count ; i++)
	{
		m_isButtonDown[i] = false;
	}
}

GamePad::~GamePad()
{
}

void GamePad::addListener(GamePadListener *listener)
{
	assert(find(m_listeners.begin(),m_listeners.end(),listener) == m_listeners.end());
	m_listeners.push_back(listener);
}

void GamePad::removeListener(GamePadListener *listener)
{
	assert(find(m_listeners.begin(),m_listeners.end(),listener) != m_listeners.end());
	m_listeners.erase(find(m_listeners.begin(),m_listeners.end(),listener));
}

void GamePad::fireDownEvent(Button button)
{
	m_isButtonDown[button] = true;
	if (m_listeners.size()>0 && isEnabled())
	{
		int numListeners = (int)m_listeners.size();
		for (int i=0 ; i<numListeners ; i++)
		{
			m_listeners[i]->onGamePadButtonDown(this,button);
		}
	}
}

void GamePad::fireUpEvent(Button button)
{
	m_isButtonDown[button] = false;
	if (m_listeners.size()>0 && isEnabled())
	{
		int numListeners = (int)m_listeners.size();
		for (int i=0 ; i<numListeners ; i++)
		{
			m_listeners[i]->onGamePadButtonUp(this,button);
		}
	}
}

void GamePad::setEnabled(bool enabled)
{
	Controller::setEnabled(enabled);
}

void GamePad::setConnected(bool connected) 
{
	// if there's no change, return:
	if (connected == isConnected())
		return;

	// connect the controller:
	Controller::setConnected(connected);

	// fire notification:
	if (connected)
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

void GamePad::setTriggers(float l, float r)
{
	m_triggerL = l;
	m_triggerR = r;
}

void GamePad::setButtonDown(Button button, bool down)
{
	if (m_isButtonDown[button]==down)
	{
		return;
	}

	m_isButtonDown[button] = down;
	if (down)
	{
		fireDownEvent(button);
	}
	else
	{
		fireUpEvent(button);
	}
}

bool GamePad::isButtonDown(Button button)
{
	assert(button<Button_Count);
	return m_isButtonDown[button];
}

const math::vec2f &GamePad::analogL()
{
	return m_analogL;
}

const math::vec2f &GamePad::analogR()
{
	return m_analogR;
}

void GamePad::setAnalogL(float x, float y)
{
	m_analogL.x = x;
	m_analogL.y = y;
}

void GamePad::setAnalogR(float x, float y)
{
	m_analogR.x = x;
	m_analogR.y = y;
}

} // namespace Guy
