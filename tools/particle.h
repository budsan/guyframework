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
	Particle(const vec2f &_pos, const vec2f &_vel, float live);
	bool Update(float GameTime, ParticleEmitter &parent);
	void Draw(ParticleEmitter &parent);

	void FillDrawArray(ParticleEmitter &parent,
		std::vector<vec2f> &vertcoords,
		std::vector<vec2f> &texcoords,
		std::vector<rgba>  &vertcolor);
protected:
	vec2f m_pos, m_vel;
	float live, live0;
};

#endif // PARTICLE_H
