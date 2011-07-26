#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "input/input.h"
#include "unit.h" //Por el define UNIT_TYPES

enum PlayerType { PLAYER_ONE, PLAYER_TWO };

struct PlayerData {
	float moveDirection;
	bool createWorker;
	int createUnit[UNIT_TYPES];
};

class Player {
public:
	Player();

	PlayerData Update();
	void setPlayerType(int _playerType);

private:
	int m_playerType;
};

#endif
