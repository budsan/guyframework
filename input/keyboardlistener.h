#pragma once

#include "keyboard.h"

namespace Guy {

class KeyboardListener
{
public:

	KeyboardListener() {}
	virtual ~KeyboardListener() {}

	virtual void onKeyUp(wchar_t unicode, Keyboard::Key key, Keyboard::Mod mods) = 0;
	virtual void onKeyDown(wchar_t unicode, Keyboard::Key key, Keyboard::Mod mods) = 0;

};

} // namespace Guy
