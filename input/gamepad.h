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

	GamePad(std::size_t _id);
	virtual ~GamePad();

	void vibrate(int leftVal, int rightVal);

	const math::vec2f &analogL();
	const math::vec2f &analogR();
	void setAnalogL(float _x, float _y);
	void setAnalogR(float _x, float _y);

	float triggerL();
	float triggerR();
	void setTriggers(float _l, float _r);

	void setButtonDown(Button _button, bool _down);
	bool isButtonDown(Button _button);

	inline std::size_t id() { return m_id; }

	virtual void setEnabled(bool _enabled);

	void addListener(GamePadListener *_listener);
	void removeListener(GamePadListener *_listener);

	// overrides:
	virtual void setConnected(bool _connected);

protected:

	void fireDownEvent(Button _button);
	void fireUpEvent(Button _button);

protected:

	std::vector<GamePadListener*> m_listeners;
	std::size_t m_id;
	math::vec2f m_analogL;
	math::vec2f m_analogR;
	float m_triggerL;
	float m_triggerR;
	bool m_isButtonDown[Button_Count];
};

} // namespace Guy
