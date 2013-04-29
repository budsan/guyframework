#include "game.h"
#include "environment.h"

int main(int argc, char *argv[])
{
    Guy::Game* game = Guy::Game::ptrInstance();
    Guy::Environment &env = Guy::Environment::instance();

    if (env.init(game)) env.run();
    env.destroy();

    return 0;
}

