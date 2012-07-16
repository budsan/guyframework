#include "deps/sdl/QNXInput.h"

#include "environment.h"
#include "log.h"

#include <cassert>

namespace Guy {

QNXInput::QNXInput() :
    m_focusState(0)
{
	m_keyboard = new Keyboard();
	m_keyboard->setConnected(true);
	m_keyboard->setEnabled(true);
}

void QNXInput::pollEvents()
{
	for(;;)
	{
		bps_event_t *event = NULL;
		rc = bps_get_event(&event, 0);

		if (event)
		{
			int domain = bps_event_get_domain(event);
			if (domain == screen_get_domain())
			{
				handleScreenEvent(event);
			}
			else if (domain == navigator_get_domain())
			{
				//handleNavigatorEvent(event);
			}
		}
		else
		{
			break;
		}
	}
}

void QNXInput::waitEvent()
{

}

int QNXInput::getKeyboardCount()
{
	return 1;
}

Keyboard& QNXInput::getKeyboard(int i)
{
	assert(i == 0);
	return *m_keyboard;
}

int QNXInput::getFocusState()
{
	return m_focusState;
}

} // namespace Guy
