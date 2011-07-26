#include "thegame.h"

int main(int argc, char *argv[])
{
	Game* game = new TheGame();
        if (game->Init()) game->Run();
	delete game;

	return 0;
}
