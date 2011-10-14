#ifndef INPUTSTATE_H
#define INPUTSTATE_H

#include "settings.h"

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
	bool myKeyState[K_SIZE];
	bool myKeyUp   [K_SIZE];
	bool myKeyDown [K_SIZE];
	friend class Input;
};

#endif // INPUTSTATE_H
