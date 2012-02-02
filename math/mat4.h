#ifndef _MATH_MAT4_DEFINED_
#define _MATH_MAT4_DEFINED_

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstring>

namespace math
{

template <typename T>
struct mat4
{
	T val[16];

	mat4()
	{

	}

	mat4(const mat4 &mat)
	{
		*this = mat;
	}

	T &operator()(unsigned int x, unsigned int y)
	{
		return val[4*x + y];
	}

	void zero(void)
	{
		val[0] = 0; val[1] = 0; val[2] = 0; val[3] = 0;
		val[4] = 0; val[5] = 0; val[6] = 0; val[7] = 0;
		val[8] = 0; val[9] = 0; val[10]= 0; val[11]= 0;
		val[12]= 0; val[13]= 0; val[14]= 0; val[15]= 0;
	}

	void identity(void)
	{
		val[0] = 1; val[1] = 0; val[2] = 0; val[3] = 0;
		val[4] = 0; val[5] = 1; val[6] = 0; val[7] = 0;
		val[8] = 0; val[9] = 0; val[10]= 1; val[11]= 0;
		val[12]= 0; val[13]= 0; val[14]= 0; val[15]= 1;
	}

	mat4<T> operator*(const mat4<T> &mat)
	{
		mat4<T> res;

		res(0,0) = (0,0)*mat(0,0) + (1,0)*mat(0,1) + (2,0)*mat(0,2) + (3,0)*mat(0,3);
		res(0,1) = (0,1)*mat(0,0) + (1,1)*mat(0,1) + (2,1)*mat(0,2) + (3,1)*mat(0,3);
		res(0,2) = (0,2)*mat(0,0) + (1,2)*mat(0,1) + (2,2)*mat(0,2) + (3,2)*mat(0,3);
		res(0,3) = (0,3)*mat(0,0) + (1,3)*mat(0,1) + (2,3)*mat(0,2) + (3,3)*mat(0,3);
		res(1,0) = (0,0)*mat(1,0) + (1,0)*mat(1,1) + (2,0)*mat(1,2) + (3,0)*mat(1,3);
		res(1,1) = (0,1)*mat(1,0) + (1,1)*mat(1,1) + (2,1)*mat(1,2) + (3,1)*mat(1,3);
		res(1,2) = (0,2)*mat(1,0) + (1,2)*mat(1,1) + (2,2)*mat(1,2) + (3,2)*mat(1,3);
		res(1,3) = (0,3)*mat(1,0) + (1,3)*mat(1,1) + (2,3)*mat(1,2) + (3,3)*mat(1,3);
		res(2,0) = (0,0)*mat(2,0) + (1,0)*mat(2,1) + (2,0)*mat(2,2) + (3,0)*mat(2,3);
		res(2,1) = (0,1)*mat(2,0) + (1,1)*mat(2,1) + (2,1)*mat(2,2) + (3,1)*mat(2,3);
		res(2,2) = (0,2)*mat(2,0) + (1,2)*mat(2,1) + (2,2)*mat(2,2) + (3,2)*mat(2,3);
		res(2,3) = (0,3)*mat(2,0) + (1,3)*mat(2,1) + (2,3)*mat(2,2) + (3,3)*mat(2,3);
		res(3,0) = (0,0)*mat(3,0) + (1,0)*mat(3,1) + (2,0)*mat(3,2) + (3,0)*mat(3,3);
		res(3,1) = (0,1)*mat(3,0) + (1,1)*mat(3,1) + (2,1)*mat(3,2) + (3,1)*mat(3,3);
		res(3,2) = (0,2)*mat(3,0) + (1,2)*mat(3,1) + (2,2)*mat(3,2) + (3,2)*mat(3,3);
		res(3,3) = (0,3)*mat(3,0) + (1,3)*mat(3,1) + (2,3)*mat(3,2) + (3,3)*mat(3,3);

		return res;
	}

	void operator*=(const mat4<T> &mat)
	{
		*this = *this * mat;
	}

	static mat4<T> rotate(T angle, T x, T y, T z)
	{
		double rad = angle * (M_PI/180);
		double s = sin(rad), c = cos(rad);
		double ci = 1.0f - c;
		double mod = sqrt(x*x + y*y + z*z);
		double invmod = 1.0f / mod;

		T x1, y1, z1;
		T xs, ys, zs;
		T x2, y2, z2;
		T xy, yz, xz;

		mat4<T> res;
		res.identity();

		x1 = x * invmod;
		y1 = y * invmod;
		z1 = z * invmod;

		xs = x1 * s;
		ys = y1 * s;
		zs = z1 * s;

		x2 = x1 * x1;
		y2 = y1 * y1;
		z2 = z1 * z1;

		xy = x1 * y1;
		yz = y1 * z1;
		xz = x1 * z1;

		res[0][0] = x2 * ci + c;  res[1][0] = xy * ci - zs; res[2][0] = xz * ci + ys;
		res[0][1] = xy * ci + zs; res[1][1] = y2 * ci + c;  res[2][1] = yz * ci - xs;
		res[0][2] = xz * ci - ys; res[1][2] = yz * ci + xs; res[2][2] = z2 * ci + c;

		return res;
	}

