#include "game.h"
#include "log.h"

namespace Guy {

Game *Game::s_game;

Game* Game::ptrInstance()
{
    return s_game;
}

Game::Game()
{
    GUY_ASSERT(s_game == NULL);
    s_game = this;
}

Game::~Game()
{

}

void Game::init()
{
	//Nothing.
}

void Game::load()
{
	//Nothing either.
}

void Game::unload()
{
	//Almost forgot. NOTHING AS WELL.
}

const char *Game::name()
{
	return "Game";
}

const char *Game::version()
{
	return "Undefined";
}

} // namespace Guy
