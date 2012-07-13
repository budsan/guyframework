#include "gamepad.h"

#include <algorithm>
#include <cassert>

#include "environment.h"
#include "gamepadlistener.h"

namespace Guy {

GamePad::GamePad(int id)
{
	mId = id;
	for (int i=0 ; i<BUTTON_COUNT ; i++)
	{
		mIsButtonDown[i] = false;
	}
}

GamePad::~GamePad()
{
}

void GamePad::addListener(GamePadListener *listener)
{
	assert(find(mListeners.begin(),mListeners.end(),listener) == mListeners.end());
	mListeners.push_back(listener);
}

void GamePad::removeListener(GamePadListener *listener)
{
	assert(find(mListeners.begin(),mListeners.end(),listener) != mListeners.end());
	mListeners.erase(find(mListeners.begin(),mListeners.end(),listener));
}

void GamePad::fireDownEvent(Button button)
{
	mIsButtonDown[button] = true;
	if (mListeners.size()>0 && isEnabled())
	{
		int numListeners = (int)mListeners.size();
		for (int i=0 ; i<numListeners ; i++)
		{
			mListeners[i]->onGamePadButtonDown(this,button);
		}
	}
}

void GamePad::fireUpEvent(Button button)
{
	mIsButtonDown[button] = false;
	if (mListeners.size()>0 && isEnabled())
	{
		int numListeners = (int)mListeners.size();
		for (int i=0 ; i<numListeners ; i++)
		{
			mListeners[i]->onGamePadButtonUp(this,button);
		}
	}
}

void GamePad::setEnabled(bool enabled)
{
	Controller::setEnabled(enabled);
}

void GamePad::setConnected(bool connected) 
{
	/*
	// if there's no change, return:
	if (connected == isConnected())
	{
		return;
	}

	// connect the controller:
	Controller::setConnected(connected);

	// fire notification:
	if (connected)
	{
		Environment::instance().fireGamePadAdded(mId);
	}
	else
	{
		Environment::instance().fireGamePadRemoved(mId);
	}
	*/
}

float GamePad::getTriggerL()
{
	return mTriggerL;
}

float GamePad::getTriggerR()
{
	return mTriggerR;
}

void GamePad::setTriggers(float l, float r)
{
	mTriggerL = l;
	mTriggerR = r;
}

void GamePad::setButtonDown(Button button, bool down)
{
	if (mIsButtonDown[button]==down)
	{
		return;
	}

	mIsButtonDown[button] = down;
	if (down)
	{
		fireDownEvent(button);
	}
	else
	{
		fireUpEvent(button);
	}
}

bool GamePad::isButtonDown(Button button)
{
	assert(button<BUTTON_COUNT);
	return mIsButtonDown[button];
}

const math::vec2f &GamePad::getAnalogL()
{
	return mAnalogL;
}

const math::vec2f &GamePad::getAnalogR()
{
	return mAnalogR;
}

void GamePad::setAnalogL(float x, float y)
{
	mAnalogL.x = x;
	mAnalogL.y = y;
}

void GamePad::setAnalogR(float x, float y)
{
	mAnalogR.x = x;
	mAnalogR.y = y;
}

} // namespace Guy
