#include <cmath>
#include "unitdefense.h"
#include "graphics/graphics.h"

#ifdef _DEBUG
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#endif

UnitDefense::UnitDefense(int _ID): Unit(_ID) {
	m_state = STATE_WALK;
	SelectAnim("Walk");
	
	// DEFENSE PARAMETERS
	m_attack = 1; //int
	m_attackRate = 150; //int
	m_defense = 6; //int
	m_health = 42; //int
	m_visionRange = 10.0f; //float
	m_attackRange = 3.0f; //float
	m_speed  = 0.05f; //float

	m_enemyUnit = NULL;
	m_attackingEnemyID = -1;
	m_invadingZone = math::vec2f();
	m_attackRateCount = 0;

	setAnimData(unitData);
}

UnitDefense::~UnitDefense(){
	//Nothing
}

SpriteAnimData *UnitDefense::unitData = NULL;
void UnitDefense::loadContent()
{
	Unit::loadContent(unitData, "data/scripts/unit_defense.anim");
}

void UnitDefense::unloadContent()
{
	Unit::unloadContent(unitData);
}

void UnitDefense::update (
		const std::vector<Unit*>& _units,
		const std::vector<Wall>&  _walls,
		std::queue<Event*>& _eventQueue)
{
	SpriteAnim::Update(SECONDS_PER_UPDATE);
	
	// Miro si estic mort.
	if (m_health <= 0 && m_state != STATE_DEAD) {
		m_state = STATE_DEAD;
		SelectAnim("Dead");
	}

	// Calculem UN COP on esta mirant la Unit.
	float facing;
	if(m_team == LEFT_TEAM) facing =  1.0f;
	else                    facing = -1.0f;

	// Calculem si hi ha la posibilitat de interectuar amb alguna wall.
	const Wall *targetWall = NULL;
	float wallDist = m_visionRange;
	for (unsigned int i = 0; i < _walls.size(); i++) {
		float auxd = (_walls[i].x()-pos.x)*facing;

		if (auxd >=0             &&
		    auxd < m_visionRange &&
		    auxd < wallDist      &&
		    _walls[i].getWallTeam() != m_team)
		{
			targetWall = &_walls[i];
			wallDist = auxd;
		}
	}

#ifdef _DEBUG
#define MAX_STATE_CHANGES 32
	int StateChanges = 0;
	int StatesChanged[MAX_STATE_CHANGES];
#endif

	// Etiqueta goto reupdate_state
	// Teoricament, una Unit si pogues fer tants canvis d'estat com pogues
	// en un update, hi hauria un punt que no hauria de canviar d'estat
	// perque totes les condicions indiquen que es el estat que li toca ser
	// Aquest goto es per detectar errades en les condicions dels canvis
	// d'estat.
	reupdate_state:

#ifdef _DEBUG
	StatesChanged[StateChanges++] = m_state;
	if (StateChanges >= MAX_STATE_CHANGES)
		assertStateChanges(MAX_STATE_CHANGES, StatesChanged);
#endif
	switch(m_state){
		case STATE_WALK:
		{
			if(targetWall != NULL){ //Tenim muralla
				//Si la unitat esta en estat walk i veu un forat a la muralla
				if(holeInSight(*targetWall)){
					if(m_invadingZone.y != pos.y) {
						m_state = STATE_INVADE; SelectAnim("Invade");
						goto reupdate_state;
					}
				}
				//Si arribem al final (muralla), pasem a DESTROY
				else if (wallDist == 0) {
					m_state = STATE_DESTROY;
					SelectAnim("Destroy"); 
					goto reupdate_state;
				}
			}
			//Altrament, mirar si hi ha algu al rang de visio i pasar a CHASE
			m_enemyUnit = NULL;
			float enemyUnitDist = m_visionRange;
			for(unsigned int i = 0; i < _units.size(); ++i){
				if(_units[i] == NULL) continue;
				//Si es enemiga i esta viva
				if(_units[i]->getTeam() != m_team &&
				   _units[i]->getHealth() > 0) {
					//Si esta darrera una muralla, ignorem
					if (targetWall != NULL &&
					   (targetWall->x()-_units[i]->x())*facing > 0) continue;
					//Si no esta dins del camp de visio, ignorem
					if (!inSight(_units[i]->position())) continue;
					//Si no esta mes a prop que els anteriors, ignorem
					float auxd = (pos - _units[i]->position()).module();
					if ( auxd < enemyUnitDist) 
					{
						m_enemyUnit = _units[i];
						enemyUnitDist = auxd;
					}
				}
			}

			if(m_enemyUnit != NULL) {
					m_state = STATE_CHASE;
					SelectAnim("Chase");
					goto reupdate_state;
			}
		}
		break;
		case STATE_CHASE:
			//comprovar que segueix en rang de visio, altrament pasem a WALK
			if (m_enemyUnit == NULL || m_enemyUnit->getHealth() <= 0 || !inSight(m_enemyUnit->position()))
			{			
				m_enemyUnit = NULL;
				m_state = STATE_WALK;
				SelectAnim("Walk");
				goto reupdate_state;
			}
			//esta en rang d'atac?  si es aixi pasem a ATTACK
			if(inAttack(m_enemyUnit->position())){
				m_state = STATE_ATTACK;
				SelectAnim("Attack");
				goto reupdate_state;
			}
			//en qualsevol altre cas, seguirem en CHASE
		break;
		case STATE_ATTACK: {
			//si no segueix dins del rang o ha mort, tornem a caminar
			if(m_enemyUnit == NULL || m_enemyUnit->getHealth() <= 0 || !inAttack(m_enemyUnit->position())) {
				
				m_enemyUnit = NULL;
				m_attackRateCount = 0;
				m_state = STATE_WALK;
				SelectAnim("Walk");
				goto reupdate_state;
			}
			else {
				//altrament, enviem event d'atac i seguirem en ATTACK
				m_attackRateCount++;
				if(m_attackRateCount >= m_attackRate)
				{
					EventAttackUnit* atev = new EventAttackUnit;
					atev->attack = m_attack;
					atev->origin = m_ID;
					atev->target = m_enemyUnit->getID();
					_eventQueue.push((Event*)atev);

					m_attackRateCount = 0;
				}
			}
		}
		break;
		case STATE_DEAD:
		{
			if ( getLoopsLeft() <= 0)
			{
				EventRemoveUnit* rmev = new EventRemoveUnit();
				rmev->target = m_ID;
				_eventQueue.push((Event*)rmev);
			}
		}
		break;
		case STATE_DESTROY:
			//Per si de cas, si no hi ha muralla, pasem a WALK
			if(targetWall == NULL) {
				m_attackRateCount = 0;
				m_state = STATE_WALK;
				SelectAnim("Walk");
				goto reupdate_state;
			}

			//Si hi ha forat, pasem a INVADE
			if(holeInSight(*targetWall)){
				m_attackRateCount = 0;
				m_invadingZone.x = targetWall->x();
				m_invadingZone.y = targetWall->getBrickByPos(pos.y);
				m_state = STATE_INVADE;
				SelectAnim("Invade");
				goto reupdate_state;
			}
			//altrament, ataquem i seguim en DESTROY
			else {
				m_attackRateCount++;
				if(m_attackRateCount >= m_attackRate)
				{
					EventAttackWall* awev = new EventAttackWall;
					awev->attack = m_attack;
					awev->positionY = pos.y;
					awev->targetWall = targetWall->getWallID();
					_eventQueue.push((Event*)awev);

					m_attackRateCount -= m_attackRate;
				}
			}
	 	break;
		case STATE_INVADE:
			//si ens han atacat i el tenim en rang d'atac, passem a ATTACK
			if (m_attackingEnemyID != -1 && inAttack(_units[m_attackingEnemyID]->position())) {
				m_enemyUnit = _units[m_attackingEnemyID];
				m_attackingEnemyID = -1;
				m_state = STATE_ATTACK;
				SelectAnim("Attack");
				goto reupdate_state;		
			}
			// si estem aliniats amb el forat, passem a WALK
			if ( m_invadingZone.y == pos.y) {
				m_state = STATE_WALK;
				SelectAnim("Walk");
				goto reupdate_state;
			}
			//en qualsevol altre cas, seguirem en INVADE
		break;
		default:
		break;
	}

	m_attackingEnemyID = -1;
}