	static mat4<T> translate(T x, T y, T z)
	{
		mat4<T> res;
		res.identity();

		res[3][0] = x;
		res[3][1] = y;
		res[3][2] = z;

		return res;
	}

	static mat4<T> scale(T x, T y, T z)
	{
		mat4<T> res;
		res.identity();

		res[0][0] = x;
		res[1][1] = y;
		res[2][2] = z;

		return res;
	}

	static mat4<T> frustum(T l, T r, T b, T t, T n, T f)
	{
		mat4<T> res;
		res.identity();

		res[0][0] = (2.0f * n) / (r - l);

		res[1][1] = (2.0f * n) / (t - b) ;

		res[2][0] = (r + l) / (r - l);
		res[2][1] = (t + b) / (t - b);
		res[2][2] = -1.0f * (f + n) / (f - n);
		res[2][3] = -1.0f;

		res[3][2] = (-2.0 * f * n) / (f - n);
		res[3][3] = 0.0f;

		return res;
	}

	static mat4<T> ortho(T l, T r, T b, T t, T n, T f)
	{
		mat4<T> res;
		res.identity();

		res[0][0] = 2.0f / (r - l);

		res[1][1] = 2.0f / (t - b);

		res[2][2] = 2.0f / (n - f);

		res[3][0] = (r + l) / (l - r);
		res[3][1] = (t + b) / (b - t);
		res[3][2] = (f + n) / (n - f);
	}

