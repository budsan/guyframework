#ifndef _MATH_VEC3_DEFINED_
#define _MATH_VEC3_DEFINED_

#define _USE_MATH_DEFINES
#include <cmath>

namespace math {

template <typename T>
struct vec3
{
	T x, y, z;

	vec3() : x(0), y(0), z(0) {}
	vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}

	double module() const
	{
		return sqrt(x*x + y*y + z*z);
	}

	void normalize()
	{
		double dist = module();
		if (dist == 0.0) return;

		x /= dist;
		y /= dist;
		z /= dist;
	}

	bool operator==(const vec3 &other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}

	bool operator!=(const vec3 &other) const
	{
		return x != other.x || y != other.y || z != other.z;
	}

	bool operator<(const vec3 &other) const
	{
		if ( x == other.x)
		{
			if( y == other.y) return z < other.z;
			return y < other.y;
		}
		return x < other.x;
	}

	bool operator>(const vec3 &other) const
	{
		if ( x == other.x)
		{
			if( y == other.y) return z > other.z;
			return y > other.y;
		}
		return x > other.x;
	}

	vec3 operator+(const vec3 &other) const
	{
		vec3 ret;
		ret.x = x + other.x;
		ret.y = y + other.y;
		ret.z = z + other.z;
		return ret;
	}

	vec3 operator-(const vec3 &other) const
	{
		vec3 ret;
		ret.x = x - other.x;
		ret.y = y - other.y;
		ret.z = z - other.z;
		return ret;
	}

	void operator+=(const vec3 &other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
	}

	void operator-=(const vec3 &other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
	}

	vec3 operator*(T num) const
	{
		vec3 ret;
		ret.x = x * num;
		ret.y = y * num;
		ret.z = z * num;
		return ret;
	}

	vec3 operator/(T num) const
	{
		vec3 ret;
		ret.x = x / num;
		ret.y = y / num;
		ret.z = z / num;
		return ret;
	}

	void operator*=(T num)
	{
		x *= num;
		y *= num;
		z *= num;
	}

	void operator/=(T num)
	{
		x /= num;
		y /= num;
		z /= num;
	}

	vec3 &operator=(const vec3 &other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}
};

typedef vec3<char>           vec3c;
typedef vec3<unsigned char>  vec3uc;
typedef vec3<short>          vec3s;
typedef vec3<unsigned short> vec3us;
typedef vec3<int>            vec3i;
typedef vec3<unsigned int>   vec3ui;
typedef vec3<long>           vec3l;
typedef vec3<unsigned long>  vec3ul;
typedef vec3<float>          vec3f;
typedef vec3<double>         vec3d;

} //namespace math

#endif
