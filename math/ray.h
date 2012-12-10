#ifndef _MATH_RAY_DEFINED_
#define _MATH_RAY_DEFINED_

#include "algebra3.h"

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
		: abort(false), gs(gridsize) {
		r.p = orig;
		r.v = dest - orig;
		tmax = r.v.module();
		r.v.normalize();
	}

	grid2_traversal(const vec2<Real> &gridsize,
			const ray<vec2<Real> > &_r2,
			Real tmax)
		: abort(false), tmax(tmax), gs(gridsize) {
		r = _r2;
		r.v.normalize();
	}

	void run()
	{
		vec2<Real> invgs(1.0/gs.x, 1.0/gs.y);
		vec2i vox(floor(r.p.x*invgs.x), floor(r.p.y*invgs.y));
		vec2i step(((r.v.x*invgs.x >= 0)<<1)-1, //(r2.v.x >= 0)?+1:-1;
			   ((r.v.y*invgs.y >= 0)<<1)-1);//(r2.v.x >= 0)?+1:-1;

		vec2<Real> tmaxc(tmax,tmax), tdelta;
		if (r.v.x != 0)
		{
			tdelta.x = gs.x/r.v.x;
			tmaxc.x = (vox.x*gs.x - r.p.x)/r.v.x;
		}

		if (r.v.y != 0)
		{
			tdelta.y = gs.y/r.v.y;
			tmaxc.y = (vox.y*gs.y - r.p.y)/r.v.y;
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
	const ray<vec2<Real> > &ray2() {return r;}

private:

	bool abort;
	Real tmax;
	vec2<Real> gs;
	ray<vec2<Real> > r;
};

} //namespace math

#endif
