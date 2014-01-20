#pragma once

#include <list>
#include <fstream>
#include <random>

#include "math/algebra3.h"
#include "graphics/color.h"
#include "particle.h"

namespace Guy {

class ParticleEmitter
{
public:
	ParticleEmitter();
	ParticleEmitter(const ParticleEmitter &oth);

	void update(double deltaTime);
	void draw();
	void restart();

	bool load(std::string path);
	void load();
	void unload();

	// SETTERS

	void setPosition(const vec2d &pos);
	void setDirection(const vec2d &dir);
	void setDirection(double dir);
	void setAngle(double angle);

	void setParticleNumber(int num);
	void setFrequency(double freq);
	void setGravity(const vec2d &grav);

	void setParticleLive(double l);
	void setParticleLive(double l0, double l1);
	void setParticleColor(const rgba &c);
	void setParticleColor(const rgba &c0, const rgba &c1);
	void setParticleSpeed(double s);
	void setParticleSpeed(double s0, double s1);
	void setParticleSize(double z);
	void setParticleSize(double z0, double z1);
	void setParticleAccumulativeColor(bool enable);
	void setParticleMaterial(std::string name);

	//GETTERS

	const vec2d &position()  { return m_pos; }
	double getDirection() { return m_dir; }
	double getAngle() { return m_angle; }

	int particleCount() { return m_num0;}
	double frequency() { return m_freq; }
	const vec2d & gravity() { return m_grav; }

	double particleLiveMin() { return m_l0; }
	double particleLiveMax() { return m_l1; }
	const rgba &particleColorStart() { return m_c0; }
	const rgba &particleColorEnd()   { return m_c1; }
	double particleSpeedMin() { return m_s0; }
	double particleSpeedMax() { return m_s1; }
	double particleSizeStart() { return m_z0; }
	double particleSizeEnd()   { return m_z1; }
	bool particleAccumulativeColor() { return m_accumBlending; }
	const std::string &particleMaterial() { return m_material; }

	bool areParticlesLeft() { return m_num || m_particles.empty();}

	void write(std::ofstream &file);
	void read(std::ifstream &file);

private:
	void newParticle();

	template <typename T>
	void read(std::ifstream &file, math::vec2<T>& v)
	{
        file.read((char*)&v.x,sizeof(v.x));
        file.read((char*)&v.y,sizeof(v.y));
	}

	template <typename T>
	void write(std::ofstream &file, const math::vec2<T>& v)
	{
        file.write((char*)&v.x,sizeof(v.x));
        file.write((char*)&v.y,sizeof(v.y ));
	}

	void read (std::ifstream &file, rgba &c);
	void write(std::ofstream &file, const rgba &c);

protected:

	vec2d m_pos;
	double m_dir;
	double m_angle;

	int m_num, m_num0;
	double m_freq;
	vec2d m_grav;

	double m_time;

	double m_l0, m_l1;
	rgba  m_c0, m_c1;
	double m_s0, m_s1;
	double m_z0, m_z1;
	bool m_accumBlending;
	std::string m_material;

	friend class Particle;
	std::list<Particle> m_particles;

	//RANDOM ENGINE
	std::default_random_engine m_random;
};

} // namespace Guy
