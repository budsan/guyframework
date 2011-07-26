#include "sprite.h"
#include "graphics.h"
#include "texturemanager.h"
#include "screen.h"
#include "color.h"

#include <iostream>
#include <cmath>

float Sprite::globalUnitsPerPixel = 1.0f;

Sprite::Sprite()
{
	unitsPerPixel = globalUnitsPerPixel;
	scaleWidth = 1.0f;
	scaleHeight= 1.0f;
	rotate = 0.0f;
}

void Sprite::Update(float GameTime)
{

}

void Sprite::setGlobalPixelsPerUnit(float value)
{
	globalUnitsPerPixel = 1.0f/value;
}

void Sprite::setPixelsPerUnit(float value)
{
	unitsPerPixel = 1.0f/value;
}

void Sprite::setScale(float value)
{
	scaleWidth = value;
	scaleHeight= value;
}

void Sprite::setScaleWidth(float value)
{
	scaleWidth = value;
}

void Sprite::setScaleHeight(float value)
{
	scaleHeight= value;
}

void Sprite::setRotation(float value)
{
	rotate = value;
}

float Sprite::getScaleWidth()
{
	return scaleWidth;
}

float Sprite::getScaleHeight()
{
	return scaleHeight;
}
float Sprite::getRotation()
{
	return rotate;
}

void Sprite::Draw()
{
	drawParams params;
	getParamsToDraw(params);
	if (params.filename == NULL) return;

	TextureManager *texman = TextureManager::pInstance();

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

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	preDrawing();

	glTranslatef(pos.x , pos.y, 0);
	glScalef(unitsPerPixel*scaleWidth, unitsPerPixel*scaleHeight, 0);
	glRotatef(rotate, 0, 0, 1);
	glTranslatef((float)-params.cx, (float)+params.cy, 0);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glVertexPointer  (2, GL_FLOAT, 0, vertcoordsArray);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoordsArray);
	glDrawArrays(GL_QUADS, 0, 4);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);	

	glPopMatrix();
	glPopAttrib();

	postDrawing();
}

void Sprite::preDrawing() {}
void Sprite::postDrawing(){}
