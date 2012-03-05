#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <vector>

#include "graphics.h"
#include "math/vec2.h"
#include "math/bbox.h"

/*
template <typename T> glVertex(T val);
template <> glVertex<math::vec2f>(math::vec2f val) {glVertex2f(val.x, val.y);}
template <> glVertex<math::vec2d>(math::vec2d val) {glVertex2d(val.x, val.y);}
template <> glVertex<math::vec2i>(math::vec2i val) {glVertex2i(val.x, val.y);}
template <> glVertex<math::vec2s>(math::vec2s val) {glVertex2s(val.x, val.y);}

template <> glVertex<math::vec3f>(math::vec3f val) {glVertex3f(val.x, val.y, val.z);}
template <> glVertex<math::vec3d>(math::vec3d val) {glVertex3d(val.x, val.y, val.z);}
template <> glVertex<math::vec3i>(math::vec3i val) {glVertex3i(val.x, val.y, val.z);}
template <> glVertex<math::vec3s>(math::vec3s val) {glVertex3s(val.x, val.y, val.z);}
*/

void glVertex2(float  x, float  y);
void glVertex2(double x, double y);
void glVertex3(float  x, float  y, float  z);
void glVertex3(double x, double y, double z);

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

template <typename T, typename U, typename V>
void draw(math::bbox<math::vec2<T> > quad,
	  std::vector<math::vec2<U> > &vertcoords,
	  std::vector<V> &indices)
{
	int indBase = vertcoords.size();
	vertcoords.push_back(math::vec2f(quad.min.x, quad.min.y));
	vertcoords.push_back(math::vec2f(quad.max.x, quad.min.y));
	vertcoords.push_back(math::vec2f(quad.max.x, quad.max.y));
	vertcoords.push_back(math::vec2f(quad.min.x, quad.max.y));

	indices.push_back(indBase + 3);
	indices.push_back(indBase + 0);
	indices.push_back(indBase + 1);
	indices.push_back(indBase + 1);
	indices.push_back(indBase + 2);
	indices.push_back(indBase + 3);
}


#endif // PRIMITIVES_H