	void inverse(void)
	{
		mat4<T> res;
		T det, invDet;

		det =	((*this)[0][3]*(*this)[1][2]*(*this)[2][1]*(*this)[3][0]) -
			((*this)[0][2]*(*this)[1][3]*(*this)[2][1]*(*this)[3][0]) -
			((*this)[0][3]*(*this)[1][1]*(*this)[2][2]*(*this)[3][0]) +
			((*this)[0][1]*(*this)[1][3]*(*this)[2][2]*(*this)[3][0]) +
			((*this)[0][2]*(*this)[1][1]*(*this)[2][3]*(*this)[3][0]) -
			((*this)[0][1]*(*this)[1][2]*(*this)[2][3]*(*this)[3][0]) -
			((*this)[0][3]*(*this)[1][2]*(*this)[2][0]*(*this)[3][1]) +
			((*this)[0][2]*(*this)[1][3]*(*this)[2][0]*(*this)[3][1]) +
			((*this)[0][3]*(*this)[1][0]*(*this)[2][2]*(*this)[3][1]) -
			((*this)[0][0]*(*this)[1][3]*(*this)[2][2]*(*this)[3][1]) -
			((*this)[0][2]*(*this)[1][0]*(*this)[2][3]*(*this)[3][1]) +
			((*this)[0][0]*(*this)[1][2]*(*this)[2][3]*(*this)[3][1]) +
			((*this)[0][3]*(*this)[1][1]*(*this)[2][0]*(*this)[3][2]) -
			((*this)[0][1]*(*this)[1][3]*(*this)[2][0]*(*this)[3][2]) -
			((*this)[0][3]*(*this)[1][0]*(*this)[2][1]*(*this)[3][2]) +
			((*this)[0][0]*(*this)[1][3]*(*this)[2][1]*(*this)[3][2]) +
			((*this)[0][1]*(*this)[1][0]*(*this)[2][3]*(*this)[3][2]) -
			((*this)[0][0]*(*this)[1][1]*(*this)[2][3]*(*this)[3][2]) -
			((*this)[0][2]*(*this)[1][1]*(*this)[2][0]*(*this)[3][3]) +
			((*this)[0][1]*(*this)[1][2]*(*this)[2][0]*(*this)[3][3]) +
			((*this)[0][2]*(*this)[1][0]*(*this)[2][1]*(*this)[3][3]) -
			((*this)[0][0]*(*this)[1][2]*(*this)[2][1]*(*this)[3][3]) -
			((*this)[0][1]*(*this)[1][0]*(*this)[2][2]*(*this)[3][3]) +
			((*this)[0][0]*(*this)[1][1]*(*this)[2][2]*(*this)[3][3]);

		if(det == 0)
			// Singular matrix
			return;

		invDet = 1/det;

		res[0][0] = invDet *	((-(*this)[1][3]*(*this)[2][2]*(*this)[3][1]) +
					((*this)[1][2]*(*this)[2][3]*(*this)[3][1]) +
					((*this)[1][3]*(*this)[2][1]*(*this)[3][2]) -
					((*this)[1][1]*(*this)[2][3]*(*this)[3][2]) -
					((*this)[1][2]*(*this)[2][1]*(*this)[3][3]) +
					((*this)[1][1]*(*this)[2][2]*(*this)[3][3]));
		res[0][1] = invDet *	(((*this)[0][3]*(*this)[2][2]*(*this)[3][1]) -
					((*this)[0][2]*(*this)[2][3]*(*this)[3][1]) -
					((*this)[0][3]*(*this)[2][1]*(*this)[3][2]) +
					((*this)[0][1]*(*this)[2][3]*(*this)[3][2]) +
					((*this)[0][2]*(*this)[2][1]*(*this)[3][3]) -
					((*this)[0][1]*(*this)[2][2]*(*this)[3][3]));
		res[0][2] = invDet *	((-(*this)[0][3]*(*this)[1][2]*(*this)[3][1]) +
					((*this)[0][2]*(*this)[1][3]*(*this)[3][1]) +
					((*this)[0][3]*(*this)[1][1]*(*this)[3][2]) -
					((*this)[0][1]*(*this)[1][3]*(*this)[3][2]) -
					((*this)[0][2]*(*this)[1][1]*(*this)[3][3]) +
					((*this)[0][1]*(*this)[1][2]*(*this)[3][3]));
		res[0][3] = invDet *	(((*this)[0][3]*(*this)[1][2]*(*this)[2][1]) -
					((*this)[0][2]*(*this)[1][3]*(*this)[2][1]) -
					((*this)[0][3]*(*this)[1][1]*(*this)[2][2]) +
					((*this)[0][1]*(*this)[1][3]*(*this)[2][2]) +
					((*this)[0][2]*(*this)[1][1]*(*this)[2][3]) -
					((*this)[0][1]*(*this)[1][2]*(*this)[2][3]));

		res[1][0] = invDet *	(((*this)[1][3]*(*this)[2][2]*(*this)[3][0]) -
					((*this)[1][2]*(*this)[2][3]*(*this)[3][0]) -
					((*this)[1][3]*(*this)[2][0]*(*this)[3][2]) +
					((*this)[1][0]*(*this)[2][3]*(*this)[3][2]) +
					((*this)[1][2]*(*this)[2][0]*(*this)[3][3]) -
					((*this)[1][0]*(*this)[2][2]*(*this)[3][3]));
		res[1][1] = invDet *	((-(*this)[0][3]*(*this)[2][2]*(*this)[3][0]) +
					((*this)[0][2]*(*this)[2][3]*(*this)[3][0]) +
					((*this)[0][3]*(*this)[2][0]*(*this)[3][2]) -
					((*this)[0][0]*(*this)[2][3]*(*this)[3][2]) -
					((*this)[0][2]*(*this)[2][0]*(*this)[3][3]) +
					((*this)[0][0]*(*this)[2][2]*(*this)[3][3]));
		res[1][2] = invDet *	(((*this)[0][3]*(*this)[1][2]*(*this)[3][0]) -
					((*this)[0][2]*(*this)[1][3]*(*this)[3][0]) -
					((*this)[0][3]*(*this)[1][0]*(*this)[3][2]) +
					((*this)[0][0]*(*this)[1][3]*(*this)[3][2]) +
					((*this)[0][2]*(*this)[1][0]*(*this)[3][3]) -
					((*this)[0][0]*(*this)[1][2]*(*this)[3][3]));
		res[1][3] = invDet *	((-(*this)[0][3]*(*this)[1][2]*(*this)[2][0]) +
					((*this)[0][2]*(*this)[1][3]*(*this)[2][0]) +
					((*this)[0][3]*(*this)[1][0]*(*this)[2][2]) -
					((*this)[0][0]*(*this)[1][3]*(*this)[2][2]) -
					((*this)[0][2]*(*this)[1][0]*(*this)[2][3]) +
					((*this)[0][0]*(*this)[1][2]*(*this)[2][3]));

		res[2][0] = invDet *	((-(*this)[1][3]*(*this)[2][1]*(*this)[3][0]) +
					((*this)[1][1]*(*this)[2][3]*(*this)[3][0]) +
					((*this)[1][3]*(*this)[2][0]*(*this)[3][1]) -
					((*this)[1][0]*(*this)[2][3]*(*this)[3][1]) -
					((*this)[1][1]*(*this)[2][0]*(*this)[3][3]) +
					((*this)[1][0]*(*this)[2][1]*(*this)[3][3]));
		res[2][1] = invDet *	(((*this)[0][3]*(*this)[2][1]*(*this)[3][0]) -
					((*this)[0][1]*(*this)[2][3]*(*this)[3][0]) -
					((*this)[0][3]*(*this)[2][0]*(*this)[3][1]) +
					((*this)[0][0]*(*this)[2][3]*(*this)[3][1]) +
					((*this)[0][1]*(*this)[2][0]*(*this)[3][3]) -
					((*this)[0][0]*(*this)[2][1]*(*this)[3][3]));
		res[2][2] = invDet *	((-(*this)[0][3]*(*this)[1][1]*(*this)[3][0]) +
					((*this)[0][1]*(*this)[1][3]*(*this)[3][0]) +
					((*this)[0][3]*(*this)[1][0]*(*this)[3][1]) -
					((*this)[0][0]*(*this)[1][3]*(*this)[3][1]) -
					((*this)[0][1]*(*this)[1][0]*(*this)[3][3]) +
					((*this)[0][0]*(*this)[1][1]*(*this)[3][3]));
		res[2][3] = invDet *	(((*this)[0][3]*(*this)[1][1]*(*this)[2][0]) -
					((*this)[0][1]*(*this)[1][3]*(*this)[2][0]) -
					((*this)[0][3]*(*this)[1][0]*(*this)[2][1]) +
					((*this)[0][0]*(*this)[1][3]*(*this)[2][1]) +
					((*this)[0][1]*(*this)[1][0]*(*this)[2][3]) -
					((*this)[0][0]*(*this)[1][1]*(*this)[2][3]));

		res[3][0] = invDet *	(((*this)[1][2]*(*this)[2][1]*(*this)[3][0]) -
					((*this)[1][1]*(*this)[2][2]*(*this)[3][0]) -
					((*this)[1][2]*(*this)[2][0]*(*this)[3][1]) +
					((*this)[1][0]*(*this)[2][2]*(*this)[3][1]) +
					((*this)[1][1]*(*this)[2][0]*(*this)[3][2]) -
					((*this)[1][0]*(*this)[2][1]*(*this)[3][2]));
		res[3][1] = invDet *	((-(*this)[0][2]*(*this)[2][1]*(*this)[3][0]) +
					((*this)[0][1]*(*this)[2][2]*(*this)[3][0]) +
					((*this)[0][2]*(*this)[2][0]*(*this)[3][1]) -
					((*this)[0][0]*(*this)[2][2]*(*this)[3][1]) -
					((*this)[0][1]*(*this)[2][0]*(*this)[3][2]) +
					((*this)[0][0]*(*this)[2][1]*(*this)[3][2]));
		res[3][2] = invDet *	(((*this)[0][2]*(*this)[1][1]*(*this)[3][0]) -
					((*this)[0][1]*(*this)[1][2]*(*this)[3][0]) -
					((*this)[0][2]*(*this)[1][0]*(*this)[3][1]) +
					((*this)[0][0]*(*this)[1][2]*(*this)[3][1]) +
					((*this)[0][1]*(*this)[1][0]*(*this)[3][2]) -
					((*this)[0][0]*(*this)[1][1]*(*this)[3][2]));
		res[3][3] = invDet *	((-(*this)[0][2]*(*this)[1][1]*(*this)[2][0]) +
					((*this)[0][1]*(*this)[1][2]*(*this)[2][0]) +
					((*this)[0][2]*(*this)[1][0]*(*this)[2][1]) -
					((*this)[0][0]*(*this)[1][2]*(*this)[2][1]) -
					((*this)[0][1]*(*this)[1][0]*(*this)[2][2]) +
					((*this)[0][0]*(*this)[1][1]*(*this)[2][2]));

		*this = res;
	}

	void transpose(void)
	{
		mat4<T> mat;

		for(int i = 0; i < 4; i++)
		{
			mat[0][i] = (*this)[i][0];
			mat[1][i] = (*this)[i][1];
			mat[2][i] = (*this)[i][2];
			mat[3][i] = (*this)[i][3];
		}

		*this = mat;
	}

	inline T *operator[](unsigned int i)
	{
		return &val[4*i];
	}

	inline const T *operator[](unsigned int i) const
	{
		return &val[4*i];
	}

	inline mat4<T> &operator=(mat4<T> const &m)
	{
		memcpy(this->val, m.val, 16 * sizeof(T));
		return *this;
	}
};

typedef mat4<float>   mat4f;
typedef mat4<double>  mat4d;

} //namespace math

#endif // _MATH_MAT4_DEFINED_
