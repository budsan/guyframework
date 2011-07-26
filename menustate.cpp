#include "menustate.h"
#include "base/game.h"
#include "enginestate.h"
#include <iostream>

MenuState:: MenuState()
{

}

MenuState:: ~MenuState()
{

}

void MenuState::Load()
{

}

void MenuState::Unload()
{

}

void MenuState::Update(float GameTime)
{
	getGame()->ChangeState(new EngineState());
}

void MenuState::Draw()
{


}
