#pragma once
#include <vector>

#include "color.h"

namespace Guy {

class Screen
{
public:
	struct Mode
	{
		Mode();
		Mode(unsigned int w, unsigned int h);

		float ratio() const;
		void operator= (const Mode& other);
		bool operator==(const Mode& other) const;
		bool operator< (const Mode& other) const;

		unsigned int w;
		unsigned int h;
	};

	virtual bool setMode(const Mode& mode, bool fullscreen = true) = 0;
	virtual const std::vector<Mode>& availableModes(unsigned int &size) = 0;
	virtual const Mode& currentMode() = 0;

	virtual void fillWithColor(const rgba &color) = 0;
	virtual void flip() = 0;
};

} // namespace Guy

