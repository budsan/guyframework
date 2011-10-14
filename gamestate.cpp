#include "gamestate.h"

GameState::GameState() : myParent(0)
{

}

void GameState::setGame(Game* parent)
{
	myParent = parent;
}

Game* GameState::getGame()
{
	return myParent;
}

