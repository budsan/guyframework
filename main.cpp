#include "mygame.h"

int main(int argc, char *argv[])
{
	Game* game = new MyGame();
        if (game->Init()) game->Run();
	delete game;

	return 0;
}
