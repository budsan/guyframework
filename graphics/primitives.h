#ifndef PRIMITIVES_H
#define PRIMITIVES_H

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
	glBegin(GL_QUADS);
	glTexCoord2f(0,0);
	glVertex2(quad.min.x, quad.min.y);
	glTexCoord2f(1,0);
	glVertex2(quad.max.x, quad.min.y);
	glTexCoord2f(1,1);
	glVertex2(quad.max.x, quad.max.y);
	glTexCoord2f(0,1);
	glVertex2(quad.min.x, quad.max.y);
	glEnd();
}


#endif // PRIMITIVES_H
