#pragma once
#include "base/game.h"
#include "graphics/tools/frameshud.h"

class MyGame : public Game
{
public:
    MyGame();

protected:
    void Configure();
    void Update(float GameTime);
    void Draw();

    //FramesHUD frames;
};

