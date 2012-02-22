#include "camera.h"

void Camera::updateOpenGLMatrices()
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(getProjectionMatrix().v);

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(getModelviewMatrix().v);
}
