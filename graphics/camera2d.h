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

	void Init();
	void resizeScreen(int width, int height);
	void resizeScreen(int height); //Get width from screen's aspect ratio.

	math::mat4f getModelviewMatrix();
	math::mat4f getProjectionMatrix();
	void updateOpenGLMatrices();
	void reset();

	void  setPos(math::vec2f pos);
	math::vec2f getPos() const { return pos;}

	void  setZoom(float zoom);
	float getZoom() const {return zoom;}

	int width()  const {return w;}
	int height() const {return h;}

private:
	bool init;
	math::vec2f pos;
	float zoom;

	float aspect;
	int w, h;
};

#endif // CAMERA2D_H
