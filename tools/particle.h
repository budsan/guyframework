#pragma once

#include <vector>
#include "math/algebra3.h"
#include "graphics/color.h"

namespace Guy {

using math::vec2f;
using math::vec2d;

class ParticleEmitter;
class Particle
{
public:
	Particle(const vec2d &_pos, const vec2d &_vel, double m_live);
	bool update(double deltaTime, ParticleEmitter &parent);
	void draw(ParticleEmitter &parent);
	void fillDrawArray(ParticleEmitter &parent,
		std::vector<vec2f> &vertcoords,
		std::vector<vec2f> &texcoords,
		std::vector<rgba>  &vertcolor,
		std::vector<unsigned int> &indices);

	void fillDrawArray(ParticleEmitter &parent,
		vec2f *vertcoords,
		vec2f *texcoords,
		rgba *vertcolor,
		unsigned int *indices, unsigned int indBase);
protected:
	vec2d m_pos0, m_pos, m_vel0;
	double m_live, m_time;

	friend class ParticleEmitter;
};

} // namespace Guy
