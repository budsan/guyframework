#pragma once

#include "inputstate.h"
#include <vector>

#define TICKS_PER_SECOND 1000

class Input
{
public:
	static Input& instance();
	static Input* ptrInstance();
        ~Input();

	void update();
	bool exit();

	float getTime();
	float getTimeRaw();
	void getKeyFromSettings();

	const InputState &getInputState(int player = 0) const {return m_state[player];}
	
private:
	static Input *m_instance;
	Settings* m_settings;
        Input();

	float m_time;
	bool  m_doExit;

	std::vector<InputState> m_state;
};

