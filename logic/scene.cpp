#include <queue>
#include <vector>
#include <iostream>
#include <algorithm>
#include <utility>

#include "scene.h"

#include "base/defines.h"
#include "graphics/graphics.h"
#include "graphics/screen.h"
#include "graphics/camera2d.h"

#include "event.h"
#include "team.h"
#include "wall.h"
#include "unit.h"

Scene::Scene()
{
	unitProperties.push_back(UnitMelee::getUnitProperties());
	unitProperties.push_back(UnitDefense::getUnitProperties());
	unitProperties.push_back(UnitScout::getUnitProperties());
	unitProperties.push_back(UnitDistance::getUnitProperties());

	m_teamL.setPlayerPos(TEAM_LEFT);
	m_teamR.setPlayerPos(TEAM_RIGHT);

	m_teamL.setGold(150);
	m_teamR.setGold(150);

	Wall m_wallL;
	Wall m_wallR;

	m_wallL.setWallPos(GAMEFIELD_LEFT +(TILE_SIZE*2));
	m_wallR.setWallPos(GAMEFIELD_RIGHT-(TILE_SIZE*2));

	m_wallL.setWallTeam(TEAM_LEFT);
	m_wallR.setWallTeam(TEAM_RIGHT);

	m_wallL.setWallHealth(300);
	m_wallR.setWallHealth(300);

	m_walls.push_back(m_wallL);
	m_walls.push_back(m_wallR);

	for (unsigned int i = 0; i < m_walls.size(); i++) m_walls[i].setWallID(i);
}
 
Scene::~Scene()
{
	
}

void Scene::Load()
{
	 m_damageLabelFont.Load("data/font/nibby.ttf", 128);
	 m_activeLabels.clear();

	Sprite::setGlobalPixelsPerUnit(PIXELS_PER_UNIT);
	m_wallSkins.resize(2);
	m_wallSkins[0].Load("data/scripts/wallred.sprite");
	m_wallSkins[1].Load("data/scripts/wallblue.sprite");

	m_walls[0].setWallSkin(&m_wallSkins[0]);
	m_walls[1].setWallSkin(&m_wallSkins[1]);

	for (unsigned int i = 0; i < unitProperties.size(); i++)
		unitProperties[i].loadFunc();
}

void Scene::Unload()
{
	m_damageLabelFont.Clean();

	m_walls[0].setWallSkin(NULL);
	m_walls[1].setWallSkin(NULL);
	m_wallSkins.clear();

	for (unsigned int i = 0; i < unitProperties.size(); i++)
		unitProperties[i].unloadFunc();
}

void Scene::Update (float GameTime)
{
	//TEAMS
	m_teamL.update (m_eventQueue);
	m_teamR.update (m_eventQueue);
	//UNITS	
	{
		std::vector<Unit*>::iterator it;
		for (it = m_units.begin(); it != m_units.end(); it++) {
			if (*it != NULL) (*it)->move(m_units, m_walls);
		} 
		for (it = m_units.begin(); it != m_units.end(); it++) {
			if (*it != NULL) (*it)->update(m_units, m_walls, m_eventQueue);
 		}
	}

	checkEvents();
	updateDamageLabels();
}

void Scene::checkEvents()
{
	while ( !m_eventQueue.empty() )
	{
		Event *event = m_eventQueue.front();
		m_eventQueue.pop();
		
		switch (event->type) {
			case EVENT_CREATE: eventCreate(event); break;
			case EVENT_ATTACK_UNIT: eventAttackUnit(event); break;
			case EVENT_ATTACK_WALL: eventAttackWall(event); break;
			case EVENT_REMOVE_UNIT: eventRemoveUnit(event); break;
			default: break;
		}
	}
}

void Scene::eventCreate(Event *event)
{
	EventCreate *e = (EventCreate*)event;
	int price = unitProperties[e->unitType].price;
	Team &t = e->team == TEAM_LEFT ? m_teamL : m_teamR;

	if ( t.getGold() < price ) return;
	else t.decreaseGold(price);

	Unit *u;
	int ID = -1;

	// Si la cola de huecos en el vector de units esta vacia...
	if (m_hollowUnits.empty()) {
		// ...metemos una nueva unit al final del vector.
		ID = (int)m_units.size();
		m_units.push_back(NULL);
	}
	else { //Sino...
		//...cojemos un hueco y le ponemos una Unit nueva.
		ID = m_hollowUnits.front();
		m_hollowUnits.pop();

	}

	u = unitProperties[e->unitType].allocFunc(ID);
	m_units[ID] = u;

	u->setTeam(e->team);
	u->setPositionY(e->position);
	if (e->team == TEAM_LEFT) u->setPositionX(GAMEFIELD_LEFT);
	if (e->team == TEAM_RIGHT) u->setPositionX(GAMEFIELD_RIGHT);

	delete event;
}

void Scene::eventAttackUnit(Event *event)
{
	EventAttackUnit *e  = (EventAttackUnit*)event;

	int damage = m_units[e->target]->hurt( e->attack, e->origin);
	addToDamageLabels(e->target, damage);
	delete event;
}

void Scene::eventAttackWall(Event *event)
{
	EventAttackWall *e  = (EventAttackWall*)event;
	m_walls[e->targetWall].DamageBrick(e->positionY,e->attack);
	delete event;
}

