#pragma once

#include "math/algebra3.h"

namespace Guy {

class Sprite
{
public:
	Sprite();

	virtual void update(double deltaTime);
	void draw();

	static void setGlobalPixelsPerUnit(double value);
	       void setPixelsPerUnit(double value);

	void setScale(double value);
	void setScaleWidth(double value);
	void setScaleHeight(double value);
	void setRotation(double value);

	double getScaleWidth();
	double getScaleHeight();
	double getRotation();

	math::vec2d &pos() { return m_pos;}
private:
	static double s_globalUnitsPerPixel;
	       double m_unitsPerPixel;

	math::vec2d m_scale;
	double m_rotate;

protected:
	struct drawParams {
		const char* filename;
		int x, y, w, h, cx, cy;
	};
	virtual void getParamsToDraw(Sprite::drawParams &params) = 0;
	virtual void preDrawing();
	virtual void postDrawing();

	math::vec2d m_pos;
};

} // namespace Guy
