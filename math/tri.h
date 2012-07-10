#ifndef _MATH_TRI_DEFINED_
#define _MATH_TRI_DEFINED_

#include <vector>
#include <list>

#include "algebra3.h"
#include "poly2.h"

namespace math {

template <typename T>
struct tri
{
	T v[3];
	tri() {v[0]=T();v[1]=T();v[2]=T();}
	tri(T v0, T v1, T v2) {v[0]=v0;v[1]=v1;v[2]=v2;}
	tri(T _v[3]) {v[0]=_v[0];v[1]=_v[1];v[2]=_v[2];}

	void reverse()
	{
		T aux = v[2];
		v[2] = v[1];
		v[1] = aux;
	}

};

typedef tri<char>           tri1c;
typedef tri<unsigned char>  tri1uc;
typedef tri<short>          tri1s;
typedef tri<unsigned short> tri1us;
typedef tri<int>            tri1i;
typedef tri<unsigned int>   tri1ui;
typedef tri<long>           tri1l;
typedef tri<unsigned long>  tri1ul;
typedef tri<float>          tri1f;
typedef tri<double>         tri1d;

typedef tri<vec2<char> >           tri2c;
typedef tri<vec2<unsigned char> >  tri2uc;
typedef tri<vec2<short> >          tri2s;
typedef tri<vec2<unsigned short> > tri2us;
typedef tri<vec2<int> >            tri2i;
typedef tri<vec2<unsigned int> >   tri2ui;
typedef tri<vec2<long> >           tri2l;
typedef tri<vec2<unsigned long> >  tri2ul;
typedef tri<vec2<float> >          tri2f;
typedef tri<vec2<double> >         tri2d;

typedef tri<vec3<char> >           tri3c;
typedef tri<vec3<unsigned char> >  tri3uc;
typedef tri<vec3<short> >          tri3s;
typedef tri<vec3<unsigned short> > tri3us;
typedef tri<vec3<int> >            tri3i;
typedef tri<vec3<unsigned int> >   tri3ui;
typedef tri<vec3<long> >           tri3l;
typedef tri<vec3<unsigned long> >  tri3ul;
typedef tri<vec3<float> >          tri3f;
typedef tri<vec3<double> >         tri3d;

//-TRI<VEC2>-------------------------------------------------------------------//
//esta en orden counter-clock-wise?
template <typename T>
bool ccw(tri<vec2<T> > t)
{
	T d = det2D(t.v[0], t.v[1], t.v[2]);
	return d >= 0;
}

//-TRI<VEC2>-VS-VEC2-----------------------------------------------------------//
// p interior a t? Casos:
// 0: No es interior
// 1: Es interior
// 2: Intersectan en la frontera
// 3: Intersectan en un vertice
template <typename T>
int inside(tri<vec2<T> > t, vec2<T> p)
{
	int cases = 1;
	for (unsigned int i = 0; i < 3; i++)
	{
		T det = det2D(t.v[i], t.v[(i+1)%3], p);
		if  (det == 0) {
			if (cases == 2) cases = 3;
			else cases = 2;
		}
		else if (det < 0) {
			cases = 0;
			break;
		}
	}

	return cases;
}

// p interior a t? Casos: los mismos que la anterior
// En caso 2 o 3, sides[i] es true si coincidia el punto
// con la arista v[i]-v[i+1].
template <typename T>
int inside(tri<vec2<T> > t, vec2<T> p, bool sides[3])
{
	sides[0] = sides[1] = sides[2] = false;
	int cases = 1;
	for (unsigned int i = 0; i < 3; i++)
	{
		T det = det2D(t.v[i], t.v[(i+1)%3], p);
		if  (det == 0) {
			sides[i] = true;
			if (cases == 2) cases = 3;
			else cases = 2;
		}
		else if (det < 0) {
			cases = 0;
			sides[0] =
			sides[1] =
			sides[2] = false;
			break;
		}
	}

	return cases;
}

//-POLY2-TRIANGULATE-----------------------------------------------------------//
//se obtiene la array de triangulos identificadores de puntos de p
//es necesario que p este en counter-clock-wise para que funcione.
template <typename T>
std::vector<tri1ui> triangulate(const poly2<T> &p)
{
	std::vector<tri1ui> tris;
	unsigned int size = p.p.size();
	if (size < 3) return tris;

	std::list<unsigned int> poly;
	for (unsigned int i = 0; i < size; i++) poly.push_back(i);

	std::list<unsigned int>::iterator itprev = poly.end(); itprev--;
	std::list<unsigned int>::iterator itcurr = poly.begin();
	std::list<unsigned int>::iterator itnext = poly.begin(); itnext++;
	while(poly.size() > 3)
	{
		tri1ui tId = tri1ui(*itprev, *itcurr, *itnext);
		tri<vec2<T> > t(p.p[*itprev], p.p[*itcurr], p.p[*itnext]);
		if (ccw(t))
		{
			bool ear = true;
			std::list<unsigned int>::iterator it = itnext; it++;
			while (ear)
			{
				if (it == poly.end()) it = poly.begin();
				if (it == itprev) break;
				bool sides[3];
				switch(inside(t, p.p[*it], sides))
				{
				case 0: break;
				case 1: ear = false; break;
				default:
					for (unsigned int i = 0; i < 3; i++) {
						if((tId.v[i]+1)%size == tId.v[(i+1)%3] && sides[0]) break;
						if (i == 2) ear = false;
					}
					break;
				}

				it++;
			}

			if (ear)
			{
				if (det2D(t.v[0], t.v[1], t.v[2]) > 0) tris.push_back(tId);
				poly.erase(itcurr);
				itprev = poly.end(); itprev--;
				itcurr = poly.begin();
				itnext = poly.begin(); itnext++;
				continue;
			}
		}

		itprev = itcurr;
		itcurr = itnext;
		itnext++;
		if (itnext == poly.end()) itnext = poly.begin();
		if (itcurr == poly.begin()) return std::vector<tri1ui>();
	}

	tris.push_back(tri1ui(*itprev, *itcurr, *itnext));
	return tris;
}

} //namespace math

#endif
