#include "inputstate.h"
#include <string.h>

#include "settings.h"

InputState::InputState()
	: actions(Settings::pInstance()->getKeybinds().getAccionCount())
{
	Reset();
}

InputState::InputState(const InputState &in)
{
	this->operator =(in);
}

const InputState &InputState::operator=(const InputState &in)
{
	myKeyState = in.myKeyState;
	myKeyDown  = in.myKeyDown;
	myKeyUp    = in.myKeyUp;

	return *this;
}

void InputState::operator+=(const InputState &in)
{
	for (unsigned int i = 0; i < actions; i++)
		myKeyState[i] = myKeyState[i] || in.myKeyState[i];

	for (unsigned int i = 0; i < actions; i++)
		myKeyDown[i] = myKeyDown[i] || in.myKeyDown[i];

	for (unsigned int i = 0; i < actions; i++)
		myKeyUp[i] = myKeyState[i] || in.myKeyUp[i];
}

InputState InputState::operator+(const InputState &in) const
{
	InputState response(*this);
	response += in;
	return response;
}

void InputState::ResetEvents()
{
	myKeyDown.assign(actions, false);
	myKeyUp  .assign(actions, false);
}

void InputState::Reset()
{
	myKeyState.assign(actions, false);
	myKeyDown.assign(actions, false);
	myKeyUp  .assign(actions, false);
}

bool InputState::getKeyState(unsigned char key) const
{
	return myKeyState[key];
}

bool InputState::getKeyDown (unsigned char key) const
{
	return myKeyDown[key];
}

bool InputState::getKeyUp   (unsigned char key) const
{
	return myKeyUp[key];
}

