#include <iostream>
#include "algebra3.h"
using namespace math;

//--OSTREAM------------------------------------------------------------------//

std::ostream &operator<<(std::ostream &s, const vec2<char> &v)
{
	s << (int) v.x << " " << (int) v.y;
	return s;
}

std::ostream &operator<<(std::ostream &s, const vec3<char> &v)
{
	s << (int) v.x << " " << (int) v.y << " " << (int) v.z;
	return s;
}

std::ostream &operator<<(std::ostream &s, const vec4<char> &v)
{
	s << (int) v.x << " " << (int) v.y << " " << (int) v.z << " " << (int) v.w;
	return s;
}

std::ostream &operator<<(std::ostream &s, const vec2<unsigned char> &v)
{
	s << (int) v.x << " " << (int) v.y;
	return s;
}

std::ostream &operator<<(std::ostream &s, const vec3<unsigned char> &v)
{
	s << (int) v.x << " " << (int) v.y << " " << (int) v.z;
	return s;
}

std::ostream &operator<<(std::ostream &s, const vec4<unsigned char> &v)
{
	s << (int) v.x << " " << (int) v.y << " " << (int) v.z << " " << (int) v.w;
	return s;
}

//--GENERIC------------------------------------------------------------------//

template <typename T>
std::ostream &operator<<(std::ostream &s, const vec2<T> &v)
{
	s << v.x << " " << v.y;
	return s;
}

template <typename T>
std::ostream &operator<<(std::ostream &s, const vec3<T> &v)
{
	s << v.x << " " << v.y << " " << v.z;
	return s;
}

template <typename T>
std::ostream &operator<<(std::ostream &s, const vec4<T> &v)
{
	s << v.x << " " << v.y << " " << v.z << " " << v.w;
	return s;
}

//--PRINT--------------------------------------------------------------------//

template <typename T>
void print(const T& v) {
	std::cout << v << std::endl;
}

//--MAIN---------------------------------------------------------------------//

int main(int argc, char *argv[])
{
	char           c  = -1;
	unsigned char  uc =  1;
	short          s  = -255;
	unsigned short us =  256;
	int            i  = -65536;
	unsigned int   ui =  65537;
	long           l  = -65536;
	unsigned long  ul =  65537;
	float          f  =  1.0f;
	double         d  =  1.0;

	vec2c  v2c(-1,-2);
	vec2uc v2uc(1, 2);
	vec2s  v2s(-255, -256);
	vec2us v2us(256, 257);
	vec2i  v2i(-65536, -65537);
	vec2ui v2ui(65537, -65538);
	vec2l  v2l(-65536, -65537);
	vec2ul v2ul(65537, -65538);
	vec2f  v2f(1.0f, -1.0f);
	vec2d  v2d(1.0 , -1.0 );

	vec3c  v3c(-1,-2, -3);
	vec3uc v3uc(1, 2,  3);
	vec3s  v3s(-255, -256, -257);
	vec3us v3us(256, 257, 258);
	vec3i  v3i(-65536, -65537, -65538);
	vec3ui v3ui(65537,  65538,  65539);
	vec3l  v3l(-65536, -65537, -65538);
	vec3ul v3ul(65537,  65538,  65539);
	vec3f  v3f(1.0f, -1.0f, 0.0f);
	vec3d  v3d(1.0 , -1.0 , 0.0 );

	vec2c r2c;
	r2c = vec2c(c);  print(r2c);
	r2c = vec2c(uc); print(r2c);
	r2c = vec2c(s);  print(r2c);
	r2c = vec2c(us); print(r2c);
	r2c = vec2c(i);  print(r2c);
	r2c = vec2c(ui); print(r2c);
	r2c = vec2c(l);  print(r2c);
	r2c = vec2c(ul); print(r2c);
	r2c = vec2c(f);  print(r2c);
	r2c = vec2c(d);  print(r2c);

	r2c = v2uc; print(r2c);
	r2c = v2s;  print(r2c);
	r2c = v2us; print(r2c);
	r2c = v2i;  print(r2c);
	r2c = v2ui; print(r2c);
	r2c = v2l;  print(r2c);
	r2c = v2ul; print(r2c);
	r2c = v2f;  print(r2c);
	r2c = v2d;  print(r2c);

	r2c = v2c + v2uc; print(r2c);
	r2c = v2c + v2s;  print(r2c);
	r2c = v2c + v2us; print(r2c);
	r2c = v2c + v2i;  print(r2c);
	r2c = v2c + v2ui; print(r2c);
	r2c = v2c + v2l;  print(r2c);
	r2c = v2c + v2ul; print(r2c);
	r2c = v2c + v2f;  print(r2c);
	r2c = v2c + v2d;  print(r2c);

	r2c = v2c - v2uc; print(r2c);
	r2c = v2c - v2s;  print(r2c);
	r2c = v2c - v2us; print(r2c);
	r2c = v2c - v2i;  print(r2c);
	r2c = v2c - v2ui; print(r2c);
	r2c = v2c - v2l;  print(r2c);
	r2c = v2c - v2ul; print(r2c);
	r2c = v2c - v2f;  print(r2c);
	r2c = v2c - v2d;  print(r2c);
}
