#pragma once

namespace Guy {

struct rgb
{
public:
	float r;
	float g;
	float b;

	rgb();
	rgb(float c);
	rgb(const rgb &c);
	rgb(float r, float g, float b);

	bool operator==(const rgb &other) const;
	bool operator!=(const rgb &other) const;
	bool operator<(const rgb &other) const;
	bool operator>(const rgb &other) const;

	void operator+=(const rgb &other);
	void operator-=(const rgb &other);

	rgb operator*(float num) const;
	rgb operator/(float num) const;
	void operator*=(float num);
	void operator/=(float num);

	rgb &operator=(const rgb &other);
	void setFromHSL(float h, float s, float l);

	//BLENDING
	static rgb interpolate(const rgb &a, const rgb &b, float weight);
	rgb operator|(const rgb &other) const;
	void operator|=(const rgb &other);

	const float *raw() const;
	float *raw();
	void clamp();
};

struct rgba
{
	float r;
	float g;
	float b;
	float a;

	rgba();
	rgba(float c);
	rgba(rgb c, float a = 1.0f);
	rgba(float r, float g, float b, float a);

	bool operator==(const rgba &other) const;
	bool operator!=(const rgba &other) const;
	bool operator<(const rgba &other) const;
	bool operator>(const rgba &other) const;

	void operator+=(const rgba &other);
	void operator-=(const rgba &other);

	rgba operator*(float num) const;
	rgba operator/(float num) const;
	void operator*=(float num);
	void operator/=(float num);

	rgba &operator=(const rgba &other);
	void setFromHSL(float h, float s, float l, float a);

	//BLENDING
	static rgba interpolate(const rgba &a, const rgba &b, float weight);
	rgba operator|(const rgba &other) const;
	void operator|=(const rgba &other);

	const float *raw() const;
	float *raw();
	void clamp();

	//CAST to crgb
	operator rgb *() {return (rgb *)(void*)this;}
	operator rgb &() {return *(rgb *)(void*)this;}
	operator const rgb *() const {return (const rgb *)(void*)this;}
	operator const rgb &() const {return *(const rgb *)(void*)this;}
};

/*
crgba operator+(const crgba &a, const crgb  &b) const;
crgba operator+(const crgb  &a, const crgba &b) const;
crgba operator+(const crgba &a, const crgba &b) const;
crgb  operator+(const crgb  &a, const crgb  &b) const;

crgba operator-(const crgba &a, const crgb  &b) const;
crgba operator-(const crgb  &a, const crgba &b) const;
crgba operator-(const crgba &a, const crgba &b) const;
crgb  operator-(const crgb  &a, const crgb  &b) const;

crgba operator*(float num, const crgba &a) const;
crgba operator/(float num, const crgba &a) const;

crgb operator*(float num, const crgb &a) const;
crgb operator/(float num, const crgb &a) const;
*/

} //namespace Guy

void glColor(const Guy::rgb &c);
void glColor(const Guy::rgb *c);
void glColor(const Guy::rgba &c);
void glColor(const Guy::rgba *c);

void glClearColor(const Guy::rgba *c);
void glClearColor(const Guy::rgba &c);


