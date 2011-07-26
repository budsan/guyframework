#ifndef _UNIT_H_
#define _UNIT_H_

#include <vector>
#include <queue>
#include "math/vec2.h"
#include "graphics/spriteanim.h"

#include "event.h"
#include "wall.h"

#define LEFT_TEAM  0
#define RIGHT_TEAM 1
#define UNIT_TYPES 4


class Unit : public SpriteAnim {
public:
	 Unit(int _ID): m_ID(_ID) {}
	~Unit() {}

	virtual void move (
        const std::vector<Unit*>& _units,
		const std::vector<Wall>&  _walls) = 0;
	virtual void update (
		const std::vector<Unit*>& _units,
		const std::vector<Wall>&  _walls,
		std::queue<Event*>& _eventQueue) = 0;
	virtual int hurt (int _attack, int _origin) =0;

	int   getID()           { return m_ID; }
	int   getAttack()       { return m_attack; }
	int   getDefense()      { return m_defense; }
	int   getHealth()       { return m_health; }
	float getVisionRange()  { return m_visionRange; }
	float getAttackRange()  { return m_attackRange; }
	float getSpeed()        { return m_speed; }
	int   getAttackRate()	{ return m_attackRate; }
	float x() { return pos.x; }
	float y() { return pos.y; }
	int   getTeam()         { return m_team;}
	void  setTeam(int _team){ m_team = _team;}

	void setPositionY(float _positionY);
	void setPositionX(float _positionX);

	static bool s_drawingRanges; //debug only :D


	struct UnitProperties {
		typedef Unit*(*UnitAllocFunc)(int);
		typedef void(*UnitLoadFunc)();
		typedef void(*UnitUnloadFunc)();

		UnitAllocFunc allocFunc;
		UnitLoadFunc loadFunc;
		UnitUnloadFunc unloadFunc;
		int price;
	};

	//Las clases hijas de Unit deberian implementar esta funcion estatica.
	//static UnitProperties getUnitProperties() = 0;
	
protected:
	const int m_ID;

	int m_state;
	math::vec2f m_invadingZone; //Posicio on hi ha el forat per envair

	int m_team;
	int m_attack;
	int m_attackRate;
	int m_defense;
	int m_health;
	float m_visionRange;
	float m_attackRange;
	float m_speed;
	

	//Mira si en la muralla _enemyWall hi ha un punt de entrada i en cas de ser-hi el coloca en m_InvadingZone i retorna true
	bool holeInSight(const Wall &_enemyWall);
	bool inSight(math::vec2f _pos);
	bool inAttack(math::vec2f _pos);

	void drawRanges();

	static void loadContent(SpriteAnimData* &unitData, const char* filename);
	static void unloadContent(SpriteAnimData* &unitData);

	virtual void preDrawing();
	virtual void postDrawing();
	
};

#endif
