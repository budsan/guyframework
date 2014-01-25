#include <windows.h>

#include "game.h"
#include "environment.h"

int guy_main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    Guy::Game* game = Guy::Game::ptrInstance();
    Guy::Environment &env = Guy::Environment::instance();

    if (env.init(game)) env.run();
    env.destroy();

    return 0;
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
    return guy_main(__argc, __argv);
}


