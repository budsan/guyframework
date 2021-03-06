#pragma once

#include "gamepad.h"

namespace Guy {

class GamePadListener
{
public:

	GamePadListener() {}
	virtual ~GamePadListener() {}

	virtual void onGamePadButtonDown(GamePad *pad, GamePad::Button button) = 0;
	virtual void onGamePadButtonUp(GamePad *pad, GamePad::Button button) = 0;

};

class GamePadPlugListener
{
public:

	GamePadPlugListener() {}
	virtual ~GamePadPlugListener() {}

	virtual void onGamePadAdded(std::size_t id) = 0;
	virtual void onGamePadRemoved(std::size_t id) = 0;

};

} // namespace Guy

