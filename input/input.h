#pragma once

#include <set>

#include "keyboard.h"
#include "gamepad.h"
#include "gamepadlistener.h"
#include "focuslistener.h"

namespace Guy {

class Input
{
public:

	virtual int       keyboardCount()     = 0;
	virtual Keyboard& keyboard(int i = 0) = 0;

	virtual int      gamePadCount()     = 0;
	virtual GamePad& gamePad(int i = 0) = 0;

	//Must be added FocusListeners manually in Game->init.
	virtual void addFocusListener(FocusListener *listener);
	virtual void removeFocusListener(FocusListener *listener);

	virtual void addGamePadPlugListener(GamePadPlugListener *listener);
	virtual void removeGamePadPlugListener(GamePadPlugListener *listener);

	// FocusStateType enums are bitmasks
	enum FocusStateType
	{
		DrawFocusState  = (1<<0),
		InputFocusState = (1<<1),
		MouseFocusState = (1<<2)
	};

	//use FocusStateType mask to extract the return value
	virtual int getFocusState() = 0;

protected:

	std::set<FocusListener*> m_focusListeners;
	std::set<GamePadPlugListener*> m_gamepadListeners;

	friend class Keyboard;
	friend class GamePad;

	virtual void fireGamePadAdded(int id);
	virtual void fireGamePadRemoved(int id);
};

} // namespace Guy

