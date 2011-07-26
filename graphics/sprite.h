#pragma once

#include "math/vec2.h"

class Sprite
{
 public:
	Sprite();

	virtual void Update(float GameTime);
	void Draw();

	static void setGlobalPixelsPerUnit(float value);
	       void       setPixelsPerUnit(float value);

	void setScale(float value);
	void setScaleWidth(float value);
	void setScaleHeight(float value);
	void setRotation(float value);

	float getScaleWidth();
	float getScaleHeight();
	float getRotation();

	math::vec2f &position() { return pos;}
 private:
	static float globalUnitsPerPixel;
	       float       unitsPerPixel;

	float scaleWidth;
	float scaleHeight;
	float rotate;

 protected:
	struct drawParams {
		const char* filename;
		int x, y, w, h, cx, cy;
	};
	virtual void getParamsToDraw(Sprite::drawParams &params) = 0;
	virtual void preDrawing();
	virtual void postDrawing();

	math::vec2f pos;
};
