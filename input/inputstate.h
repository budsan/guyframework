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

	void ResetEvents();
	void Reset();
private:
	unsigned int actions;

	std::vector<bool> myKeyState;
	std::vector<bool> myKeyUp;
	std::vector<bool> myKeyDown;
	friend class Input;
};

#endif // INPUTSTATE_H
