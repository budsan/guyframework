#include "player.h"

#include "input/input.h"
#include "base/settings.h"
#include "unit.h"

Player::Player() {}

void Player::setPlayerType(int _playerType)
{
	m_playerType = _playerType;
}

PlayerData Player::Update()
{
	Input &input = Input::Instance();
	const InputState &state = input.getInputState(m_playerType);
	PlayerData pdata;
	
	pdata.moveDirection = 0.0f;
	if (state.getKeyState(K_UP) && !state.getKeyState(K_DOWN)) pdata.moveDirection =  1.0f;
	if (state.getKeyState(K_DOWN) && !state.getKeyState(K_UP)) pdata.moveDirection = -1.0f;

	if (state.getKeyDown(K_WORKER)) pdata.createWorker = true;
	else pdata.createWorker = false;

	for (int i = 0; i < UNIT_TYPES; i++) pdata.createUnit[i] = 0;
	if (state.getKeyDown(K_UNIT1)) pdata.createUnit[0] = 1;                  
	if (state.getKeyDown(K_UNIT2)) pdata.createUnit[1] = 1;
	if (state.getKeyDown(K_UNIT3)) pdata.createUnit[2] = 1;
	if (state.getKeyDown(K_UNIT4)) pdata.createUnit[3] = 1;

	return pdata;
}

