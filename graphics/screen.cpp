#include "screen.h"

Screen::Mode::Mode()
	: w(0), h(0), bpp(0)
{

}

Screen::Mode::Mode(unsigned int w, unsigned int h)
	: w(w), h(h)
{

}

float Screen::Mode::getRatio() const
{
	return float(w)/float(h);
}

void Screen::Mode::operator=(const Mode& other) const
{
	 this->w   = other.w;
	 this->h   = other.h;
}

bool Screen::Mode::operator==(const Mode& other) const
{
	return (this->w   == other.w   || this->w   == 0 || other.w   == 0) &&
	       (this->h   == other.h   || this->h   == 0 || other.h   == 0) &&
	       (this->bpp == other.bpp || this->bpp == 0 || other.bpp == 0);
}

bool Screen::Mode::operator< (const Mode& other) const
{
	unsigned int pixelCountA = (this->w * this->h);
	unsigned int pixelCountB = (other.w * other.h);
	return pixelCountA < pixelCountB; //SORTED GREATER-->SMALLER
}
