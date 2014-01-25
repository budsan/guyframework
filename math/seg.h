#ifndef _MATH_SEG_DEFINED_
#define _MATH_SEG_DEFINED_

#include "algebra3.h"
#include "bbox.h"

namespace math {

template <typename T>
struct seg //linesegment
{
	T b, e; //begin, end.

	seg() : b(T()), e(T()) {}
	seg(T _b, T _e) : b(_b), e(_e) {}

	seg &operator=(const seg &other)
	{
		b = other.b;
		e = other.e;
		return *this;
	}
};

typedef seg<char>           seg1c;
typedef seg<unsigned char>  seg1uc;
typedef seg<short>          seg1s;
typedef seg<unsigned short> seg1us;
typedef seg<int>            seg1i;
typedef seg<unsigned int>   seg1ui;
typedef seg<long>           seg1l;
typedef seg<unsigned long>  seg1ul;
typedef seg<float>          seg1f;
typedef seg<double>         seg1d;

typedef seg<vec2<char> >           seg2c;
typedef seg<vec2<unsigned char> >  seg2uc;
typedef seg<vec2<short> >          seg2s;
typedef seg<vec2<unsigned short> > seg2us;
typedef seg<vec2<int> >            seg2i;
typedef seg<vec2<unsigned int> >   seg2ui;
typedef seg<vec2<long> >           seg2l;
typedef seg<vec2<unsigned long> >  seg2ul;
typedef seg<vec2<float> >          seg2f;
typedef seg<vec2<double> >         seg2d;

typedef seg<vec3<char> >           seg3c;
typedef seg<vec3<unsigned char> >  seg3uc;
typedef seg<vec3<short> >          seg3s;
typedef seg<vec3<unsigned short> > seg3us;
typedef seg<vec3<int> >            seg3i;
typedef seg<vec3<unsigned int> >   seg3ui;
typedef seg<vec3<long> >           seg3l;
typedef seg<vec3<unsigned long> >  seg3ul;
typedef seg<vec3<float> >          seg3f;
typedef seg<vec3<double> >         seg3d;

template <typename T>
T det2D(const seg<vec2<T> > s, const vec2<T> &r)
{
	return math::det2D(s.b, s.e, r);
}

//lexicographically order
template <typename T>
void sort(seg<vec2<T> > &ls)
{
	if (ls.e < ls.b) {
		vec2<T> aux = ls.b;
		ls.b = ls.e;
		ls.e = aux;
	}
}

//lexicographically order
template <typename T>
void sort(seg<vec2<T> > &min, seg<vec2<T> > &max)
{
	sort(min);
	sort(max);

	if (max.b < min.b) {
		seg<vec2<T> > aux = min;
		min = max;
		max = aux;
	}
}

//-SEG2-VS-SEG2---------------------------------------------------------------//

//CASOS
// 0: No intersectan
// 1: Intersectan en un punto interior de los dos segmentos
// 2: Intersectan en un extremo de los dos segmentos
// 3: Intersectan en un extremo de un segmento y en el interior del otro
// 4: Se solapan en uno al otro parcialmente
// 5: Uno esta incluido dentro del otro segmento

template <typename T>
int hit(const seg<vec2<T> >& s, const seg<vec2<T> >& t)
{
	int hitcase;

	T DetSTB = det2D(s, t.b);
	T DetSTE = det2D(s, t.e);
	T DetTSB = det2D(t, s.b);
	T DetTSE = det2D(t, s.e);

	T ST = DetSTB * DetSTE;
	T TS = DetTSB * DetTSE;

	if ( ST > 0 || TS > 0) hitcase = 0;
	else if ( ST  < 0 && TS  < 0) hitcase = 1;
	else if ((ST == 0 && TS  < 0) || (ST < 0 && TS == 0)) hitcase = 3;
	else if ( ST == 0 && TS == 0 && (DetSTB != 0 || DetSTE != 0)) hitcase = 2;
	else {
		seg<vec2<T> > min = s, max = t;
		sort(min, max);

		if (min.b == max.b) hitcase = 5;
		else if (max.b < min.e) {
			if (max.e > min.e)hitcase = 4;
			else hitcase = 5;
		}
		else if (max.b == min.e) hitcase = 2;
		else hitcase = 0;
	}

	return hitcase;
}

//-BBOX-VS-SEG---------------------------------------------------------------//
// Calcula els outcode de cohen-sutherland per un punt i una bounding box.
// http://en.wikipedia.org/wiki/Cohen-Sutherland
template <typename Real>
bool cohen_sutherland(const bbox<vec3<Real> > &bb, const seg<vec3<Real> > &r);
template <typename Real>
bool cohen_sutherland(const bbox<vec2<Real> > &bb, const seg<vec2<Real> > &r);

template <typename T>
bool hit(const bbox<vec3<T> > &bb, const seg<vec3<T> > &rr)
{
	bbox3f b(vec3f(bb.min.x,bb.min.y,bb.min.z),
		 vec3f(bb.max.x,bb.max.y,bb.max.z));
	seg3f  r(vec3f(rr.p.x, rr.p.y, rr.p.z),
		 vec3f(rr.v.x, rr.v.y, rr.v.z));

	return cohen_sutherland(b, r);
}

template <typename T>
bool hit(const bbox<vec2<T> > &bb, const seg<vec3<T> > &rr)
{
	bbox2d b(vec2d(bb.min.x,bb.min.y),
		 vec2d(bb.max.x,bb.max.y));
	seg2d  r(vec3d(rr.p.x, rr.p.y),
		 vec3d(rr.v.x, rr.v.y));

	return cohen_sutherland(b, r);
}

bool hit(const bbox3f &bb, const seg3f &r);
bool hit(const bbox3d &bb, const seg3d &r);
bool hit(const bbox2f &bb, const seg2f &r);
bool hit(const bbox2d &bb, const seg2d &r);

} //namespace math

#endif
