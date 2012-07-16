#pragma once

#include "graphics.h"
#include "math/algebra3.h"
#include "math/bbox.h"

namespace Guy {

template <typename T>
void draw(math::bbox<math::vec2<T> > quad)
{
	float vertcoords[] = {
			quad.min.x, quad.min.y,
			quad.max.x, quad.min.y,
			quad.max.x, quad.max.y,
			quad.min.x, quad.max.y
	};

	unsigned short indices[] = { 3, 0, 1, 1, 2, 3 };

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer  (2, GL_FLOAT, 0, vertcoords);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, indices);
	glDisableClientState(GL_VERTEX_ARRAY);
}

} // namespace Guy

