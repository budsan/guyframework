#ifndef _MATH_BBOX_DEFINED_
#define _MATH_BBOX_DEFINED_

#include <climits>
#include <cfloat>

#include "algebra3.h"

namespace math {

//-LIMITS--------------------------------------------------------------------//

inline void minvalue(char &val) {val = SCHAR_MIN;}
inline void minvalue(unsigned char &val) {val = 0;}
inline void minvalue(short &val) {val = SHRT_MIN;}
inline void minvalue(unsigned short &val) {val = 0;}
inline void minvalue(int &val) {val = INT_MIN;}
inline void minvalue(unsigned int &val) {val = 0;}
inline void minvalue(long &val) {val = LONG_MIN;}
inline void minvalue(unsigned long &val) {val = 0;}
inline void minvalue(float &val) {val = -FLT_MAX;}
inline void minvalue(double &val) {val = -DBL_MAX;}

template <typename T>
inline void minvalue(vec2<T> &val)
{
	minvalue(val.x);
	minvalue(val.y);
}

template <typename T>
inline void minvalue(vec3<T> &val)
{
	minvalue(val.x);
	minvalue(val.y);
	minvalue(val.z);
}

inline void maxvalue(char &val) {val = SCHAR_MAX;}
inline void maxvalue(unsigned char &val) {val = UCHAR_MAX;}
inline void maxvalue(short &val) {val = SHRT_MAX;}
inline void maxvalue(unsigned short &val) {val = USHRT_MAX;}
inline void maxvalue(int &val) {val = INT_MAX;}
inline void maxvalue(unsigned int &val) {val = UINT_MAX;}
inline void maxvalue(long &val) {val = LONG_MAX;}
inline void maxvalue(unsigned long &val) {val = ULONG_MAX;}
inline void maxvalue(float &val) {val = FLT_MAX;}
inline void maxvalue(double &val) {val = DBL_MAX;}

template <typename T>
inline void maxvalue(vec2<T> &val)
{
	maxvalue(val.x);
	maxvalue(val.y);
}

template <typename T>
inline void maxvalue(vec3<T> &val)
{
	maxvalue(val.x);
	maxvalue(val.y);
	maxvalue(val.z);
}
//-UPDATE--------------------------------------------------------------------//

template <typename T>
inline void updatemin(T &min, const T &pnt)
{
	if (pnt < min) min = pnt;
}

template <typename T>
inline void updatemin(vec2<T> &min, const vec2<T> &pnt)
{
	updatemin(min.x, pnt.x);
	updatemin(min.y, pnt.y);
}

template <typename T>
inline void updatemin(vec3<T> &min, const vec3<T> &pnt)
{
	updatemin(min.x, pnt.x);
	updatemin(min.y, pnt.y);
	updatemin(min.z, pnt.z);
}

template <typename T>
inline void updatemax(T &max, const T &pnt)
{
	if (pnt > max) max = pnt;
}

template <typename T>
inline void updatemax(vec2<T> &max, const vec2<T> &pnt)
{
	updatemax(max.x, pnt.x);
	updatemax(max.y, pnt.y);
}

template <typename T>
inline void updatemax(vec3<T> &max, const vec3<T> &pnt)
{
	updatemax(max.x, pnt.x);
	updatemax(max.y, pnt.y);
	updatemax(max.z, pnt.z);
}

//-BBOX----------------------------------------------------------------------//
template <typename T>
struct bbox
{
	T min;
	T max;

	bbox();
	bbox(T _min, T _max);

	void clear()
	{
		minvalue(max);
		maxvalue(min);
	}

	void update(const T &pnt)
	{
		updatemin(min, pnt);
		updatemax(max, pnt);
	}

