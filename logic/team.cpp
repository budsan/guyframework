#include "graphics/graphics.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <stdlib.h>

#include "base/defines.h"
#include "team.h"
#include "player.h"
#include "unit.h"

Team::Team() {
    m_gold = 0;
	m_transitionMoney.setPos((float)m_gold);
    m_workers = 10;
    m_goldPerWorker = 2;
    m_costWorker = FACTOR_COST_WORKER;
	m_ticksToIncom = 0;
	m_ticksPerIncom = 100;
	m_y  = 0.0;
	m_v  = 0.0;
	m_vy = 0.5;
	m_ay = 0.04f;
	m_fontLoaded = false;
	m_typeMove = ACCEL_MOVE;
}

Team::~Team() {}

void Team::setPlayerPos(int _playerPos) {
         if (_playerPos == TEAM_LEFT) player.setPlayerType(PLAYER_ONE);
    else if (_playerPos == TEAM_RIGHT) player.setPlayerType(PLAYER_TWO);
    m_playerPos = _playerPos;
}

void Team::setWorkers(int _workers) {
    m_workers = _workers;
}

void Team::setCostWorker(int _gold) {
    m_costWorker = _gold;
}

void Team::setGold(int _gold) {
    m_gold = _gold;
	m_transitionMoney.setPos((float)m_gold);
}

void Team::setTypeMove(TypeMove type) {
	m_typeMove = type;
}

void Team::addWorker() {
    if (m_gold >= m_costWorker*m_workers) {
        m_gold -= m_costWorker*m_workers;
        ++m_workers;
		m_transitionMoney.goPos((float)m_gold);
    }
}

int Team::getGold() {
    return m_gold;
}

void Team::decreaseGold(int _g) {
    m_gold -= _g;
	m_transitionMoney.goPos((float)m_gold);
}

void Team::update (std::queue<Event*> &_eventQueue) {
	
	if (++m_ticksToIncom == m_ticksPerIncom) {
		m_ticksToIncom = 0;
		m_gold += m_goldPerWorker*m_workers;
		m_transitionMoney.setPos((float)m_gold);
	}
	PlayerData dat = player.Update();
	switch (m_typeMove) {
		case CONSTANT_MOVE:
    		m_y += dat.moveDirection * m_vy;
		break;
		case ACCEL_MOVE:
		{
			if (0 != dat.moveDirection) m_v += dat.moveDirection * m_ay;
			else {
				float abs_v = fabs(m_v);
				if (abs_v < m_ay) m_v = 0;
				else m_v -= (float)((m_v/abs_v) * m_ay * 0.5);
			}

			if      (m_v > +m_vy) m_v = +m_vy;
			else if (m_v < -m_vy) m_v = -m_vy;
			m_y += m_v;
		}
		break;
	}

    if (m_y > GAMEFIELD_TOP-(TILE_SIZE/2)) {
		m_y = GAMEFIELD_TOP-(TILE_SIZE/2) ;
		m_v = 0;
	}
    else if (m_y < GAMEFIELD_BOTTOM+(TILE_SIZE/2)) {
		m_y = GAMEFIELD_BOTTOM+(TILE_SIZE/2);
		m_v = 0;
	}

    if (dat.createWorker) addWorker();
    for (int i = 0; i < UNIT_TYPES; ++i) {
        if (dat.createUnit[i]) {
            EventCreate* e = new EventCreate();
            e->team = m_playerPos;
            e->unitType = i;
            e->position = m_y;
            _eventQueue.push(e);
        }
    }

	m_transitionMoney.update(SECONDS_PER_UPDATE);
}

void Team::draw() 
{
	glColor4f(1,1,0,1);
	glBegin(GL_TRIANGLES);
    switch(m_playerPos) {
		case TEAM_LEFT:
		glVertex3f(GAMEFIELD_LEFT          ,m_y-(TILE_SIZE/2), 0);
		glVertex3f(GAMEFIELD_LEFT+TILE_SIZE,m_y, 0);
		glVertex3f(GAMEFIELD_LEFT          ,m_y+(TILE_SIZE/2), 0);
		break;
		case TEAM_RIGHT:
		glVertex3f(GAMEFIELD_RIGHT          ,m_y+(TILE_SIZE/2), 0);
		glVertex3f(GAMEFIELD_RIGHT-TILE_SIZE,m_y, 0);
		glVertex3f(GAMEFIELD_RIGHT          ,m_y-(TILE_SIZE/2), 0);
		break;
		default:
		break;
	}
	glEnd();

	if (!m_fontLoaded) {
		m_font.Load("data/font/nibby.ttf", FONT_SIZE);
		m_fontLoaded = true;
	    if (m_playerPos == TEAM_LEFT)
			 m_font.setAlignment(Font::TOP, Font::LEFT);
		else m_font.setAlignment(Font::TOP, Font::LEFT);
	}
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glScalef((-GAMEFIELD_CENTER_CORRECTION/FONT_SIZE)*0.5f,
	         (-GAMEFIELD_CENTER_CORRECTION/FONT_SIZE)*0.5f, 0);

	glColor4f(1,1,1,1);

	math::vec2f LbPos_gold, LbPos_worker;
	if (m_playerPos == TEAM_LEFT) {
		LbPos_gold   = math::vec2f(VIEWPORT_LEFT+ 2, GAMEFIELD_TOP-GAMEFIELD_CENTER_CORRECTION);
		LbPos_worker = math::vec2f(VIEWPORT_LEFT+32, GAMEFIELD_TOP-GAMEFIELD_CENTER_CORRECTION);
   	}                   
	else {
		LbPos_gold   = math::vec2f( 2, GAMEFIELD_TOP-GAMEFIELD_CENTER_CORRECTION);
		LbPos_worker = math::vec2f(32, GAMEFIELD_TOP-GAMEFIELD_CENTER_CORRECTION);
	}
	m_font.Print(LbPos_gold, "Gold: %d", (int)m_transitionMoney.getPos());
	m_font.Print(LbPos_worker, "Workers: %d", m_workers, m_costWorker*m_workers);

	glPopMatrix();
}
