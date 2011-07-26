#include "graphics/graphics.h"
#include "graphics/texturemanager.h"
#include "base/defines.h"
#include "unit.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

bool Unit::s_drawingRanges = false;

bool Unit::inSight(math::vec2f _pos){
	if (m_team == LEFT_TEAM  && pos.x > _pos.x) return false;
	if (m_team == RIGHT_TEAM && pos.x < _pos.x) return false;	
	return (pos - _pos).module() < m_visionRange;
}

bool Unit::inAttack(math::vec2f _pos){
    if(m_team == LEFT_TEAM){  //La unidad se desplaza de izq a Der
		if(_pos.x - m_attackRange > pos.x) return false;
		if(_pos.y - m_attackRange > pos.y) return false;
	}
	if(m_team == RIGHT_TEAM){ //La unidad se desplaza de Der a izq
		if(_pos.x + m_attackRange < pos.x) return false;
		if(_pos.y + m_attackRange < pos.y) return false;
	}
	return (pos - _pos).module() < m_attackRange;
}

bool Unit::holeInSight(const Wall &_wall){
	math::vec2f WallPoint(_wall.x(), pos.y); //Es el punt mes proper de la muralla
	if(!inSight(WallPoint)) return false;
	float Y0, Y1;
	float Range = m_visionRange;
	//Pot ser que un dels dos es surti de pantalla, pero Wall ja ho sabra
	Y0 = pos.y+Range;
	Y1 = pos.y-Range; 
	return _wall.getHoleInRange(Y0, Y1, m_invadingZone);
}	

void Unit::setPositionY(float _positionY){
	pos = math::vec2f(pos.x, _positionY);
}

void Unit::setPositionX(float _positionX){
	pos = math::vec2f(_positionX, pos.y);
}

/*
void Unit::draw()
{
	float x = pos.x - (TILE_SIZE/4.0f);
	float y = pos.y - (TILE_SIZE/4.0f);
	
	if(m_team == LEFT_TEAM) glColor4f(1,0,0,1);
	else glColor4f(0,0,1,1);

	glBegin(GL_QUADS);
	glVertex3f( x              , y              , -1);
	glVertex3f( x+(TILE_SIZE/2), y              , -1);
	glVertex3f( x+(TILE_SIZE/2), y+(TILE_SIZE/2), -1);
	glVertex3f( x              , y+(TILE_SIZE/2), -1);
	glEnd();

#ifdef _DEBUG
	drawRanges();
#endif
}
*/

void Unit::drawRanges()
{
	if (!Unit::s_drawingRanges) return;

	float x = pos.x;
	float y = pos.y;
	
	float angle;
	if (m_team == LEFT_TEAM)  angle = (float)-M_PI/2;
	else					  angle = (float) M_PI/2;
	glColor4f(0.7f, 0.7f, 0.7f, 0.3f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f( x, y, -1);
	for (int i = 1; i <= 13; i++) {	
		glVertex3f( x + m_visionRange*cos(angle), 
					y + m_visionRange*sin(angle), 
					 -1);
		angle += (float)M_PI/12.0f;
	}
	glEnd();

	if (m_team == LEFT_TEAM)  angle = (float)-M_PI/2;
	else					  angle = (float) M_PI/2;
	glColor4f(0.3f, 0.3f, 0.3f, 0.3f);
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f( x, y,  -1);
	for (int i = 1; i <= 13; i++) {	
		glVertex3f( x + m_attackRange*cos(angle), 
					y + m_attackRange*sin(angle), 
					 -1);
		angle += (float)M_PI/12.0f;
	}
	glEnd();
}

void Unit::loadContent(SpriteAnimData* &unitData, const char* filename)
{
	if (unitData == NULL)
	{
		unitData = new SpriteAnimData();
		if (!unitData->Load(filename))
		{
			delete unitData;
			unitData = NULL;
			return;
		}
		
		std::set<std::string> content;
		unitData->getContentFilename(content);

		TextureManager &texman = TextureManager::Instance();
		std::set<std::string>::iterator it = content.begin();
		for (;it != content.end(); ++it) {
			texman.getTexture(*it);
		}
		
	}	
}

void Unit::unloadContent(SpriteAnimData* &unitData)
{
	if (unitData != NULL)
	{
		std::set<std::string> content;
		unitData->getContentFilename(content);

		TextureManager &texman = TextureManager::Instance();
		std::set<std::string>::iterator it = content.begin();
		for (;it != content.end(); ++it) {
			texman.DeleteTexture(*it);
		}

		delete unitData;
		unitData = NULL;
	}
}

void Unit::preDrawing()
{
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GEQUAL, 0.5f);

	if (m_team == RIGHT_TEAM)  {
		setScaleWidth(-1* getScaleWidth());
		glColor4f(0.8f, 0.8f, 1.0f, 1.0f);
	}
	else glColor4f(1.0f, 0.8f, 0.8f, 1.0f);
	//glTranslatef(0,0, -(pos.y/GAMEFIELD_TOP)*VIEWPORT_ZRADIO);
}

void Unit::postDrawing()
{
	glDisable(GL_ALPHA_TEST);
	if (m_team == RIGHT_TEAM)  {
		setScaleWidth(-1* getScaleWidth());
	}
}
