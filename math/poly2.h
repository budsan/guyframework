#ifndef _MATH_POLY2_DEFINED_
#define _MATH_POLY2_DEFINED_

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

#include "vec2.h"
#include "seg.h"

namespace math {

template <typename T>
struct poly2
{
	std::vector<vec2<T> > p;

	poly2() : p(std::vector<vec2<T> >()) {}
	poly2(std::vector<vec2<T> > p) : p(p) {}

	static poly2<T>* cast(std::vector<vec2<T> > *p)
	{
		return (poly2<T>*)(void*)p;
	}
	static const poly2<T>* cast(const std::vector<vec2<T> > *p)
	{
		return (const poly2<T>*)(const void*)p;
	}

	poly2 &operator=(const poly2 &other)
	{
		p=other.p;
		return *this;
	}

	//area puede ser negativa si esta ordenado clock-wise
	T area() const
	{
		vec2<T> ref(0,0);
		T area = 0;
		unsigned int size = p.size();
		for (unsigned int i = 0; i < size; i++)
		{
			seg<vec2<T> > curr(p[i], p[(i+1)%size]);
			area += det2D(curr, ref);
		}

		return area;
	}

	// invierte el orden de los vertices
	void reverse()
	{
		unsigned int halfsize = p.size()/2;
		unsigned int offset = p.size() - 1;
		for (unsigned int i = 0 ; i < halfsize; i++)
		{
			unsigned int j = offset - i;
			vec2<T> aux = p[i];
			p[i] = p[j];
			p[j] = aux;
		}
	}

	//dice si esta en orden counter-clock-wise
	bool ccw()
	{
		float a = area;
		return a > 0;
	}

	//fuerza el orden counter-clock-wise
	void force_ccw()
	{
		if(!ccw) reverse();
	}
};

typedef poly2<char>           poly2c;
typedef poly2<unsigned char>  poly2uc;
typedef poly2<short>          poly2s;
typedef poly2<unsigned short> poly2us;
typedef poly2<int>            poly2i;
typedef poly2<unsigned int>   poly2ui;
typedef poly2<long>           poly2l;
typedef poly2<unsigned long>  poly2ul;
typedef poly2<float>          poly2f;
typedef poly2<double>         poly2d;


//-POLY2-VS-SEG2---------------------------------------------------------------//
//s intersecta la frontera de p?
template <typename T>
bool hit(const poly2<T>& p, const seg<vec2<T> >& s)
{
	int count = 0;
	int size = (int)p.p.size();
	for (int i = 0; i < size; i++) {
		seg2f curr(p.p.at(i), p.p.at((i+1)%size));
		int hitcase = hit(s, curr);
		if (hitcase) return true;
	}
	return false;
}

//caso concreto de colision entre la frontera de "p" y el segmento "s"
//utilizado en dizzying.
template <typename T>
bool hit2(const poly2<T>& p, const seg<vec2<T> >& s)
{
	int count = 0;
	int size = (int)p.p.size();
	for (int i = 0; i < size; i++) {
		seg2f curr(p.p.at(i), p.p.at((i+1)%size));
		int hitcase = hit(s, curr);
		if (hitcase == 1) {
			return true;
		}
		else if (hitcase)
		{
			switch(hitcase)
			{
			case 2: count+=1; break;
			case 3: count+=2; break;
			case 4: case 5: count-=1; break;
			}

			if (count > 3)
			{
				T detb = det2D(curr.b, curr.e, s.b);
				T dete = det2D(curr.b, curr.e, s.e);
				if (detb > 0 || dete > 0) return true;
			}
		}
	}
	return false;
}

//-POINT-INSIDE-POLY2----------------------------------------------------------//
// pnt interior a p? Casos:
// 0: No es interior
// 1: Es interior
// 2: Intersectan en la frontera
// 3: Intersectan en un vertice
template <typename T>
int inside(const poly2<T>& p, const vec2<T> &pnt)
{
	vec2<T> aux = p.p.at(0);
	for (unsigned int i = 1; i < p.p.size(); i++) {
		vec2<T> aux2 = p.p.at(i);
		if (aux.x < aux2.x) aux.x = aux2.x;
	}

	aux.x += 0.0235f; //Numero al azar.
	aux.y  = 0.0235f + pnt.y;

	aux.y /= p.p.size();
	seg<vec2<T> > s(pnt,aux);
	int count = 0;
	int size = (int)p.p.size();
	for (int i = 0; i < size; i++) {
		seg2f curr(p.p.at(i), p.p.at((i+1)%size));
		int hitcase = hit(s, curr);
		switch(hitcase)
		{
		case 0: // No intersectan
			break;
		case 1: // Intersectan en un punto interior de los dos segmentos
			count++;
			break;
		case 2: // Intersectan en un extremo de los dos segmentos
			return 3;
		case 3: // Intersectan en un extremo de un segmento y en el interior del otro
		case 4: // Se solapan en uno al otro parcialmente
		case 5: // Uno esta incluido dentro del otro segmento
			return 2;
		}
	}

	if (count%2 != 0) return 1;
	return 0;
}

} //namespace math

#endif
