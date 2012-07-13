#pragma once

#include <set>

#include "keyboard.h"
#include "focuslistener.h"

namespace Guy {

class Input
{
public:

	virtual int       getKeyboardCount()     = 0;
	virtual Keyboard& getKeyboard(int i = 0) = 0;

	//Must be added FocusListeners manually in Game->init.
	virtual void addFocusListener(FocusListener *listener);
	virtual void removeFocusListener(FocusListener *listener);

	// FocusStateType enums are bitmasks
	enum FocusStateType {
		DRAW  = (1<<0),
		INPUT = (1<<1),
		MOUSE = (1<<2)
	};

	//use FocusStateType mask to extract the return value
	virtual int getFocusState() = 0;
protected:

	std::set<FocusListener*> m_FocusListeners;
};

} // namespace Guy