void Scene::eventRemoveUnit(Event *event)
{
	EventRemoveUnit *e  = (EventRemoveUnit*)event;

	Unit *u = m_units[e->target];
	delete u;
	m_units[e->target] = NULL;

	//Metemos un hueco nuevo en la cola de huecos.
	m_hollowUnits.push(e->target);
	delete event;
}

void Scene::updateDamageLabels()
{
	std::list<DamageLabel*>::iterator it;
	for (it = m_labels.begin(); it != m_labels.end(); it++) {
		if (*it == NULL) {
			it = m_labels.erase(it);
		}
		else if (*it != NULL && !(*it)->update()) {
			DamageLabel* toDelete = (*it);
			it = m_labels.erase(it);
			delete toDelete;
		}

		if (it == m_labels.end()) break;
	}

	std::map<int, DamageLabel*>::iterator iter;
	//int i = m_activeLabels.size();
	for (iter = m_activeLabels.begin(); iter != m_activeLabels.end(); iter++) {
		if (((*iter).second)->thresholdPassed()) {
			std::map<int, DamageLabel*>::iterator toDelete = iter;
			bool begin = iter == m_activeLabels.begin(); iter--;
			m_activeLabels.erase(toDelete);

			if (begin) iter = m_activeLabels.begin();
			if (m_activeLabels.size() == 0) break;
		}
	}
}

void Scene::addToDamageLabels(int _ID, int _damage)
{
	std::map<int, DamageLabel*>::iterator it = m_activeLabels.find(_ID);
	if (it == m_activeLabels.end())
	{
		DamageLabel* dl = new DamageLabel(&m_damageLabelFont, 128);
		m_labels.push_back(dl);
		m_activeLabels[_ID] = dl;

		dl->add(_damage);
		dl->setPosition(m_units[_ID]->position());
		switch(m_units[_ID]->getTeam())
		{
			case LEFT_TEAM:
			{
				float color[4] = {1.0f, 0.5f, 0.5f, 1.0f};
				dl->setColor(color);
				dl->setHorzAlignment(Font::RIGHT);
			}
			break;
			case RIGHT_TEAM:
			{
				float color[4] = {0.5f, 0.5f, 1.0f, 1.0f};
				dl->setColor(color);
				dl->setHorzAlignment(Font::LEFT);
			}
			break;
		}
	}
	else
	{
		((*it).second)->add(_damage);
	}
}

void Scene::Draw ()
{
	Camera2D camera;
	camera.resizeScreen(VIEWPORT_HEIGHT);
	camera.setOpenGLMatrices();

	//-CONFIG-NEW-ATTRIBS-/
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//-------------------------------------------------------------------//
	glDepthMask(GL_FALSE);
	//VIEWPORT - DEBUG
	glColor4f(0.4f,0.25f,0.0f,1.0f);
	glBegin(GL_QUADS);
	glVertex3f(VIEWPORT_LEFT ,VIEWPORT_BOTTOM, 0);
	glVertex3f(VIEWPORT_RIGHT,VIEWPORT_BOTTOM, 0);
	glVertex3f(VIEWPORT_RIGHT,VIEWPORT_TOP   , 0);
	glVertex3f(VIEWPORT_LEFT ,VIEWPORT_TOP   , 0);
	glEnd();

	//Centramos la posicion (0,0) al centro del GAMEFIELD
	glTranslatef(0,GAMEFIELD_CENTER_CORRECTION,0);

	//GAMEFIELD - DEBUG
	glColor4f(0.30f,0.615f,0.0f,1.0f);
	glBegin(GL_QUADS);
	glVertex3f(GAMEFIELD_LEFT ,GAMEFIELD_BOTTOM, 0);
	glVertex3f(GAMEFIELD_RIGHT,GAMEFIELD_BOTTOM, 0);
	glVertex3f(GAMEFIELD_RIGHT,GAMEFIELD_TOP   , 0);
	glVertex3f(GAMEFIELD_LEFT ,GAMEFIELD_TOP   , 0);
	glEnd();

	glDepthMask(GL_TRUE);

	std::vector<Wall>::iterator it0;
	for (it0 = m_walls.begin(); it0 != m_walls.end(); it0++)
	{
		(*it0).Draw();
	}

	//Ordenamos las unidades para pintarlas con el algoritmo del pintor.
	std::vector<std::pair<float, Unit*> > paint_order;
	paint_order.reserve(m_units.size()-m_hollowUnits.size());
	std::vector<Unit*>::iterator it1;
	for (it1 = m_units.begin(); it1 != m_units.end(); it1++) {
		if (*it1 != NULL) {
			paint_order.push_back(std::pair<float, Unit*>(-(*it1)->y(), *it1));
		}
	}

	std::sort(paint_order.begin(), paint_order.end());
	std::vector<std::pair<float, Unit*> >::iterator it2;
	for (it2 = paint_order.begin(); it2 != paint_order.end(); it2++) {
		(it2->second)->Draw();
	}

	m_teamL.draw();
	m_teamR.draw();
	
	{
		std::list<DamageLabel*>::iterator it;
		for (it = m_labels.begin(); it != m_labels.end(); it++) {
			if (*it != NULL) (*it)->draw();
		}
	}
	//-------------------------------------------------------------------//
}


