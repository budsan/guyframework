#pragma once

#include "graphics.h"
#include "math/algebra3.h"

namespace Guy {

class Camera
{
public:
	Camera() {}

	virtual math::mat4f getModelviewMatrix()  = 0;
	virtual math::mat4f getProjectionMatrix() = 0;
};

} //namespace Guy
