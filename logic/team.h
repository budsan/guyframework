#ifndef _TEAM_H_
#define _TEAM_H_

#include <queue>
#include <iostream>

#include "graphics/graphics.h"
#include "graphics/font.h"

#include "event.h"
#include "tools/transition.h"

#include "player.h"

#define FACTOR_COST_WORKER 15

enum TypeMove {
	CONSTANT_MOVE,
	ACCEL_MOVE
};

enum Teams {
	 TEAM_LEFT, 
	 TEAM_RIGHT, 
	 TEAM_NEUTRAL
	};

class Team {
public:
	 Team();
	~Team();
	
	void update (std::queue<Event*> &_eventQueue);
	void draw(); // Dibuja los marcadores de informacion de los jugadores
	
	void setPlayerPos(int _playerPos); //hay que llamarlo justo después de la constructora
	void setWorkers(int _workers);
	void setGold(int _gold);
	void setCostWorker(int _gold);
	void setTypeMove(TypeMove type);

	int getGold();
	void decreaseGold(int _g);
private:
	void addWorker();
	Player player;
	int m_playerPos; //izquierda/derecha: por el enum
	int m_gold;
	int m_workers;
	int m_goldPerWorker; //cuanto se gana por cada worker por update
	int m_costWorker; //cuanto vale crear un worker
	float m_y;
	float m_v;
	float m_vy; //la variable que cambia la aceleracion
	float m_ay;
	bool m_fontLoaded;
	int m_ticksToIncom, m_ticksPerIncom;
	TypeMove m_typeMove;
	Font m_font;

	TransitionSinusFadeOut m_transitionMoney;
};

#endif
