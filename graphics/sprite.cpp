#include "sprite.h"
#include "graphics.h"
#include "texturemanager.h"
#include "screen.h"
#include "color.h"

#include <iostream>
#include <cmath>

float Sprite::s_globalUnitsPerPixel = 1.0f;

Sprite::Sprite()
{
	m_unitsPerPixel = s_globalUnitsPerPixel;
	m_scaleWidth = 1.0f;
	m_scaleHeight= 1.0f;
	m_rotate = 0.0f;
}

void Sprite::update(float deltaTime)
{

}

void Sprite::setGlobalPixelsPerUnit(float value)
{
	s_globalUnitsPerPixel = 1.0f/value;
}

void Sprite::setPixelsPerUnit(float value)
{
	m_unitsPerPixel = 1.0f/value;
}

void Sprite::setScale(float value)
{
	m_scaleWidth = value;
	m_scaleHeight= value;
}

void Sprite::setScaleWidth(float value)
{
	m_scaleWidth = value;
}

void Sprite::setScaleHeight(float value)
{
	m_scaleHeight= value;
}

void Sprite::setRotation(float value)
{
	m_rotate = value;
}

float Sprite::getScaleWidth()
{
	return m_scaleWidth;
}

float Sprite::getScaleHeight()
{
	return m_scaleHeight;
}
float Sprite::getRotation()
{
	return m_rotate;
}

void Sprite::draw()
{
	drawParams params;
	getParamsToDraw(params);
	if (params.filename == nullptr) return;

	TextureManager *texman = TextureManager::ptrInstance();

	const Texture &tex = texman->getTexture(params.filename);
	tex.setFiltering(GL_NEAREST);
	tex.setWrap(GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, rgba(0,0,0,0).raw());

	struct { float x0, y0, x1, y1; } normCoords;
	normCoords.x0 = float(params.x)/float(tex.w());
	normCoords.y0 = 1.0f - (float(params.y)/float(tex.h()));
	normCoords.x1 = (float)normCoords.x0 +(float(params.w)/float(tex.w()));
	normCoords.y1 = (float)normCoords.y0 -(float(params.h)/float(tex.h()));

	struct { float u, v; } texcoordsArray[4] = {
		{ normCoords.x0, normCoords.y0 },
		{ normCoords.x0, normCoords.y1 },
		{ normCoords.x1, normCoords.y1 },
		{ normCoords.x1, normCoords.y0 }
	};

	float h = abs(params.h), w = abs(params.w);
	struct { float x, y; } vertcoordsArray[4] = {
		{0, 0},{0,-h},{w,-h},{w, 0}
	};

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	preDrawing();

	glTranslatef(m_pos.x , m_pos.y, 0);
	glScalef(m_unitsPerPixel*m_scaleWidth, m_unitsPerPixel*m_scaleHeight, 0);
	glRotatef(m_rotate, 0, 0, 1);
	glTranslatef((float)-params.cx, (float)+params.cy, 0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer  (2, GL_FLOAT, 0, vertcoordsArray);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoordsArray);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);	

	glPopMatrix();
	postDrawing();
}

void Sprite::preDrawing() {}
void Sprite::postDrawing(){}
