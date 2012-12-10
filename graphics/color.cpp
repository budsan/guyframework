#include "color.h"
#include "graphics.h"

namespace Guy {

rgb::rgb() : r(0), g(0), b(0)
{

}

rgb::rgb(float c) : r(c), g(c), b(c)
{

}

rgb::rgb(const rgb &c) : r(c.r), g(c.g), b(c.b)
{

}

rgb::rgb(float _r, float _g, float _b) : r(_r), g(_g), b(_b)
{

}

bool rgb::operator==(const rgb &other) const
{
	return r == other.r && g == other.g && b == other.b;
}

bool rgb::operator!=(const rgb &other) const
{
	return r != other.r || g != other.g || b != other.b;
}

bool rgb::operator<(const rgb &other) const
{
	if ( r == other.r)
	{
		if( g == other.g)
		{
			return b < other.b;
		}
		return g < other.g;
	}
	return r < other.r;
}

bool rgb::operator>(const rgb &other) const
{
	if ( r == other.r)
	{
		if( g == other.g)
		{
			return b > other.b;
		}
		return g > other.g;
	}
	return r > other.r;
}

void rgb::operator+=(const rgb &other)
{
	r += other.r;
	g += other.g;
	b += other.b;
}

void rgb::operator-=(const rgb &other)
{
	r -= other.r;
	g -= other.g;
	b -= other.b;
}

rgb rgb::operator*(float num) const
{
	rgb ret;
	ret.r = r * num;
	ret.g = g * num;
	ret.b = b * num;
	return ret;
}

rgb rgb::operator/(float num) const
{
	rgb ret;
	ret.r = r / num;
	ret.g = g / num;
	ret.b = b / num;
	return ret;
}

void rgb::operator*=(float num)
{
	r *= num;
	g *= num;
	b *= num;
}

void rgb::operator/=(float num)
{
	r /= num;
	g /= num;
	b /= num;
}

rgb &rgb::operator=(const rgb &other)
{
	r = other.r;
	g = other.g;
	b = other.b;
	return *this;
}

void rgb::setFromHSL(float h, float s, float l)
{
	float temp1, temp2, tempr, tempg, tempb;
	if (s == 0) r = g = b = l;
	else
	{
		//Calculamos los valores temporales
		if(l < 0.5f) temp2 = l * (1 + s);
		else temp2 = (l + s) - (l * s);
		temp1 = 2 * l - temp2;
		tempr = h + 1.0f / 3.0f;
		if(tempr > 1) tempr--;
		tempg = h;
		tempb = h - 1.0f / 3.0f;
		if(tempb < 0) tempb++;

		//R (Rojo)
		if(tempr < 1.0f / 6.0f) r = temp1 + (temp2 - temp1) * 6.0f * tempr;
		else if(tempr < 0.5f) r = temp2;
		else if(tempr < 2.0f / 3.0f) r = temp1 + (temp2 - temp1) * ((2.0f / 3.0f) - tempr) * 6.0f;
		else r = temp1;

		//G (Verde)
		if(tempg < 1.0f / 6.0f) g = temp1 + (temp2 - temp1) * 6.0f * tempg;
		else if(tempg < 0.5f) g = temp2;
		else if(tempg < 2.0f / 3.0f) g = temp1 + (temp2 - temp1) * ((2.0f / 3.0f) - tempg) * 6.0f;
		else g = temp1;

		//B (Azul)
		if(tempb < 1.0f / 6.0f) b = temp1 + (temp2 - temp1) * 6.0f * tempb;
		else if(tempb < 0.5f) b = temp2;
		else if(tempb < 2.0f / 3.0f) b = temp1 + (temp2 - temp1) * ((2.0f / 3.0f) - tempb) * 6.0f;
		else b = temp1;
	}
}

//BLENDING
rgb rgb::interpolate(const rgb &a, const rgb &b, float weight)
{
	float wa = weight, wb = 1-weight;
	return rgb(a.r*wa + b.r*wb, a.g*wa + b.g*wb, a.b*wa + b.b*wb);
}

rgb rgb::operator|(const rgb &other) const
{
	return rgb((r+other.r)/2,(g+other.r)/2, (b+other.r)/2);
}

void rgb::operator|=(const rgb &other)
{
	r = (r+other.r)/2;
	g = (g+other.r)/2;
	b = (b+other.r)/2;
}

const float *rgb::raw() const
{
	return (const float*) this;
}

float *rgb::raw()
{
	return (float*) this;
}

void rgb::clamp()
{
	if (r > 1.0f)       r = 1.0f;
	else if ( r < 0.0f) r = 0.0f;

	if (g > 1.0f)       g = 1.0f;
	else if ( g < 0.0f) g = 0.0f;

	if (b > 1.0f)       b = 1.0f;
	else if ( b < 0.0f) b = 0.0f;
}

rgba::rgba(float c) : r(c), g(c), b(c), a(c)
{

}

rgba::rgba(rgb c, float _a) : r(c.r), g(c.g), b(c.b), a(_a)
{

}

rgba::rgba() : r(0), g(0), b(0), a(0)
{

}

rgba::rgba(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a)
{

}

bool rgba::operator==(const rgba &other) const
{
	return r == other.r && g == other.g && b == other.b && a == other.a;
}

bool rgba::operator!=(const rgba &other) const
{
	return r != other.r || g != other.g || b != other.b || a != other.a;
}

bool rgba::operator<(const rgba &other) const
{
	if ( r == other.r)
	{
		if( g == other.g)
		{
			if( b == other.b)
			{
				return a < other.a;
			}
			return b < other.b;
		}
		return g < other.g;
	}
	return r < other.r;
}

bool rgba::operator>(const rgba &other) const
{
	if ( r == other.r)
	{
		if( g == other.g)
		{
			if( b == other.b)
			{
				return a > other.a;
			}
			return b > other.b;
		}
		return g > other.g;
	}
	return r > other.r;
}

void rgba::operator+=(const rgba &other)
{
	r += other.r;
	g += other.g;
	b += other.b;
	a += other.a;
}

void rgba::operator-=(const rgba &other)
{
	r -= other.r;
	g -= other.g;
	b -= other.b;
	a -= other.a;
}

rgba rgba::operator*(float num) const
{
	rgba ret;
	ret.r = r * num;
	ret.g = g * num;
	ret.b = b * num;
	ret.a = a * num;
	return ret;
}

rgba rgba::operator/(float num) const
{
	rgba ret;
	ret.r = r / num;
	ret.g = g / num;
	ret.b = b / num;
	ret.a = a / num;
	return ret;
}

void rgba::operator*=(float num)
{
	r *= num;
	g *= num;
	b *= num;
	a *= num;
}

void rgba::operator/=(float num)
{
	r /= num;
	g /= num;
	b /= num;
	a /= num;
}

rgba &rgba::operator=(const rgba &other)
{
	r = other.r;
	g = other.g;
	b = other.b;
	a = other.a;
	return *this;
}

void rgba::setFromHSL(float h, float s, float l, float _a)
{
	a = _a;
	((rgb*)this)->setFromHSL(h,s,l);
}

//BLENDING
rgba rgba::interpolate(const rgba &a, const rgba &b, float weight)
{
	float wa = weight, wb = 1-weight;
	return rgba(a.r*wa + b.r*wb, a.g*wa + b.g*wb, a.b*wa + b.b*wb, a.a*wa + b.a*wb);
}

rgba rgba::operator|(const rgba &other) const
{
	return rgba((r+other.r)/2,(g+other.r)/2,
		     (b+other.r)/2,(a+other.r)/2);
}

void rgba::operator|=(const rgba &other)
{
	r = (r+other.r)/2;
	g = (g+other.r)/2;
	b = (b+other.r)/2;
	a = (a+other.r)/2;
}

const float *rgba::raw() const
{
	return (const float*) this;
}

float *rgba::raw()
{
	return (float*) this;
}

void rgba::clamp()
{
	if (r > 1.0f)       r = 1.0f;
	else if ( r < 0.0f) r = 0.0f;

	if (g > 1.0f)       g = 1.0f;
	else if ( g < 0.0f) g = 0.0f;

	if (b > 1.0f)       b = 1.0f;
	else if ( b < 0.0f) b = 0.0f;

	if (a > 1.0f)       a = 1.0f;
	else if ( a < 0.0f) a = 0.0f;
}

} // namespace Guy

void glColor(const Guy::rgb *c)
{
	glColor4f(c->r, c->g, c->b, 1.0f);
}

void glColor(const Guy::rgb &c)
{
	glColor4f(c.r, c.g, c.b, 1.0f);
}

void glColor(const Guy::rgba *c)
{
	glColor4f(c->r, c->g, c->b, c->a);
}

void glColor(const Guy::rgba &c)
{
	glColor4f(c.r, c.g, c.b, c.a);
}

void glClearColor(const Guy::rgba *c)
{
	glClearColor(c->r, c->g, c->b, c->a);
}

void glClearColor(const Guy::rgba &c)
{
	glClearColor(c.r, c.g, c.b, c.a);
}
