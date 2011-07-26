#pragma once
#include "tilemapcharacter.h"

class Player : public TilemapCharacter
{
public:
    Player(Tilemap &parent);
    virtual void Update(float GameTime);

    void Reset();
private:
    static bool firstPlayer;
    static SpriteAnimData data;

    //Mario-like player
    bool crouched;
    bool grounded;
    char onWall;
    float jumpTimeLeft;

    enum {ONWALL_LEFT = -1, ONWALL_NO = 0, ONWALL_RIGHT = 1};
protected:

    virtual bool onLeftCollision(int x, int j);
    virtual bool onRightCollision(int x, int j);
    virtual bool onUpCollision(int x, int j);
    virtual bool onDownCollision(int x, int j);
};

