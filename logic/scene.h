#ifndef _LOGIC_ENGINE_H_
#define _LOGIC_ENGINE_H_

#include <queue>
#include <vector>
#include <list>
#include <map>

#include "graphics/graphics.h"
#include "graphics/spritestatic.h"
#include "graphics/font.h"

#include "unit.h"
#include "unitmelee.h"
#include "unitdefense.h"
#include "unitscout.h"
#include "unitdistance.h"

#include "event.h"
#include "team.h"
#include "wall.h"
#include "damagelabel.h"

#define MAX_UNITS_IN_GAME 1000

typedef Unit*(*UnitAllocFunc)(int);

class Scene {
public:
	
	 Scene();
	~Scene();

	void Update(float GameTime);
	void Draw ();

	void Load();
	void Unload();

private:

	void checkEvents();
	void eventCreate(Event *event);
	void eventAttackUnit(Event *event);
	void eventAttackWall(Event *event);
	void eventRemoveUnit(Event *event);


	void updateDamageLabels();
	void addToDamageLabels(int _ID, int damage);

	std::vector<Unit::UnitProperties> unitProperties;

	Team m_teamL;
	Team m_teamR;

	std::vector<Wall>  m_walls;
	std::vector<SpriteLibrary> m_wallSkins;

	std::vector<Unit*> m_units;
	std::queue<int>    m_hollowUnits;
	std::queue<Event*> m_eventQueue;
	std::list<DamageLabel*> m_labels;
	std::map<int, DamageLabel*> m_activeLabels;

	//Grafics
	Font m_damageLabelFont;
};

#endif

