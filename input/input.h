#pragma once

#include "inputstate.h"
#include <vector>

#define TICKS_PER_SECOND 1000

class Input
{
public:
	static Input& Instance();
	static Input* pInstance();
        ~Input();

	void Update();
	bool Exit();

	float getGlobalTime();
	float getGlobalTimeRaw();
	void getKeyFromSettings();

	const InputState &getInputState(int player = 0) const {return state[player];}
	
private:
	static Input *myInstance;
	Settings* mySettings;
        Input();

	float myGlobalTime;
	bool  myDoExit;

	std::vector<InputState> state;
};

