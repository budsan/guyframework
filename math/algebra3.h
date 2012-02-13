#ifndef _MATH_ALGEBRA3_DEFINED_
#define _MATH_ALGEBRA3_DEFINED_

#define _USE_MATH_DEFINES
#include <cmath>

namespace math {

template <typename T> struct vec2;
template <typename T> struct vec3;
template <typename T> struct vec4;

//-VEC2----------------------------------------------------------------------//
template <typename T>
struct vec2
{
	T x, y;

	vec2() : x(0), y(0) {}
	vec2(T _x, T _y) : x(_x), y(_y) {}
	vec2(T a) : x(a), y(a) {}

	double module() const {
		return sqrt(moduleSqr());
	}

	T moduleSqr() const {
		return x*x + y*y;
	}

	vec2<T> normalized() {
		double dist = module();
		if (dist == 0.0) return *this;
		return *this * (1.0/dist);
	}

	vec2<T> &normalize() {
		double dist = module();
		if (dist == 0.0) return *this;
		*this *= (1.0/dist);
		return *this;
	}

	T &operator [] (int i) {
		return ((T*)this)[i];
	}

	const T &operator [] (int i) const {
		return ((T*)this)[i];
	}


	vec2<T> operator - () {
		return vec2<T>(-x, -y);
	}

	template <typename U> vec2<T> &operator += (const vec2<U> &other) {
		x += other.x;
		y += other.y;
		return *this;
	}

	template <typename U> vec2<T> &operator -= (const vec2<U> &other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}

	template <typename U> vec2<T> &operator*=(U num) {
		x *= num;
		y *= num;
		return *this;
	}

	vec2<T> &operator/=(double num) {
		return *this *= (1.0/num);
	}

	vec2<T> &operator/=(float num) {
		return *this *= (1.0f/num);
	}

	template <typename U> vec2<T> &operator/=(U num) {
		x /= num;
		y /= num;
		return *this;
	}

	template <typename U>
	vec2<T> &operator=(const vec2<U> &other)
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


//-VEC3----------------------------------------------------------------------//
template <typename T>
struct vec3
{
	T x, y, z;

	vec3() : x(0), y(0), z(0) {}
	vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z) {}
	vec3(T a) : x(a), y(a), z(a) {}

	double module() const {
		return sqrt(moduleSqr());
	}

	T moduleSqr() const {
		return x*x + y*y + z*z;
	}

	vec3<T> normalized() {
		double dist = module();
		if (dist == 0.0) return *this;
		return *this * (1.0/dist);
	}

	vec3<T> &normalize() {
		double dist = module();
		if (dist == 0.0) return *this;
		*this *= (1.0/dist);
		return *this;
	}

	T &operator [] (int i) {
		return ((T*)this)[i];
	}

	const T &operator [] (int i) const {
		return ((T*)this)[i];
	}


	vec3<T> operator - () {
		return vec3<T>(-x, -y, -z);
	}

