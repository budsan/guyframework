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


//Based on: Fast Voxel Traversal Algorithm for Ray Tracing
//By: John Amanatides at al.
template<typename Real>
class grid2_traversal
{
public:
	grid2_traversal(const vec2<Real> &gridsize,
			const vec2<Real> &orig,
			const vec2<Real> &dest)
		: abort(false), gridsize(gridsize) {
		ray2.p = orig;
		ray2.v = dest - orig;
		tmax = ray2.v.module();
		ray2.v.normalize();
	}

	grid2_traversal(const vec2<Real> &gridsize,
			const ray<vec2<Real> > &_r2,
			Real tmax)
		: abort(false), tmax(tmax), gridsize(gridsize) {
		ray2 = _r2;
		ray2.v.normalize();
	}

	void run()
	{
		vec2<Real> invgs(1.0/gridsize.x, 1.0/gridsize.y);
		vec2i vox(floor(ray2.p.x*invgs.x), floor(ray2.p.y*invgs.y));
		vec2i step(((ray2.v.x*invgs.x >= 0)<<1)-1, //(r2.v.x >= 0)?+1:-1;
			   ((ray2.v.y*invgs.y >= 0)<<1)-1);//(r2.v.x >= 0)?+1:-1;

		vec2<Real> tmaxc(tmax,tmax), tdelta;
		if (ray2.v.x != 0)
		{
			tdelta.x = tmax/ray2.v.x;
			tmaxc.x = (vox.x - ray2.p.x) * tdelta.x;
		}

		if (ray2.v.y != 0)
		{
			tdelta.y = tmax/ray2.v.y;
			tmaxc.y = (vox.y - ray2.p.y) * tdelta.y;
		}

		Real tcurr = 0;
		while(!abort && tcurr < tmax)
		{
			if(tmaxc.x < tmaxc.y)
			{
				tcurr = tmaxc.x;
				tmaxc.x = tmaxc.x + tdelta.x;
				vox.x  = vox.x  + step.x;
			}
			else
			{
				tcurr = tmaxc.x;
				tmaxc.y = tmaxc.y + tdelta.y;
				vox.y  = vox.y  + step.y;
			}
			next(vox, tcurr);
		}

	}

protected:

	void exit() { abort = true; }
	virtual void next(const vec2i &voxel, Real tcurr) = 0;
	const ray<vec2<Real> > &ray() {return ray2;}

private:

	bool abort;
	Real tmax;
	vec2<Real> gridsize;
	ray<vec2<Real> > ray2;
};

} //namespace math

#endif
