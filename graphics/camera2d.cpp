#include "camera2d.h"
#include "graphics.h"
#include "screen.h"
#include "log.h"

#ifdef _WINDOWS
#undef min
#undef max
#endif

Camera2D::Camera2D()
{
	init = false;
	reset();
}

void Camera2D::Init()
{
	const Screen::vmode* vmode = Screen::Instance().getCurrentVideoMode();
	resizeScreen(vmode->w, vmode->h);
}

math::bbox2f Camera2D::getBounding()
{
	float zoominv = (1.0f/zoom);
	float zoomw = zoominv*w*0.5f;
	float zoomh = zoominv*h*0.5f;
	math::vec2f min(pos.x-zoomw,pos.y-zoomh);
	math::vec2f max(pos.x+zoomw,pos.y+zoomh);
	return math::bbox2f(min,max);
}

void Camera2D::setOpenGLMatrices()
{
	if (!init)
	{
		LOG << "Warning: Must be call Init or resizeScreen" << std::endl;
		return;
	}

	Screen &screen = Screen::Instance();
	screen.resetViewport();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	float zoominv = (1.0f/zoom);
	float zoomw = zoominv*w*0.5f;
	float zoomh = zoominv*h*0.5f;
	glOrtho(pos.x-zoomw,pos.x+zoomw,
			pos.y-zoomh,pos.y+zoomh , -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Camera2D::reset()
{
	pos = math::vec2f(0, 0);
	zoom = 1.0f;
}

void Camera2D::resizeScreen(int width, int height)
{
	w = width; h = height;
	aspect = (float)w/(float)h;
	init = true;
}

void Camera2D::resizeScreen(int height)
{
	Screen &screen = Screen::Instance();
	float ratio = screen.getRatio();
	if (ratio == 0)
	{
		const Screen::vmode* vmode = screen.getCurrentVideoMode();
		aspect = float(vmode->w) / float(vmode->h);
		w = (int)(float(height) * aspect);
		h = height;
		init = true;
	}
	else
	{
		aspect = ratio;
		w = (int)(float(height) * aspect);
		h = height;
		init = true;
	}
}

void Camera2D::setPos(math::vec2f pos)
{
	this->pos = pos;
}

void Camera2D::setZoom(float _zoom)
{
	zoom = _zoom;
}
