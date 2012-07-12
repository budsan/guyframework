#pragma once

#include "input/input.h"

#include <vector>
#include <set>
#include <SDL/SDL.h>

class LinuxInput : public Input
{
public:
	LinuxInput();
	~LinuxInput();

	void pollEvents();
	void waitEvent();

	virtual int       getKeyboardCount();
	virtual Keyboard& getKeyboard(int i = 0);

	virtual int getFocusState();

private:
	static void SDLkeysymToGuyKey(SDL_keysym keysym, wchar_t &unicode, Keyboard::Key &key, Keyboard::Mod &mod);

	void handleEvent(const SDL_Event &event);

	Keyboard *m_keyboard;

	unsigned char m_focusState;
	bool m_waitUntilResumeIsCalled;
};

