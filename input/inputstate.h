#ifndef INPUTSTATE_H
#define INPUTSTATE_H

#include "settings.h"
#include <vector>

class InputState
{
public:
	InputState();
	InputState(const InputState &in);

	const InputState &operator=(const InputState &in);
	void operator+=(const InputState &in);
	InputState operator+(const InputState &in) const;

	bool getKeyState(unsigned char key) const;
	bool getKeyDown (unsigned char key) const;
	bool getKeyUp   (unsigned char key) const;

	void resetEvents();
	void reset();
private:
	unsigned int m_actions;

	std::vector<bool> m_keyState;
	std::vector<bool> m_keyUp;
	std::vector<bool> m_keyDown;
	friend class Input;
};

#endif // INPUTSTATE_H
