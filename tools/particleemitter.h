#pragma once

#include <list>
#include <fstream>
#include "boost/random.hpp"

#include "math/algebra3.h"
#include "graphics/color.h"
#include "particle.h"

namespace Guy {

class ParticleEmitter
{
public:
	ParticleEmitter();
	ParticleEmitter(const ParticleEmitter &oth);

	void update(float deltaTime);
	void draw();
	void restart();

	bool load(std::string path);
	void load();
	void unload();

	// SETTERS

	void setPosition(const vec2f &pos);
	void setDirection(const vec2f &dir);
	void setDirection(float dir);
	void setAngle(float angle);

	void setParticleNumber(int num);
	void setFrequency(float freq);
	void setGravity(const vec2f &grav);

	void setParticleLive(float l);
	void setParticleLive(float l0, float l1);
	void setParticleColor(const rgba &c);
	void setParticleColor(const rgba &c0, const rgba &c1);
	void setParticleSpeed(float s);
	void setParticleSpeed(float s0, float s1);
	void setParticleSize(float z);
	void setParticleSize(float z0, float z1);
	void setParticleAccumulativeColor(bool enable);
	void setParticleMaterial(std::string name);

	//GETTERS

	const vec2f &position()  { return m_pos; }
	float getDirection() { return m_dir; }
	float getAngle() { return m_angle; }

	int getParticleNumber() { return m_num0;}
	float getFrequency() { return m_freq; }
	const vec2f & getGravity() { return m_grav; }

	float getParticleLiveMin() { return m_l0; }
	float getParticleLiveMax() { return m_l1; }
	const rgba &getParticleColorStart() { return m_c0; }
	const rgba &getParticleColorEnd()   { return m_c1; }
	float getParticleSpeedMin() { return m_s0; }
	float getParticleSpeedMax() { return m_s1; }
	float getParticleSizeStart() { return m_z0; }
	float getParticleSizeEnd()   { return m_z1; }
	bool getParticleAccumulativeColor() { return m_accumBlending; }
	const std::string &getParticleMaterial() { return m_material; }

	bool areParticlesLeft() { return m_num || m_particles.empty();}

	void write(std::ofstream &file);
	void read(std::ifstream &file);

private:
	void newParticle();

	void read (std::ifstream &file, vec2f &v);
	void write(std::ofstream &file, const vec2f &v);
	void read (std::ifstream &file, rgba &c);
	void write(std::ofstream &file, const rgba &c);

protected:

	vec2f m_pos;
	float m_dir;
	float m_angle;

	int m_num, m_num0;
	float m_freq;
	vec2f m_grav;

	float m_time;

	float m_l0, m_l1;
	rgba  m_c0, m_c1;
	float m_s0, m_s1;
	float m_z0, m_z1;
	bool m_accumBlending;
	std::string m_material;

	friend class Particle;
	std::list<Particle> m_particles;

	//RANDOM ENGINE
	boost::mt19937 m_random;
};

} // namespace Guy
