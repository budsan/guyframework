#include "screen.h"

namespace Guy {

Screen::Mode::Mode()
	: w(0), h(0)
{

}

Screen::Mode::Mode(unsigned int _w, unsigned int _h)
	: w(_w), h(_h)
{

}

float Screen::Mode::ratio() const
{
	return float(w)/float(h);
}

void Screen::Mode::operator=(const Mode& other)
{
	 this->w   = other.w;
	 this->h   = other.h;
}

bool Screen::Mode::operator==(const Mode& other) const
{
	return (this->w   == other.w   || this->w   == 0 || other.w   == 0) &&
	       (this->h   == other.h   || this->h   == 0 || other.h   == 0);
}

bool Screen::Mode::operator< (const Mode& other) const
{
	unsigned int pixelCountA = (this->w * this->h);
	unsigned int pixelCountB = (other.w * other.h);
	return pixelCountA < pixelCountB; //SORTED GREATER-->SMALLER
}

} //namespace Guy
