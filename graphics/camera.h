#pragma once

#include "graphics.h"
#include "math/algebra3.h"

namespace Guy {

class Camera
{
public:
	Camera() {}

	virtual math::mat4f viewMatrix()  = 0;
	virtual math::mat4f projectionMatrix() = 0;
};

} //namespace Guy
