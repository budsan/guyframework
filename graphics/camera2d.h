#ifndef CAMERA2D_H
#define CAMERA2D_H

#include "camera.h"

#include "math/algebra3.h"
#include "math/bbox.h"

class Camera2D : public Camera
{
public:
	Camera2D();
	math::bbox2f getBounding();

	void init();
	void resizeScreen(int width, int height);
	void resizeScreen(int height); //Get width from screen's aspect ratio.

	math::mat4f getModelviewMatrix();
	math::mat4f getProjectionMatrix();
	void reset();

	void  setPos(math::vec2f m_pos);
	math::vec2f getPos() const { return m_pos;}

	void  setZoom(float m_zoom);
	float getZoom() const {return m_zoom;}

	int width()  const {return m_w;}
	int height() const {return m_h;}

private:
	bool m_init;
	math::vec2f m_pos;
	float m_zoom;

	float m_aspect;
	int m_w, m_h;
};

#endif // CAMERA2D_H
