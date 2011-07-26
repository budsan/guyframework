#include "mygame.h"

#include <iostream>

#include "base/log.h"
#include "enginestate.h"
#include "splashstate.h"

MyGame::MyGame()
{

}

void MyGame::Configure()
{
	setFramesPerSecond(100);
	setStableGameTime(true, true);
	ChangeState(new EngineState());
	myScreen->setRatio(16.0/9.0);

//	if (!frames.LoadFont("data/font/SketchRockwell-Bold.ttf"))
//	{
//		LOG << "ERROR: Loading frames font" << std::endl;
//	}
}

void MyGame::Update(float GameTime)
{
//	frames.Update(GameTime);
}

void MyGame::Draw()
{
//	frames.Draw();
}

