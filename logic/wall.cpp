#include "wall.h"
#include <cassert>

#include "graphics/graphics.h"
#include "graphics/spritestatic.h"
#include "team.h"


Wall::Wall(unsigned int numBricks) : m_numBricks(numBricks) , m_skin(NULL) {
	m_brickWidth = GAMEFIELD_HEIGHT/float(m_numBricks);
	m_bricks.resize(m_numBricks);

	for (unsigned int i = 0; i < m_bricks.size(); i++)
	{
		m_bricks[i].m_posY = (float(i)*m_brickWidth)-GAMEFIELD_TOP;
	}
}

Wall::~Wall(){
	//You can't do it Starfox
}

void Wall::Draw()
{
	if (m_skin == NULL || m_skin->Empty())
	{
		DrawNoSkin();
		return;
	}

	SpriteStatic wallsprite(m_skin);
	wallsprite.position().x = m_posX;
	int numsprites = m_skin->frames.size() -1;

	switch(m_team)
	{
		case TEAM_LEFT: wallsprite.setScaleWidth( 1) ;break;
		case TEAM_RIGHT:wallsprite.setScaleWidth(-1) ;break;
		default: break;
	}

	for (unsigned int i = 0; i < m_bricks.size(); i++)
	{
		int frame = floor(numsprites* (1.0f - (
			float(m_bricks[i].m_currentBrickHealth)/
			float(m_bricks[i].m_initialBrickHealth))));

		wallsprite.SelectFrame(frame);
		wallsprite.position().y = (i+0.5f)*m_brickWidth - GAMEFIELD_TOP;
		wallsprite.setScaleHeight(PIXELS_PER_UNIT*m_brickWidth/(float)m_skin->frames[frame].h);
		glColor4f(1,1,1,1);
		wallsprite.Draw();
	}
}

void Wall::DrawNoSkin()
{
	float R, G, B, A;
	float x = m_posX - (TILE_SIZE/2);
	switch(m_team) {
		case TEAM_LEFT:
			R = 0.4f; G = 0.275f; B = 0.275f;
			x -= (TILE_SIZE/2);
			break;
		case TEAM_RIGHT:
			R = 0.275f; G = 0.275f; B = 0.4f;
			x += (TILE_SIZE/2);
			break;
		default:
			R = 0.4f; G = 0.4f; B = 0.4f;
			break;
	}

	for (unsigned int i = 0; i < m_bricks.size(); i++)
	{
		A = float(m_bricks[i].m_currentBrickHealth)/float(m_bricks[i].m_initialBrickHealth);

		glColor4f(R,G,B,A);
		glBegin(GL_QUADS);
		glVertex3f( x          ,((i+0)*m_brickWidth)-GAMEFIELD_TOP, 0);
		glVertex3f( x+TILE_SIZE,((i+0)*m_brickWidth)-GAMEFIELD_TOP, 0);
		glVertex3f( x+TILE_SIZE,((i+1)*m_brickWidth)-GAMEFIELD_TOP, 0);
		glVertex3f( x          ,((i+1)*m_brickWidth)-GAMEFIELD_TOP, 0);
		glEnd();
	}
}

void Wall::setWallHealth(int _health)
{
	for (unsigned int i = 0; i < m_bricks.size(); i++)
	{
		m_bricks[i].m_initialBrickHealth = _health;
		m_bricks[i].m_currentBrickHealth = _health;
	}
}

void Wall::setWallSkin(SpriteLibrary *skin)
{
	m_skin = skin;
}

void Wall::DamageBrick(float _posY, int _damage){
	//Li fot mal al brick de la posicio X
	int nBrick = int(_posY+GAMEFIELD_TOP)/m_brickWidth; //Posicio en el vector de bricks
	m_bricks[nBrick].m_currentBrickHealth-=_damage; //LogicEngine ja veura si ha pringat o no
}

int Wall::getBricksHealth(float _posY) const { 
	//Retorna la vida del brick de la posicio X :D!	
	int nBrick = int(_posY+GAMEFIELD_TOP)/m_brickWidth; //Posicio en el vector de bricks
	assert(nBrick < (int)m_bricks.size());
	assert(nBrick >= 0);
	return m_bricks[nBrick].m_currentBrickHealth;
}

float Wall::getBrickByPos(float _posY) const {
	int nBrick = int(_posY+GAMEFIELD_TOP)/m_brickWidth;
	return m_bricks[nBrick].m_posY + (m_brickWidth/2);
}

bool Wall::getHoleInRange(float _posY0, float _posY1, math::vec2f &_Hole) const
{
	int nMinBrick = (_posY1-GAMEFIELD_BOTTOM)/m_brickWidth;
	int nMaxBrick = (_posY0-GAMEFIELD_BOTTOM)/m_brickWidth;
	//Comprovacions per si la Unit veu mes enlla del camp de joc
	if(nMinBrick < 0) nMinBrick = 0;
	if(nMaxBrick >= (int)m_bricks.size()) nMaxBrick = m_bricks.size()-1;
	//Busca el forat primer per el mig (que es on la unit el te mes aprop i despres per el final
	int midPoint1 = nMaxBrick-(nMaxBrick-nMinBrick)/2;
	int midPoint2 = midPoint1+1; 
	if(midPoint2 >= (int)m_bricks.size()) --midPoint2;
	//Recorrem del centre cap als costats buscant un foradot
	if(m_bricks[midPoint1].m_currentBrickHealth <= 0) {
		_Hole = math::vec2f(m_posX, m_bricks[midPoint1].m_posY+(m_brickWidth/2));
		return true;
	}
	--midPoint1;
	for (int i = 1; i < (nMaxBrick-nMinBrick); ++i){
		if(i%2 == 0){
			if(m_bricks[midPoint1].m_currentBrickHealth <= 0){
				_Hole = math::vec2f(m_posX, m_bricks[midPoint1].m_posY+(m_brickWidth/2));
				return true;
			}
			--midPoint1;
		}
		else{
			if(m_bricks[midPoint2].m_currentBrickHealth <= 0) {
				_Hole = math::vec2f(m_posX, m_bricks[midPoint2].m_posY+(m_brickWidth/2));
				return true;
			} 
			++midPoint2;
		}
	}
	
	return false;	
}
