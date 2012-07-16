#pragma once

#include "input/input.h"

#include <bps/event.h>

namespace Guy {

class QNXInput : public Input
{
public:
	QNXInput();
	virtual ~QNXInput();

	void pollEvents();
	void waitEvent();

	virtual int       getKeyboardCount();
	virtual Keyboard& getKeyboard(int i = 0);

	virtual int getFocusState();

private:

	void handleScreenEvent(bps_event_t *event);

	Keyboard *m_keyboard;

	unsigned char m_focusState;
	bool m_waitUntilResumeIsCalled;
};

} // namespace Guy

