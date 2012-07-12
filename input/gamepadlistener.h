#pragma once

#include "gamepad.h"

class GamePadListener
{
public:

	GamePadListener() {}
	virtual ~GamePadListener() {}

	virtual void onGamePadButtonDown(GamePad *pad, GamePad::Button button) = 0;
	virtual void onGamePadButtonUp(GamePad *pad, GamePad::Button button) = 0;

};

