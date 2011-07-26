#include "enginestate.h"
#include "graphics/graphics.h"

EngineState:: EngineState() {}
EngineState::~EngineState() {}

void EngineState::Update(float GameTime)
{
	myScene.Update(GameTime);
}

void EngineState::Load()
{
	myScene.Load();
}

void EngineState::Unload()
{
	myScene.Unload();
}

void EngineState::Draw()
{
	myScene.Draw();
}

