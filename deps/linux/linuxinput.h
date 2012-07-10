#pragma once

#include "input/input.h"
#include "input/inputstate.h"

#include <vector>
#include <set>
#include <SDL/SDL.h>

class LinuxInput : public Input
{
public:
	 LinuxInput();
	~LinuxInput();

	bool init();

	void update();
	bool exit();

	const InputState &getInputState(int player = 0) const {return m_state[player];}

	void pollEvents();
	void waitEvent();

	virtual int getFocusState() {return m_focusState;}

private:
	void handleEvent(const SDL_Event &event);

	bool  m_doExit;
	unsigned char m_focusState;
	bool m_waitUntilResumeIsCalled;

	std::vector<InputState> m_state;
};

