#include "gamestate.h"

GameState::GameState() : m_parent(0)
{

}

void GameState::setGame(Game *parent)
{
	m_parent = parent;
}

Game* GameState::getGame()
{
	return m_parent;
}

