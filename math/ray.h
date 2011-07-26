#ifndef _MATH_RAY_DEFINED_
#define _MATH_RAY_DEFINED_

#include "vec2.h"
#include "vec3.h"

namespace math {

template <typename T>
struct ray
{
	ray() : p(T()) {}
	ray(T _p, T _v) : p(_p), v(_v) {}

	T p; // origen
	T v; // vector que define la recta (normalizado)
};

//un rayo 1D no tiene mucho sentido
//omitimos los typedef 1D.
typedef ray<vec2<char> >           ray2c;
typedef ray<vec2<unsigned char> >  ray2uc;
typedef ray<vec2<short> >          ray2s;
typedef ray<vec2<unsigned short> > ray2us;
typedef ray<vec2<int> >            ray2i;
typedef ray<vec2<unsigned int> >   ray2ui;
typedef ray<vec2<long> >           ray2l;
typedef ray<vec2<unsigned long> >  ray2ul;
typedef ray<vec2<float> >          ray2f;
typedef ray<vec2<double> >         ray2d;

typedef ray<vec3<char> >           ray3c;
typedef ray<vec3<unsigned char> >  ray3uc;
typedef ray<vec3<short> >          ray3s;
typedef ray<vec3<unsigned short> > ray3us;
typedef ray<vec3<int> >            ray3i;
typedef ray<vec3<unsigned int> >   ray3ui;
typedef ray<vec3<long> >           ray3l;
typedef ray<vec3<unsigned long> >  ray3ul;
typedef ray<vec3<float> >          ray3f;
typedef ray<vec3<double> >         ray3d;

} //namespace math

#endif
