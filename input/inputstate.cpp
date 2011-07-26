#include "inputstate.h"
#include <string.h>

InputState::InputState()
{
	Reset();
}

InputState::InputState(const InputState &in)
{
	this->operator =(in);
}

const InputState &InputState::operator=(const InputState &in)
{
	memcpy(myKeyState, in.myKeyState, sizeof(myKeyState));
	memcpy(myKeyDown , in.myKeyDown , sizeof(myKeyDown));
	memcpy(myKeyUp   , in.myKeyUp   , sizeof(myKeyUp));

	return *this;
}

void InputState::operator+=(const InputState &in)
{
	for (unsigned int i = 0; i < K_SIZE; i++)
		myKeyState[i] = myKeyState[i] || in.myKeyState[i];

	for (unsigned int i = 0; i < K_SIZE; i++)
		myKeyDown[i] = myKeyDown[i] || in.myKeyDown[i];

	for (unsigned int i = 0; i < K_SIZE; i++)
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
	memset(myKeyDown , 0, sizeof(myKeyDown));
	memset(myKeyUp   , 0, sizeof(myKeyUp));
}

void InputState::Reset()
{
	memset(myKeyState, 0, sizeof(myKeyState));
	memset(myKeyDown , 0, sizeof(myKeyDown));
	memset(myKeyUp   , 0, sizeof(myKeyUp));
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

