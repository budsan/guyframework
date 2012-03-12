#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>
#include "math/vec2.h"
#include "graphics/color.h"
using math::vec2f;

class ParticleEmitter;
class Particle
{
public:
	Particle(const vec2f &_pos, const vec2f &_vel, float m_live);
	bool update(float deltaTime, ParticleEmitter &parent);
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
	vec2f m_pos0, m_pos, m_vel0;
	float m_live, m_time;



	friend class ParticleEmitter;
};

#endif // PARTICLE_H
