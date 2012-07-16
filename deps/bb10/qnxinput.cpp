#include "deps/bb10/qnxinput.h"

#include "environment.h"
#include "log.h"

#include "math/algebra3.h"

#include <cassert>

#include <bps/navigator.h>
#include <bps/bps.h>
#include <bps/event.h>
#include <bps/screen.h>

namespace Guy {

QNXInput::QNXInput()
	: m_focusState(0)
{
	m_keyboard = new Keyboard();
	m_keyboard->setConnected(true);
	m_keyboard->setEnabled(true);
}

void QNXInput::pollEvents()
{
	int rc = BPS_SUCCESS;

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

void QNXInput::handleScreenEvent(bps_event_t *event)
{
	int screen_val, buttons;
	math::vec2i pos;

	screen_event_t screen_event = screen_event_get_event(event);

	//Query type of screen event and its location on the screen
	screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_TYPE, &screen_val);
	screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_SOURCE_POSITION, (int*)&pos);

	//There is a difference between touch screen events and mouse events
	if (screen_val == SCREEN_EVENT_MTOUCH_RELEASE)
	{
		//Handle touch screen event

	}
	else if (screen_val == SCREEN_EVENT_MTOUCH_TOUCH)
	{

	}
	else if (screen_val == SCREEN_EVENT_POINTER)
	{
		//This is a mouse move event, it is applicable to a device with a usb mouse or simulator
		screen_get_event_property_iv(screen_event, SCREEN_PROPERTY_BUTTONS, &buttons);

		if (buttons == SCREEN_LEFT_MOUSE_BUTTON)
		{

		}
		else
		{

		}
	}
}

} // namespace Guy
