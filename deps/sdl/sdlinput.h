#pragma once

#include "input/input.h"

#include <SDL2/SDL.h>

namespace Guy {

class SDLInput : public Input
{
public:
	SDLInput();
	virtual ~SDLInput();

	void pollEvents();
	void waitEvent();

	virtual std::size_t keyboardCount();
	virtual Keyboard& keyboard(std::size_t i = 0);

	virtual std::size_t gamePadCount();
	virtual GamePad& gamePad(std::size_t i = 0);

	virtual int getFocusState();

private:
	static void SDLkeysymToGuyKey(SDL_Keysym keysym, wchar_t &unicode, Keyboard::Key &key, Keyboard::Mod &mod);

	void handleEvent(const SDL_Event &event);

	Keyboard *m_keyboard;

	int  m_focusState;
	bool m_waitUntilResumeIsCalled;
};

} // namespace Guy

