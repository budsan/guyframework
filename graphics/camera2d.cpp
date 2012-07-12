#include "camera2d.h"

#include "environment.h"
#include "graphics.h"
#include "screen.h"
#include "log.h"

#ifdef _WINDOWS
#undef min
#undef max
#endif

Camera2D::Camera2D()
{
	m_init = false;
	reset();
}

void Camera2D::init()
{
	const Screen::Mode& mode = Environment::instance().getScreenManager().getCurrentMode();
	resizeScreen(mode.w, mode.h);
}

math::bbox2f Camera2D::getBounding()
{
	float zoominv = 1.0f / m_zoom;
	float zoomw = zoominv * (float) m_w * 0.5f;
	float zoomh = zoominv * (float) m_h * 0.5f;
	math::vec2f min(m_pos.x - zoomw, m_pos.y - zoomh);
	math::vec2f max(m_pos.x + zoomw, m_pos.y + zoomh);
	return math::bbox2f(min, max);
}

math::mat4f Camera2D::getModelviewMatrix()
{
	return math::mat4f::fromIdentity();
}

math::mat4f Camera2D::getProjectionMatrix()
{
	if (!m_init) init();

	float zoominv = 1.0f / m_zoom;
	float zoomw = zoominv * (float) m_w * 0.5f;
	float zoomh = zoominv * (float) m_h * 0.5f;
	return math::mat4f::fromOrtho(m_pos.x - zoomw, m_pos.x + zoomw,
				      m_pos.y - zoomh, m_pos.y + zoomh , -1, 1);
}

void Camera2D::reset()
{
	m_pos = math::vec2f(0, 0);
	m_zoom = 1.0f;
}

void Camera2D::resizeScreen(int w, int h)
{
	m_w = w; m_h = h;
	m_aspect = (float)m_w/(float)m_h;
	m_init = true;
}

void Camera2D::resizeScreen(int h)
{
	Screen &screen = Environment::instance().getScreenManager();
	float ratio = screen.getCurrentMode().getRatio();
	if (ratio == 0)
	{
		const Screen::Mode& mode = screen.getCurrentMode();
		m_aspect = float(mode.w) / float(mode.h);
		m_w = (int)(float(h) * m_aspect);
		m_h = h;
		m_init = true;
	}
	else
	{
		m_aspect = ratio;
		m_w = (int)(float(h) * m_aspect);
		m_h = h;
		m_init = true;
	}
}

void Camera2D::setPos(math::vec2f pos)
{
	this->m_pos = pos;
}

void Camera2D::setZoom(float _zoom)
{
	m_zoom = _zoom;
}