	void update(const bbox<T> &bb)
	{
		update(bb.min);
		update(bb.max);
	}
};

typedef bbox<vec2c>  bbox2c;
typedef bbox<vec2uc> bbox2uc;
typedef bbox<vec2s>  bbox2s;
typedef bbox<vec2us> bbox2us;
typedef bbox<vec2i>  bbox2i;
typedef bbox<vec2ui> bbox2ui;
typedef bbox<vec2l>  bbox2l;
typedef bbox<vec2ul> bbox2ul;
typedef bbox<vec2f>  bbox2f;
typedef bbox<vec2d>  bbox2d;

typedef bbox<vec3c>  bbox3c;
typedef bbox<vec3uc> bbox3uc;
typedef bbox<vec3s>  bbox3s;
typedef bbox<vec3us> bbox3us;
typedef bbox<vec3i>  bbox3i;
typedef bbox<vec3ui> bbox3ui;
typedef bbox<vec3l>  bbox3l;
typedef bbox<vec3ul> bbox3ul;
typedef bbox<vec3f>  bbox3f;
typedef bbox<vec3d>  bbox3d;

typedef bbox<char>           bbox1c;
typedef bbox<unsigned char>  bbox1uc;
typedef bbox<short>          bbox1s;
typedef bbox<unsigned short> bbox1us;
typedef bbox<int>            bbox1i;
typedef bbox<unsigned int>   bbox1ui;
typedef bbox<long>           bbox1l;
typedef bbox<unsigned long>  bbox1ul;
typedef bbox<float>          bbox1f;
typedef bbox<double>         bbox1d;

template <typename T> bbox<T>::bbox()
{
	clear();
}

template <typename T>
bbox<T>::bbox(T _min, T _max)
{
	min = _min;
	max = _max;
}

//-POINT-VS-BBOX-------------------------------------------------------------//
//p es interior a bb?

template <typename T>
bool inside(const bbox<T> &bb, const T &p)
{
	return p>=bb.min && p<=bb.max;
}

template <typename T>
bool inside(const bbox<vec2<T> > &bb, const vec2<T> &p)
{
	return p.x>=bb.min.x && p.x<=bb.max.x &&
	       p.y>=bb.min.y && p.y<=bb.max.y;
}

template <typename T>
bool inside(const bbox<vec3<T> > &bb, const vec3<T> &p)
{
	return p.x>=bb.min.x && p.x<=bb.max.x &&
	       p.y>=bb.min.y && p.y<=bb.max.y &&
	       p.z>=bb.min.z && p.z<=bb.max.z;
}

//-BBOX-VS-BBOX--------------------------------------------------------------//
//b1 intersecta con b2?

template <typename T>
bool hit (const bbox<T> &b1, const bbox<T> &b2)
{
	return b1.min <= b2.max && b1.max >= b2.min;
}

template <typename T>
bool hit (const bbox<vec2<T> > &b1, const bbox<vec2<T> > &b2)
{
	return b1.min.x <= b2.max.x && b1.max.x >= b2.min.x &&
	       b1.min.y <= b2.max.y && b1.max.y >= b2.min.y;
}

template <typename T>
bool hit (const bbox<vec3<T> > &b1, const bbox<vec3<T> > &b2)
{
	return b1.min.x <= b2.max.x && b1.max.x >= b2.min.x &&
	       b1.min.y <= b2.max.y && b1.max.y >= b2.min.y &&
	       b1.min.z <= b2.max.z && b1.max.z >= b2.min.z;
}

//-BBOX-VS-BBOX--------------------------------------------------------------//
//b1 intersecta con b2?

template <typename T>
bool inside (const bbox<T> &b1, const bbox<T> &b2)
{
	return b1.min < b2.max && b1.max > b2.min;
}

template <typename T>
bool inside (const bbox<vec2<T> > &b1, const bbox<vec2<T> > &b2)
{
	return b1.min.x < b2.max.x && b1.max.x > b2.min.x &&
	       b1.min.y < b2.max.y && b1.max.y > b2.min.y;
}

template <typename T>
bool inside (const bbox<vec3<T> > &b1, const bbox<vec3<T> > &b2)
{
	return b1.min.x < b2.max.x && b1.max.x > b2.min.x &&
	       b1.min.y < b2.max.y && b1.max.y > b2.min.y &&
	       b1.min.z < b2.max.z && b1.max.z > b2.min.z;
}

} //namespace math

#endif
