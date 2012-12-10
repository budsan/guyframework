#pragma once

#include "input/input.h"

#include <SDL/SDL.h>

namespace Guy {

class SDLInput : public Input
{
public:
	SDLInput();
	virtual ~SDLInput();

	void pollEvents();
	void waitEvent();

	virtual int       keyboardCount();
	virtual Keyboard& keyboard(int i = 0);

	virtual int      gamePadCount();
	virtual GamePad& gamePad(int i = 0);

	virtual int getFocusState();

private:
	static void SDLkeysymToGuyKey(SDL_keysym keysym, wchar_t &unicode, Keyboard::Key &key, Keyboard::Mod &mod);

	void handleEvent(const SDL_Event &event);

	Keyboard *m_keyboard;

	unsigned char m_focusState;
	bool m_waitUntilResumeIsCalled;
};

} // namespace Guy

