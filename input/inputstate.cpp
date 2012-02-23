#include "inputstate.h"
#include <string.h>

#include "settings.h"

InputState::InputState()
	: m_actions(Settings::pInstance()->getKeybinds().getAccionCount())
{
	reset();
}

InputState::InputState(const InputState &in)
{
	this->operator =(in);
}

const InputState &InputState::operator=(const InputState &in)
{
	m_keyState = in.m_keyState;
	m_keyDown  = in.m_keyDown;
	m_keyUp    = in.m_keyUp;

	return *this;
}

void InputState::operator+=(const InputState &in)
{
	for (unsigned int i = 0; i < m_actions; i++)
		m_keyState[i] = m_keyState[i] || in.m_keyState[i];

	for (unsigned int i = 0; i < m_actions; i++)
		m_keyDown[i] = m_keyDown[i] || in.m_keyDown[i];

	for (unsigned int i = 0; i < m_actions; i++)
		m_keyUp[i] = m_keyState[i] || in.m_keyUp[i];
}

InputState InputState::operator+(const InputState &in) const
{
	InputState response(*this);
	response += in;
	return response;
}

void InputState::resetEvents()
{
	m_keyDown.assign(m_actions, false);
	m_keyUp  .assign(m_actions, false);
}

void InputState::reset()
{
	m_keyState.assign(m_actions, false);
	m_keyDown.assign(m_actions, false);
	m_keyUp  .assign(m_actions, false);
}

bool InputState::getKeyState(unsigned char key) const
{
	return m_keyState[key];
}

bool InputState::getKeyDown (unsigned char key) const
{
	return m_keyDown[key];
}

bool InputState::getKeyUp   (unsigned char key) const
{
	return m_keyUp[key];
}

