#ifndef THEGAME_H
#define THEGAME_H

#include "game.h"
#include "splashstate.h"
#include "graphics/tools/frameshud.h"

//THE GAME, YOU LOST IT
class TheGame : public Game
{
public:
    TheGame();

protected:
    void Configure();
    void Update(float GameTime);
    void Draw();

private:

    FramesHUD frames;
};

#endif // THEGAME_H
