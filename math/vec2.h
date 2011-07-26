#ifndef _MATH_VEC2_DEFINED_
#define _MATH_VEC2_DEFINED_

#define _USE_MATH_DEFINES
#include <cmath>

namespace math {

template <typename T>
struct vec2
{
	T x, y;

	vec2() : x(0), y(0) {}
	vec2(T _x, T _y) : x(_x), y(_y) {}

	double module() const
	{
		return sqrt(x*x + y*y);
	}

	void normalize()
	{
		double dist = module();
		if (dist == 0.0) return;

		x /= dist;
		y /= dist;
	}

	bool operator==(const vec2 &other) const
	{
		return x == other.x && y == other.y;
	}

	bool operator!=(const vec2 &other) const
	{
		return x != other.x || y != other.y;
	}

	bool operator<(const vec2 &other) const
	{
		if ( x == other.x) return y < other.y;
		return x < other.x;
	}

	bool operator>(const vec2 &other) const
	{
		if ( x == other.x) return y > other.y;
		return x > other.x;
	}

	vec2 operator+(const vec2 &other) const
	{
		vec2 ret;
		ret.x = x + other.x;
		ret.y = y + other.y;
		return ret;
	}

	vec2 operator-(const vec2 &other) const
	{
		vec2 ret;
		ret.x = x - other.x;
		ret.y = y - other.y;
		return ret;
	}

	void operator+=(const vec2 &other)
	{
		x += other.x;
		y += other.y;
	}

	void operator-=(const vec2 &other)
	{
		x -= other.x;
		y -= other.y;
	}

	vec2 operator*(T num) const
	{
		vec2 ret;
		ret.x = x * num;
		ret.y = y * num;
		return ret;
	}

	vec2 operator/(T num) const
	{
		vec2 ret;
		ret.x = x / num;
		ret.y = y / num;
		return ret;
	}

	void operator*=(T num)
	{
		x *= num;
		y *= num;
	}

	void operator/=(T num)
	{
		x /= num;
		y /= num;
	}

        vec2 &operator=(const vec2 &other)
        {
		x = other.x;
		y = other.y;
		return *this;
        }
};

typedef vec2<char>           vec2c;
typedef vec2<unsigned char>  vec2uc;
typedef vec2<short>          vec2s;
typedef vec2<unsigned short> vec2us;
typedef vec2<int>            vec2i;
typedef vec2<unsigned int>   vec2ui;
typedef vec2<long>           vec2l;
typedef vec2<unsigned long>  vec2ul;
typedef vec2<float>          vec2f;
typedef vec2<double>         vec2d;

template <typename T>
T det2D(const vec2<T> &p, const vec2<T> &q, const vec2<T> &r)
{
	T matrix[2*2] = {
		q.x - p.x, r.x - p.x,
		q.y - p.y, r.y - p.y
	};

	return (matrix[0]*matrix[3])-(matrix[1]*matrix[2]);
}

} //namespace math

#endif
