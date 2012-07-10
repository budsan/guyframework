#pragma once

#include "keybind.h"
#include "inputstate.h"

#include <set>

class Input
{
public:
	void setKeybinds(const Keybinds &keys);
	const Keybinds &getKeybinds() const;
	const InputState &getInputState(int player = 0);

	struct FocusListener
	{
		virtual void onGainDrawFocus() { }
		virtual void onLoseDrawFocus() { }

		virtual void onGainInputFocus() { }
		virtual void onLoseInputFocus() { }

		virtual void onGainMouseFocus() { }
		virtual void onLoseMouseFocus() { }
	};

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

