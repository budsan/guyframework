#pragma once

#include "math/vec2.h"

class Sprite
{
 public:
	Sprite();

	virtual void update(float deltaTime);
	void draw();

	static void setGlobalPixelsPerUnit(float value);
	       void       setPixelsPerUnit(float value);

	void setScale(float value);
	void setScaleWidth(float value);
	void setScaleHeight(float value);
	void setRotation(float value);

	float getScaleWidth();
	float getScaleHeight();
	float getRotation();

	math::vec2f &pos() { return m_pos;}
 private:
	static float s_globalUnitsPerPixel;
	       float       m_unitsPerPixel;

	float m_scaleWidth;
	float m_scaleHeight;
	float m_rotate;

 protected:
	struct drawParams {
		const char* filename;
		int x, y, w, h, cx, cy;
	};
	virtual void getParamsToDraw(Sprite::drawParams &params) = 0;
	virtual void preDrawing();
	virtual void postDrawing();

	math::vec2f m_pos;
};