	template <typename U> vec3<T> &operator += (const vec3<U> &other) {
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	template <typename U> vec3<T> &operator -= (const vec3<U> &other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	template <typename U> vec3<T> &operator*=(U num) {
		x *= num;
		y *= num;
		z *= num;
		return *this;
	}

	vec3<T> &operator/=(double num) {
		return *this *= (1.0/num);
	}

	vec3<T> &operator/=(float num) {
		return *this *= (1.0f/num);
	}

	template <typename U> vec3<T> &operator/=(U num) {
		x /= num;
		y /= num;
		z /= num;
		return *this;
	}

	template <typename U>
	vec3<T> &operator=(const vec3<U> &other)
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

//-VEC4----------------------------------------------------------------------//
template <typename T>
struct vec4
{
	T x, y, z, w;

	vec4() : x(0), y(0), z(0), w(0) {}
	vec4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w) {}
	vec4(T a) : x(a), y(a), z(a) {}

	double module() const {
		return sqrt(moduleSqr());
	}

	T moduleSqr() const {
		return x*x + y*y + z*z + w*w;
	}

	vec4<T> normalized() {
		double dist = module();
		if (dist == 0.0) return *this;
		return *this / dist;
	}

	vec4<T> &normalize() {
		double dist = module();
		if (dist == 0.0) return *this;
		return *this /= dist;
	}

	T &operator [] (int i) {
		return ((T*)this)[i];
	}

	const T &operator [] (int i) const {
		return ((T*)this)[i];
	}


	vec4<T> operator - () {
		return vec4<T>(-x, -y, -z, -w);
	}

	template <typename U> vec4<T> &operator += (const vec4<U> &other) {
		x += other.x;
		y += other.y;
		z += other.z;
		w += other.w;
		return *this;
	}

	template <typename U> vec4<T> &operator -= (const vec4<U> &other) {
		x -= other.x;
		y -= other.y;
		z -= other.z;
		w -= other.w;
		return *this;
	}

	template <typename U> vec4<T> &operator*=(U num) {
		x *= num;
		y *= num;
		z *= num;
		w *= num;
		return *this;
	}

	vec4<T> &operator/=(double num) {
		return *this *= (1.0/num);
	}

	vec4<T> &operator/=(float num) {
		return *this *= (1.0f/num);
	}

	template <typename U> vec4<T> &operator/=(U num) {
		x /= num;
		y /= num;
		z /= num;
		w /= num;
		return *this;
	}

	template <typename U>
	vec4<T> &operator=(const vec4<U> &other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		w = other.w;
		return *this;
	}
};

typedef vec4<char>           vec4c;
typedef vec4<unsigned char>  vec4uc;
typedef vec4<short>          vec4s;
typedef vec4<unsigned short> vec4us;
typedef vec4<int>            vec4i;
typedef vec4<unsigned int>   vec4ui;
typedef vec4<long>           vec4l;
typedef vec4<unsigned long>  vec4ul;
typedef vec4<float>          vec4f;
typedef vec4<double>         vec4d;

//-VEC2----------------------------------------------------------------------//

template <typename T, typename U>
bool operator == (const vec2<T> &a, const vec2<U> &b) {
	return a.x == b.x && a.y == b.y;
}

template <typename T, typename U>
bool operator != (const vec2<T> &a, const vec2<U> &b) {
	return a.x != b.x || a.y != b.y;
}

template <typename T, typename U>
bool operator < (const vec2<T> &a, const vec2<U> &b) {
	if ( a.x == b.x) return a.y < b.y;
	return a.x < b.x;
}

template <typename T, typename U>
bool operator > (const vec2<T> &a, const vec2<T> &b) {
	if ( a.x == b.x) return a.y > b.y;
	return a.x > b.x;
}

template <typename T, typename U>
auto operator + (const vec2<T> &a, const vec2<U> &b) -> vec2<decltype(a.x+b.x)> {
	return vec2<decltype(a.x+b.x)>(a.x + b.x, a.y + b.y);
}

template <typename T, typename U>
auto operator - (const vec2<T> &a, const vec2<U> &b) -> vec2<decltype(a.x-b.x)> {
	return vec2<decltype(a.x-b.x)>(a.x - b.x, a.y - b.y);
}

template <typename T, typename U>
auto operator * (const vec2<T> &a, U num) -> vec2<decltype(a.x*num)> {
	return vec2<decltype(a.x*num)>(a.x*num, a.y*num);
}

template <typename T, typename U>
auto operator * (U num, const vec2<T> &a) -> vec2<decltype(a.x*num)> {
	return a*num;
}

//cross product
template <typename T, typename U>
auto operator ^ (const vec2<T> &a, const vec2<U> &b) -> vec3<decltype(a.x*b.x)> {
	return vec3<decltype(a.x*b.x)>( 0, 0,
		a.x*b.y - a.y*b.x);
}

//float specialization
template <typename T>
auto operator / (const vec2<T> &a, float num) -> vec2<decltype(a.x/num)> {
	float inv = 1.0f/num;
	return vec2<decltype(a.x/num)>(a.x*inv, a.y*inv);
}

//double specialization
template <typename T>
auto operator / (const vec2<T> &a, double num) -> vec2<decltype(a.x/num)> {
	double inv = 1.0/num;
	return vec2<decltype(a.x/num)>(a.x*inv, a.y*inv);
}

//general case
template <typename T, typename U>
auto operator / (const vec2<T> &a, U num) -> vec2<decltype(a.x/num)> {
	return vec2<decltype(a.x/num)>(a.x/num, a.y/num);
}

//-VEC3----------------------------------------------------------------------//
template <typename T, typename U>
bool operator == (const vec3<T> &a, const vec3<U> &b) {
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

template <typename T, typename U>
bool operator != (const vec3<T> &a, const vec3<U> &b) {
	return a.x != b.x || a.y != b.y || a.z != b.z;
}

template <typename T, typename U>
bool operator < (const vec3<T> &a, const vec3<U> &b) {
	if ( a.x == b.x) {
		if( a.y == b.y) return a.z < b.z;
		return a.y < b.y;
	}
	return a.x < b.x;
}

template <typename T, typename U>
bool operator > (const vec3<T> &a, const vec3<T> &b) {
	if ( a.x == b.x)
	{
		if( a.y == b.y) return a.z > b.z;
		return a.y > b.y;
	}
	return a.x > b.x;
}

template <typename T, typename U>
auto operator + (const vec3<T> &a, const vec3<U> &b) -> vec3<decltype(a.x+b.x)> {
	return vec3<decltype(a.x+b.x)>(a.x + b.x, a.y + b.y, a.z + b.z);
}

template <typename T, typename U>
auto operator - (const vec3<T> &a, const vec3<U> &b) -> vec3<decltype(a.x-b.x)> {
	return vec3<decltype(a.x-b.x)>(a.x - b.x, a.y - b.y, a.z - b.z);
}

template <typename T, typename U>
auto operator * (const vec3<T> &a, U num) -> vec3<decltype(a.x*num)> {
	return vec3<decltype(a.x*num)>(a.x*num, a.y*num, a.z*num);
}

template <typename T, typename U>
auto operator * (U num, const vec3<T> &a) -> vec3<decltype(a.x*num)> {
	return a*num;
}

//dot product
template <typename T, typename U>
auto operator * (const vec3<T> &a, const vec3<U> &b) -> decltype(a.x*b.x) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

//cross product
template <typename T, typename U>
auto operator ^ (const vec3<T> &a, const vec3<U> &b) -> vec3<decltype(a.x*b.x)> {
	return vec3<decltype(a.x*b.x)>(
				a.y*b.z - a.z*b.y,
				a.z*b.x - a.x*b.z,
				a.x*b.y - a.y*b.x);
}

//float specialization
template <typename T>
auto operator / (const vec3<T> &a, float num) -> vec3<decltype(a.x/num)>{
	float inv = 1.0f/num;
	return vec3<decltype(a.x/num)>(a.x*inv, a.y*inv, a.z*inv);
}

//double specialization
template <typename T>
auto operator / (const vec3<T> &a, double num) -> vec3<decltype(a.x/num)>{
	double inv = 1.0/num;
	return vec3<decltype(a.x/num)>(a.x*inv, a.y*inv, a.z*inv);
}

//general case
template <typename T, typename U>
auto operator / (const vec3<T> &a, U num) -> vec3<decltype(a.x/num)>{
	return vec3<decltype(a.x/num)>(a.x/num, a.y/num, a.z/num);
}

/* TODO
vec3<T>  operator *  (const mat4 &a, const vec3<T> &v);  // M . v
vec3<T>  operator *  (const vec3<T> &v, const mat4 &a);  // v . M
*/

//-VEC4----------------------------------------------------------------------//
template <typename T, typename U>
bool operator == (const vec4<T> &a, const vec4<U> &b) {
	return a.x == b.x && a.y == b.y && a.z == b.z && a.w == b.w;
}

template <typename T, typename U>
bool operator != (const vec4<T> &a, const vec4<U> &b) {
	return a.x != b.x || a.y != b.y || a.z != b.z || a.w != b.w;
}

template <typename T, typename U>
bool operator < (const vec4<T> &a, const vec4<U> &b) {
	if ( a.x == b.x) {
		if( a.y == b.y) {
			if (a.z == b.z) return a.w < b.w;
			return a.z < b.z;
		}
		return a.y < b.y;
	}
	return a.x < b.x;
}

template <typename T, typename U>
bool operator > (const vec4<T> &a, const vec4<T> &b) {
	if ( a.x == b.x) {
		if( a.y == b.y) {
			if (a.z == b.z) return a.w > b.w;
			return a.z > b.z;
		}
		return a.y > b.y;
	}
	return a.x > b.x;
}

template <typename T, typename U>
auto operator + (const vec4<T> &a, const vec4<U> &b) -> vec4<decltype(a.x+b.x)> {
	return vec4<decltype(a.x+b.x)>(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

template <typename T, typename U>
auto operator - (const vec4<T> &a, const vec4<U> &b) -> vec4<decltype(a.x-b.x)> {
	return vec4<decltype(a.x-b.x)>(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

template <typename T, typename U>
auto operator * (const vec4<T> &a, U num) -> vec4<decltype(a.x*num)> {
	return vec4<decltype(a.x*num)>(a.x*num, a.y*num, a.z*num, a.w*num);
}

template <typename T, typename U>
auto operator * (U num, const vec4<T> &a) -> vec4<decltype(a.x*num)> {
	return a*num;
}

//dot product
template <typename T, typename U>
auto operator * (const vec4<T> &a, const vec4<U> &b) -> decltype(a.x*b.x) {
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

//float specialization
template <typename T>
auto operator / (const vec4<T> &a, float num) -> vec4<decltype(a.x/num)>{
	float inv = 1.0f/num;
	return vec4<decltype(a.x/num)>(a.x*inv, a.y*inv, a.z*inv);
}

//double specialization
template <typename T>
auto operator / (const vec4<T> &a, double num) -> vec4<decltype(a.x/num)>{
	double inv = 1.0/num;
	return vec4<decltype(a.x/num)>(a.x*inv, a.y*inv, a.z*inv);
}

//general case
template <typename T, typename U>
auto operator / (const vec4<T> &a, U num) -> vec4<decltype(a.x/num)>{
	return vec4<decltype(a.x/num)>(a.x/num, a.y/num, a.z/num);
}

//---------------------------------------------------------------------------//

/* TODO
vec4<T>  operator *  (const mat4 &a, const vec4<T> &v);  // M . v
vec4<T>  operator *  (const vec4<T> &v, const mat4 &a);  // v . M
*/

} //namespace math
#endif // _MATH_ALGEBRA3_DEFINED_
