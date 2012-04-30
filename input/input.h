#pragma once

#include "inputstate.h"
#include <vector>

#define TICKS_PER_SECOND 1000

union SDL_Event;
class Input
{
public:
	static Input& Instance();
	static Input* ptrInstance();
	~Input();

	void update();
	bool exit();

	void delay(unsigned int ms);

	float getTime();
	float getTimeRaw();
	void getKeyFromSettings();

	const InputState &getInputState(int player = 0) const {return m_state[player];}

	void pollEvents();
	void waitEvent();

	struct FocusObserver {
		virtual void onGainDrawFocus() { }
		virtual void onLoseDrawFocus() { }

		virtual void onGainInputFocus() { }
		virtual void onLoseInputFocus() { }

		virtual void onGainMouseFocus() { }
		virtual void onLoseMouseFocus() { }
	};
	void setFocusObserver(FocusObserver *observer) { m_focusObserver = observer;}

	enum FocusStateType { DRAW = 1, INPUT = 2, MOUSE = 4 };
	int getFocusState() { return (int) m_focusState; }

private:
	static Input *m_instance;
	Settings* m_settings;
	Input();

	void handleEvent(const SDL_Event &event);

	float m_time;
	bool  m_doExit;
	unsigned char m_focusState;
	bool m_waitUntilResumeIsCalled;
	FocusObserver* m_focusObserver;

	std::vector<InputState> m_state;
};