int UnitDefense::hurt (int _attack, int _origin) {
	int damage = _attack - m_defense; //per exemple
	if(damage <= 0) damage = 1; //Aixi evitem inmortals xDDD
	m_health -= damage;
	m_attackingEnemyID = _origin;
	
	return damage;
}

void UnitDefense::move (
	const std::vector<Unit*>& _units,
	const std::vector<Wall>&  _walls) 
{
	if (m_health <= 0) return;

	// Calculem UN COP on esta mirant la Unit.
	float facing;
	if(m_team == LEFT_TEAM) facing =  1.0f;
	else                    facing = -1.0f;

	// Calculem si hi ha la posibilitat de interectuar amb alguna wall.
	const Wall *targetWall = NULL;
	float wallDist = m_visionRange;
	for (unsigned int i = 0; i < _walls.size(); i++) {
		float auxd = (_walls[i].x()-pos.x)*facing;

		if (auxd >=0             &&
		    auxd < m_visionRange &&
		    auxd < wallDist      &&
		    _walls[i].getWallTeam() != m_team)
		{
			targetWall = &_walls[i];
			wallDist = auxd;
		}
	}

	math::vec2f movement(0,0);
	switch(m_state){
		case STATE_WALK:
			movement = math::vec2f(m_speed*facing, 0.0f);
		break;
		case STATE_CHASE:
			if (m_enemyUnit == NULL) break;

			movement = (m_enemyUnit->position() - pos);
			if (movement.module() > m_speed) {
				movement.normalize();
				movement *= m_speed;
			}
		break;
		case STATE_INVADE:
			movement = (m_invadingZone - pos);
			if (movement.module() > m_speed) {
				movement.normalize();
				movement *= m_speed;
			}
		break;
		default:
		break;
	}
	
	if (targetWall != NULL && wallDist < (movement.x*facing) &&
		targetWall->getBricksHealth(pos.y + movement.y) > 0) {
			movement.x = wallDist*facing;
	}
	
	pos += movement;
}

#ifdef _DEBUG
void UnitDefense::assertStateChanges(int _numStateChanges, int* _statesStack)
{
	fprintf(stderr, "UnitDefense assert: too many state changes in one update\n\n");
	fprintf(stderr, "States stack dumped:\n");
	for ( int i = 0 ; i < _numStateChanges ; i++)
	{
		switch ( _statesStack[i] )
		{
			case STATE_WALK:   fprintf(stderr, "STATE_WALK\n"   ); break;
			case STATE_CHASE:  fprintf(stderr, "STATE_CHASE\n"  ); break;
			case STATE_ATTACK: fprintf(stderr, "STATE_ATTACK\n" ); break;
			case STATE_DEAD:   fprintf(stderr, "STATE_DEAD\n"   ); break;
			case STATE_DESTROY:fprintf(stderr, "STATE_DESTROY\n"); break;
			case STATE_INVADE: fprintf(stderr, "STATE_INVADE\n" ); break;
			default: fprintf(stderr, "STATE_UNKNOWN\n"); break;
		}
	}

	exit(0);
}
#endif
