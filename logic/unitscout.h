#ifndef _UNIT_SCOUT_H_
#define _UNIT_SCOUT_H_

#define SCOUT_PRICE 50

#include "unit.h"

#include <queue>
#include <vector>
#include <algorithm>


class UnitScout: public Unit{

public:
    UnitScout(int _ID);
    ~UnitScout();

	void move (
                const std::vector<Unit*>& _units,
		const std::vector<Wall>&  _walls);
	void update (
		const std::vector<Unit*>& _units,
		const std::vector<Wall>&  _walls,
		std::queue<Event*>& _eventQueue);

	int hurt (int _attack, int _origin);

	static void loadContent();
	static void unloadContent();

	static Unit* UnitAllocator(int id) {return new UnitScout(id);}
	static UnitProperties getUnitProperties()
	{
		UnitProperties toReturn = {
			UnitAllocator,
			loadContent,
			unloadContent,
			SCOUT_PRICE
		};
		return toReturn;
	}

private: 
#ifdef _DEBUG
	void assertStateChanges(int _numStateChanges, int* _statesStack);
#endif
	enum States
	{
		STATE_WALK,
		STATE_ATTACK,
		STATE_DEAD, 
		STATE_DESTROY,
		STATE_INVADE,
		NUM_STATES
	};

	int m_attackRateCount;
	int     m_attackingEnemyID;
	Unit*   m_enemyUnit;    //En cas de perseguir/atacar algu s'ha de saber a qui

	static SpriteAnimData *unitData;
};


#endif
