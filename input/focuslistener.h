#pragma once

namespace Guy {

class FocusListener
{
public:
	virtual void onGainDrawFocus() { }
	virtual void onLoseDrawFocus() { }

	virtual void onGainInputFocus() { }
	virtual void onLoseInputFocus() { }

	virtual void onGainMouseFocus() { }
	virtual void onLoseMouseFocus() { }
};

} // namespace Guy
