#ifndef _WALL_H_
#define _WALL_H_

#define DEFAULT_NUM_BRICKS 10

#include "base/defines.h"
#include "math/vec2.h"
#include "graphics/spritestatic.h"

class Wall {
public:
	Wall(unsigned int numBricks = DEFAULT_NUM_BRICKS);
	~Wall();
	void Draw();
	
	void setWallID    (int   _ID    ) {m_WallID = _ID;  }
	void setWallPos   (float _posX  ) {m_posX   = _posX;}
	void setWallTeam  (int   _team  ) {m_team   = _team;}
	void setWallHealth(int   _health);
	void setWallSkin(SpriteLibrary *skin);

	int getWallTeam() const { return m_team; }
	int getWallID() const { return m_WallID; }
	int getBricksHealth(float _posY) const;
	float getBrickByPos(float _posY) const;
	bool getHoleInRange(float _posY0, float _posY1, math::vec2f &_Hole) const;
	float x() const { return m_posX; }

	//Li fot mal al brick de la posicio Y
	void DamageBrick(float _posY, int _damage);
private:
	
	struct Brick {
		int m_initialBrickHealth;
		int m_currentBrickHealth;
		float m_posY;
	};

	int m_WallID;
	unsigned int m_numBricks;
	std::vector<Brick> m_bricks;
	float m_posX;
	int m_team;
	
	SpriteLibrary *m_skin;
	void DrawNoSkin();

	//Necessari per a una unitat per si veu el foradot de un brick petat
	float m_brickWidth;
};

#endif
