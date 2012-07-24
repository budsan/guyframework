#pragma once

#include "math/algebra3.h"

#include "controller.h"
#include <vector>

namespace Guy {

class GamePadListener;
class GamePad : public Controller
{
public:

	enum Button
	{
		Button_Unknown = -1,
		Button_0 = 0,
		Button_1,
		Button_2,
		Button_3,
		Button_LShoulder,
		Button_RShoulder,
		Button_LStick,
		Button_RStick,
		Button_Start,
		Button_Aux,
		Button_DPadUp,
		Button_DPadDown,
		Button_DPadLeft,
		Button_DPadRight,
		Button_Count
	};

	GamePad(int id);
	virtual ~GamePad();

	void vibrate(int leftVal, int rightVal);

	const math::vec2f &analogL();
	const math::vec2f &analogR();
	void setAnalogL(float x, float y);
	void setAnalogR(float x, float y);

	float triggerL();
	float triggerR();
	void setTriggers(float l, float r);

	void setButtonDown(Button button, bool down);
	bool isButtonDown(Button button);

	inline int id() { return m_id; }

	virtual void setEnabled(bool enabled);

	void addListener(GamePadListener *listener);
	void removeListener(GamePadListener *listener);

	// overrides:
	virtual void setConnected(bool connected);

protected:

	void fireDownEvent(Button button);
	void fireUpEvent(Button button);

protected:

	std::vector<GamePadListener*> m_listeners;
	int m_id;
	math::vec2f m_analogL;
	math::vec2f m_analogR;
	float m_triggerL;
	float m_triggerR;
	bool m_isButtonDown[Button_Count];
};

} // namespace Guy
